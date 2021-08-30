#!/bin/sh -e
CFLAGS="-pedantic -Wall -Wfatal-errors -std=c99 -D_POSIX_C_SOURCE=200809L $CFLAGS"
OS="$(uname)"
: ${CC:=$(command -v cc)}
: ${PREFIX:=/usr/local}
case "$OS" in *BSD*) CFLAGS="$CFLAGS -D_BSD_SOURCE" ;; esac

run() {
	printf '%s\n' "$*"
	"$@"
}

install() {
	[ -x vi ] || build
	run mkdir -p "$DESTDIR$PREFIX/bin/"
	run cp -f vi "$DESTDIR$PREFIX/bin/vi"
}

build() {
	run "$CC" "vi.c" $CFLAGS -o vi
}

"${@:-build}"
