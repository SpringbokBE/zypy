import struct
from os.path import isabs

import numpy as np
from numpy import *
from pynq import allocate
from pynq.buffer import PynqBuffer

###############################################################################
###############################################################################

class ZyPy():

    _current_bitfile_name = None

###############################################################################

    """
    Glue the accelerable Python methods to their actual hardware drivers.
    """

    def add_call_f4_cffi(x1, x2, out):
        calc.add_call_f4(x1.physical_address, x2.physical_address, out.physical_address, x1.nbytes)

    def add_call_i4_cffi(x1, x2, out):
        calc.add_call_i4(x1.physical_address, x2.physical_address, out.physical_address, x1.nbytes)

    def subtract_call_f4_cffi(x1, x2, out):
        calc.subtract_call_f4(x1.physical_address, x2.physical_address, out.physical_address, x1.nbytes)

    def subtract_call_i4_cffi(x1, x2, out):
        calc.subtract_call_i4(x1.physical_address, x2.physical_address, out.physical_address, x1.nbytes)

    def multiply_call_f4_cffi(x1, x2, out):
        calc.multiply_call_f4(x1.physical_address, x2.physical_address, out.physical_address, x1.nbytes)

    def multiply_call_i4_cffi(x1, x2, out):
        calc.multiply_call_i4(x1.physical_address, x2.physical_address, out.physical_address, x1.nbytes)

    def true_divide_call_f4_cffi(x1, x2, out):
        calc.true_divide_call_f4(x1.physical_address, x2.physical_address, out.physical_address, x1.nbytes)

    def true_divide_call_i4_cffi(x1, x2, out):
        calc.true_divide_call_i4(x1.physical_address, x2.physical_address, out.physical_address, x1.nbytes)

    def sin_call_f4_cffi(x1, out):
        calc.sin_call_f4(x1.physical_address, out.physical_address, x1.nbytes)

    def sqrt_call_f4_cffi(x1, out):
        calc.sqrt_call_f4(x1.physical_address, out.physical_address, x1.nbytes)

    def sqrt_call_i4_cffi(x1, out):
        calc.sqrt_call_i4(x1.physical_address, out.physical_address, x1.nbytes)

    def arctan_call_f4_cffi(x1, out):
        calc.arctan_call_f4(x1.physical_address, out.physical_address, x1.nbytes)

    def sinh_call_f4_cffi(x1, out):
        calc.sinh_call_f4(x1.physical_address, out.physical_address, x1.nbytes)

    def exp_call_f4_cffi(x1, out):
        calc.exp_call_f4(x1.physical_address, out.physical_address, x1.nbytes)

    def exp_call_i4_cffi(x1, out):
        calc.exp_call_i4(x1.physical_address, out.physical_address, x1.nbytes)

    def log_call_f4_cffi(x1, out):
        calc.log_call_f4(x1.physical_address, out.physical_address, x1.nbytes)

    def log_call_i4_cffi(x1, out):
        calc.log_call_i4(x1.physical_address, out.physical_address, x1.nbytes)

    def filter_avg_f4_cffi(x1, out):
        calc.filter_avg_f4(x1.physical_address, out.physical_address, *x1.shape)

    def sad_reduce_all_f4_cffi(x1, x2):
        return calc.sad_reduce_all_f4(x1.physical_address, x2.physical_address, x1.nbytes)

    def add_reduce_all_f4_cffi(x1):
        return calc.add_reduce_all_f4(x1.physical_address, x1.nbytes)

    def subtract_reduce_all_f4_cffi(x1):
        return x1.flat[0] - calc.add_reduce_all_f4(x1.physical_address + 4, x1.nbytes - 4)

    def multiply_reduce_all_f4_cffi(x1):
        return calc.multiply_reduce_all_f4(x1.physical_address, x1.nbytes)

    def true_divide_reduce_all_f4_cffi(x1):
        return x1.flat[0] / calc.multiply_reduce_all_f4(x1.physical_address + 4, x1.nbytes - 4)

    def logaddexp_reduce_all_f4_cffi(x1):
        return calc.logaddexp_reduce_all_f4(x1.physical_address, x1.nbytes)

    def logaddexp2_reduce_all_f4_cffi(x1):
        return calc.logaddexp_reduce_all_f4(x1.physical_address, x1.nbytes) / np.log(2)

    def dot_f4_cffi(x1, x2):
        return calc.dot_f4(x1.physical_address, x2.physical_address, x1.nbytes)

    def sum_of_squares_f4_cffi(x1):
        return calc.sum_of_squares_f4(x1.physical_address, x1.nbytes)

    def sad_f4_cffi(x1, x2):
        return calc.sad_f4(x1.physical_address, x2.physical_address, x1.nbytes)

    def add_reduce_all_i4_cffi(x1):
        return calc.add_reduce_all_i4(x1.physical_address, x1.nbytes)

    def subtract_reduce_all_i4_cffi(x1):
        return x1.flat[0] - calc.add_reduce_all_i4(x1.physical_address + 4, x1.nbytes - 4)

    def multiply_reduce_all_i4_cffi(x1):
        return calc.multiply_reduce_all_i4(x1.physical_address, x1.nbytes)

    def dot_i4_cffi(x1, x2):
        return calc.dot_i4(x1.physical_address, x2.physical_address, x1.nbytes)

    def sum_of_squares_i4_cffi(x1):
        return calc.sum_of_squares_i4(x1.physical_address, x1.nbytes)

    def sad_i4_cffi(x1, x2):
        return calc.sad_i4(x1.physical_address, x2.physical_address, x1.nbytes)

