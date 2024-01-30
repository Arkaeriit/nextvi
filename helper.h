#ifndef HELPER_H
#define HELPER_H

/* System headers */
#if NEXTVI_WITH_COSMO
#include "cosmopolitan.h"
#else
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <poll.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#endif

/* helper macros */
#define LEN(a)		(int)(sizeof(a) / sizeof((a)[0]))
#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) < (b) ? (b) : (a))
/* for debug; printf() but to file */
#define p(s, ...)\
	{FILE *f = fopen("file", "a");\
	fprintf(f, s, ##__VA_ARGS__);\
	fclose(f);}\

/* sbuf string buffer, variable-sized string */
#define SBUFSZ		128
#define ALIGN(n, a)	(((n) + (a) - 1) & ~((a) - 1))
#define NEXTSZ(o, r)	ALIGN(MAX((o) * 2, (o) + (r)), SBUFSZ)
typedef struct sbuf {
	char *s;	/* allocated buffer */
	int s_n;	/* length of the string stored in s[] */
	int s_sz;	/* size of memory allocated for s[] */
} sbuf;

#define sbuf_extend(sb, newsz) \
{ \
	char *s = sb->s; \
	sb->s_sz = newsz; \
	sb->s = malloc(sb->s_sz); \
	memcpy(sb->s, s, sb->s_n); \
	free(s); \
} \

#define sbuf_make(sb, newsz) \
{ \
	sb = malloc(sizeof(*sb)); \
	sb->s_sz = newsz; \
	sb->s = malloc(newsz); \
	sb->s_n = 0; \
} \

#define sbuf_chr(sb, c) \
{ \
	if (sb->s_n + 1 == sb->s_sz) \
		sbuf_extend(sb, NEXTSZ(sb->s_sz, 1)) \
	sb->s[sb->s_n++] = c; \
} \

#define sbuf_(sb, x, len, func) \
if (sb->s_n + len >= sb->s_sz) \
	sbuf_extend(sb, NEXTSZ(sb->s_sz, len + 1)) \
mem##func(sb->s + sb->s_n, x, len); \
sb->s_n += len; \

#define sbuf_free(sb) { free(sb->s); free(sb); }
#define sbuf_set(sb, ch, len) { sbuf_(sb, ch, len, set) }
#define sbuf_mem(sb, s, len) { sbuf_(sb, s, len, cpy) }
#define sbuf_str(sb, s) { const char *p = s; while(*p) sbuf_chr(sb, *p++) }
#define sbuf_cut(sb, len) { sb->s_n = len; }
/* sbuf functions that NULL terminate strings */
#define sbuf_null(sb) { sb->s[sb->s_n] = '\0'; }
#define sbufn_done(sb) { char *s = sb->s; sbuf_null(sb) free(sb); return s; }
#define sbufn_make(sb, newsz) { sbuf_make(sb, newsz) sbuf_null(sb) }
#define sbufn_set(sb, ch, len) { sbuf_set(sb, ch, len) sbuf_null(sb) }
#define sbufn_mem(sb, s, len) { sbuf_mem(sb, s, len) sbuf_null(sb) }
#define sbufn_str(sb, s) { sbuf_str(sb, s) sbuf_null(sb) }
#define sbufn_cut(sb, len) { sbuf_cut(sb, len) sbuf_null(sb) }
#define sbufn_chr(sb, c) { sbuf_chr(sb, c) sbuf_null(sb) }

#endif

