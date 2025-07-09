#pragma once

#include "obj.h"
#define CLIBP
#include "../stdlib/clibp.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

typedef struct sock_t {
    str_t                   ip;
    int                     port;
    int                     sock;
    struct sockaddr_in      addr;
    int                     buffer_len;
} sock_t;

sock_t *create_socket(hostname_t ip_t, str_t ipaddr, int prt);
int sock_set_read_timeout(sock_t *s, int timeout);
int sock_bind(sock_t *s);
int sock_connect(sock_t *s);
int sock_listen(sock_t *s, int concurrent);
sock_t sock_accept(sock_t *s, int bufflen);
str_t *sock_read(sock_t *s);
int sock_write(sock_t *s, const str data);
void sock_get_client_ip(sock_t *s);
void sock_destroy(sock_t *s);