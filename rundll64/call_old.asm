EXTERN __imp_strtoll :PROC
.code

invokeF PROC ; rcx = function pointer, rdx = pArg, r8 = argc, r9 = double flags
; ------- FUNCTION PREPARE
    push rbp
    push r10
    push r11
    push r12
    mov rbp, rsp
    sub rsp, 08h ; if remove this then will be ACCESS_VIOLATION_ERROR
; ------- FUNCTION PREPARE

    mov rax, rcx ; rax = function pointer
    mov r10, rdx ; r10 = pArg
    mov r11, r8 ; r11 = argc
    mov r12, r9 ; r12 = double flags

    cmp r11, 0 ; first arg
    je ArgPushEnd

    cmp byte ptr [r12], 0 ; if double move to xmm else r**
    jne ifDouble1
    mov rcx, qword ptr [r10]
    jmp ifDouble1End
ifDouble1:
    movq xmm0, qword ptr [r10]
ifDouble1End:
    add r12, 01h

    add r10, 08h
    dec r11

    cmp r11, 0 ; second arg
    je ArgPushEnd

    cmp byte ptr [r12], 0 ; if double move to xmm else r**
    jne ifDouble2
    mov rdx, qword ptr [r10]
    jmp ifDouble2End
ifDouble2:
    movq xmm1, qword ptr [r10]
ifDouble2End:
    add r12, 01h

    add r10, 08h
    dec r11

    cmp r11, 0 ; third arg
    je ArgPushEnd

    cmp byte ptr [r12], 0 ; if double move to xmm else r**
    jne ifDouble3
    mov r8, qword ptr [r10]
    jmp ifDouble3End
ifDouble3:
    movq xmm3, qword ptr [r10]
ifDouble3End:
    add r12, 01h

    add r10, 08h
    dec r11

    cmp r11, 0 ; fouth arg
    je ArgPushEnd
    
    cmp byte ptr [r12], 0 ; if double move to xmm else r**
    jne ifDouble4
    mov r9, qword ptr [r10]
    jmp ifDouble4End
ifDouble4:
    movq xmm4, qword ptr [r10]
ifDouble4End:

    add r10, 08h
    dec r11

    lea r12, [r11-1]
    shl r12, 3
    add r10, r12
ExtraArgPush:
    cmp r11, 0 ; extra args
    je ArgPushEnd
    push qword ptr [r10]
    sub r10, 08h
    dec r11
    jmp ExtraArgPush
ArgPushEnd:

    sub rsp, 20h
    call rax
    
;------ FUNCTION ENDING
    mov rsp, rbp
    pop r12
    pop r11
    pop r10
    pop rbp
    ret
;------ FUNCTION ENDING

invokeF ENDP



invokeF_new PROC
;ARG @argv_end:QWORD PTR,@argc:DWORD,@method:QWORD
    mov rax, r8

    cmp rdx, 4
    jz zero_args
    jl jump_args
    push rsp
    jmp push_more_args

push_more_args:
    cmp rdx, 4
    je @call_with_stack_fix
push_more_args_first:
    push qword ptr [rcx]
    sub rcx, 08h
    dec rdx
    jmp push_more_args ; try loopne
; -- loop end

jump_args:
    
    lea r8, [OFFSET zero_args+1]    
    shl rdx, 3                      ; mul by 8
    sub r8, rdx                     ; argc * 8 - 1 ie 7 * argc

    jmp r8
    
fouth_args:
    mov r9, qword ptr [rcx]
    sub rcx, 08h
three_args:
    mov r8, qword ptr [rcx]
    sub rcx, 08h
two_args:
    mov rdx, qword ptr [rcx]
    sub rcx, 08h
one_args:
    mov rcx, qword ptr [rcx]
    jmp zero_args               ; placeholder (3 bytes)
zero_args:

    jmp rax
    

@call_with_stack_fix PROC

    mov r9, qword ptr [rcx]
    sub rcx, 08h
    mov r8, qword ptr [rcx]
    sub rcx, 08h
    mov rdx, qword ptr [rcx]
    sub rcx, 08h
    mov rcx, qword ptr [rcx]
    
    call rax

    pop rbp
    ret

@call_with_stack_fix ENDP

invokeF_new ENDP
END