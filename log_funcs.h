#ifndef LOG_FUNCS_H
#define LOG_FUNCS_H

#include <stdio.h>

extern FILE* LOG_FILE;

//
//! @brief file creation
//!
//!
//! a file is created and opened
void CloseLogFile ();

//
//! @brief closing a file
//!
//!
//! closes the file
void OpenLogFile (const char* FILE_NAME, const char* mode);

#endif
