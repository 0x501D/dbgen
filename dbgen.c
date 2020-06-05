#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include <errno.h>

void usage();

int main(int argc, char **argv) {
    int fd, opt;
    unsigned char *buf = NULL, *p;
    unsigned char pattern[] = { 0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf };
    char *path = NULL;
    const char *optstr = "f:m:k:h";
    size_t len = 0;
    int mult = 0;
    
    static const struct option longopts[] = {
        { "file", required_argument,   NULL, 'f' },
        { "mb",   required_argument,   NULL, 'm' },
        { "kb",   required_argument,   NULL, 'k' },
        { "help",   no_argument,       NULL, 'h' },
        { NULL,     0,                 NULL,  0  }
    };

    while ((opt = getopt_long(argc, argv, optstr, longopts, NULL)) != -1) {
        switch (opt) {
        case 'f':
            if ((path = strdup(optarg)) == NULL) {
                fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
                exit(EXIT_FAILURE);
            }
            break;
        case 'm':
            /* TODO use strtoul */
            len = atoi(optarg);
            mult = 1048576;
            break;
        case 'k':
            /* TODO use strtoul */
            len = atoi(optarg);
            mult = 1024;
            break;
        case 'h':
            usage();
            exit(EXIT_SUCCESS);
        default:
            usage();
            exit(EXIT_FAILURE);
        }
    }

    if (!path || !mult) {
        usage();
        exit(EXIT_FAILURE);
    }

    unlink(path);

    if ((fd = open(path, O_WRONLY | O_CREAT, 0660)) == -1) {
        fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
        exit(EXIT_FAILURE);
    }

    len *= mult;

    if ((buf = malloc(len)) == NULL) {
        fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
        exit(EXIT_FAILURE);
    }

    p = buf;

    for (size_t n = len; n; n -= 8, p += 8) {
        memcpy(p, pattern, sizeof(pattern));
    }

    write(fd, buf, len);

    close(fd);
    free(buf);
    free(path);

    return 0;
}

void usage()
{
    printf("-f, --file   <path>        path to generated file\n");
    printf("-m, --mb     <count>       set file size in Mb\n");
    printf("-k, --kb     <count>       set file size in Kb\n");
    printf("-h, --help                 print this help\n");
}
