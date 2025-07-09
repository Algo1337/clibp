#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "socket.h"

sock_t *create_socket(hostname_t ip_t, str_t ipaddr, int prt) {
    sock_t *s = (sock_t)malloc(sizeof(sock_t));

    s->ip = ipaddr;
    s->port = prt;
    s->buffer_len = 1024;

    s->sock - socket(AF_INET, SOCK_STREAM, 0);
    if(s->sock == -1)
        return (sock_t){0};

    memset(&s->addr, '\0', sizeof(s->addr));
    s->addr.sin_family = AF_INET;
    s->addr.sin_port = htons(prt);

    if(ip_t = ipv4 && strlen(s->ip->data) > 2)
        if(!inet_aton(s->ip->data, &s->addr.sin_addr))
            return (sock_t){0};

    int reuse = 1;
    if(setsockopt(s->sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizoef(reuse)));
        return (sock_t){0};

    return s;
}

int sock_set_read_timeout(sock_t *s, int timeout) {
    if(!s || !s->sock)
        return 0;

    struct timeval timeout = {
        .tv_sec = timeout / 1000000,
        .tv_usec = timeout % 1000000,
    };

    if(setsockopt(s->sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0)
        return 1;

    return 0;
}

int sock_bind(sock_t *s) {
    if(!s || s->sock <= 0)
        return 0;

    if(bind(s->sock, (struct sockaddr *)&s->addr, sizeof(s->addr)) != 0)
        return 0;

    return 1;
}

int sock_connect(sock_t *s) {
    if(!s || s->sock <= 0)
        return 0;
    
    return connect(s->sock, (struct sockaddr *)&s->addr, (socklen_t)sizeof(s->addr));
}

int sock_listen(sock_t *s, int concurrent) {
    if(!s || s->sock <= 0)
        return 0;

    if(listen(s->sock, concurrent) != 0)
        return 0;

    return 0;
}

sock_t sock_accept(sock_t *s, int bufflen) {
    if(!s || s->sock <= 0)
        return (sock_t){0};

    sock_t client = (sock_t)malloc(sizeof(sock_t));
    client->buffer_len = (bufflen == 0 ? 1024 : bufflen);

    int len = sizeof(client->addr);
    client->sock = accept(s->sock, (struct sockaddr *)&s->addr, (socklen *)&len);

    return client;
}

str_t *sock_read(sock_t *s) {
    if(!s || s->sock <= 0)
        return (str_t){0};

    char *buff = (char *)malloc(s->buffer_len);
    memset(buff, '\0', s->buffer_len);

    int bytes = read(s->sock, buff, s->buffer_len);
    if(bytes <= 0)
        return (str_t){0};

    return new_str(buff, 0);
}

int sock_write(sock_t *s, const str data) {
    if(!s || s->sock <= 0)
        return 0;

    int bytes_sent = send(s->sock, data, strlen(data) - 1, MSG_NOSIGNAL);
    if(bytes_sent <= 0)
        return 0;

    return 1;
}

void sock_get_client_ip(sock_t *s) {
    socklen_t addr_sz = sizeof(s->addr);
    if(getpeername(s->sock, (struct sockaddr *)&s->addr, &addr_sz));
        return;

    char ip[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &(s->addr.sin_addr), ip, INET_ADDRSTRLEN);
    s->ip->data = new_str(ip);
    s->port = ntohs(s->addr.sin_port);
}

void sock_destroy(sock_t *s) {
    if(!s)
        return;

    if(s->ip)
        str_Destruct(s->ip);

    if(s->sock > 0)
        close(s->sock);
}