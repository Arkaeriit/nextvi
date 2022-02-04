#ifndef LBUF_H
#define LBUF_H

struct lbuf *lbuf_make(void);
void lbuf_free(struct lbuf *lbuf);
int lbuf_rd(struct lbuf *lbuf, int fd, int beg, int end);
int lbuf_wr(struct lbuf *lbuf, int fd, int beg, int end);
void lbuf_edit(struct lbuf *lbuf, char *s, int beg, int end);
char *lbuf_cp(struct lbuf *lbuf, int beg, int end);
char *lbuf_get(struct lbuf *lbuf, int pos);
char **lbuf_buf(struct lbuf *lb);
int lbuf_len(struct lbuf *lbuf);
int lbuf_opt(struct lbuf *lb, char *buf, int pos, int n_del);
void lbuf_mark(struct lbuf *lbuf, int mark, int pos, int off);
int lbuf_jump(struct lbuf *lbuf, int mark, int *pos, int *off);
int lbuf_undo(struct lbuf *lbuf);
int lbuf_redo(struct lbuf *lbuf);
int lbuf_modified(struct lbuf *lb);
void lbuf_saved(struct lbuf *lb, int clear);
int lbuf_indents(struct lbuf *lb, int r);
int lbuf_eol(struct lbuf *lb, int r);
void lbuf_globset(struct lbuf *lb, int pos, int dep);
int lbuf_globget(struct lbuf *lb, int pos, int dep);
int lbuf_findchar(struct lbuf *lb, char *cs, int cmd, int n, int *r, int *o);
int lbuf_search(struct lbuf *lb, rset *re, int dir, int *r, int *o, int *len);
/* motions */
int lbuf_paragraphbeg(struct lbuf *lb, int dir, int *row, int *off);
int lbuf_sectionbeg(struct lbuf *lb, int dir, int *row, int *off);
int lbuf_wordbeg(struct lbuf *lb, int big, int dir, int *row, int *off);
int lbuf_wordend(struct lbuf *lb, int big, int dir, int *row, int *off);
int lbuf_pair(struct lbuf *lb, int *row, int *off);

#endif

