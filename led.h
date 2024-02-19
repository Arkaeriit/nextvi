#ifndef LED_H
#define LED_H

int dstrlen(const char *s, char delim);
char *led_prompt(const char *pref, char *post, char *insert, int *kmap);
sbuf *led_input(char *pref, char *post, int *kmap, int row);
void led_render(char *s0, int row, int cbeg, int cend);
#define led_print(msg, row) led_render(msg, row, xleft, xleft + xcols)
#define led_reprint(msg, row) { rstate->ren_laststr = NULL; led_print(msg, row); }
void led_printmsg(char *s, int row);
char *led_read(int *kmap, int c);
int led_pos(char *s, int pos);
void led_done(void);

int dstrlen(const char *s, char delim);

#endif

