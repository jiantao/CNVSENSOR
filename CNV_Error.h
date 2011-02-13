#ifndef  CNV_ERROR_H
#define  CNV_ERROR_H

// max length for error message
#define	MAXLINE_ERR 4096

// Fatal error related to a system call.
// Print a message, dump core, and terminate. 
void CNV_ErrDump(const char *, ...);	

// Nonfatal error unrelated to a system call.
// Print a message and return. 
void CNV_ErrMsg(const char *, ...);

// Fatal error unrelated to a system call.
// Print a message and terminate. 
void CNV_ErrQuit(const char *, ...);

// Nonfatal error related to a system call.
// Print a message and return. 
void CNV_ErrRet(const char *, ...);

// Fatal error related to a system call.
// Print a message and terminate. 
void CNV_ErrSys(const char *, ...);

#endif  //CNV_ERROR_H
