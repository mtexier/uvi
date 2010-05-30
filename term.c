#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "util/list.h"
#include "range.h"
#include "buffer.h"
#include "term.h"
#include "command.h"

#include "config.h"

#define IN_SIZE 256

static buffer_t *buffer;
static char saved = 1;

static int	qfunc(const char *);
static void pfunc(const char *, ...);
static enum gret gfunc(char *, int);
static void wrongfunc(void);
static void shellout(const char *);

static void wrongfunc()
{
	fputs("?\n", stdout);
}

static enum gret gfunc(char *s, int i)
{
	char *ret = fgets(s, i, stdin);
	if(!ret)
		return g_EOF;

	if(!strcmp(s, ".\n"))
		return g_EOF;

	return g_CONTINUE;
}

static int qfunc(const char *s)
{
	int ans;
	fputs(s, stdout);

	ans = getchar();

	if(ans == EOF){
		putchar('\n');
		return 0;
	}else if(ans != '\n')
		/* swallow the rest of the line */
		do
			switch(getchar()){
				case EOF:
				case '\n':
					goto getchar_break;
			}
		while(1);
getchar_break:

	return ans == '\n' || tolower(ans) == 'y';
}

static void pfunc(const char *s, ...)
{
	va_list l;
	va_start(l, s);
	vprintf(s, l);
	va_end(l);
	putchar('\n');
}

static void shellout(const char *cmd)
{
	int ret = system(cmd);
	if(ret == -1)
		perror("system()");
	else
		printf("\"%s\" returned %d\n", cmd, WEXITSTATUS(ret));
}


int term_main(const char *filename, char readonly)
{
	char in[IN_SIZE], hadeof = 0;
	int curline = 0;

  if(!(buffer = command_readfile(filename, &saved, pfunc))){
    fprintf(stderr, PROG_NAME": %s: ", filename);
    perror(NULL);
    return 1;
  }

	do{
		char *s;

		if(!fgets(in, IN_SIZE, stdin)){
			if(hadeof){
				if(saved || hadeof >= 2)
					break;
				else
					puts("not saved");
			}else
				wrongfunc();
			hadeof++;
			continue;
		}

		hadeof = 0;
		s = strchr(in, '\n');
		if(s)
			*s = '\0';

		if(!command_run(in,
					&saved, &readonly,
					&curline, buffer,
					&wrongfunc, &pfunc,
					&gfunc, &qfunc, &shellout))
			break;
	}while(1);

	buffer_free(buffer);

	return 0;
}
