#ifndef REN_H
#define REN_H

typedef struct {
	char **ren_lastchrs;
	int *ren_lastpos;
	char *ren_laststr; 	/* to prevent redundant computations, ensure pointer uniqueness */
	int ren_lastn;
	int ren_torg; 		/* compute tab width from this position origin */
} ren_state;
extern ren_state *rstate;
void ren_done(void);
void ren_save(int nstate, int torg);
int *ren_position(char *s, char ***c, int *n);
int ren_next(char *s, int p, int dir);
int ren_eol(char *s, int dir);
int ren_pos(char *s, int off);
int ren_cursor(char *s, int pos);
int ren_noeol(char *s, int p);
int ren_off(char *s, int pos);
int ren_region(char *s, int c1, int c2, int *l1, int *l2, int closed);
char *ren_translate(char *s, char *ln);
int ren_cwid(char *s, int pos);
/* text direction */
int dir_context(char *s);
void dir_init(void);
void dir_done(void);
/* syntax highlighting */
#define SYN_BD		0x010000
#define SYN_IT		0x020000
#define SYN_RV		0x040000
#define SYN_FGMK(f)	(0x100000 | (f))
#define SYN_BGMK(b)	(0x200000 | ((b) << 8))
#define SYN_FLG		0xff0000
#define SYN_FGSET(a)	((a) & 0x1000ff)
#define SYN_BGSET(a)	((a) & 0x20ff00)
#define SYN_FG(a)	((a) & 0xff)
#define SYN_BG(a)	(((a) >> 8) & 0xff)
extern int syn_reload;
extern int syn_blockhl;
char *syn_setft(char *ft);
void syn_scdir(int scdir);
void syn_highlight(int *att, char *s, int n);
char *syn_filetype(char *path);
int syn_merge(int old, int new);
void syn_reloadft();
int syn_addhl(char *reg, int func, int reload);
void syn_init(void);
void syn_done(void);

#endif

