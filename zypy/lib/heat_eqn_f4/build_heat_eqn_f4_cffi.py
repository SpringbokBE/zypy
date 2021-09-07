
from cffi import FFI
ffibuilder = FFI()

# Add all C definitions below...

ffibuilder.cdef(
    r"""
void filter_avg_f4(unsigned int x1_addr, unsigned int out_addr, unsigned int height, unsigned int width);
float sad_reduce_all_f4(unsigned int x1_addr, unsigned int x2_addr, unsigned int nbytes);
    """
)

with open("heat_eqn_f4.c") as f:
    ffibuilder.set_source("heat_eqn_f4_cffi", f.read())

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)
