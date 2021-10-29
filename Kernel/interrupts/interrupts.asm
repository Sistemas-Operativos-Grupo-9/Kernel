
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


	; newLine
	; printStack
	mov rdi, %1 ; pasaje de parametro
	mov rsi, [rsp + 8 * 15] ; Instruction pointer
	; mov rsi, rsp
	call exceptionDispatcher

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
	call irqDispatcher

	popState

	; if (schedulerEnabled)
	; 	_switchContext();

	push rax
	mov al, [schedulerEnabled]
	test al, al
	pop rax
	jz ret00
	call _switchContext
	ret00:
	EOI
	iretq

;nextProcess interruption
_nextProcess:
	cli

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

	push rax
	mov al, [schedulerEnabled]
	test al, al
	pop rax
	jz ret01
	call _switchContext
	ret01:
	; sti
	iretq

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

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
	call syscallDispatcher

	iretq



%macro newLine 0
	mov rsi, 0Ah
	call printChar
%endmacro

%macro print64bit 1
	mov rsi, %1
	mov rdx, 16
	mov rcx, 16
	call printUnsignedN
%endmacro

%macro printRegister 1
	pushState
	push %1
	call getCurrentProcess
	mov rdi, [rax + 11]
	push rdi
	mov rsi, %1%+String
	call print
	pop rdi
	push rdi
	mov rsi, equalString
	call print

	pop rdi
	pop rsi
	push rdi
	print64bit rsi
	pop rdi
	newLine
	popState
%endmacro

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0
;Invalid opcode
_exception6Handler:
	exceptionHandler 6

_printRegisters:
	printRegister rdi
	printRegister rsi
	printRegister rax
	printRegister rbx
	printRegister rcx
	printRegister rdx
	printRegister rsp
	printRegister rbp
	; printRegister rip
	printRegister r8
	printRegister r9
	printRegister r10
	printRegister r11
	printRegister r12
	printRegister r13
	printRegister r14
	printRegister r15
	ret

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