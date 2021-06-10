
extern enqueueItem
extern dequeueItem
extern printChar
extern schedulerEnabled
extern readyQueue
extern getCurrentProcess
extern PID
extern getProcessPID
extern getLength

%macro pushState 0
	pushfq
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
	popfq
%endmacro

global _startScheduler
_startScheduler:
	call _killAndNextProcess


global _switchContext
global _killAndNextProcess
_switchContext:
    pushState
	cli
	mov r12, [rsp + 8*(16)]
    
    call getCurrentProcess
    mov BYTE [rax + 8], 1 ; set initialized to true
    mov [rax + 0], rsp ; save rsp in process struct
    ; mov rdi, 1
    ; mov rsi, 'a'
    ; call printChar
    mov rdi, readyQueue
	mov rsi, rax
	call enqueueItem
	jmp afterKill

_killAndNextProcess:
	cli
	mov r12, iretq
	mov byte [schedulerEnabled], 1
afterKill:
	; get process to resume
    mov rdi, readyQueue
checkQueueNotEmpty:
	call getLength
	test rax, rax
	jz checkQueueNotEmpty
    call dequeueItem
    mov rsp, [rax + 0]
    mov BYTE bl, [rax + 8]  ; if initialized is true, restore from stack
	mov rdi, rax
	call getProcessPID
	mov [PID], rax
    test bl, bl
    jz after_restore

	mov [rsp + 8*(16)], r12
    popState
	jmp ret
    after_restore:

	mov [rsp], r12
ret:
	; sti
    ret

iretq:
	iretq