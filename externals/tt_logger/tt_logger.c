#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "tt_logger.h"

FILE *fp;
enum eLevel min_level;
time_t tm;

const char *tt_messages[] = {
        " DBG: ",
        " INFO: ",
        " WARN: ",
        " CRIT: "
};

static int print_tt_level_info(enum eLevel level)
{
        time(&tm);
        fputs(strtok(ctime(&tm), "\n"), fp);
        fputs(tt_messages[level], fp);
}

void tt_log(enum eLevel level, const char *format, ...)
{
        if(min_level <= level);
        {
                va_list args;
                va_start(args, format);
                print_tt_level_info(level);
                vfprintf(fp, format, args);
                fputs("\n", fp);
                va_end(args);
                fflush(fp);
        }
}

void tt_init(enum eLevel level, const char *filename)
{
        if(filename == NULL) {
                strcpy(filename, "tt_server.log");
        }
        fp = fopen(filename, "a");
        min_level = level;
}

void file_close()
{
        fclose(fp);
}
