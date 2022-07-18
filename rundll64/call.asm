EXTERN __imp_strtoll :PROC
.code

callF PROC ; rcx = function pointer, rdx = argc, r8 = argv
; --------------- FUNCTION PREPARE START
    push rbp ; save rbp
    push r9 ; save r9
    push r10 ; save r10
    push r11 ; save r11
    mov rbp, rsp ; move stack frame to base address
    sub rsp, 20h ; shadow space
; --------------- FUNCTION PREPARE END

    mov r10, rdx ; r10 = argc
    mov r11, r8 ; r11 = argv

    mov qword ptr [rbp], rcx ; save function pointer
    mov qword ptr [rbp+08h], r10 ; save argc
    mov qword ptr [rbp+10h], r11 ; save argv
ToNumLoop:
    cmp r10, 0
    jz ToNumLoopEnd
    mov rcx, qword ptr [r11] ; first arg
    mov qword ptr [rbp+18h], 0 ; second arg to nullptr
    lea rdx, [rbp+18h] ; second arg
    mov r8, 0Ah ; third arg
    call qword ptr [__imp_strtoll] ; strtoll( 
    mov rcx, qword ptr [rbp+18h]
    cmp byte ptr [rcx], 0
    jnz NotNum
    mov qword ptr [r11], rax
NotNum:
    dec r10 ; decrement argc
    add r11, 08h ; increment argv
    jmp ToNumLoop
ToNumLoopEnd:
    mov r10, qword ptr [rbp+08h] ; reset argc
    mov r11, qword ptr [rbp+10h] ; reset argv

    cmp r10, 0 ; push first arg
    jz ArgPushEnd
    mov rcx, qword ptr [r11]
    add r11, 08h
    dec r10

    cmp r10, 0 ; push second arg
    jz ArgPushEnd
    mov rdx, qword ptr [r11]
    add r11, 08h
    dec r10

    cmp r10, 0 ; push third arg
    jz ArgPushEnd
    mov r8, qword ptr [r11]
    add r11, 08h
    dec r10

    cmp r10, 0 ; push fourth arg
    jz ArgPushEnd
    mov r9, qword ptr [r11]
    add r11, 08h
    dec r10

    lea rax, [r10-1] ; move argc to rax
    shl rax, 3 ; multiply by 8 (pointer size)
    add r11, rax ; add argc * 8 to argv
    mov qword ptr [rbp+08h], rsp
PushExtraArgsLoop:
    cmp r10, 0 ; push extra args
    jz ArgPushEnd
    push qword ptr [r11]
    sub r11, 08h ; decrement argv
    dec r10 ; decrement argc
    jmp PushExtraArgsLoop
ArgPushEnd:
    sub rsp, 20h

    call qword ptr [rbp] ; call function

    mov rsp, qword ptr [rbp+08h]

; --------------- FUNCTION ENDING
    mov rsp, rbp ; reset rsp
    pop r11 ; reset r11
    pop r10 ; reset r10
    pop r9 ; reset r11
    pop rbp ; reset rbp
    ret
callF ENDP

END