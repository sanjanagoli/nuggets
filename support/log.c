/* 
 * log module - a simple way to log messages to a file
 * 
 * David Kotz, May 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include "log.h"

/*********** global variables ****************/
/* This is an example of a judicious use of a global variable.
 * This module stashes a file pointer(FP) for use in all the logging 
 * functions, so the module user need not pass the FP to every call.
 * One disadvantage is when these logging functions are used by many 
 * modules within a larger program; they will all log to the same file,
 * or not.  (In some instances it might be nice for them to log to separate
 * files, but this module does not support that option.)
 */
static FILE *logFP = NULL;            // open file to write log messages

/**************** log_init ****************/
/* Initialize the logging module by stashing a copy of the file pointer.
 * fp = NULL disables logging.
 */
void log_init(FILE *fp)
{
  logFP = fp; // cache the file pointer in our global variable
  log_v("START OF LOG");
}

/**************** log_s ****************/
/* 
 * log a string to the logfile, if logging is enabled.
 * The string `format` can reference '%s' to incorporate `str`.
 */
void
log_s(const char *format, const char *str)
{
  if (logFP != NULL && format != NULL && str != NULL) {
    fprintf(logFP, format, str);
    fputc('\n', logFP);
    fflush(logFP);
  }
}

/**************** log_d ****************/
/* 
 * log an integer to the logfile, if logging is enabled.
 * The string `format` can reference '%d' to incorporate `num`.
 */
void
log_d(const char *format, const int num)
{
  if (logFP != NULL && format != NULL) {
    fprintf(logFP, format, num);
    fputc('\n', logFP);
    fflush(logFP);
  }
}

/**************** log_c ****************/
/* 
 * log a character to the logfile, if logging is enabled.
 * The string `format` can reference '%c' to incorporate `ch`.
 */
void
log_c(const char *format, const char ch)
{
  if (logFP != NULL && format != NULL) {
    fprintf(logFP, format, ch);
    fputc('\n', logFP);
    fflush(logFP);
  }
}

/**************** log_v ****************/
/* 
 * log a message to the logfile, if logging is enabled.
 */
void
log_v(const char *str)
{
  if (logFP != NULL && str != NULL) {
    fputs(str, logFP);
    fputc('\n', logFP);
    fflush(logFP);
  }
}

/**************** log_e ****************/
/* 
 * log an error to the logfile, if logging is enabled.
 * Expects the global variable errno (sys/errno.h) to indicate the error,
 * so this is best used immediately after a system call.
 */
void
log_e(const char *str)
{
  if (logFP != NULL && str != NULL) {
    fprintf(logFP, "%s: %s\n", str, strerror(errno));
    fflush(logFP);
  }
}

/**************** logDone ****************/
/* 
 * Done with logging.  Notes this, then disables logging. 
 */
void
log_done(void)
{
  log_v("END OF LOG");
  logFP = NULL;
}
