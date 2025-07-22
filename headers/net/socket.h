#pragma once

#include "obj.h"
#define CLIBP
#include "../clibp.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

typedef struct sock_T {
    str_t                   ip;
    int                     port;
    int                     sock;
    struct sockaddr_in      addr;
    int                     buffer_len;
} sock_T;

typedef sock_T *sock_t;

sock_T *create_socket(hostname_t ip_t, str_t ipaddr, int_T prt);
int sock_set_read_timeout(sock_T *s, int timeout);
int sock_bind(sock_T *s);
int sock_connect(sock_T *s);
int sock_listen(sock_T *s, int concurrent);
sock_T *sock_accept(sock_T *s, int bufflen);
str_t sock_read(sock_T *s);
int sock_write(sock_T *s, const str data);
void sock_get_client_ip(sock_T *s);
void sock_Destruct(sock_T *s);