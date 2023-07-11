EXTERN __imp_memcpy :PROC
.CODE

; import memcpy
;

applyF PROC
; rcx function, rdx argv, r8 argc
    cmp r8, 4
    ja _applyFStackArgs
    mov rax, rcx
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
; rax function, rdx argv_last, r8 argc
    push rbp ; save rbp
    mov rbp, rsp
    sub rsp, 18h ; shadow space (3 variable)
    
    mov [rbp-8h], rcx    ; 1st var = function
    mov [rbp-10h], rdx ; 2nd var = argv
    lea r8, [r8*8-20h]    ; size: (argc - 4) * 8
    add rdx, 20h          ; src: (longlong)argv + 4
    lea rcx, [rsp-8h]    
    sub rcx, r8           ; dest: rsp - size - 8(reserve ret addr)
    call qword ptr [__imp_memcpy]

    lea rsp, [rax-20h] ; rsp = dest - (4*8)
    mov rcx, [rbp-10h]  ; retain argv (2nd var)

    mov r9, qword ptr [rcx + 18h]
    mov r8, qword ptr [rcx + 10h]
    mov rdx, qword ptr [rcx + 8h]
    mov rcx, qword ptr [rcx]

    call qword ptr [rbp-8h]

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