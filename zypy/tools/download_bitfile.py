import numpy as np
import struct
from os.path import isabs

"""
Bitfile parsing and downloading.
"""
def download_bitfile(bitfile_name):
    """
    Download the bitfile onto the FPGA.
    """
    def parse_bitfile_header(bitfile_name):
        """
        Parse the header of the bitfile and strip off the binary part that will be written to the FPGA.
        """
        with open(bitfile_name, 'rb') as bitf:
            contents = bitf.read()

        finished = False
        offset = 0
        bit_dict = {}

        # Strip the (2+n)-byte first field (2-bit length, n-bit data)
        length = struct.unpack('>h', contents[offset:offset + 2])[0]
        offset += 2 + length

        # Strip a two-byte unknown field (usually 1)
        offset += 2

        # Strip the remaining headers. 0x65 signals the bit data field
        while not finished:
            desc = contents[offset]
            offset += 1

            if desc != 0x65:
                length = struct.unpack('>h',
                                       contents[offset:offset + 2])[0]
                offset += 2
                fmt = ">{}s".format(length)
                data = struct.unpack(fmt,
                                     contents[offset:offset + length])[0]
                data = data.decode('ascii')[:-1]
                offset += length

            if desc == 0x61:
                s = data.split(";")
                bit_dict['design'] = s[0]
                bit_dict['version'] = s[-1]
            elif desc == 0x62:
                bit_dict['part'] = data
            elif desc == 0x63:
                bit_dict['date'] = data
            elif desc == 0x64:
                bit_dict['time'] = data
            elif desc == 0x65:
                finished = True
                length = struct.unpack('>i',
                                       contents[offset:offset + 4])[0]
                offset += 4
                # Expected length values can be verified in the chip TRM
                bit_dict['length'] = str(length)
                if length + offset != len(contents):
                    raise RuntimeError("Invalid length found")
                bit_dict['data'] = contents[offset:offset + length]
            else:
                raise RuntimeError("Unknown field: {}".format(hex(desc)))
        return bit_dict

    """
    Download new bitstream onto FPGA.
    """

    assert isabs(bitfile_name), f"bitfile_name = {bitfile_name} is not an absolute path!"
    binfile_name = bitfile_name.split("/")[-1] + ".bin"
    firmware_path = "/lib/firmware/" + binfile_name

    # Copy .bin part of .bit file into /lib/firmware folder.
    bit = parse_bitfile_header(bitfile_name)
    bit_buffer = np.frombuffer(bit["data"], "i4")
    bin_buffer = bit_buffer.byteswap()
    bin_buffer.tofile(firmware_path, "")

    # Send the new .bin file to the FPGA manager.

    BS_FPGA_MAN = "/sys/class/fpga_manager/fpga0/firmware"
    BS_FPGA_MAN_FLAGS = "/sys/class/fpga_manager/fpga0/flags"

    with open(BS_FPGA_MAN_FLAGS, 'w') as fd:
        fd.write("0")

    with open(BS_FPGA_MAN, 'w') as fd:
        fd.write(binfile_name)
        return bitfile_name