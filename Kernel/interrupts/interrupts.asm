
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL _syscallHandler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallDispatcher

EXTERN schedulerEnabled
EXTERN _switchContext
GLOBAL _nextProcess
GLOBAL ret00

EXTERN printUnsignedN
EXTERN printChar
EXTERN print
EXTERN getCurrentProcess
GLOBAL _printRegisters
extern startLock
extern endLock
extern lock_count

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro EOI 0 
	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	EOI

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	pushState
	
	push QWORD [rsp+15*8] ; get RIP from frame
	push QWORD [rsp+(16+3)*8] ; get RSP from frame
	push QWORD [rsp+(17+2)*8] ; get RFlags from frame

	; newLine
	; printStack
	mov rdi, %1 ; pasaje de parametro
	mov rsi, rsp ; pasaje de parametro
	call exceptionDispatcher

	add rsp, 8*3
	popState
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

; GLOBAL _nextProcess
; _nextProcess:
; 	; Create fake iretq stack
; 	sub rsp, 40
; 	push rbp
; 	mov rbp, rsp
; 	add rbp, 48

; 	mov qword [rbp-8*1], 0
; 	mov qword [rbp-8*2], rbp
; 	mov qword [rbp-8*3], 0x202
; 	mov qword [rbp-8*4], 8
; 	push rdx
; 	mov rdx, [rbp]
; 	mov qword [rbp-8*5], rdx
; 	pop rdx
	
; 	pop rbp

; 	push rax
; 	mov al, [schedulerEnabled]
; 	test al, al
; 	pop rax
; 	jz ret01
; 	call _switchContext
; 	ret01:
; 	iretq

EXTERN switchContext
;8254 Timer (Timer Tick)
_irq00Handler:
	; EOI
	; iretq
	pushState
	mov rdi, 0 ; pasaje de parametro
	; mov rsi, rsp
	sub rsp, 8
	call irqDispatcher
	add rsp, 8

	popState

	; if (schedulerEnabled)
	; 	_switchContext();

	push rax
	mov al, [schedulerEnabled]
	test al, al
	pop rax
	jz ret00
	sub rsp, 8
	call _switchContext
	add rsp, 8
	ret00:
	push rax
	EOI
	pop rax
	iretq

;nextProcess interruption
_nextProcess:

	; trap frame

	; sub rsp, 8*5
	; push rbp
	; mov rbp, rsp
	; add rbp, 8*6
	; push rdx
	; mov rdx, [rbp - 8*0]
	; mov qword [rbp - 8*1], 0
	; mov qword [rbp - 8*2], rbp
	; mov qword [rbp - 8*3], 0x202
	; mov qword [rbp - 8*4], 8
	; mov qword [rbp - 8*5], rdx
	; pop rdx
	; pop rbp

	; if (schedulerEnabled)
	; 	_switchContext();
	
	cli
	push rax
	mov al, [schedulerEnabled]
	test al, al
	pop rax
	jz ret01
	sub rsp, 8
	inc QWORD [lock_count]
	;call startLock
	call _switchContext
	dec QWORD [lock_count]
	;call endLock
	add rsp, 8
	ret01:
	; sti
	iretq

EXTERN keyboard_handler
;Keyboard
_irq01Handler:
	pushState
	push QWORD [rsp+15*8]
	push QWORD [rsp+(16+3)*8]
	push QWORD [rsp+(17+2)*8]

	mov rdi, rsp ; pasaje de parametro
	call keyboard_handler

	EOI

	add rsp, 3*8
	
	popState
	iretq
;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

_syscallHandler:
	;sti
	call syscallDispatcher
	iretq


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0
;Invalid opcode
_exception6Handler:
	exceptionHandler 6


;EXTERN registersState
;GLOBAL _storeRegisters 
;_storeRegisters:
	;mov [registersState + 0 * 8], rdi
	;mov [registersState + 1 * 8], rsi
	;mov [registersState + 2 * 8], rax
	;mov [registersState + 3 * 8], rbx
	;mov [registersState + 4 * 8], rcx
	;mov [registersState + 5 * 8], rdx
	;pop [registersState + 6 * 8] ; Save RIP (it has to be pushed to the stack)
	;mov [registersState + 7 * 8], rsp
	;mov [registersState + 8 * 8], rbp
	;mov [registersState + 9 * 8], r8
	;mov [registersState + 10 * 8], r9
	;mov [registersState + 11 * 8], r10
	;mov [registersState + 12 * 8], r11
	;mov [registersState + 13 * 8], r12
	;mov [registersState + 14 * 8], r13
	;mov [registersState + 15 * 8], r14
	;mov [registersState + 16 * 8], r15
	;pushfq
	;pop QWORD [registersState + 17 * 8]
	;ret

haltcpu:
	cli
	hlt
	ret


SECTION .rodata
	equalString db " = ", 0

	rdiString db "RDI", 0
	rsiString db "RSI", 0
	raxString db "RAX", 0
	rbxString db "RBX", 0
	rcxString db "RCX", 0
	rdxString db "RDX", 0
	rspString db "RSP", 0
	rbpString db "RBP", 0
	ripString db "RIP", 0
	r8String  db "R8 ", 0
	r9String  db "R9 ", 0
	r10String  db "R10", 0
	r11String  db "R11", 0
	r12String  db "R12", 0
	r13String  db "R13", 0
	r14String  db "R14", 0
	r15String  db "R15", 0

SECTION .bss
	aux resq 1
