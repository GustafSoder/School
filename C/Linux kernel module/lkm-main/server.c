#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   
#include <arpa/inet.h>    
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <poll.h>
#include <time.h>
#include <linux/netlink.h>
#include <stdint.h>
#include <semaphore.h>

sem_t mutex;

#define MAX_VALUE 64
#define MAX_BUFFER 1024
#define NETLINK_USER 31

typedef struct {
    int operation;
    int key;
    char value[MAX_VALUE];
} params;

#define PORT 8884
#define BUF_SIZE 2000
#define CLADDR_LEN 100
#define MAX_CLIENTS 255
#define DUMP 5

void init_conn_to_kernel();
void send_rcv_to_kernel();

// Variables for netlink, aka sending to kernel module
struct sockaddr_nl saddr, daddr;
int sock_fd;
struct msghdr msg;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
params data;
params *rcv_data;

int main(void) {
    FILE *fp; // create a file pointer
    fp = fopen("hashtable_dump.txt", "w");
    fclose(fp);
    struct sockaddr_in addr, cl_addr;
    int sockfd, newsockfd, len, num_clients, rc, i;
    char buffer[MAX_BUFFER];
    struct pollfd fds[MAX_CLIENTS];
    num_clients = 0;

    char send_buffer[MAX_BUFFER];

    sem_init(&mutex, 0, 1);

    //socket creation for client
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }
    printf("Socket created...\n");
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    
    rc = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    if (rc < 0) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
    for (i = 1; i < MAX_CLIENTS; i++) {
        fds[i].fd = -1;
    }
    listen(sockfd, 32);
    printf("Server listening on port %d\n", PORT);
    // LISTEN FOR CONNECTIONS.
    for (;;) { //infinite loop
        if (poll(fds, num_clients + 1, -1) < 0) {
            perror("poll()");
            exit(EXIT_FAILURE);
        }

        if (fds[0].revents & POLLIN) {
            len = sizeof(cl_addr);
            newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);
            if (newsockfd < 0) {
                perror("accept()");
                exit(EXIT_FAILURE);
            }
            printf("New client connected, sockets fd is %d\n", newsockfd);

            for (i = 1; i < MAX_CLIENTS; i++) {
                if (fds[i].fd < 0) {
                    fds[i].fd = newsockfd;
                    fds[i].events = POLLIN;
                    break;
                }
            }
            if (i == MAX_CLIENTS) {
                printf("Maximum amount of clients reached\n");
                close(newsockfd);
            }
            
            num_clients++;
        }
        // Check for incoming data or client disconnects
        for (i = 1; i < num_clients + 1; i++) {
            if (fds[i].revents & POLLIN) {
                int n = recv(fds[i].fd, buffer, BUF_SIZE, 0);
                sem_wait(&mutex); 
                data.operation = 0;
                memset(&data, 0, sizeof(params));
                memcpy(&data,buffer,sizeof(params));
                printf("data.operation: %d\n", data.operation);
                if (n < 0) {
                    perror("recv()");
                    exit(EXIT_FAILURE);
                } else if (n == 0) {
                    // Client has disconnected
                    printf("Client %d disconnected\n", fds[i].fd);
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    num_clients--;
                } else if (data.operation == DUMP) {
                    fp = fopen("hashtable_dump.txt", "a"); // open the file for appending                   
                    sprintf(buffer, "1,%d,%s\n", data.key, data.value);
                    fputs(buffer, fp);
                    strcpy(buffer, "we got it sir");
                    send(fds[i].fd, buffer, sizeof(buffer), 0);
                    fclose(fp);
                } else {
                    init_conn_to_kernel();
                    memcpy(NLMSG_DATA(nlh), &data, sizeof(data));
                    send_rcv_to_kernel();
                    rcv_data = (params*)NLMSG_DATA(nlh);
                    printf("Received message payload: %s, %d\n", rcv_data->value, rcv_data->key);
                    rc = send(fds[i].fd, rcv_data, sizeof(params), 0);
                    if (rc < 0) {
                        perror("send()");
                        exit(EXIT_FAILURE);
                    }
                    close(sock_fd);
                }
                sem_post(&mutex); 
            }
        }
    } 
    return 1;
}

void init_conn_to_kernel() {
    // Setup socket FD
    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
    if(sock_fd<0) {
        printf("Error creating socket\n");
        exit(EXIT_FAILURE);
    }

    memset(&saddr, 0, sizeof(saddr));
    saddr.nl_family = AF_NETLINK;
    saddr.nl_pid = getpid();
    saddr.nl_groups = 0;

    if(bind(sock_fd, (struct sockaddr*)&saddr, sizeof(saddr))) {
        perror("bind() error\n");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    memset(&daddr, 0, sizeof(daddr));
    daddr.nl_family = AF_NETLINK;
    daddr.nl_pid = 0;
    daddr.nl_groups = 0;

    // Send messages
    nlh = (struct nlmsghdr*)malloc(NLMSG_SPACE(MAX_BUFFER));
    memset(nlh, 0, NLMSG_SPACE(MAX_BUFFER));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_BUFFER);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;
    // set up the IO vector
    memset(&iov, 0, sizeof(struct iovec));
    iov.iov_base = (void*)nlh;
    iov.iov_len = nlh->nlmsg_len;
    memset(&msg, 0, sizeof(struct msghdr));
    msg.msg_name = (void *)&daddr;
    msg.msg_namelen = sizeof(daddr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
}

void send_rcv_to_kernel() {
    int rc;
    rc = sendmsg(sock_fd, &msg, MSG_NOSIGNAL);
    if (rc < 0) {
        perror("sending message");
        exit(EXIT_FAILURE);
    }
    rc = recvmsg(sock_fd, &msg, 0);
    if (rc < 0) {
        perror("receiving message:");
        exit(EXIT_FAILURE);
    }
}
