#ifndef DEBUG_H
#define DEBUG_H

enum {
	DEBUG_BUF   = 1 << 0,
	DEBUG_LEX   = 1 << 1,
	DEBUG_ACT   = 1 << 2
};

extern unsigned debug;

#endif

