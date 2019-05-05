/* 
 * log module - a simple way to log messages to a file
 * 
 * David Kotz, May 2019
 */

#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <stdlib.h>

/*********** logging-related functions ****************/
/* log_init: to begin logging, provide an fp open for writing;
 * to do no logging, provide logFP = NULL.
 * Call log_done() before the program exits.
 */
void log_init(FILE *logFP);

/* log_[sdc]: printf to the log, using the given format string.
 * Each expects exactly one format specifier within the string,
 * corresponding to the one argument.  A newline is added.
 * Examples:
 *   char *userName = ...; log_s("Your name is '%s'", userName);
 *   int age = ...;        log_d("You are %d years old.", age);
 *   char player = ...;    log_c("Player %c is winning.", player);
 */
void log_s(const char *format, const char *str); // format %s
void log_d(const char *format, const int  num);  // format %d
void log_c(const char *format, const char ch);	 // format %c

/* log_v: like the above, but used when no additional argument is needed.
 * Thus v stands for 'void'.
 */
void log_v(const char *str);

/* log_e: print the given string to the log, with a message representing
 * an internal error.  See 'man errno' and 'man perror';
 * This function is best used immediately after a system call.
 */
void log_e(const char *str);  // like perror(str)

/* log_done: call this when finished logging, or when you want to pause
 * logging for a while.  Call log_init() to resume.
 * It is the caller's responsibility to close the file, if desired.
 */
void log_done(void);

#endif // _LOG_H_
