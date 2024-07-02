#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <stdint.h>
#include <sys/socket.h>  
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>

//Definitions.
#define INSERT 1
#define REMOVE 2
#define LOOKUP 3
#define READFILE 4
#define EXIT 5
#define MAX_BUFFER 1024
#define MAX_VALUE 64
#define BUF_SIZE 2000 

/**
 * Struct for values to module, operations decides if we
 * should do a lookup, insert or remove and key and value is
 * the key and value to remove or lookup.
 */
typedef struct {
    int operation;
    int key;
    char value[MAX_VALUE];
} params;

void send_rcv_insert(params data);
void send_rcv_lookup(params data);
void send_rcv_remove(params data);

int sockfd, ret;  
params data;
params recv_data;
char recv_buffer[sizeof(params)];
char buffer[sizeof(params)];
int main(int argc, char **argv){
    
    clock_t start, end;
    double cpu_time_used;
    int len = 0;
    char filename[100];
    FILE *file;
    char line[1000];
    char *token;
    int key_val = 0;
    int i = 0;
    int arg = 0;
    int key_to_remove;
    int key_to_LOOKUP;
    int file_descriptor;
    struct sockaddr_in addr, cl_addr;  

    struct hostent *server;
    char *serverAddr;



    if (argc != 3) {
        printf("usage: client < ip address > < port >\n");
        exit(EXIT_FAILURE);  
    }

    serverAddr = argv[1]; 
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);  
    if (sockfd < 0) {  
        perror("socket()");
        exit(1);  
    }  
    printf("Socket created...\n");   

    memset(&addr, 0, sizeof(addr));  
    addr.sin_family = AF_INET;  
    addr.sin_addr.s_addr = inet_addr(serverAddr);
    addr.sin_port = ntohs(atoi(argv[2]));     

    ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
    if (ret < 0) {  
        perror("connect()");  
        exit(EXIT_FAILURE);
    }  
    printf("Connected to the server...\n");  
    // main loop. ATM 10 iterations just to not fuck the module
    while(1) {
        char command_choice, write_buf[100], read_buff[100];
        printf(" 1) Insert\n 2) Remove\n 3) Lookup\n 4) Read from file\n 5) Exit program\n");
        printf("select input: ");
        scanf("%d", &arg);
        switch (arg){
            case INSERT:
                data.operation = arg;
                
                printf("Message: ");
                scanf("%s",read_buff);
                strcpy(data.value, read_buff);
                
                printf("Key value: ");
                scanf("%d", &key_val);
                data.key = key_val;
               
                printf("Inserting value %s to hashtable with key %d into kernel\n", data.value, data.key);
                
                send_rcv_insert(data);
                break;

            case REMOVE:
                printf("Select key to remove: ");
                scanf("%d", &key_to_remove);
                data.operation = arg;
                data.key = key_to_remove;
                strcpy(data.value, "");
                printf("Removing value from hashtable in kernel\n");
                
                send_rcv_remove(data);
                break;

            case LOOKUP:
                printf("Select key to Lookup: ");
                scanf("%d", &key_to_LOOKUP);
                data.operation = arg;
                data.key = key_to_LOOKUP;
                strcpy(data.value, "test");
                printf("Calling lookup from hashtable in kernel\n");
                
                send_rcv_lookup(data);
                break;
            case READFILE:
                printf("Enter the files name: ");
                scanf("%s", filename);
                if (access(filename, F_OK) == -1) {
                    printf("File does not exist\n");
                    break;
                }
                file = fopen(filename, "r");
                if (file == NULL) {
                    printf("Error opening file.");
                    fclose(file);
                    break;
                }
                params data;
                int counter = 0;
                start = clock();
                while (fgets(line, sizeof(line), file)) {
                    line[strcspn(line, "\n")] = '\0';
                    token = strtok(line, ",");
                    data.operation = atoi(token);
                    token = strtok(NULL, ",");
                    data.key = atoi(token);
                    token = strtok(NULL, ",");                    
                    strcpy(data.value, token);
                    if (data.operation == INSERT) {
                        send_rcv_insert(data);
                    } else if (data.operation == REMOVE) {
                        send_rcv_remove(data);
                    } else if (data.operation == LOOKUP) {
                        send_rcv_lookup(data);
                    }
                    counter++;
                }
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

                printf("Execution time: %f seconds to do %d operations\n", cpu_time_used, counter);
                printf("Time per instruction: %f\n", cpu_time_used / counter);
                break;
            case EXIT:
                printf("Exiting...\n");
                close(sockfd);
                exit(EXIT_SUCCESS);
        }    
    }
    close(sockfd);
    
    // close(file_descriptor);
    return 0;
}

void send_rcv_insert(params data) {
    memcpy(buffer, &data, sizeof(data));
    ret = send(sockfd, buffer, sizeof(buffer), 0);
    if (ret < 0) {
        perror("send()");
        exit(EXIT_FAILURE);
    }
    ret = recv(sockfd, recv_buffer, BUF_SIZE, 0);
    if (ret < 0) {
        perror("recv()");
        exit(EXIT_FAILURE);
    }
    memset(&recv_data, 0, sizeof(params));
    memcpy(&recv_data, recv_buffer, sizeof(params));
    printf("Inserted value: %s, key: %d\n", recv_data.value, recv_data.key);
}

void send_rcv_remove(params data) {
    memcpy(buffer, &data, sizeof(data));
    ret = send(sockfd, buffer, sizeof(buffer), 0);
    if (ret < 0) {
        perror("send()");
        exit(EXIT_FAILURE);
    }
    ret = recv(sockfd, recv_buffer, BUF_SIZE, 0);
    if (ret < 0) {
        perror("recv()");
        exit(EXIT_FAILURE);
    }
    memset(&recv_data, 0, sizeof(params));
    memcpy(&recv_data, recv_buffer, sizeof(params));
    printf("Entry with key: %d has been removed\n", recv_data.key);
}


void send_rcv_lookup(params data) {

    memcpy(buffer, &data, sizeof(data));
    ret = send(sockfd, buffer, sizeof(buffer), 0);
    if (ret < 0) {
        perror("send()");
        exit(EXIT_FAILURE);
    }
    ret = recv(sockfd, recv_buffer, BUF_SIZE, 0);
    if (ret < 0) {
        perror("recv()");
        exit(EXIT_FAILURE);
    }
    memset(&recv_data, 0, sizeof(params));
    memcpy(&recv_data, recv_buffer, sizeof(params));
    printf("Lookup | value: %s, key: %d\n", recv_data.value, recv_data.key);
}
