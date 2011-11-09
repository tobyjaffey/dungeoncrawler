#include "common.h"
#include "cmdline.h"
#include <getopt.h>

static char *saveframes = NULL;

char *cmdline_saveframes(void)
{
    return saveframes;
}

static void usage(const char *app)
{
    fprintf(stderr, "Usage %s\n", app);
    fprintf(stderr, "  -h help\n");
    fprintf(stderr, "  -s <filename> save frames to <filename>-%%08d.bmp\n");
}

void cmdline_parse(int argc, char *argv[])
{
    int c;

    while ((c = getopt (argc, argv, "hs:")) != -1)
    {
        switch (c)
        {
            case 'h':
            case '?':
            usage(argv[0]);
            exit(0);
            break;

            case 's':
            saveframes = optarg;
            break;
        }
    }
}

