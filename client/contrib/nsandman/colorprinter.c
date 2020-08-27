/*
 * For some reason eighth grade me wrote this all as one file 
 * This is hastily separated to avoid symbol dupliction 
 * It is still not good
 */

#include "colorprinter.h"

color_t colors[] = {
	// ANSI colors
	31, 32, 33, 36, 37, 91, 92, 93, 96, 97,
	// blue
	94, 34,
	//purple
	34, 25,
	// pink
	35, 95,
	// xterm 256-color
	208, 237, 64, 89
};

size_t strlen(const char *str) {
	size_t i = 0;
	while(str[++i]);
	return i;
}

void startprint(color_t c, FILE *s) {
    #ifndef NO256
    if (c >= ANSI_OFFSET)
        fprintf(s, "\033[38;5;%dm", colors[c]);	
    else
    #endif	// !NO256
        fprintf(s, "\033[0;%dm", colors[c]);
}

int cfnputs(const char *m, color_t c, size_t n, FILE *s) {
    for(startprint(c, s); n>0 && *m!='\0'; m++, --n)
        putc(*m, s);
    return endprint(s);
}

int cfprintf(FILE *s, color_t c, const char *fmt, ...) {
    startprint(c, s);
    va_list arg;
    va_start(arg, fmt);
    int w = vfprintf(s, fmt, arg);
    va_end(arg);
    endprint(s);
    return w;
}

int cfnprintf(FILE *s, color_t c, size_t n, const char *fmt, ...) {
    char buf[n];
    va_list arg;
    va_start(arg, fmt);
    int w = vsnprintf(buf, n, fmt, arg);
    va_end(arg);
    cfnputs(buf, c, n, s);
    return w;
}