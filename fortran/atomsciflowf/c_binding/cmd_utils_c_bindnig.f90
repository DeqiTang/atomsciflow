module cmd_utils_c_binding
    use iso_c_binding
    use c_f_string_c_binding, only : c_f_string
    use cmd_utils, only : log_cmd_start 

    implicit none
    
    contains 
    
    subroutine c_log_cmd_start(cmd) bind(c)
        type(c_ptr), target, intent(in) :: cmd
        character(len=128) :: cmd_f
        
        call c_f_string(c_loc(cmd), cmd_f)

        call log_cmd_start(cmd_f)
    end subroutine c_log_cmd_start
end module cmd_utils_c_binding
