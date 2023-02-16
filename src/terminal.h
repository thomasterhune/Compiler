#ifndef k_terminal
#define k_terminal

/*! \file terminal.h
	\brief Windows Terminal Macros
	
	Provides macros for using Console Virtual Terminal Sequences

	Enable Virtual Terminal Sequences flag must be set in Console Mode to use.

	See: https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
	
	See: https://learn.microsoft.com/en-us/windows/console/getconsolemode
	
	\author Karl Miller

*/


/* Call with 'wprintf' */

#define ESC "\x1B[" 
#define CSI "\x1B"

#define GRAPHIC "m"

#define UNDERLINE 4
#define NO_UNDERLINE 24

#define FG_BLACK L"\x1b[30m"
#define FG_DEFAULT L"\x1b[39m"
#define FG_YELLOW L"\x1b[33m"
#define FG_MAGENTA L"\x1b[35m"
#define FG_CYAN L"\x1b[36m"
#define FG_WHITE L"\x1b[37m"
#define FG_BLUE L"\x1b[34m"
#define FG_RED L"\x1b[31m"
#define FG_GREEN L"\x1b[32m"

#define FG_BRT_RED L"\x1b[91m"
#define FG_BRT_GREEN L"\x1b[92m"
#define FG_BRT_BLACK L"\x1b[90m"
#define FG_BRT_YELLOW L"\x1b[93m"
#define FG_BRT_BLUE L"\x1b[94m"
#define FG_BRT_GREEN L"\x1b[92m"
#define FG_BRT_MAGENTA L"\x1b[95m"
#define FG_BRT_CYAN L"\x1b[96m"
#define FG_BRT_WHITE L"\x1b[97m"

#define BG_DEFAULT L"\x1b[49m"
#define BG_BLUE L"\x1b[44m"
#define BG_MAGENTA L"\x1b[45m"
#define BG_RED L"\x1b[41m"
#define BG_GREEN L"\x1b[42m"
#define BG_BLACK L"\x1b[40m"
#define BG_WHITE L"\x1b[47m"
#define BG_YELLOW L"\x1b[43m"

#define BG_BRT_RED L"\x1b[101m"
#define BG_BRT_BLACK L"\x1b[100m"
#define BG_BRT_GREEN L"\x1b[102m"
#define BG_BRT_YELLOW L"\x1b[103m"
#define BG_BRT_BLUE L"\x1b[104m"
#define BG_BRT_MAGENTA L"\x1b[105m"
#define BG_BRT_CYAN L"\x1b[106m"
#define BG_BRT_WHITE L"\x1b[107m"

#endif


