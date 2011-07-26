#ifndef GLOBAL_H
#define GLOBAL_H

struct settings
{
	int tabstop;
	int showtabs;
	int list;
	int autoindent;
	int colour;
	int textwidth;
	int fsync;
};

extern struct settings global_settings;

extern int global_running;

#endif
