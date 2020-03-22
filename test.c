#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unix.h"

static void usage(void) {
  fprintf(stderr, "usage\n");
  exit(2);
}

static void panic_errno(const char *which) {
  fprintf(stderr, "%s: %s\n", which, strerror(errno));
  exit(2);
}

static void do_readlink(const char *path) {
  char *link;
  if (unix_readlink(path, &link) == -1)
    panic_errno("readlink");
  printf("%s\n", link);
  free(link);
}

int main(int argc, char **argv) {
  const char *which;

  if (argc < 2)
    usage();
  which = argv[1];
  if (!strcmp(which, "readlink")) {
    if (argc != 3)
      usage();
    do_readlink(argv[2]);
  } else {
    usage();
  }
}
