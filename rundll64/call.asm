EXTERN __imp_memcpy :PROC
.CODE

; import memcpy
;

applyF PROC
; rcx function, rdx argv, r8 argc
    mov rax, rcx
    cmp r8, 4
    ja _applyFStackArgs
    mov rcx, rdx ; maybe remove
    mov r8, qword ptr [r8 * 8 + OFFSET arg_jumps]
    jmp r8

args_0::
    jmp rax

args_1::
    mov rcx, qword ptr [rcx]
    jmp rax

args_2::
    mov rdx, qword ptr [rcx + 8h]
    mov rcx, qword ptr [rcx]
    jmp rax

args_3::
    mov r8, qword ptr [rcx + 10h]
    mov rdx, qword ptr [rcx + 8h]
    mov rcx, qword ptr [rcx]
    jmp rax

args_4::
    mov r9, qword ptr [rcx + 18h]
    mov r8, qword ptr [rcx + 10h]
    mov rdx, qword ptr [rcx + 8h]
    mov rcx, qword ptr [rcx]
    jmp rax
applyF ENDP

_applyFStackArgs PROC
; rax function, rdx argv, r8 argc
    push rbp ; save rbp
    mov rbp, rsp

    sub r8, 4h          ; argc -= 4
    mov rcx, r8         ; repeat count = argc - 4
    shl r8, 3           ; argc *= 8
    lea rsi, [rdx+20h]  ; src: argv + (4*8)
    sub rsp, r8         ; rsp -= argc
    mov rdi, rsp        ; dest: rsp
    rep movsq

    sub rsp, 20h        ; reserve 4 arguments (4*8)

    mov r9, qword ptr [rdx + 18h]
    mov r8, qword ptr [rdx + 10h]
    mov rcx, qword ptr [rdx]
    mov rdx, qword ptr [rdx + 8h]

    call rax

    mov rsp, rbp
    pop rbp ; retain rbp
    ret

_applyFStackArgs ENDP

.DATA

arg_jumps:
dq OFFSET args_0
dq OFFSET args_1
dq OFFSET args_2
dq OFFSET args_3
dq OFFSET args_4

END