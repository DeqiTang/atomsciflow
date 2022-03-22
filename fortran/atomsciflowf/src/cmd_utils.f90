module cmd_utils
    implicit none

    contains

    subroutine log_cmd_start(cmd)
        implicit none
        character(len=*), intent(in) :: cmd
        character(len=72) :: cmd_centered
        integer :: len_cmd_trim, len_each_side, tmp_int

        len_cmd_trim = len_trim(cmd)
        len_each_side = (72 - len_cmd_trim) / 2
        tmp_int = len_each_side
        
        cmd_centered = trim(cmd)
        do while (tmp_int > 0) 
            cmd_centered = " " // cmd_centered
            tmp_int = tmp_int - 1
        end do    
        
        write(*, 99) "************************************************************************"
        99 format(A72)
        write(*, 100) cmd_centered
        100 format("*", A70, "*")
        write(*, 99) "************************************************************************"

    end subroutine log_cmd_start
end module cmd_utils
