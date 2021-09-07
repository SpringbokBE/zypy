import os

ips = os.listdir(os.path.join(os.path.dirname(__file__), "hw\\drivers"))

paths = [f"ps7_cortexa9_0\\standalone_domain\\bsp\\ps7_cortexa9_0\\libsrc\\{ip}\\src\\Makefile" for ip in ips] +\
        [f"zynq_fsbl\\zynq_fsbl_bsp\\ps7_cortexa9_0\\libsrc\\{ip}\\src\\Makefile" for ip in ips]

paths = [os.path.join(os.path.dirname(__file__), path) for path in paths]

for path in paths:
    try:
        with open(path, "r") as infile:
            buffer = infile.readlines()

        with open(path, "w") as outfile:
            for line in buffer:
                if "#echo" not in line:
                    outfile.write(line)
            print("Fix has been successfully applied!")
    except FileNotFoundError:
        print(f"Skipping {path}")