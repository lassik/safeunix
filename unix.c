#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "unix.h"

char *unix_readlink(const char *path)
{
    char *buf;
    char *newbuf;
    size_t cap, len;
    int save;

    buf = 0;
    for (cap = 64; cap != 0; cap *= 2) {
        if (!(newbuf = realloc(buf, cap)))
            break;
        buf = newbuf;
        len = (size_t)readlink(path, buf, cap);
        if (len == (size_t)-1)
            break;
        if (len < cap) {
            memset(buf + len, 0, cap - len);
            return buf;
        }
    }
    save = errno;
    free(buf);
    errno = save;
    return 0;
}