###############################################################################

    """
    Register new hardware accelerators here.
    """

    hw_accelerators = {
        "add.__call__": {
            "add_call_f4_cffi": {
                "driver": add_call_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].dtype == args[1].dtype == np.float32 and args[0].shape == args[1].shape,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_f4.bit",
                "imports": "from lib.ufunc_call_f4_cffi import lib as calc",
            },
            "add_call_i4_cffi": {
                "driver": add_call_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].dtype == args[1].dtype == np.int32 and args[0].shape == args[1].shape,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_i4.bit",
                "imports": "from lib.ufunc_call_i4_cffi import lib as calc",
            }
        },
        "add.reduce": {
            "add_reduce_all_f4_cffi": {
                "driver": add_reduce_all_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32 and (args[0].ndim == 1 or (kwargs.get("axis", 0) == None)),
                "provides_acceleration": lambda x: x > 256,
                "provides_acceleration_reconfig": lambda x: x > np.inf,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_f4.bit",
                "imports": "from lib.ufunc_reduce_all_f4_cffi import lib as calc",
            },
            "add_reduce_all_i4_cffi": {
                "driver": add_reduce_all_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.int32 and (args[0].ndim == 1 or (kwargs.get("axis", 0) == None)),
                "provides_acceleration": lambda x: x > 256,
                "provides_acceleration_reconfig": lambda x: x > np.inf,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_i4.bit",
                "imports": "from lib.ufunc_reduce_all_i4_cffi import lib as calc",
            },
        },
        "subtract.__call__": {
            "subtract_call_f4_cffi": {
                "driver": subtract_call_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].dtype == args[1].dtype == np.float32 and args[0].shape == args[1].shape,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_f4.bit",
                "imports": "from lib.ufunc_call_f4_cffi import lib as calc",
            },
            "subtract_call_i4_cffi": {
                "driver": subtract_call_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].dtype == args[1].dtype == np.int32 and args[0].shape == args[1].shape,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_i4.bit",
                "imports": "from lib.ufunc_call_i4_cffi import lib as calc",
            }
        },
        "subtract.reduce": {
            "subtract_reduce_all_f4_cffi": {
                "driver": subtract_reduce_all_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32 and (args[0].ndim == 1 or (kwargs.get("axis", 0) == None)),
                "provides_acceleration": lambda x: x > 1024,
                "provides_acceleration_reconfig": lambda x: x > np.inf,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_f4.bit",
                "imports": "from lib.ufunc_reduce_all_f4_cffi import lib as calc",
            },
            "subtract_reduce_all_i4_cffi": {
                "driver": subtract_reduce_all_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.int32 and (args[0].ndim == 1 or (kwargs.get("axis", 0) == None)),
                "provides_acceleration": lambda x: x > 1024,
                "provides_acceleration_reconfig": lambda x: x > np.inf,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_i4.bit",
                "imports": "from lib.ufunc_reduce_all_i4_cffi import lib as calc",
            },
        },
        "multiply.__call__": {
            "multiply_call_f4_cffi": {
                "driver": multiply_call_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].dtype == args[1].dtype == np.float32 and args[0].shape == args[1].shape,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_f4.bit",
                "imports": "from lib.ufunc_call_f4_cffi import lib as calc",
            },
            "multiply_call_i4_cffi": {
                "driver": multiply_call_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].dtype == args[1].dtype == np.int32 and args[0].shape == args[1].shape,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_i4.bit",
                "imports": "from lib.ufunc_call_i4_cffi import lib as calc",
            }
        },
        "multiply.reduce": {
                "multiply_reduce_all_f4_cffi": {
                "driver": multiply_reduce_all_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32 and (args[0].ndim == 1 or (kwargs.get("axis", 0) == None)),
                "provides_acceleration": lambda x: x > 256,
                "provides_acceleration_reconfig": lambda x: x > np.inf,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_f4.bit",
                "imports": "from lib.ufunc_reduce_all_f4_cffi import lib as calc",
            },
                "multiply_reduce_all_i4_cffi": {
                "driver": multiply_reduce_all_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.int32 and (args[0].ndim == 1 or (kwargs.get("axis", 0) == None)),
                "provides_acceleration": lambda x: x > 256,
                "provides_acceleration_reconfig": lambda x: x > np.inf,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_i4.bit",
                "imports": "from lib.ufunc_reduce_all_i4_cffi import lib as calc",
            },
        },
        "true_divide.__call__": {
            "true_divide_call_f4_cffi": {
                "driver": true_divide_call_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].dtype == args[1].dtype == np.float32 and args[0].shape == args[1].shape,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_f4.bit",
                "imports": "from lib.ufunc_call_f4_cffi import lib as calc",
            },
            "true_divide_call_i4_cffi": {
                "driver": true_divide_call_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].dtype == args[1].dtype == np.int32 and args[0].shape == args[1].shape,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 262144,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_i4.bit",
                "imports": "from lib.ufunc_call_i4_cffi import lib as calc",
            }
        },
        "true_divide.reduce": {
                "true_divide_reduce_all_f4_cffi": {
                "driver": true_divide_reduce_all_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32 and (args[0].ndim == 1 or (kwargs.get("axis", 0) == None)),
                "provides_acceleration": lambda x: x > 1024,
                "provides_acceleration_reconfig": lambda x: x > np.inf,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_f4.bit",
                "imports": "from lib.ufunc_reduce_all_f4_cffi import lib as calc",
            },
        },
        "sin.__call__": {
            "sin_call_f4_cffi": {
                "driver": sin_call_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_f4.bit",
                "imports": "from lib.ufunc_call_f4_cffi import lib as calc",
            }
        },
        "sqrt.__call__": {
            "sqrt_call_f4_cffi": {
                "driver": sqrt_call_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > np.inf,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_f4.bit",
                "imports": "from lib.ufunc_call_f4_cffi import lib as calc",
            },
            "sqrt_call_i4_cffi": {
                "driver": sqrt_call_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.int32,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_i4.bit",
                "imports": "from lib.ufunc_call_i4_cffi import lib as calc",
            }
        },
        "arctan.__call__": {
            "arctan_call_f4_cffi": {
                "driver": arctan_call_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_f4.bit",
                "imports": "from lib.ufunc_call_f4_cffi import lib as calc",
            }
        },
        "sinh.__call__": {
            "sinh_call_f4_cffi": {
                "driver": sinh_call_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 262144,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_f4.bit",
                "imports": "from lib.ufunc_call_f4_cffi import lib as calc",
            }
        },
        "exp.__call__": {
            "exp_call_f4_cffi": {
                "driver": exp_call_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_f4.bit",
                "imports": "from lib.ufunc_call_f4_cffi import lib as calc",
            },
            "exp_call_i4_cffi": {
                "driver": exp_call_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.int32,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_i4.bit",
                "imports": "from lib.ufunc_call_i4_cffi import lib as calc",
            }
        },
        "log.__call__": {
            "log_call_f4_cffi": {
                "driver": log_call_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_f4.bit",
                "imports": "from lib.ufunc_call_f4_cffi import lib as calc",
            },
            "log_call_i4_cffi": {
                "driver": log_call_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.int32,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 262144,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_call_i4.bit",
                "imports": "from lib.ufunc_call_i4_cffi import lib as calc",
            }
        },
        "logaddexp.reduce": {
            "logaddexp_reduce_all_f4_cffi": {
                "driver": logaddexp_reduce_all_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32 and (args[0].ndim == 1 or (kwargs.get("axis", 0) == None)),
                "provides_acceleration": lambda x: x > 64,
                "provides_acceleration_reconfig": lambda x: x > 262144,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_f4.bit",
                "imports": "from lib.ufunc_reduce_all_f4_cffi import lib as calc",
            },
        },
        "logaddexp2.reduce": {
                "logaddexp2_reduce_all_f4_cffi": {
                "driver": logaddexp2_reduce_all_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32 and (args[0].ndim == 1 or (kwargs.get("axis", 0) == None)),
                "provides_acceleration": lambda x: x > 256,
                "provides_acceleration_reconfig": lambda x: x > 262144,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_f4.bit",
                "imports": "from lib.ufunc_reduce_all_f4_cffi import lib as calc",
            },
        },
        "filter_avg": {
            "filter_avg_f4_cffi": {
                "driver": filter_avg_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].ndim == 2 and args[0].shape[1] < 2048 and args[0].dtype == np.float32,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 262144,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/heat_eqn_f4.bit",
                "imports": "from lib.heat_eqn_f4_cffi import lib as calc",
            }
        },
        "sad": {
            "sad_f4_cffi": {
                "driver": sad_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].dtype == args[1].dtype == np.float32 and args[0].shape == args[1].shape,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_f4.bit",
                "imports": "from lib.ufunc_reduce_all_f4_cffi import lib as calc",
            },
            "sad_i4_cffi": {
                "driver": sad_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].dtype == args[1].dtype == np.int32 and args[0].shape == args[1].shape,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_i4.bit",
                "imports": "from lib.ufunc_reduce_all_i4_cffi import lib as calc",
            },
            "sad_reduce_all_f4_cffi": {
                "driver": sad_reduce_all_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].shape == args[1].shape and args[0].dtype == args[1].dtype == np.float32,
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > 262144,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/heat_eqn_f4.bit",
                "imports": "from lib.heat_eqn_f4_cffi import lib as calc",
            },
        },
        "dot": {
            "dot_f4_cffi": {
                "driver": dot_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].dtype == args[1].dtype == np.float32 and args[0].shape == args[1].shape and args[0].ndim == 1,
                "provides_acceleration": lambda x: x > 256,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_f4.bit",
                "imports": "from lib.ufunc_reduce_all_f4_cffi import lib as calc",
            },
            "dot_i4_cffi": {
                "driver": dot_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 2 and args[0].dtype == args[1].dtype == np.int32 and args[0].shape == args[1].shape and args[0].ndim == 1,
                "provides_acceleration": lambda x: x > 256,
                "provides_acceleration_reconfig": lambda x: x > 524288,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_i4.bit",
                "imports": "from lib.ufunc_reduce_all_i4_cffi import lib as calc",
            },
        },
        "sum_of_squares": {
            "sum_of_squares_f4_cffi": {
                "driver": sum_of_squares_f4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.float32 and (args[0].ndim == 1 or (kwargs.get("axis", 0) == None)),
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > np.inf,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_f4.bit",
                "imports": "from lib.ufunc_reduce_all_f4_cffi import lib as calc",
            },
            "sum_of_squares_i4_cffi": {
                "driver": sum_of_squares_i4_cffi,
                "accepts": lambda args, kwargs: len(args) == 1 and args[0].dtype == np.int32 and (args[0].ndim == 1 or (kwargs.get("axis", 0) == None)),
                "provides_acceleration": lambda x: x > 0,
                "provides_acceleration_reconfig": lambda x: x > np.inf,
                "bitfile": "/home/xilinx/jupyter_notebooks/zypy/overlays/ufunc_reduce_all_i4.bit",
                "imports": "from lib.ufunc_reduce_all_i4_cffi import lib as calc",
            },
        }
    }

