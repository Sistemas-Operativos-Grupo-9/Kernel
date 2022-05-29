
#pragma once

enum Syscalls {
	READ = 0,
	WRITE = 1,

	SEMPOST = 10,
	SEMWAIT,
	SEMINIT,
	SEMOPEN,
	SEMCLOSE,
	SEMPRINTLIST,

	PIPEINIT = 20,
	PIPEPRINTLIST,

	GETPID = 39,
	EXEC,
	EXECVE = 59,
	FORK,
	WAITPID,
	KILL = 62,
	PROCCOUNT,
	GETPROCS,
	GETTIME,
	GETREGISTERS,
	MICROSLEEP,
	MILLIS,
	SETGRAPHIC,
	DRAWFIGURE,
	SETFOREGROUND,
	GETWINDOWINFO,
	DRAWBITMAP,
	FLIP,
	SWITCHTODESKTOP,

	MALLOC = 100,
	FREE,
	GETMEMORYSTATE,
};

enum Figures {
	CIRCLE = 0,
	RECTANGLE,
	TEXT,
	LINE,
};

#define EOF -1
#define TIMEOUT -2
