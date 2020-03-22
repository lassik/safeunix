int unix_read(int fd, char *buf, size_t cap, size_t *out);
int unix_write(int fd, char *buf, size_t cap, size_t *out);
int unix_getgroups(gid_t **out_gids, size_t *out_ngid);
int unix_getcwd(char **out);
int unix_readlink(const char *path, char **out);
int unix_bind_ipv4(int fd, unsigned int ip, unsigned int port);
int unix_connect_ipv4(int fd, unsigned int ip, unsigned int port);
