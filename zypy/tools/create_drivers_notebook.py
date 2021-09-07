import sys
import xml.etree.ElementTree as ET
from os import makedirs, linesep
from os.path import basename, exists

import nbformat as nbf

################################################################################
################################################################################

def parse_hwh(hwh_file):
    """
    Fetch the required information from the .hwh file.
    """
    def parameter(obj, name):
        return obj.find(f".//PARAMETER[@NAME='{name}']").attrib["VALUE"]

    def property(obj, name):
        return obj.find(f".//PROPERTY[@NAME='{name}']").attrib["VALUE"]

    ips = []        # name, instance, dtype
    addresses = []  # instance, addr
    fields = []     # instance, reg_name, reg_offset, name, offset, width, access
 

    tree = ET.parse(hwh_file).getroot()
    for module in tree.iter("MODULE"):

        instance_name = module.attrib["INSTANCE"]
        vlnv = module.attrib["VLNV"]

        if "zypy" in vlnv:
            ips.append({
                "name": vlnv.split(":")[-2],
                "instance": instance_name,
                "dtype": vlnv.split(":")[-2].split("_")[-1],
            })

            addresses.append({
                "instance": instance_name,
                "addr": parameter(module, "C_S_AXI_CONTROL_BASEADDR"),
            })
        elif any(_ in vlnv for _ in ("axi_dma", "axis_switch")):
            addresses.append({
                "instance": instance_name,
                "addr": parameter(module, "C_BASEADDR"),
            })

        for register in module.findall(f".//REGISTER"):

            reg_offset = property(register, "ADDRESS_OFFSET")
            if "0x" in reg_offset:
                reg_offset = int(reg_offset, 16)

            for field in register.findall(f".//FIELD"):

                fields.append({
                    "instance": instance_name,
                    "reg_name": register.attrib["NAME"],
                    "reg_offset": reg_offset,
                    "name": field.attrib["NAME"],
                    "offset": property(field, "BIT_OFFSET"),
                    "width": property(field, "BIT_WIDTH"),
                    "access": property(field, "ACCESS"),
                })

    return ips, addresses, fields

################################################################################

