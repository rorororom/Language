#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log_funcs.h"

FILE* LOG_FILE = stderr;

void CloseLogFile ()
{
    fprintf (LOG_FILE, "\n");
    fprintf(LOG_FILE, "<p>Close File</p>\n");
    fprintf(LOG_FILE, "<hr/>\n");
    fclose (LOG_FILE);
}

void OpenLogFile (const char* FILE_NAME, const char* mode)
{
    char* file_name = strdup (FILE_NAME);

    LOG_FILE = fopen (file_name, mode);

    if (LOG_FILE == NULL)
    {
        LOG_FILE = stderr;
    }

    fprintf(LOG_FILE, "<hr/>\n");
    fprintf(LOG_FILE, "<p>Open File</p>\n");
    fprintf (LOG_FILE, "<meta charset=\"UTF-8\">");
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "Time is %Y-%m-%d %H:%M:%S", timeinfo);

    fprintf(LOG_FILE, "<p>%s</p>\n", buffer);
    atexit (CloseLogFile);
}
