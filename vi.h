#ifndef VI_H
#define VI_H

/*
Nextvi main header
==================
The purpose of this file is to provide high level overview
of entire Nextvi. Due to absence of any build system some of
these definitions may not be required to successfully compile
Nextvi. They are kept here for your benefit and organization.
If something is listed here, it must be used across multiple
files and thus is never static.
*/
#include "helper.h"
#include "nextvi_regex.h"

/* main functions */
void vi(int init);

char *vi_regget(int c, int *lnmode);
void vi_regput(int c, const char *s, int lnmode);
char *itoa(int n, char s[]);
/* file system */
void dir_calc(char *cur_dir);
#define mdir_calc(path) { char buf[4096]; strcpy(buf, path); dir_calc(buf); }
/* global variables */
extern sbuf *xacreg;
extern rset *fsincl;
extern char *fslink;
extern char *fs_exdir;
extern int fstlen;
extern int fspos;
extern int fscount;
extern int vi_hidch;
extern int vi_insmov;

#endif