def get_defines(addresses, fields):
    """
    Create the C/C++ defines, including the IP base addresses, getters and
    setters for all fields/registers and the mmap() calls.
    """
    defines = (
        f"#include <unistd.h>\n"
        f"#include <fcntl.h>\n"
        f"#include <termios.h>\n"
        f"#include <sys/mman.h>\n"
        f"\n"
        f"///////////////////\n"
        f"// IP addresses. //\n"
        f"///////////////////\n"
        f"\n"
    )

    for (instance, address) in [_.values() for _ in addresses]:
        defines += f"#define {instance}_addr {address}\n".lower()

    defines += (
        f"\n"
        f"//////////////////////////////////////\n"
        f"// IP register getters and setters. //\n"
        f"//////////////////////////////////////\n"
        f"\n"
    )

    for (instance, reg_name, reg_offset, name, offset, width, access) in [_.values() for _ in fields]:
        fw = (1 << int(width)) - 1
        full_name = f"#define {instance}_{reg_name}_{name}"
        read = f"({instance}_vaddr[{reg_offset} >> 2] & ({fw:#x} << {offset}))"
        write = f"({instance}_vaddr[{reg_offset} >> 2] = ((x & {fw:#x}) << {offset}))"
        # readwrite = f"({instance}_vaddr[{reg_offset} >> 2] |= ((x & {fw:#x}) << {offset}))"

        # read = f"*((volatile unsigned int *)({instance}_vaddr + {reg_offset})) & ({fw:#x} << {offset})"
        # readwrite = f"*((volatile unsigned int *)({instance}_vaddr + {reg_offset})) |= ((x & {fw:#x}) << {offset})"
        # write = f"*((volatile unsigned int *)({instance}_vaddr + {reg_offset})) = ((x & {fw:#x}) << {offset})"

        if access == "read-only":
            defines += f"{full_name}_r() {read}\n".lower()
        elif access == "write-only":
            defines += f"{full_name}_w(x) {write}\n".lower()
        elif access == "read-write":
            defines += f"{full_name}_rw(x) x ? {write} : {read}\n".lower()

    for (instance, address) in [_.values() for _ in addresses]:
        if "axis_switch" in instance:
            # defines += (
            #     f"#define {instance}_disable_m(m) *((volatile unsigned int *)({instance}_vaddr + 0x40 + 4 * m)) = 0x80000000\n"
            #     f"#define {instance}_route_m_to_s(m, s) *((volatile unsigned int *)({instance}_vaddr + 0x40 + 4 * m)) = s\n"
            #     f"#define {instance}_update() *((volatile unsigned int *) {instance}_vaddr) |= 0x02;\n"
            # )
            defines += (
                f"#define {instance}_disable_m(m) ({instance}_vaddr[(0x40 + 4 * m) >> 2] = 0x80000000)\n"
                f"#define {instance}_route_m_to_s(m, s) ({instance}_vaddr[(0x40 + 4 * m) >> 2] = s)\n"
                f"#define {instance}_update(m) ({instance}_vaddr[0x00 >> 2] |= 0x02)\n"
            )

    defines += (
        f"\n"
        f"////////////////////////////////////////\n"
        f"// Physical to virtual memory mapping //\n"
        f"////////////////////////////////////////\n"
        f"\n"
        f"#define init()\\\n"
        f"    int fd = open(\"/dev/mem\", O_RDWR | O_SYNC);\\\n"
    )

    for (instance, address) in [_.values() for _ in addresses]:
        assert int(address, 16) & (~0x1000) == int(address, 16), "Address not aligned to page boundary (4kB)!"
        defines += f"    volatile unsigned int *{instance}_vaddr = (volatile unsigned int *) mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, fd, {address});\\\n"

    defines += (
        f"    close(fd);"
        f"\n\n"
        f"#define exit()\\\n"
    )

    for (instance, address) in [_.values() for _ in addresses]:
        defines += f"    munmap((unsigned int *) {instance}_vaddr, 65535);\\\n"

    return defines

################################################################################
################################################################################

