
from cffi import FFI
ffibuilder = FFI()

# Add all C definitions below...

ffibuilder.cdef(
    r"""
float add_reduce_all_f4(unsigned int x1_addr, unsigned int nbytes);
float multiply_reduce_all_f4(unsigned int x1_addr, unsigned int nbytes);
float logaddexp_reduce_all_f4(unsigned int x1_addr, unsigned int nbytes);
float dot_f4(unsigned int x1_addr, unsigned int x2_addr, unsigned int nbytes);
float sum_of_squares_f4(unsigned int x1_addr, unsigned int nbytes);
float sad_f4(unsigned int x1_addr, unsigned int x2_addr, unsigned int nbytes);
    """
)

with open("ufunc_reduce_all_f4.c") as f:
    ffibuilder.set_source("ufunc_reduce_all_f4_cffi", f.read())

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)
