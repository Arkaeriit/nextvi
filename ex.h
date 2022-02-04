#ifndef EX_H
#define EX_H

#include "helper.h"
#include "nextvi_regex.h"

struct buf {
	char ft[32];			/* file type */
	char *path;			/* file path */
	struct lbuf *lb;
	int row, off, top;
	long mtime;			/* modification time */
	signed char td;			/* text direction */
};
extern struct buf *ex_buf;
extern struct buf *bufs;
#define EXLEN	512	/* ex line length */
#define ex_path ex_buf->path
#define ex_filetype ex_buf->ft
#define xb ex_buf->lb
void temp_open(int i, char *name, char *ft);
void temp_switch(int i);
void temp_write(int i, char *str);
void temp_done(int i);
char *temp_curstr(int i, int sub);
char *temp_get(int i, int row);
void temp_pos(int i, int row, int off, int top);
void ex(void);
int ec_setdir(const char *loc, char *cmd, char *arg);
void ex_command(const char *cmd);
char *ex_read(const char *msg);
void ex_print(char *line);
void ex_show(char *msg);
int ex_init(char **files);
void ex_bufpostfix(int i);
void ex_done(void);
int ex_krs(rset **krs, int *dir);
void ex_krsset(char *kwd, int dir);
int ex_edit(const char *path);
void ec_bufferi(int id);
void bufs_switch(int idx);
#define bufs_switchwft(idx) { bufs_switch(idx); syn_setft(ex_buf->ft); }

/* global variables */
extern int xrow;
extern int xoff;
extern int xtop;
extern int xleft;
extern int xvis;
extern int xled;
extern int xquit;
extern int xic;
extern int xai;
extern int xtd;
extern int xotd;
extern int xshape;
extern int xorder;
extern int xhl;
extern int xhll;
extern int xhlw;
extern int xhlp;
extern int xhlr;
extern int xkmap;
extern int xkmap_alt;
extern int xtabspc;
extern int xqexit;
extern int xish;
extern int xgrp;
extern int xpac;
extern int xbufcur;

#endif