if __name__ == "__main__":
    assert len(sys.argv) == 2, f"Usage: python {basename(__file__)} <OVERLAY_NAME>"

    OVERLAY_NAME = sys.argv[1]
    ZYPY_DIR = "/home/xilinx/jupyter_notebooks/zypy"
    OUTPUT_DIR = f"../lib/{OVERLAY_NAME}"

    ips, addresses, fields = parse_hwh(f"../overlays/{OVERLAY_NAME}.hwh")

    if not exists(OUTPUT_DIR):
        makedirs(OUTPUT_DIR)

    intro_text = (
        f"# {OVERLAY_NAME}\n"
        f"This notebook will build two modules in which the overlay is controlled using pybind11 and CFFI respectively. The `.so` files will be copied into the `zypy/lib/` folder so they can be used with ZyPy. The generic initalisation code is found below. After that, four implementatiations will be made and compared: the default NumPy one, the hardware accelerated version using PYNQ, the hardware accelerated version using pybind11 and the hardware accelerated version using CFFI. A timing analysis at the end will reveal the minimum stream size required for the accelerators to be more beneficial than the default NumPy implementation. To build and do the analysis, click on `Kernel > Restart & Run All`."
    )

    intro_code = (
        f"from pynq import Overlay, allocate\n"
        f"import numpy as np\n"
        f"\n"
        f"overlay = Overlay(\"{ZYPY_DIR}/overlays/{OVERLAY_NAME}.bit\")\n"
        f"\n"
        f"# Adjust this as required...\n"
        f"\n"
        f"M, N = 100, 100\n"
        f"\n"
        f"x1 = allocate(shape=(M, N), dtype=\"{ips[0]['dtype']}\")\n"
        f"x2 = allocate(shape=(M, N), dtype=\"{ips[0]['dtype']}\")\n"
        f"out = allocate(shape=(M, N), dtype=\"{ips[0]['dtype']}\")\n"
        f"\n"
        f"x1[:] = np.random.rand(*x1.shape) + 1\n"
        f"x2[:] = np.random.rand(*x1.shape) + 1\n"
        f"out[:] = np.zeros(out.shape)"
    )

    numpy_text = (
        f"# NumPy\n"
        f"This is just a wrapper around the default NumPy implementation. The wrapper is required so we can compare it in the timing analysis."
    )

    numpy_code = ""

    for (name, instance, dtype) in [_.values() for _ in ips]:
        numpy_code += (
            f"def {name}_numpy(x1, x2, out):\n"
            f"    # Implement here...\n"
            f"    out = x1 + x2\n"
            f"\n"
        )

    numpy_code += f"# Add more covered NumPy methods here (implementation)...\n\n"

    for (name, instance, dtype) in [_.values() for _ in ips]:
        numpy_code += (
            f"out[:] = 0\n"
            f"{name}_numpy(x1, x2, out)\n"
            f"print(f\"{name}_numpy = {{out}}\")\n"
            f"\n"
        )

    numpy_code += f"# Add more covered NumPy methods here (test)..."

    pynq_text = (
        f"# PYNQ\n"
        f"This is the default PYNQ implementation, in which the AXI4-Lite registers are controlled using the built-in MMIO module."
    )

    pynq_code = ""

    for (name, instance, dtype) in [_.values() for _ in ips]:
        pynq_code += (
            f"def {name}_pynq(x1, x2, out):\n"
            f"    # Implement here...\n"
            f"    overlay.{instance}.mmio.write(0x00, 0x01)  # Start the IP core\n"
            f"\n"
        )

        for (instance, _) in [_.values() for _ in addresses]:
            if "axis_switch" in instance:
                pynq_code += (
                    f"    overlay.{instance}.mmio.write(0x40 + 4 * i, 0x8000_0000)  # Disable master i\n"
                    f"    overlay.{instance}.mmio.write(0x40 + 4 * i, j)  # Route master i to slave j\n"
                    f"    x = overlay.{instance}.mmio.read(0x00)\n"
                    f"    overlay.{instance}.mmio.write(0x00, (x & 0x2))  # Do the update\n"
                    f"\n"
                )

        i = 1
        for (instance, _) in [_.values() for _ in addresses]:
            if "axi_dma" in instance:
                pynq_code += (
                    f"    overlay.{instance}.sendchannel.transfer(x{i})\n"
                    f"    overlay.{instance}.recvchannel.transfer(out)\n"
                    f"    overlay.{instance}.recvchannel.wait()\n"
                    f"\n"
                )
                i += 1

    pynq_code += f"# Add more covered PYNQ methods here (implementation)...\n\n"

    for (name, instance, dtype) in [_.values() for _ in ips]:
        pynq_code += (
            f"out[:] = 0\n"
            f"{name}_pynq(x1, x2, out)\n"
            f"print(f\"{name}_pynq = {{out}}\")\n"
            f"\n"
        )

    pynq_code += f"# Add more covered PYNQ methods here (test)..."

    pybind11_text = (
        f"# pybind11\n"
        f"This is the pybind11 implementation, in which all registers are controlled by `mmap`ing the physical memory `/dev/mem/`."
    )

    pybind11_code_1 = (
        f"%%pybind11 {OVERLAY_NAME}_pybind11\n"
        f"\n"
        f"#include \"defines.h\"\n"
        f"\n"
    )

    for (name, ip, dtype) in [_.values() for _ in ips]:
        pybind11_code_1 += (
            f"void {name}(\n"
            f"    unsigned int x1_addr,\n"
            f"    unsigned int x2_addr,\n"
            f"    unsigned int out_addr,\n"
            f"    unsigned int height,\n"
            f"    unsigned int width)\n"
            f"{{\n"
            f"    init();\n"
            f"    \n"
            f"    // Implement here...\n"
            f"\n"
            f"    // Start IP core here\n"
            f"    {ip}_ctrl_ap_start_rw(0x1);\n"
            f"\n"
        )

        for (instance, _) in [_.values() for _ in addresses]:
            if "axis_switch" in instance:
                pybind11_code_1 += (
                    f"    // Configure AXI-stream switch here\n"
                    f"    {instance}_disable_m(0);\n"
                    f"    {instance}_route_m_to_s(1, 2);\n"
                    f"    {instance}_update();\n"
                    f"\n"
                )
        pybind11_code_1 += (
            f"    // Do AXI DMA transfers here\n"
            f"    unsigned int length = height * width * sizeof(); \n"
            f"\n"
            f"    // Caution: S2MM and MM2S resets need to be grouped.\n"
        )

        for (instance, _) in [_.values() for _ in addresses]:
            if "axi_dma" in instance:
                pybind11_code_1 += (
                    f"    {instance}_mm2s_dmacr_reset_rw(0x1);\n"
                    f"    {instance}_s2mm_dmacr_reset_rw(0x1);\n"
                    f"\n"
                )

        i = 1
        for (instance, _) in [_.values() for _ in addresses]:
            if "axi_dma" in instance:
                pybind11_code_1 += (
                    f"    {instance}_mm2s_dmacr_rs_rw(0x1);\n"
                    f"    {instance}_mm2s_sa_source_address_rw(x{i}_addr);\n"
                    f"    {instance}_mm2s_length_length_rw(length);\n"
                    f"    while(!({instance}_mm2s_dmasr_idle_r()));\n"
                    f"\n"
                    f"    {instance}_s2mm_dmacr_rs_rw(0x1);\n"
                    f"    {instance}_s2mm_da_destination_address_rw(out_addr);\n"
                    f"    {instance}_s2mm_length_length_rw(length);\n"
                    f"    while(!({instance}_s2mm_dmasr_idle_r()));\n"
                    f"\n"
                )
                i += 1

        pybind11_code_1 += (
            f"    // If the core does not return a value, wait for completetion too.\n"
            f"    while(!({ip}_ctrl_ap_done_r()));\n"
            f"\n"
            f"    exit();\n"
            f"}}\n"
            f"\n"
        )

    pybind11_code_1 += f"// Add more covered pybind11 methods here (implementation)..."

    pybind11_code_2 = (
        f"# This cell writes the above source code to a file, so that it can be reused for the CFFI implementation.\n"
        f"# Please make sure the cell above is the last cell executed!\n"
        f"\n"
        f"with open(f\"{OVERLAY_NAME}.c\", \"w\") as f:\n"
        f"    code = (\"#\" + _ih[-2].split(\"#\", 1)[1])[:-2].replace(\"\\\\n\", \"\\n\")\n"
        f"    f.write(code)"
    )

    pybind11_code_3 = f"import {OVERLAY_NAME}_pybind11 as calc\n\n"

    for (name, instance, dtype) in [_.values() for _ in ips]:
        pybind11_code_3 += (
            f"def {name}_pybind11(x1, x2, out):\n"
            f"    calc.{name}(x1.physical_address, x2.physical_address, out.physical_address, *x1.shape)\n"
            f"\n"
        )

    pybind11_code_3 += f"# Add more covered pybind11 methods here (implementation)...\n\n"


    for (name, instance, dtype) in [_.values() for _ in ips]:
        pybind11_code_3 += (
            f"out[:] = 0\n"
            f"{name}_pybind11(x1, x2, out)\n"
            f"print(f\"{name}_pybind11 = {{out}}\")\n"
            f"\n"
    )
    
    pybind11_code_3 += f"# Add more covered pybind11 methods here (test)..."

    cffi_text = (
        f"# CFFI\n"
        f"This is the CFFI implementation, of which the source code is identical to the pybind11 implementation.\n"
        f"The only thing that needs to be added are the C definitions."
    )

    cffi_code_1 = (
        f"%%writefile build_{OVERLAY_NAME}_cffi.py\n"
        f"\n"
        f"from cffi import FFI\n"
        f"ffibuilder = FFI()\n"
        f"\n"
        f"# Add all C definitions below...\n"
        f"\n"
        f"ffibuilder.cdef(\n"
        f"    r\"\"\"\n"
    )

    for (name, ip, dtype) in [_.values() for _ in ips]:
        cffi_code_1 += (
            f"void {name}(\n"
            f"    unsigned int x1_addr,\n"
            f"    unsigned int x2_addr,\n"
            f"    unsigned int out_addr,\n"
            f"    unsigned int height,\n"
            f"    unsigned int width\n"
            f");\n"
        )

    cffi_code_1 += (
        f"    \"\"\"\n"
        f")\n"
        f"\n"
        f"with open(\"{OVERLAY_NAME}.c\") as f:\n"
        f"    ffibuilder.set_source(\"{OVERLAY_NAME}_cffi\", f.read())\n"
        f"\n"
        f"if __name__ == \"__main__\":\n"
        f"    ffibuilder.compile(verbose=True)"
    )

    cffi_code_2 = (
        f"%%bash\n"
        f"\n"
        f"/home/xilinx/perf_env/bin/python  build_{OVERLAY_NAME}_cffi.py\n"
        f"cp {OVERLAY_NAME}_cffi.cpython-37m-arm-linux-gnueabihf.so ../\n"
        f"cp {OVERLAY_NAME}_pybind11.so ../"
    )

    cffi_code_3 = f"from {OVERLAY_NAME}_cffi import lib as calc\n\n"

    for (name, instance, dtype) in [_.values() for _ in ips]:
        cffi_code_3 += (
            f"def {name}_cffi(x1, x2, out):\n"
            f"    calc.{name}(x1.physical_address, x2.physical_address, out.physical_address, *x1.shape)\n"
            f"\n"
        )

    cffi_code_3 += f"# Add more covered CFFI methods here (implementation)...\n\n"

    for (name, instance, dtype) in [_.values() for _ in ips]:
        cffi_code_3 += (
            f"out[:] = 0\n"
            f"{name}_cffi(x1, x2, out)\n"
            f"print(f\"{name}_cffi = {{out}}\")\n"
            f"\n"
    )
    
    cffi_code_3 += f"# Add more covered CFFI methods here (test)..."

    timing_text = (
        f"# Timing analysis\n"
        f"The timing analysis is done for each of the functions that is accelerated in hardware. The values of the stream size for which the hardware implementation provides acceleration are given by `pynq_min`, `pybind11_min` and `cffi_min` respectively. Equivalently `pynq_min_reconf`, `pybind11_min_reconf` and `cffi_min_recong` indicate the size required in case reconfiguration is taken into account as well."
    )

    timings_text, timings_code = [], []

    for (name, instance, dtype) in [_.values() for _ in ips]:

        timings_text.append(
            f"## {name}\n"
        )

        timings_code.append(
            f"from perfplot._main import PerfplotData\n"
            f"import matplotlib.pyplot as plt\n"
            f"import time\n"
            f"import os\n"
            f"import sys\n"
            f"\n"
            f"# Import the lower-level ZyPy module\n"
            f"zypy_path = os.path.abspath(os.path.join(\"../..\"))\n"
            f"\n"
            f"if zypy_path not in sys.path:\n"
            f"    sys.path.append(zypy_path)\n"
            f"    \n"
            f"from tools.download_bitfile import download_bitfile\n"
            f"\n"
            f"overlay = Overlay(\"{ZYPY_DIR}/overlays/{OVERLAY_NAME}.bit\")  # Reload the overlay or PYNQ won't restart DMAs (because of bitstream download using ZyPy).\n"
            f"\n"
            f"def _setup(n):\n"
            f"\n"
            f"    # Adjust the setup as required. The return value(s) will be passed as arguments to the kernels.\n"
            f"\n"
            f"    M = N = int(np.sqrt(n))\n"
            f"    x1 = allocate(shape=(M, N), dtype=\"{dtype}\")\n"
            f"    x2 = allocate(shape=(M, N), dtype=\"{dtype}\")\n"
            f"    out = allocate(shape=(M, N), dtype=\"{dtype}\")\n"
            f"    \n"
            f"    x1[:] = np.random.rand(*x1.shape)\n"
            f"    x2[:] = np.random.rand(*x1.shape)\n"
            f"    out[:] = np.zeros(out.shape)\n"
            f"\n"
            f"    return x1, x2, out\n"
            f"\n"
            f"iters = 10  # Specify the number of iterations per kernel per input size here.\n"
            f"n_range = [2 ** k for k in range(4, 22)]  # Specify the input size range here.\n"
            f"\n"
            f"kernels = [{name}_numpy, {name}_pynq, {name}_pybind11, {name}_cffi]\n"
            f"timings_s = np.empty((len(kernels), len(n_range)), dtype=float)\n"
            f"labels = [\"numpy\", \"pynq\", \"pybind11\", \"cffi\"]\n"
            f"xlabel = \"N\"\n"
            f"\n"
            f"# Do the actual timings.\n"
            f"for i, n in enumerate(n_range):\n"
            f"    t0_ns = time.time_ns()\n"
            f"    for _ in range(10):\n"
            f"        pass\n"
            f"    t1_ns = time.time_ns()\n"
            f"    loop_overhead_ns = t1_ns - t0_ns\n"
            f"    \n"
            f"    data = _setup(n)\n"
            f"\n"
            f"    for j, kernel in enumerate(kernels):\n"
            f"        t0_ns = time.time_ns()\n"
            f"        for _ in range(iters):\n"
            f"            kernel(*data)\n"
            f"        t1_ns = time.time_ns()\n"
            f"        timings_s[j,i] = 1e-9 * ((t1_ns - t0_ns - loop_overhead_ns) / iters)\n"
            f"        print(f\"kernel = {{labels[j]}}, n = {{n}}, t = {{timings_s[j,i]}}\")\n"
            f"\n"
            f"    for x in data:\n"
            f"        x.freebuffer()\n"
            f"\n"
            f"# Print and plot the timing results.\n"
            f"\n"
            f"p = PerfplotData(\n"
            f"    n_range,\n"
            f"    timings_s / n_range,\n"
            f"    None,\n"
            f"    labels,\n"
            f"    xlabel\n"
            f")\n"
            f"\n"
            f"print(p)\n"
            f"plt.figure(figsize=(8, 6), dpi=90)\n"
            f"p.plot()\n"
            f"plt.ylabel('Runtime per element [s]')\n"
            f"plt.show()\n"
            f"\n"
            f"# Find the intersection points between the NumPy version and the PYNQ, pybind11 and CFFI versions, respectively.\n"
            f"\n"
            f"pynq_min = 0 if timings_s[1][0] < timings_s[0][0] else np.inf\n"
            f"pybind11_min = 0 if timings_s[2][0] < timings_s[0][0] else np.inf\n"
            f"cffi_min = 0 if timings_s[3][0] < timings_s[0][0] else np.inf\n"
            f"\n"
            f"intersections = np.argwhere(np.diff(np.sign(timings_s[0] - timings_s[1]))).flatten()\n"
            f"if intersections.size > 0: pynq_min = n_range[intersections[-1]]\n"
            f"intersections = np.argwhere(np.diff(np.sign(timings_s[0] - timings_s[2]))).flatten()\n"
            f"if intersections.size > 0: pybind11_min = n_range[intersections[-1]]  \n"
            f"intersections = np.argwhere(np.diff(np.sign(timings_s[0] - timings_s[3]))).flatten()\n"
            f"if intersections.size > 0: cffi_min = n_range[intersections[-1]]\n"
            f"    \n"
            f"print(f\"pynq_min = {{pynq_min}}\")\n"
            f"print(f\"pybind11_min = {{pybind11_min}}\")\n"
            f"print(f\"cffi_min = {{cffi_min}}\")\n"
            f"\n"
            f"# Find the bitstream download time.\n"
            f"\n"
            f"t0_ns = time.time_ns()\n"
            f"download_bitfile(\"{ZYPY_DIR}/overlays/{OVERLAY_NAME}.bit\")\n"
            f"t1_ns = time.time_ns()\n"
            f"\n"
            f"bitstream_dl_time = 1e-9 * (t1_ns - t0_ns)\n"
            f"\n"
            f"print(f\"Bitstream download time = {{bitstream_dl_time}}\")\n"
            f"\n"
            f"# Print and plot the timing results (including reconfiguration overhead).\n"
            f"\n"
            f"q = PerfplotData(\n"
            f"    n_range,\n"
            f"    np.array([(timing_s + bitstream_dl_time) / n_range if i else timing_s / n_range for i, timing_s in enumerate(timings_s)]),\n"
            f"    None,\n"
            f"    np.array([label + \" - reconfig\" if i else label for i, label in enumerate(labels)]),\n"
            f"    xlabel\n"
            f")\n"
            f"\n"
            f"print(q)\n"
            f"plt.figure(figsize=(8, 6), dpi=90)\n"
            f"q.plot()\n"
            f"plt.ylabel('Runtime per element [s]')\n"
            f"plt.show()\n"
            f"\n"
            f"# Find the intersection points taking into account the bitstream download overhead.\n"
            f"\n"
            f"pynq_min_reconf, pybind11_min_reconf, cffi_min_reconf = np.inf, np.inf, np.inf\n"
            f"\n"
            f"pynq_min = 0 if timings_s[1][0] + bitstream_dl_time < timings_s[0][0] else np.inf\n"
            f"pybind11_min = 0 if timings_s[2][0] + bitstream_dl_time < timings_s[0][0] else np.inf\n"
            f"cffi_min = 0 if timings_s[3][0] + bitstream_dl_time < timings_s[0][0] else np.inf\n"
            f"\n"
            f"intersections = np.argwhere(np.diff(np.sign(timings_s[0] - (timings_s[1] + bitstream_dl_time)))).flatten()\n"
            f"if intersections.size > 0:pynq_min_reconf = n_range[intersections[-1]]\n"
            f"intersections = np.argwhere(np.diff(np.sign(timings_s[0] - (timings_s[2] + bitstream_dl_time)))).flatten()\n"
            f"if intersections.size > 0: pybind11_min_reconf = n_range[intersections[-1]]\n"
            f"intersections = np.argwhere(np.diff(np.sign(timings_s[0] - (timings_s[3] + bitstream_dl_time)))).flatten()\n"
            f"if intersections.size > 0: cffi_min_reconf = n_range[intersections[-1]]\n"
            f"\n"
            f"print(f\"pynq_min_reconf = {{pynq_min_reconf}}\")\n"
            f"print(f\"pybind11_min_reconf = {{pybind11_min_reconf}}\")\n"
            f"print(f\"cffi_min_reconf = {{cffi_min_reconf}}\")"
        )

    # Write definitions to header file in output directory for sake of clarity.
    with open(f"{OUTPUT_DIR}/defines.h", "w") as f:
        f.write(get_defines(addresses, fields))

    # Write notebook to output directory.
    with open(f"{OUTPUT_DIR}/build_{OVERLAY_NAME}.ipynb", "w") as f:
        nb = nbf.v4.new_notebook()
        nb["cells"] = [
            nbf.v4.new_markdown_cell(intro_text),
            nbf.v4.new_code_cell(intro_code),
            nbf.v4.new_markdown_cell(numpy_text),
            nbf.v4.new_code_cell(numpy_code),
            nbf.v4.new_markdown_cell(pynq_text),
            nbf.v4.new_code_cell(pynq_code),
            nbf.v4.new_markdown_cell(pybind11_text),
            nbf.v4.new_code_cell(pybind11_code_1),
            nbf.v4.new_code_cell(pybind11_code_2),
            nbf.v4.new_code_cell(pybind11_code_3),
            nbf.v4.new_markdown_cell(cffi_text),
            nbf.v4.new_code_cell(cffi_code_1),
            nbf.v4.new_code_cell(cffi_code_2),
            nbf.v4.new_code_cell(cffi_code_3),
            nbf.v4.new_markdown_cell(timing_text),
        ]
        
        for (t, c) in zip(timings_text, timings_code):
            nb["cells"].extend([nbf.v4.new_markdown_cell(t), nbf.v4.new_code_cell(c)])

        nbf.write(nb, f)

