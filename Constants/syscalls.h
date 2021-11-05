
#pragma once

enum Syscalls {
	READ = 0,
	WRITE = 1,
	GETPID = 39,
	EXECVE = 59,
	KILL = 62,
	PROCCOUNT = 102,
	GETTIME = 103,
	GETREGISTERS = 104,
	MICROSLEEP = 105,
	MILLIS = 106,
	SETGRAPHIC = 107,
	DRAWFIGURE = 108,
	SETFOREGROUND = 109,
	GETWINDOWINFO = 110,
	DRAWBITMAP,
	FLIP,
};

enum Figures {
	CIRCLE = 0,
	RECTANGLE,
	TEXT,
	LINE,
};

#define EOF -1
#define TIMEOUT -2