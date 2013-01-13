// Copyright(c) KIPR, 2013
// Full screen management functions for the display window as implemented on the KIPR LINK controller
//   display_printf(int column, int row, <printf arguments>)
//
// Console screen display window size is 10 rows and limited to 42 columns, indexed from 0
//
// Initial version: 1/8/2013 - cnw (Charles Winton)
// Revisions: none yet
// stdarg.h provides macros for accessing a function's argument list ... see K&R

#include "kovan/display.h"
#include "kovan/console.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define _MAPy 10  // 10 rows
#define _MAPx 43  // 42 columns is max fit for screen (array has 43 to accommodate \0)
char _display_map[_MAPy][_MAPx];
int _initialize_ = 1;

// clears console and sets display map to all spaces
void display_clear()
{
	console_clear();
	for(int i = 0; i < _MAPy; ++i) {
		for(int j = 0; j < _MAPx; ++j) _display_map[i][j] = ' ';  //  initialize to spaces
	}
	for(int i = 0; i < _MAPy; ++i) {
		_display_map[i][_MAPx-1] = '\0'; // make each row is a string
	}
}

// column, row, string with format phrases, args
void display_printf(int col, int row, const char *t, ...)
{
	va_list argp;              // argp is typed for traversing the variable part of the arg list
	int i; char *c; double d;  // working variables to receive arg values
	const char *cp;
	char *fmte, sc;       // cp traverses format string t, fmte marks end of each format phrase, sc is switch control

	int y;                   // row index
	char *dp;                // pointer into display
	int maxw;                // available room on line
	char fws[_MAPx];         // formatted phrase work area
	char fmt[_MAPx]; int fl; // fmt is a working string for each format extracted

	va_start (argp,t);  // t is last named argument in link_printf's function header;
	// this initializes argp to point to first variable arg
	
	// bad col so wrap to next line
	if (col >= _MAPx) {
		row = row + 1; col = 0;
	}
	if (row >= _MAPy) row = _MAPy - 1;          // bad row so print on last line
	console_clear();                            // ready the display
	
	// clear map to spaces on first call
	if (_initialize_ == 1) {
		display_clear();
		_initialize_ = 0;
	}
	dp = &_display_map[row][col];      // starting point for printf
	maxw = _MAPx - col;                // space remaining on line
	// process printf string; stop when *cp = '\0'
	for (cp = t; *cp; cp++) {
		// if not a format phrase
		if (*cp != '%') {
			if(strspn(cp,"\n\t\v\b\r\f") > 0) {  // is it a spec char? if so treat as if \n
				for (i = 0; i < maxw; ++i) { // clear balance of line
					*dp=' '; dp++;
				}
				if (row == _MAPy) break; // out of rows so proceed to display refresh
				++row;
				dp = &_display_map[row][0]; maxw = _MAPx; // set up for new line
			} else { // nothing special about this one so insert it
				*dp = *cp; dp++;
				maxw--; if (maxw == 0) break; // no more room on line so proceed to display refresh
			}
			continue;             // return to top
		}
		// OK, if we're here we may have hit a format phrase
		fmte = strpbrk(cp + 1, "dioxXucsfeEgG%"); // look for format end
		// strpbrk returns the location of 1st character of its argument that is in the scan string
		
		// what's left is not a format phrase so insert % and return to top 
		if (fmte == NULL) {
			*dp='%'; dp++;
			maxw--; if (maxw==0) break;  // no more room on line so proceed to display refresh
			continue;                    // return to top
		}
		// OK, there looks to be a format phrase
		sc = *fmte;                // set switch control for the case
		fl = 1 + fmte-cp;          // pick off phrase (pointed to by cp)
		strncpy(fmt, cp, fl);      // capture the format phrase
		fmt[fl] = '\0';            // make it a string
		switch (sc)                // process the kind of format specified
		{
			case 'd': case 'i': case 'o': case 'x': case 'X': case 'u': case 'c':
			i = va_arg(argp, int);    // return next parm as type int and step argp
			sprintf(fws, fmt, i);       // use sprintf to do the formatting to fws
			break;
			case 's':
			c = va_arg(argp, char *); // return next parm as type char * and step argp
			sprintf(fws, fmt, c);       // use sprintf to do the formatting to fws
			break;
			case 'f': case 'e': case 'E': case 'g': case 'G':
			d = va_arg(argp, double); // return next parm as type double and step argp
			sprintf(fws, fmt, d);       // use sprintf to do the formatting to fws
			break;
			case '%':                 // no format specified between %'s
			sprintf(fws, fmt);
			break;
		}
		
		for(i = 0; i < strlen(fws); ++i) { // insert formatted phrase in display map
			if (maxw == 0) break;      // if no more room get out of this
			*dp = fws[i];              // insert next character from formatted phrase
			dp++; maxw--;
		}
		if (maxw==0) break;           // if no more room proceed to display refresh
		cp = fmte;                    // set cp for next pass
	}
	va_end(argp);                         // clean up
	for(y = 0; y <  _MAPy - 1; ++y) printf("%s\n", _display_map[y]); // refresh the display
	printf("%s", _display_map[y]);
}

