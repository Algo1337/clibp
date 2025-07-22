#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../headers/net/socket.h"

sock_T *create_socket(hostname_t ip_t, str_t ipaddr, int_T prt) {
    sock_T *s = (sock_T *)malloc(sizeof(sock_T));

    s->ip = ipaddr;
    s->port = prt;
    s->buffer_len = 1024;

    s->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(s->sock == -1)
        return NULL;

    memset(&s->addr, '\0', sizeof(s->addr));
    s->addr.sin_family = AF_INET;
    s->addr.sin_port = htons(prt);

    if(ip_t = _ipv4 && strlen(s->ip->data) > 2)
        if(!inet_aton(s->ip->data, &s->addr.sin_addr))
            return NULL;

    int reuse = 1;
    if(setsockopt(s->sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse)) < 0)
        return NULL;

    return s;
}

int sock_set_read_timeout(sock_T *s, int delay) {
    if(!s || !s->sock)
        return 0;

    struct timeval timeout = {
        .tv_sec = delay / 1000000,
        .tv_usec = delay % 1000000,
    };

    if(setsockopt(s->sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0)
        return 1;

    return 0;
}

int sock_bind(sock_T *s) {
    if(!s || s->sock <= 0)
        return 0;

    if(bind(s->sock, (struct sockaddr *)&s->addr, sizeof(s->addr)) != 0)
        return 0;

    return 1;
}

int sock_connect(sock_T *s) {
    if(!s || s->sock <= 0)
        return 0;
    
    return connect(s->sock, (struct sockaddr *)&s->addr, (socklen_t)sizeof(s->addr));
}

int sock_listen(sock_T *s, int concurrent) {
    if(!s || s->sock <= 0)
        return 0;

    if(listen(s->sock, concurrent) != 0)
        return 0;

    return 0;
}

sock_T *sock_accept(sock_T *s, int bufflen) {
    if(!s || s->sock <= 0)
        return NULL;

    sock_T *client = (sock_T *)malloc(sizeof(sock_T));
    client->buffer_len = (bufflen == 0 ? 1024 : bufflen);

    int len = sizeof(client->addr);
    client->sock = accept(s->sock, (struct sockaddr *)&s->addr, (socklen_t *)&len);

    return client;
}

str_t sock_read(sock_T *s) {
    if(!s || s->sock <= 0)
        return NULL;

    char *buff = (char *)malloc(s->buffer_len);
    if(!buff)
        return NULL;

    memset(buff, '\0', s->buffer_len);

    int bytes = read(s->sock, buff, s->buffer_len);
    if(bytes <= 0) {
        free(buff);
        return NULL;
    }

    return new_str(buff, 0);
}

int sock_write(sock_T *s, const str data) {
    if(!s || s->sock <= 0)
        return 0;

    int bytes_sent = send(s->sock, data, strlen(data) - 1, MSG_NOSIGNAL);
    if(bytes_sent <= 0)
        return 0;

    return 1;
}

void sock_get_client_ip(sock_T *s) {
    socklen_t addr_sz = sizeof(s->addr);
    if(getpeername(s->sock, (struct sockaddr *)&s->addr, &addr_sz) != 0)
        return;

    char ip[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &(s->addr.sin_addr), ip, INET_ADDRSTRLEN);
    if(s->ip)
        str_Destruct(s->ip);

    s->ip = new_str(strdup(ip), 0);
    s->port = ntohs(s->addr.sin_port);
}

void sock_Destruct(sock_T *s) {
    if(!s)
        return;

    if(s->ip)
        str_Destruct(s->ip);

    if(s->sock > 0)
        close(s->sock);
}