###############################################################################

    """
    Bitfile parsing and downloading.
    """

    @classmethod
    def download_bitfile(cls, bitfile_name):
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
            cls._current_bitfile_name = bitfile_name

        return cls._current_bitfile_name

###############################################################################

    @classmethod
    def download_bitfile_for_method(cls, method, *args, **kwargs):
        """
        Download a bitfile that can accelerate the given method.
        """
        if method in ZyPy.hw_accelerators:
            for impl in ZyPy.hw_accelerators[method].values():
                if len(args) == 0 or impl["accepts"](args, kwargs):
                    print(f"Downloading bitfile `{impl['bitfile']}`")
                    ZyPy.download_bitfile(impl["bitfile"])
                    exec(impl["imports"], globals())
                    return cls._current_bitfile_name

###############################################################################

    """
    Override PynqBuffer ufunc mechanism.
    """

    @staticmethod
    def intercept_ufunc(self, ufunc, method, *args, **kwargs):

        MIN_INPUT_SIZE = 64

        f = {
            "reduce": ufunc.reduce,
            "accumulate": ufunc.accumulate,
            "reduceat": ufunc.reduceat,
            "outer": ufunc.outer,
            "at": ufunc.at,
            "__call__": ufunc,
        }

        name = f"{ufunc.__name__}.{method}"
        print(f"ufunc `{name}` called")
        if (hasattr(args[0], "size") and
            args[0].size > MIN_INPUT_SIZE and
            name in ZyPy.hw_accelerators):
            for impl in ZyPy.hw_accelerators[name].values():
                print(f"Checking implementation `{impl['bitfile']}`")
                if impl["accepts"](args, kwargs):
                    bitfile_loaded = (ZyPy._current_bitfile_name == impl["bitfile"])
                    print(f"Bitfile loaded = {bitfile_loaded}")
                    if(bitfile_loaded and impl["provides_acceleration"](args[0].size)):
                        pass
                    elif(not bitfile_loaded and impl["provides_acceleration_reconfig"](args[0].size)):
                        print(f"Downloading bitfile `{impl['bitfile']}`")
                        ZyPy.download_bitfile(impl["bitfile"])
                        exec(impl["imports"], globals())
                    else:
                        continue

                    print(f"`{name}` can be accelerated using driver `{impl['driver']}`")

                    if("out" in impl["driver"].__code__.co_varnames):
                        if ("out" not in kwargs):
                            out = allocate(shape=args[0].shape, dtype=args[0].dtype)
                            print(f"Allocated fresh `out` argument")
                            print("Runnning hardware accelerator!")
                            impl["driver"](*args, out)
                            return out
                        print("Runnning hardware accelerator!")
                        return impl["driver"](*args, out=kwargs["out"][0]) # For some reason the kwargs values are tuples.

                    print("Runnning hardware accelerator!")
                    return impl["driver"](*args)

        if "out" in kwargs:
            out = kwargs["out"][0]
            if isinstance(out, np.ndarray):
                print(f"Casting `out` to ndarray!")
                kwargs["out"] = out.view(np.ndarray)
        print(f"Running default")
        return f[method](*(i.view(np.ndarray) if isinstance(i, np.ndarray) else i for i in args), **kwargs)

