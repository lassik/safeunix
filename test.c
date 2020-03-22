// Copyright 2020 Lassi Kortela
// SPDX-License-Identifier: ISC

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "unix.h"

static void usage(void)
{
    fprintf(stderr, "usage\n");
    exit(2);
}

static void panic_errno(const char *which)
{
    fprintf(stderr, "%s: %s\n", which, strerror(errno));
    exit(2);
}

static void do_getgroups(void)
{
    gid_t *gids;
    size_t ngid, i;

    if (unix_getgroups(&gids, &ngid) == -1)
        panic_errno("getcwd");
    i = 0;
    while (i < ngid - 1) {
        printf("%ld ", (long)gids[i++]);
    }
    printf("%ld\n", (long)gids[i]);
    free(gids);
}

static void do_getcwd(void)
{
    char *dir;
    if (unix_getcwd(&dir) == -1)
        panic_errno("getcwd");
    printf("%s\n", dir);
    free(dir);
}

static void do_readlink(const char *path)
{
    char *link;
    if (unix_readlink(path, &link) == -1)
        panic_errno("readlink");
    printf("%s\n", link);
    free(link);
}

int main(int argc, char **argv)
{
    const char *which;

    if (argc < 2)
        usage();
    which = argv[1];
    if (!strcmp(which, "getgroups")) {
        if (argc != 2)
            usage();
        do_getgroups();
    } else if (!strcmp(which, "getcwd")) {
        if (argc != 2)
            usage();
        do_getcwd();
    } else if (!strcmp(which, "readlink")) {
        if (argc != 3)
            usage();
        do_readlink(argv[2]);
    } else {
        usage();
    }
}
