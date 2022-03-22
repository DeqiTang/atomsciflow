
cdef extern:
    void c_log_cmd_start(char* cmd)

def log_cmd_start(str cmd):
    py_byte_string = cmd.encode("UTF-8")
    cdef char* cmd_c = py_byte_string

    c_log_cmd_start(cmd_c)

