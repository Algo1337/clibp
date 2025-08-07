#define CLIBP

#include <clibp.h>
#include <str.h>
#include <net/socket.h>

int main() {
    str_T *ip = new_str(strdup("51.81.203.117"), 0);
    sock_t server = create_socket(_ipv4, ip, 8080);
    if(!server) {
        printf("Error, Unable to start socket!\n");
        return 1;
    }

    sock_bind(server);
    sock_listen(server, 9999);

    printf("Main Socket: %d\n", server->sock);
    while(server->sock != 0) {
        sock_t client = sock_accept(server, 1024);
        if(!client)
            continue;
        
        sock_set_read_timeout(client, 0);
        sock_get_client_ip(client);
        printf("A new socket connected: %s\n", client->ip->data);

        unsigned char no_echo[] = { 255, 251, 1 };
        send(client->sock, no_echo, 3, 0);
        unsigned char suppress_go_ahead[] = { 255, 251, 3 };
        send(client->sock, suppress_go_ahead, 3, 0);
        
        sock_write(client, "[ + ] Welcome to C CNC!\n");

        str_T *client_input = NULL;
        int bytes;
        char c;
        while((bytes = read(client->sock, &c, 1)) != 0) {
            // if(!client_input)
            //     continue;

            printf("[ %s ] Client Input: %c:%d\n", client->ip->data, c, c);
            // if(!strcmp(client_input->data, "start") || !strcmp(client_input->data, "START"))
            //     sock_write(client, "The official C Server, Version 1.00\n");
            // if(BUFF)

            str_Destruct(client_input);
        }
        // while((client_input = sock_read(client)) != NULL) {
        //     if(!client_input)
        //         continue;

        //     printf("[ %s ] Client Input: %s\n", client->ip->data, client_input->data);
        //     if(!strcmp(client_input->data, "start") || !strcmp(client_input->data, "START"))
        //         sock_write(client, "The official C Server, Version 1.00\n");

        //     str_Destruct(client_input);
        // }
        sock_Destruct(client);
    }
    sock_Destruct(server);
    return 0;
}