###############################################################################

    """
    Intercept custom hardware-accelerable methods.
    """

    @staticmethod
    def intercept_custom(method, *args, **kwargs):

        MIN_INPUT_SIZE = 64

        print(f"method `{ufunc}` called")
        if (hasattr(args[0], "size") and
            args[0].size > MIN_INPUT_SIZE and
            method in ZyPy.hw_accelerators):
            for impl in ZyPy.hw_accelerators[method].values():
                print(f"Checking implementation `{impl['bitfile']}`")
                if impl["accepts"](args, kwargs):
                    bitfile_loaded = (ZyPy._current_bitfile_name == impl["bitfile"])
                    print(f"Bitfile loaded = {bitfile_loaded}")
                    if(bitfile_loaded and impl["provides_acceleration"](args[0].size)):
                        pass
                    elif(not bitfile_loaded and impl["provides_acceleration_reconfig"](args[0].size)):
                        print(f"Downloading bitfile `{impl['bitfile']}`")
                        ZyPy.download_bitfile(impl["bitfile"])
                        exec(impl["imports"], globals())
                    else:
                        continue

                    print(f"`{method}` can be accelerated using driver `{impl['driver']}`")

                    if("out" in impl["driver"].__code__.co_varnames and "out" not in kwargs):
                        out = allocate(shape=args[0].shape, dtype=args[0].dtype)
                        print(f"Allocated fresh `out` argument")
                        print("Runnning hardware accelerator (new-alloc)!")
                        impl["driver"](*args, out)
                        return out

                    print("Runnning hardware accelerator (pre-alloc)!")
                    return impl["driver"](*args, **kwargs)

###############################################################################
###############################################################################

"""
Custom hardware-accelerable methods.
"""

def sad(x1, x2):
    res = ZyPy.intercept_custom("sad", x1, x2)
    if res is not None:
        return res
    return np.sum(np.absolute(x1 - x2))

def dot(x1, x2):
    res = ZyPy.intercept_custom("dot", x1, x2)
    if res is not None:
        return res
    return np.dot(x1, x2)

def sum_of_squares(x1):
    res = ZyPy.intercept_custom("sum_of_squares", x1)
    if res is not None:
        return res
    return np.sum(x1 ** 2)

def filter_avg(x1, out=None):
    res = ZyPy.intercept_custom("filter_avg", x1, out=out)
    if res is not None:
        return res
    return NotImplemented

###############################################################################
###############################################################################

PynqBuffer.__array_ufunc__ = ZyPy.intercept_ufunc
hint = ZyPy.download_bitfile_for_method
