
#pragma once

enum Syscalls {
    READ = 0,
    WRITE = 1,
    GETPID = 39,
    EXECVE = 59,
    PROCCOUNT = 102,
};

#define EOF -1