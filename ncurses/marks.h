#ifndef MARKS_H
#define MARKS_H

void mark_set(int, int, int);
char mark_get(int, int *, int *);
char mark_isset(int);

#define validmark(c) ('a' <= (c) && (c) <= 'z')

#endif
