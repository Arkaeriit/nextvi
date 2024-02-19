#ifndef TERM_H
#define TERM_H

extern sbuf *term_sbuf;
extern int term_record;
extern int xcols, xrows;
extern unsigned int ibuf_pos, ibuf_cnt, icmd_pos;
void term_write(const char* data, size_t size);
void term_init(void);
void term_done(void);
void term_clean(void);
void term_suspend(void);
void term_out(char *s);
void term_chr(int ch);
void term_pos(int r, int c);
void term_kill(void);
void term_room(int n);
int term_rows(void);
int term_cols(void);
int term_read(void);
void term_commit(void);
char *term_att(int att);
void term_push(char *s, unsigned int n);
char *term_cmd(int *n);
#define term_exec(s, n, precode, postcode) \
{ \
	int pbuf_cnt = ibuf_cnt; \
	int pbuf_pos = ibuf_pos; \
	ibuf_pos = pbuf_cnt; \
	precode \
	term_push(s, n); \
	postcode \
	vi(0); \
	ibuf_cnt = pbuf_cnt; \
	ibuf_pos = pbuf_pos; \
	xquit = 0; \
} \

/* process management */
char *cmd_pipe(char *cmd, char *ibuf, int oproc);
int cmd_exec(char *cmd);
char *xgetenv(char* q[]);

#define TK_CTL(x)	((x) & 037)
#define TK_INT(c)	((c) < 0 || (c) == TK_ESC || (c) == TK_CTL('c'))
#define TK_ESC		(TK_CTL('['))

#endif

