/* 
 * ColorPrinter.h - Noah Sandman
 * 
 * NOTE: To my memory, this was written before my freshman year of high
 * school. It makes a lot of questionable choices and should be rewritten
 * very soon.
 */

#ifndef _COLORPRINTER_H
#define _COLORPRINTER_H

#	include <stdarg.h> // va_list, va_start(), va_end()
#	include <stdio.h> // fprintf(), fputs(), vfprintf(), vsnprintf(), stdout, FILE*, size_t

// "unsigned char" is annoying to type all the time
typedef unsigned char color_t;

// How many ANSI colors are before the xterm-256 ones (starting from zero)?
#define ANSI_OFFSET 	15

extern color_t colors[];

// define the colors as macros with their position in the array
#define RED    					0
#define GREEN  					1
#define YELLOW 					2
#define CYAN   					3
#define LIGHT_GRAY				4

#define BRIGHT_RED 				5
#define BRIGHT_GREEN			6 
#define BRIGHT_YELLOW			7
#define LIGHT_BLUE				8
#define WHITE					9

// These are xterm-256 colors, and if "NO256" is defined, they will show up black.
#define ORANGE 					ANSI_OFFSET + 1
#define DARK_GRAY				ANSI_OFFSET + 2
#define OLIVE					ANSI_OFFSET + 3
#define MAGENTA					ANSI_OFFSET + 4

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
#	define BLUE 				10
#	define PURPLE				12
#	define PINK 				14
#else
#	define BLUE 				11
#	define PURPLE 				13
#	define PINK 				15
#endif // __APPLE__ && !MAC_OVERRIDE

// If you're a Brit and you keep forgetting about "gray" vs. "grey", here
#ifdef UK
#	define LIGHT_GREY			LIGHT_GRAY
#	define DARK_GREY			DARK_GRAY
#	undef  LIGHT_GRAY
#	undef  DARK_GRAY
#endif // UK

size_t strlen(const char*);
void startprint(color_t, FILE*);
int  cfnputs(const char*, color_t, size_t, FILE*);
int  cfprintf(FILE*, color_t, const char*, ...);
int  cfnprintf(FILE*, color_t, size_t, const char*, ...);

#define endprint(s) fputs("\033[0m", s)		// Reset to default color

// m = msg, f = format string, c = color, s = stream
#define cfputc(a, b, c)			cputc(a, b, c)
#define cputc(m, c, s)			startprint(c, s); \
									putc(m, s); \
									endprint(s)
#define cfputs(a, b, c) 		cfnputs(a, b, strlen(a), c)
#define cputchar(m, c)			cputc(m, c, stdout)
#define cputs(m, c) 			cfputs(m, c, stdout); \
									putchar('\n');

#define cnputs(m, c, n) 		cfnputs(m, c, n, stdout); \
									putchar('\n');

#define cprintf(c, f, ...) 		cfprintf(stdout, c, f, __VA_ARGS__)
#define cnprintf(c, n, f, ...)	cfnprintf(stdout, c, n, f, __VA_ARGS__)

#endif // _COLORPRINTER_H