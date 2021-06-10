
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
GLOBAL _exception13Handler
GLOBAL _syscallHandler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN syscallDispatcher

EXTERN schedulerEnabled
EXTERN _nextProcess
GLOBAL ret00

; EXTERN printUnsignedN
; EXTERN printChar
; EXTERN print
; GLOBAL _printRegisters

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


; %macro newLine 0
; 	mov rdi, 0Ah
; 	call printChar
; %endmacro

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

; %macro print64bit 1
; 	mov rdi, %1
; 	mov rsi, 16
; 	mov rdx, 16
; 	call printUnsignedN
; %endmacro

; %macro printStackValue 1
; 	print64bit [esp + %1 * 8]
; 	newLine
; %endmacro

; %macro printStack 0
; 	printStackValue 0
; 	printStackValue 1
; 	printStackValue 2
; 	printStackValue 3
; 	printStackValue 4
; 	printStackValue 5
; 	printStackValue 6
; 	printStackValue 7
; %endmacro

%macro exceptionHandler 1
	pushState

	; newLine
	; printStack
	mov rdi, %1 ; pasaje de parametro
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
	; 	_nextProcess();

	push rax
	mov al, [schedulerEnabled]
	test al, al
	pop rax
	jz ret00
	call _nextProcess
	ret00:

	EOI
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


; GLOBAL _exception0Handler
; GLOBAL _exception6Handler
; GLOBAL _exception13Handler
; ;Zero Division Exception
; _exception0Handler:
; 	exceptionHandler 0
; ;Invalid Opcode
; _exception6Handler:
; 	exceptionHandler 6
; ;General Protection Exception
; _exception13Handler:
; 	exceptionHandler 13

; GLOBAL _generalExceptionHandler
; _generalExceptionHandler:
; 	exceptionHandler -1

; %macro exceptionHandlerMaster 1
; GLOBAL _exception%+%1%+Handler
; _exception%+%1%+Handler:
; 	exceptionHandler %1
; %endmacro

; exceptionHandlerMaster 0
; exceptionHandlerMaster 1
; exceptionHandlerMaster 2
; exceptionHandlerMaster 3
; exceptionHandlerMaster 4
; exceptionHandlerMaster 5
; exceptionHandlerMaster 6
; exceptionHandlerMaster 7
; exceptionHandlerMaster 8
; exceptionHandlerMaster 9
; exceptionHandlerMaster 10
; exceptionHandlerMaster 11
; exceptionHandlerMaster 12
; exceptionHandlerMaster 13
; exceptionHandlerMaster 14
; exceptionHandlerMaster 15
; exceptionHandlerMaster 16
; exceptionHandlerMaster 17
; exceptionHandlerMaster 18
; exceptionHandlerMaster 19
; exceptionHandlerMaster 20

; %macro printRegister 1
; 	pushState
; 	push %1
; 	mov rdi, %1%+String
; 	call print
; 	mov rdi, equalString
; 	call print

; 	pop rdi
; 	print64bit rdi
; 	newLine
; 	popState
; %endmacro

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0
_exception6Handler:
	exceptionHandler 6
_exception13Handler:
	exceptionHandler 13

; _printRegisters:
; 	printRegister rdi
; 	printRegister rsi
; 	printRegister rax
; 	printRegister rbx
; 	printRegister rcx
; 	printRegister rdx
; 	printRegister rsp
; 	printRegister rbp
; 	; printRegister rip
; 	printRegister r8
; 	printRegister r9
; 	printRegister r10
; 	printRegister r11
; 	printRegister r12
; 	printRegister r13
; 	printRegister r14
; 	printRegister r15
; 	newLine
; 	ret

haltcpu:
	cli
	hlt
	ret


; SECTION .rodata
; 	equalString db " = ", 0

; 	rdiString db "RDI", 0
; 	rsiString db "RSI", 0
; 	raxString db "RAX", 0
; 	rbxString db "RBX", 0
; 	rcxString db "RCX", 0
; 	rdxString db "RDX", 0
; 	rspString db "RSP", 0
; 	rbpString db "RBP", 0
; 	ripString db "RIP", 0
; 	r8String  db "R8 ", 0
; 	r9String  db "R9 ", 0
; 	r10String  db "R10", 0
; 	r11String  db "R11", 0
; 	r12String  db "R12", 0
; 	r13String  db "R13", 0
; 	r14String  db "R14", 0
; 	r15String  db "R15", 0

SECTION .bss
	aux resq 1