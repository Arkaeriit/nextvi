#ifndef UC_H
#define UC_H

extern unsigned char utf8_length[256];
/* return the length of a utf-8 character */
#define uc_len(dst, s) dst = utf8_length[(unsigned char)s[0]];
/* the unicode codepoint of the given utf-8 character */
#define uc_code(dst, s) \
dst = (unsigned char)s[0]; \
if (dst < 192); \
else if (dst < 224) \
	dst = ((dst & 0x1f) << 6) | (s[1] & 0x3f); \
else if (dst < 240) \
	dst = ((dst & 0x0f) << 12) | ((s[1] & 0x3f) << 6) | (s[2] & 0x3f); \
else if (dst < 248) \
	dst = ((dst & 0x07) << 18) | ((s[1] & 0x3f) << 12) | \
		((s[2] & 0x3f) << 6) | (s[3] & 0x3f); \
else \
	dst = 0; \

int uc_wid(int c);
int uc_slen(char *s);
char *uc_chr(char *s, int off);
int uc_off(char *s, int off);
char *uc_sub(char *s, int beg, int end);
char *uc_dup(const char *s);
int uc_isspace(char *s);
int uc_isprint(char *s);
int uc_isdigit(char *s);
int uc_isalpha(char *s);
int uc_kind(char *c);
int uc_isbell(int c);
int uc_acomb(int c);
char **uc_chop(char *s, int *n);
char *uc_next(char *s);
char *uc_prev(char *beg, char *s);
char *uc_beg(char *beg, char *s);
char *uc_end(char *s);
char *uc_shape(char *beg, char *s);
char *uc_lastline(char *s);

#endif

