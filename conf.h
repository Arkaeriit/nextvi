/* Header to include the content of the configuration file */
#ifndef CONF_H
#define CONF_H

/* map file names to file types */
struct filetype {
	char *ft;		/* file type */
	char *pat;		/* file name pattern */
};
extern struct filetype fts[];
extern int ftslen;
/* syntax highlighting patterns */
struct highlight {
	char *ft;		/* the filetype of this pattern */
	char *pat;		/* regular expression */
	int att[16];		/* attributes of the matched groups */
	signed char end[16];	/* the group ending this pattern;
				if set on multi-line the block emits all other matches in rset
				else defines hl continuation for the group:
				positive value - continue at rm_so
				zero (default) - continue at rm_eo
				negative value - continue at sp+1 */
	char blkend;		/* the ending group for multi-line patterns */
	char func;		/* if func > 0 some function will use this hl based on this id */
};
extern struct highlight hls[];
extern int hlslen;
/* direction context patterns; specifies the direction of a whole line */
struct dircontext {
	int dir;
	char *pat;
};
extern struct dircontext dctxs[];
extern int dctxlen;
/* direction marks; the direction of a few words in a line */
struct dirmark {
	int ctx;	/* the direction context for this mark; 0 means any */
	int dir;	/* the direction of the matched text */
	int grp;	/* the nested subgroup; 0 means no groups */
	char *pat;
};
extern struct dirmark dmarks[];
extern int dmarkslen;
/* character placeholders */
struct placeholder {
	int cp;		/* the source character codepoint */
	char *d;	/* the placeholder */
	int wid;	/* the width of the placeholder */
};
extern struct placeholder placeholders[];
extern int placeholderslen;
int conf_hlrev(void);
int conf_mode(void);
char **conf_kmap(int id);
int conf_kmapfind(char *name);
char *conf_digraph(int c1, int c2);

#endif

