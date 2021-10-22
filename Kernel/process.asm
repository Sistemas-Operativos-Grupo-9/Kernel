
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
	cli
	mov byte [schedulerEnabled], 1
	call _killAndNextProcess ; We "kill" this process and start processing the queue


extern processReturned
extern enqueueHalt
extern processes
global _switchContext
global _killAndNextProcess
_switchContext:
    pushState
	cli
	mov r12, [rsp + 8*(16)]
    
    call getCurrentProcess
    mov BYTE [rax + 8], 1 ; set initialized to true
    mov [rax + 0], rsp ; save rsp in process struct
	mov dl, [rax + 9] ; get "toKill" flag
	test dl, dl
	jz dontKill
	call processReturned
dontKill:

	mov rdi, rax
	call getProcessPID
	cmp rax, 0 ; Check if process is 0 (halt process)
	mov rax, rdi
	je afterKill

	mov dl, [rax + 10] ; get "waiting" flag
	test dl, dl
	jz toNext
	
toWaiting:
	mov rdi, waitingQueue
	jmp enqueue
toNext:
	mov rdi, readyQueue
	jmp enqueue

enqueue:
	mov rsi, rax
	call enqueueItem
	jmp afterKill

_killAndNextProcess: ; call here for not pushing the process to the queue
	cli
	mov r12, iretq
afterKill:
	; get process to resume
    mov rdi, readyQueue
	jmp checkQueueNotEmpty
checkQueueNotEmpty:
	call getLength
	test rax, rax
	jnz queueNotEmpty
	call enqueueHalt ; if ready queue is empty, push halt process
queueNotEmpty:
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
	; Pop argv and argc from stack
	pop rdi
	pop rsi
	mov [rsp], r12
ret:
	; sti
    ret

iretq:
	iretq