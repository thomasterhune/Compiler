
/*! \file terminal.h
	\brief Windows Terminal Macros
	
	Provides macros for using Console Virtual Terminal Sequences

	Enable Virtual Terminal Sequences flag must be set in Console Mode to use.

	See: https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
	
	\author Karl Miller
	\date 4/13/2022

*/


#ifndef k_terminal
#define k_terminal

#include <iostream>

/**
	@name Virtual Terminal Prefixes
	
	These character sequences are printed at the beginning of VCE sequences
	
*/
///@{
#define ESC "\x1B["    // its not quite clear whether this should be "\x1b" or "\x1B[". This is working though.
#define CSI "\x1B"
///@}



/**
	@name Cursor Movement
	
	This macro allows for quick movement of the cursor to a specified row and column

*/
///@{
/**
	Move the cursor to x y, where x is a target column and y is a target row.
*/
#define CURSOR_TO(x, y) std::cout << ESC << y << ";" << x << "f"
#define CURSOR_BACK(n) std::cout << ESC << n << "D"
///@}

/**
	@name Graphic Codes

	Virtual Console Escape sequences for setting color and other graphics options.
	
	Example setting foreground color to yellow:
	
	std::cout << "\x1B[33m";
	
	or, with defined constants, for improved readability:
	
	std::cout << ESC << FG_YELLOW << GRAPHIC;

*/
///@{
#define GRAPHIC "m"

#define UNDERLINE 4
#define NO_UNDERLINE 24

#define FG_YELLOW 33
#define FG_MAGENTA 35
#define FG_DEFAULT 39
#define FG_CYAN 36
#define FG_WHITE 37
#define FG_BLUE 34
#define FG_BLACK 30
#define FG_RED 31
#define FG_GREEN 32

#define FG_BRT_RED 91
#define FG_BRT_GREEN 92
#define FG_BRT_BLACK 90
#define FG_BRT_YELLOW 93
#define FG_BRT_BLUE 94
#define FG_BRT_GREEN 92
#define FG_BRT_MAGENTA 95
#define FG_BRT_CYAN 96
#define FG_BRT_WHITE 97

#define BG_BLUE 44
#define BG_MAGENTA 45
#define BG_RED 41
#define BG_DEFAULT 49
#define BG_GREEN 42
#define BG_BLACK 40
#define BG_WHITE 47
#define BG_YELLOW 43

#define BG_BRT_BLACK 100
#define BG_BRT_RED 101
#define BG_BRT_GREEN 102
#define BG_BRT_YELLOW 103
#define BG_BRT_BLUE 104
#define BG_BRT_MAGENTA 105
#define BG_BRT_CYAN 106
#define BG_BRT_WHITE 107

#define SET_COLOR(fg, bg) std::cout << ESC << fg << GRAPHIC << ESC << bg << GRAPHIC;
#define COLOR_DEFAULT() std::cout << ESC << FG_DEFAULT << GRAPHIC << ESC << BG_DEFAULT << GRAPHIC << ESC << NO_UNDERLINE << GRAPHIC;


///@{
/**
@name Cursor view macros

Set cursor to blink, show, hide, etc.

*/
#define CURSOR_BLINK_ENABLE() std::cout << ESC << "?12h"
#define CURSOR_BLINK_DISABLE() std::cout << ESC << "?12l"
#define CURSOR_HIDE() std::cout << ESC << "?25l"
#define CURSOR_SHOW() std::cout << ESC << "?25h"

///@{

///@}

/**
@name Border macros

When line mode is enabled, different border character are printed instead of regular alphabet

*/

///@{
#define ESC_S "\x1b"

#define LINE_BOTTOM_RIGHT "j"
#define LINE_TOP_RIGHT "k"
#define LINE_TOP_LEFT "l"
#define LINE_BOTTOM_LEFT "m"
#define LINE_HORIZONTAL "q"
#define LINE_VERTICAL "x"

#define LINE_ENABLE() std::cout << "\x1b(0"
#define LINE_DISABLE() std::cout << "\x1b(B"

///@}



/**
	@name Unused Macros
	
	These are older macros designed to work with the printf statement.
	
	Since this program is using cout for everything, these are unused, but are left in in case we need to go back or re-write these macros for cout.

	e.g  printf(CURSOR_FORWARD, 3); would move cursor forward 3.
*/
///@{
#define ERASE_DISPLAY ESC "[2J"
#define CURSOR_FORWARD ESC "[%dC"
#define CURSOR_DOWN ESC "[%dB"
#define CURSOR_UP ESC "[%dA"

#define CURSOR_MOVE_HOR ESC "[%dG"
#define CURSOR_MOVE_VER ESC "[%dd"
#define CURSOR_MOVE_TO ESC "[%d;%df"

///@}



#endif


