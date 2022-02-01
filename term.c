sbuf *term_sbuf;
int term_record;
static int rows, cols;
static struct termios termios;

void term_init(void)
{
	struct winsize win;
	struct termios newtermios;
	sbufn_make(term_sbuf, 2048)
	tcgetattr(0, &termios);
	newtermios = termios;
	newtermios.c_lflag &= ~(ICANON | ISIG);
	newtermios.c_lflag &= ~ECHO;
	tcsetattr(0, TCSAFLUSH, &newtermios);
	if (getenv("LINES"))
		rows = atoi(getenv("LINES"));
	if (getenv("COLUMNS"))
		cols = atoi(getenv("COLUMNS"));
	if (!ioctl(0, TIOCGWINSZ, &win)) {
		cols = win.ws_col;
		rows = win.ws_row;
	}
	cols = cols ? cols : 80;
	rows = rows ? rows : 25;
	term_out("\33[m");
}

void term_done(void)
{
	term_commit();
	sbuf_free(term_sbuf)
	tcsetattr(0, 0, &termios);
}

void term_clean(void)
{
	write(1, "\x1b[2J", 4);	/* clear screen */
	write(1, "\x1b[H", 3);	/* cursor topleft */
}

void term_suspend(void)
{
	term_done();
	kill(getpid(), SIGSTOP);
	term_init();
}

void term_commit(void)
{
	write(1, term_sbuf->s, term_sbuf->s_n);
	sbuf_cut(term_sbuf, 0)
	term_record = 0;
}

void term_out(char *s)
{
	if (term_record)
		sbufn_str(term_sbuf, s)
	else
		write(1, s, strlen(s));
}

void term_chr(int ch)
{
	char s[4] = {ch};
	term_out(s);
}

void term_kill(void)
{
	term_out("\33[K");
}

void term_room(int n)
{
	char cmd[16];
	if (n < 0)
		sprintf(cmd, "\33[%dM", -n);
	if (n > 0)
		sprintf(cmd, "\33[%dL", n);
	if (n)
		term_out(cmd);
}

void term_pos(int r, int c)
{
	char buf[32] = "\r";
	if (c < 0)
		c = 0;
	if (c >= xcols)
		c = cols - 1;
	if (r < 0)
		sprintf(buf, "\r\33[%d%c", abs(c), c > 0 ? 'C' : 'D');
	else
		sprintf(buf, "\33[%d;%dH", r + 1, c + 1);
	term_out(buf);
}

int term_rows(void)
{
	return rows;
}

int term_cols(void)
{
	return cols;
}

static char ibuf[4096];			/* input character buffer */
static char icmd[4096];			/* read after the last term_cmd() */
unsigned int ibuf_pos, ibuf_cnt;	/* ibuf[] position and length */
unsigned int icmd_pos;			/* icmd[] position */

/* read s before reading from the terminal */
void term_push(char *s, unsigned int n)
{
	n = MIN(n, sizeof(ibuf) - ibuf_cnt);
	memcpy(ibuf + ibuf_cnt, s, n);
	ibuf_cnt += n;
}

/* return a static buffer containing inputs read since the last term_cmd() */
char *term_cmd(int *n)
{
	*n = icmd_pos;
	icmd_pos = 0;
	return icmd;
}

int term_read(void)
{
	struct pollfd ufds[1];
	int n;
	if (ibuf_pos >= ibuf_cnt) {
		ufds[0].fd = STDIN_FILENO;
		ufds[0].events = POLLIN;
		if (poll(ufds, 1, -1) <= 0)
			return -1;
		/* read a single input character */
		if ((n = read(STDIN_FILENO, ibuf, 1)) <= 0)
			return -1;
		ibuf_cnt = n;
		ibuf_pos = 0;
	}
	if (icmd_pos < sizeof(icmd))
		icmd[icmd_pos++] = (unsigned char)ibuf[ibuf_pos];
	return (unsigned char)ibuf[ibuf_pos++];
}

/* return a static string that changes text attributes to att */
char *term_att(int att)
{
	static char buf[128];
	char *s = buf;
	int fg = SYN_FG(att);
	int bg = SYN_BG(att);
	*s++ = '\x1b';
	*s++ = '[';
	if (att & SYN_BD)
		{*s++ = ';'; *s++ = '1';}
	if (att & SYN_IT)
		{*s++ = ';'; *s++ = '3';}
	else if (att & SYN_RV)
		{*s++ = ';'; *s++ = '7';}
	if (SYN_FGSET(att)) {
		*s++ = ';';
		if ((fg & 0xff) < 8)
			s = itoa(30 + (fg & 0xff), s);
		else
			s = itoa(fg & 0xff, (char*)memcpy(s, "38;5;", 5)+5);
	}
	if (SYN_BGSET(att)) {
		*s++ = ';';
		if ((bg & 0xff) < 8)
			s = itoa(40 + (bg & 0xff), s);
		else
			s = itoa(bg & 0xff, (char*)memcpy(s, "48;5;", 5)+5);
	}
	s[0] = 'm';
	s[1] = '\0';
	return buf;
}

