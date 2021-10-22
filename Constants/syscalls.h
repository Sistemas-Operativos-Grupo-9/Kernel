
#pragma once

enum Syscalls {
	READ = 0,
	WRITE = 1,
	GETPID = 39,
	EXECVE = 59,
	KILL = 62,
	PROCCOUNT = 102,
	GETTIME = 103,
	PRINTREG = 104,
	MICROSLEEP = 105,
};

#define EOF -1