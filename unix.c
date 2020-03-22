#include <sys/socket.h>

#include <netinet/in.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "unix.h"

int unix_read(int fd, char *buf, size_t cap, size_t *out) {
  size_t len;

  while ((len = (size_t)read(fd, buf, cap)) == (size_t)-1) {
    if (errno != EINTR) {
      *out = 0;
      return -1;
    }
  }
  *out = len;
  return 0;
}

int unix_write(int fd, char *buf, size_t cap, size_t *out) {
  size_t len;

  while ((len = (size_t)write(fd, buf, cap)) == (size_t)-1) {
    if (errno != EINTR) {
      *out = 0;
      return -1;
    }
  }
  *out = len;
  return 0;
}

int unix_getcwd(char **out) {
  char *buf;
  char *newbuf;
  size_t cap;
  int err;

  buf = 0;
  for (cap = 64; cap != 0; cap *= 2) {
    if (!(newbuf = realloc(buf, cap)))
      break;
    buf = newbuf;
    if (getcwd(buf, cap)) {
      *out = buf;
      return 0;
    }
    if (errno != ERANGE)
      break;
  }
  err = errno;
  free(buf);
  errno = err;
  *out = 0;
  return -1;
}

int unix_readlink(const char *path, char **out) {
  char *buf;
  char *newbuf;
  size_t cap, len;
  int err;

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
      *out = buf;
      return 0;
    }
  }
  err = errno;
  free(buf);
  errno = err;
  *out = 0;
  return -1;
}

static int fill_ipv4(struct sockaddr_in *addr, unsigned int ip,
                     unsigned int port) {
  memset(addr, 0, sizeof(*addr));
  addr->sin_family = AF_INET;
  addr->sin_port = htons(port);
  addr->sin_addr.s_addr = htonl(ip);
  return 0;
}

int unix_bind_ipv4(int fd, unsigned int ip, unsigned int port) {
  struct sockaddr_in addr;

  if (fill_ipv4(&addr, ip, port) == -1)
    return -1;
  return bind(fd, (struct sockaddr *)&addr, sizeof(addr));
}

int unix_connect_ipv4(int fd, unsigned int ip, unsigned int port) {
  struct sockaddr_in addr;

  if (fill_ipv4(&addr, ip, port) == -1)
    return -1;
  return connect(fd, (struct sockaddr *)&addr, sizeof(addr));
}
