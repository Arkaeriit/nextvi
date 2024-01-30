#ifndef NEXTVI_REGEX_H
#define NEXTVI_REGEX_H

/* regex.c regular expression sets */
#define NEXTVI_REG_ICASE	0x01
#define NEXTVI_REG_NEWLINE	0x02 /* Unlike posix, controls termination by '\n' */
#define NEXTVI_REG_NOTBOL	0x04
#define NEXTVI_REG_NOTEOL	0x08
typedef struct {
	char *rm_so;
	char *rm_eo;
} nextvi_regmatch_t;
typedef struct {
	int unilen;
	int len;
	int sub;
	int presub;
	int splits;
	int flg;
	int insts[];
} rcode;
/* regular expression set */
typedef struct {
	rcode *regex;		/* the combined regular expression */
	int n;			/* number of regular expressions in this set */
	int *grp;		/* the group assigned to each subgroup */
	int *setgrpcnt;		/* number of groups in each regular expression */
	int grpcnt;		/* group count */
} rset;
rset *rset_make(int n, char **pat, int flg);
int rset_find(rset *re, char *s, int n, int *grps, int flg);
void rset_free(rset *re);
char *re_read(char **src);

#endif

