/* 
 * ColorPrinter.h - Noah Sandman
 * 
 * NOTE: To my memory, this was written before my freshman year of high
 * school. It makes a lot of questionable choices and should be rewritten
 * very soon.
 */

#ifndef _COLORPRINTER_H
#define _COLORPRINTER_H

#ifndef __EMBEDDED__
#	include <stdarg.h> // va_list, va_start(), va_end()
#	include <stdio.h> // fprintf(), fputs(), vfprintf(), vsnprintf(), stdout, FILE*, size_t
#else
 	// stdarg.h from Linux 0.0.1; va_end is from Microsoft
	typedef char *va_list;
#	define __va_rounded_size(TYPE)	(((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))
#	define va_start(AP, LASTARG) 	(AP = ((char *) &(LASTARG) + __va_rounded_size (LASTARG)))
#	define va_end(AP)      	  		(AP = (va_list)0)
#	define va_arg(AP, TYPE)	  		(AP += __va_rounded_size (TYPE), \
								  	*((TYPE *)(AP - __va_rounded_size (TYPE))))
	// end stdarg.h
	// all the stdio functions are way too long to paste in, so...
#	include "bundledio.h"
	// FOR NOW, do this:
	// #include <stdio.h>

	// Also, for embedded systems:
#	ifndef NULL
#		define NULL ((void*)0)
#	endif		// NULL
	// just in case.
	typedef unsigned int size_t;
#endif

// "unsigned char" is annoying to type all the time
typedef unsigned char color_t;

// How many ANSI colors are before the xterm-256 ones (starting from zero)?
#define ANSI_OFFSET 	15
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

// define the colors as macros with their position in the array
#define red    					0
#define green  					1
#define yellow 					2
#define cyan   					3
#define light_gray				4

#define bright_red 				5
#define bright_green			6 
#define bright_yellow			7
#define light_blue				8
#define white					9

// These are xterm-256 colors, and if "NO256" is defined, they will show up black.
#define orange 					ANSI_OFFSET + 1
#define dark_gray				ANSI_OFFSET + 2
#define olive					ANSI_OFFSET + 3
#define magenta					ANSI_OFFSET + 4

/*
 * On Macs, the colors are weird and screwed up.
 * Blue looks like purple.
 * Purple looks like pink.
 * So just add pink while we're at it.
 * However, it's also possible to manually fix the colors
 * using Terminal preferences, so if the user has, they can
 * define MAC_OVERRIDE at compile time with gcc (or probably clang,
 * seeing as that's the default C compiler on OSX).
 */
#if defined(__APPLE__) && !defined(MAC_OVERRIDE)
#	define blue 				10
#	define purple				12
#	define pink 				14
#else
#	define blue 				11
#	define purple 				13
#	define pink 				15
#endif // __APPLE__ && !MAC_OVERRIDE

// If you're a Brit and you keep forgetting about "gray" vs. "grey", here
#ifdef UK
#	define light_grey			light_gray
#	define dark_grey			dark_gray
#	undef  light_gray
#	undef  dark_gray
#endif // UK

size_t strlen(const char*);
#ifndef __EMBEDDED__
	// Function prototypes... 'nuff said
	void startprint(color_t, FILE*);
	int  cfnputs(const char*, color_t, size_t, FILE*);
	int  cfprintf(FILE*, color_t, const char*, ...);
	int  cfnprintf(FILE*, color_t, size_t, const char*, ...);
#else
	void startprint(color_t);
	void endprint();
#endif // __EMBEDDED__

size_t strlen(const char *str) {
	size_t i = 0;
	while(str[++i]);
	return i;
}

#ifndef __EMBEDDED__
#	define endprint(s) fputs("\033[0m", s)		// Reset to default color
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
#else
//#	define endprint	 puts("\033[0m")		// Reset to default color
	void endprint() {
		puts("\033[0m");
	}

	void startprint(color_t c) {
		#ifndef NO256
		if (c >= ANSI_OFFSET)
			printf("\033[38;5;%dm", colors[c]);	
		else
		#endif	// !NO256
			printf("\033[0;%dm", colors[c]);
	}
#endif // __EMBEDDED__

// m = msg, f = format string, c = color, s = stream
#ifndef __EMBEDDED__		// don't use streams on embedded systems
#	define cfputc(a, b, c)			cputc(a, b, c)
#	define cputc(m, c, s)			startprint(c, s); \
									putc(m, s); \
									endprint(s)
#	define cfputs(a, b, c) 			cfnputs(a, b, strlen(a), c)
#	define cputchar(m, c)			cputc(m, c, stdout)
#	define cputs(m, c) 				cfputs(m, c, stdout); \
									putchar('\n');

#	define cnputs(m, c, n) 			cfnputs(m, c, n, stdout); \
									putchar('\n');

#	define cprintf(c, f, ...) 		cfprintf(stdout, c, f, __VA_ARGS__)
#	define cnprintf(c, n, f, ...)	cfnprintf(stdout, c, n, f, __VA_ARGS__)
#else
#	define cputchar(m, c)			startprint(c); \
									putchar(m);
									endprint();
#	define cprintf(c, f, ...)		startprint(c); \
									printf(f, __VA_ARGS__);
									endprint();
#	define cputs(m, c)				startprint(c); \
									puts(m);
									endprint();	
#endif		// __EMBEDDED__

#endif // _COLORPRINTER_H