
extern enqueueItem
extern dequeueItem
extern printChar
extern schedulerEnabled
extern readyQueue
extern waitingQueue
extern getCurrentProcess
extern PID
extern getProcessPID
extern getLength
extern haltProcess

%macro pushState 0
	push rdi
	push rsi
	pushfq
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
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
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
	popfq
	pop rsi
	pop rdi
%endmacro

global _startScheduler
_startScheduler:
	cli
	mov byte [schedulerEnabled], 1
	call _killAndNextProcess ; We "kill" this process and start processing the queue
	iretq

global _yield
_yield:
	int 0x81
	ret


; extern processReturned
extern doSwitch
; extern enqueueHalt
; extern processes
global _switchContext
_switchContext:
	pushState
	mov r12, [rsp + (8 * 16)] ; get return pointer
	mov rdi, 1
	mov rsi, rsp
	call doSwitch
	mov rsp, rax
	mov [rsp + (8 * 16)], r12
	popState
	ret


global _killAndNextProcess
_killAndNextProcess:
	pushState
	mov r12, [rsp + (8 * 16)]
	mov rdi, 0
	mov rsi, rsp
	call doSwitch
	mov rsp, rax
	mov [rsp + (8 * 16)], r12
	popState
	ret