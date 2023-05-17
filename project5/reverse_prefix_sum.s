# Name: Darius Kianersi
# UID: 118799810
# Username: radius

    .text

reverse_prefix_sum:
    # PROLOGUE
    subu    $sp, $sp, 8     # grow the stack by 8
    sw      $ra, 8($sp)     # push the return address
    sw      $fp, 4($sp)     # push the frame pointer
    addu    $fp, $sp, 8     # set the frame pointer

    # BODY
    lw      $t0, ($a0)      # $t0 = *arr
    bne     $t0, -1, recur  # continue function if not equal to -1
    li		$v0, 0		    # $v0 = 0
    j       ret

recur:
    subu    $sp, $sp, 4     # grow stack by 4
    sw      $a0, 4($sp)     # push arr to stack
    addu    $a0, $a0, 4     # a0 = arr+1
    jal     reverse_prefix_sum
    lw      $t0, 4($sp)     # pop arr from stack -> $t0
    addu    $sp, $sp, 4     # grow stack by 4
    lw      $t1, ($t0)      # $t1 = *arr
    addu    $t2, $v0, $t1   # r = $t2 = $v0 + *arr
    sw      $t2, ($t0)      # *arr = r
    move    $v0, $t2        # $v0 = r
    j       ret

ret:
    # EPILOGUE
    move    $sp, $fp        # set stack pointer
    lw      $ra, ($fp)      # pop return address
    lw      $fp, -4($fp)    # restore frame pointer
    j       $ra             # return