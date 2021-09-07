
from cffi import FFI
ffibuilder = FFI()

# Add all C definitions below...

ffibuilder.cdef(
    r"""
void add_call_f4(unsigned int x1_addr, unsigned int x2_addr, int out_addr, unsigned int nbytes);
void subtract_call_f4(unsigned int x1_addr, unsigned int x2_addr, int out_addr, unsigned int nbytes);
void multiply_call_f4(unsigned int x1_addr, unsigned int x2_addr, int out_addr, unsigned int nbytes);
void true_divide_call_f4(unsigned int x1_addr, unsigned int x2_addr, int out_addr, unsigned int nbytes);
void sin_call_f4(unsigned int x1_addr, unsigned int out_addr, unsigned int nbytes);
void sqrt_call_f4(unsigned int x1_addr, unsigned int out_addr, unsigned int nbytes);
void arctan_call_f4(unsigned int x1_addr, unsigned int out_addr, unsigned int nbytes);
void sinh_call_f4(unsigned int x1_addr, unsigned int out_addr, unsigned int nbytes);
void exp_call_f4(unsigned int x1_addr, unsigned int out_addr, unsigned int nbytes);
void log_call_f4(unsigned int x1_addr, unsigned int out_addr, unsigned int nbytes);
    """
)

with open("ufunc_call_f4.c") as f:
    ffibuilder.set_source("ufunc_call_f4_cffi", f.read())

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)
