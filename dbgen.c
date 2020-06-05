#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv) {

    int fd;
    unsigned char buf[10] = {0};

    if ((fd = open("/tmp/gen.txt", O_WRONLY | O_CREAT, 0660)) == -1) {
        fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
        exit(EXIT_FAILURE);
    }

    *buf = 0xd;
    *(buf + 1) = 0xe;

    write(fd, buf, 2);

    close(fd);

    return 0;
}
