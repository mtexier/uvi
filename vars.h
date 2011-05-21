#ifndef VARS_H
#define VARS_H

enum vartype
{
	VARS_READONLY,
	VARS_MODIFIED,
	VARS_EOL,
	VARS_TABSTOP,
	VARS_SHOWTABS,
	VARS_AUTOINDENT,
#if VIEW_COLOUR
	VARS_COLOUR,
#endif
	VARS_UNKNOWN
} vars_gettype(const char *);

int vars_set(enum vartype, buffer_t *, int);

int vars_isbuffervar(enum vartype);
int vars_isbool(enum vartype);

int *vars_bufferget(enum vartype, buffer_t *);
int *vars_settingget(enum vartype);
int *vars_get(enum vartype, buffer_t *);


const char *vars_tostring(enum vartype);

#endif
