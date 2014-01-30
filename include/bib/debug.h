#ifndef BIB_DEBUG_H
#define BIB_DEBUG_H

enum {
	DEBUG_BUF   = 1 << 0,
	DEBUG_LEX   = 1 << 1,
	DEBUG_ACT   = 1 << 2
};

extern unsigned debug;

#endif

