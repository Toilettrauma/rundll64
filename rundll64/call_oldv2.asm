EXTERN __imp_memcpy :PROC
.CODE

; import memcpy
;

applyF PROC
; rcx argv_last, rdx argc, r8 function
    mov rax, r8
    cmp dl, 4
    ja _applyFStackArgs
    mov r8, qword ptr [rdx * 8 + OFFSET arg_jumps]
    jmp r8

args_4::
    mov r9, qword ptr [rcx]
    sub rcx, 8
args_3::
    mov r8, qword ptr [rcx]
    sub rcx, 8
args_2::
    mov rdx, qword ptr [rcx]
    sub rcx, 8
args_1::
    mov rcx, qword ptr [rcx]
args_0::
    jmp rax
applyF ENDP

_applyFStackArgs PROC
; rcx argv_last, rdx argc, rax function
    push rbp
    mov rbp, rsp

    lea r8, [rdx*8-40]       ; size: ( argc - 4 - 1) * 8
    mov rdx, rcx             ; rdx = argv
    sub rdx, r8              ; src: argv - argc * ( sizeof(__int64) + 4 )
    lea rcx, [rsp+rdx]       ; dest: rsp (stack)
    call qword ptr [__imp_memcpy]

    mov rsp, rax
    lea rcx, [r8 - 8]
    

    mov r9, qword ptr [rcx]
    sub rcx, 8

    mov r8, qword ptr [rcx]
    sub rcx, 8

    mov rdx, qword ptr [rcx]
    sub rcx, 8

    mov rcx, qword ptr [rcx]

    call rax

    mov rsp, rbp
    pop rbp
    ret

_applyFStackArgs ENDP

applyF_AVX PROC
    ret
applyF_AVX ENDP

.DATA

arg_jumps:
dq OFFSET args_0
dq OFFSET args_1
dq OFFSET args_2
dq OFFSET args_3
dq OFFSET args_4

END