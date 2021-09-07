
from cffi import FFI
ffibuilder = FFI()

ffibuilder.cdef(
    r"""
void filter_avg_f4(
    unsigned int x1_addr,
    unsigned int out_addr,
    unsigned int height,
    unsigned int width
);
    """
)

with open("filter_avg_f4.c") as f:
    ffibuilder.set_source("filter_avg_f4_cffi", f.read())

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)
