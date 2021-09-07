from struct import pack, unpack
from pynq import allocate, Overlay
import numpy as np

def heat2d_pynq(h, w, epsilon=0.005):
    
    def to_float(u32):
        return unpack('f', pack('I', u32))[0]

    def filter_avg_f4_pynq(x1, out):
        overlay.filter_avg_f4_0.mmio.write(0x10, (x1.shape[0] << 0) | (x1.shape[1] << 16))  # Set args
        overlay.filter_avg_f4_0.mmio.write(0x00, 0x01)  # Start the IP core

        overlay.axis_switch_0.mmio.write(0x40 + 4 * 0, 0)  # Route master 0 to slave 0
        overlay.axis_switch_0.mmio.write(0x40 + 4 * 1, 0x8000_0000)  # Disable master 1
        overlay.axis_switch_0.mmio.write(0x40 + 4 * 2, 0x8000_0000)  # Disable master 2
        overlay.axis_switch_0.mmio.write(0x40 + 4 * 3, 2)  # Route master 3 to slave 2
        x = overlay.axis_switch_0.mmio.read(0x00)
        overlay.axis_switch_0.mmio.write(0x00, (x | 0x2))  # Do the update

        overlay.axi_dma_0.sendchannel.transfer(x1)
        overlay.axi_dma_0.recvchannel.transfer(out)
        overlay.axi_dma_0.recvchannel.wait()

    def sad_reduce_all_f4_pynq(x1, x2):
        overlay.sad_reduce_all_f4_0.mmio.write(0x00, 0x01)  # Start the IP core

        overlay.axis_switch_0.mmio.write(0x40 + 4 * 0, 0x8000_0000)  # Disable master 0
        overlay.axis_switch_0.mmio.write(0x40 + 4 * 1, 0)  # Route master 1 to slave 0
        overlay.axis_switch_0.mmio.write(0x40 + 4 * 2, 1)  # Route master 2 to slave 1
        overlay.axis_switch_0.mmio.write(0x40 + 4 * 3, 0x8000_0000)  # Disable master 3
        x = overlay.axis_switch_0.mmio.read(0x00)
        overlay.axis_switch_0.mmio.write(0x00, (x | 0x2))  # Do the update

        overlay.axi_dma_0.sendchannel.transfer(x1)
        overlay.axi_dma_1.sendchannel.transfer(x2)

        while not (overlay.sad_reduce_all_f4_0.mmio.read(0x00) & 0x2): pass  # Wait until done

        out = overlay.sad_reduce_all_f4_0.mmio.read(0x10)

        return to_float(out)
    
    overlay = Overlay("/home/xilinx/jupyter_notebooks/zypy/overlays/heat_eqn_f4.bit")
    
    grids = [
        allocate((h + 2, w + 2), dtype=np.float32),
        allocate((h + 2, w + 2), dtype=np.float32)
    ]
    
    grids[0][:] = 0
    grids[0][:, 0] = -273.15
    grids[0][:, -1] = -273.15
    grids[0][-1, :] = -273.15
    grids[0][0, :] = 40.0
    
    delta = epsilon + 1
    
    i = 0
    switch = False
    while delta > epsilon:
        if not (i % (10)): print(delta)
        filter_avg_f4_pynq(grids[switch], grids[not switch])
        delta = sad_reduce_all_f4_pynq(grids[0], grids[1])
        switch = not switch
        i += 1

    return grids[0]
