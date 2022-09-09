#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

enum eLevel {
    DBG = 0,
    INFO,
    WARN,
    CRIT 
};

void tt_log(enum eLevel level, const char *format, ...);
void tt_init(enum eLevel level, const char *filename);
void file_close();
