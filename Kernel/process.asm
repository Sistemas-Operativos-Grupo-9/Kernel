
extern enqueueItem
extern dequeueItem
extern printChar
extern schedulerEnabled
extern readyQueue
extern getCurrentProcess
extern PID
extern getProcessPID

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
    mov rdi, readyQueue
    call dequeueItem
    mov rsp, [rax + 0]
	mov rdi, rax
	call getProcessPID
	mov [PID], rax
	mov byte [schedulerEnabled], 1
	add rsp, 8 ; Remove ret00 address
	pop rax ; Simulate iretq
	add rsp, 8 * 5
    push rax
	ret


global _nextProcess
_nextProcess:
    pushState
    
    call getCurrentProcess
    mov BYTE [rax + 8], 1 ; set initialized to true
    mov [rax + 0], rsp ; save rsp in process struct
    ; mov rdi, 1
    ; mov rsi, 'a'
    ; call printChar
    mov rdi, readyQueue
	mov rsi, rax
	call enqueueItem
    call dequeueItem
    mov rsp, [rax + 0]
    mov BYTE bl, [rax + 8]  ; if initialized is true, restore from stack
	mov rdi, rax
	call getProcessPID
	mov [PID], rax
    test bl, bl
    jz after_restore


    popState

    after_restore:

    ret