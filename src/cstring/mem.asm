BITS 64

global memcpy
global memmove
global memset

memmove: ; :p, probably not right
memcpy:
    mov rcx, rdx
    cld
    rep movsb
    mov rax, rdi
    ret

memset:
    mov rax, rsi
    mov rcx, rdx
    cld
    rep stosb
    mov rax, rdi
    ret
