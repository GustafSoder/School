#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>
#include <linux/init.h>	
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/hashtable.h> // hashtable API
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/inet.h>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8884

#define NETLINK_USER 31

#define INSERT 1
#define REMOVE 2
#define LOOKUP 3
#define DUMP 5

#define DEVICE_NAME "mydevice"
#define MAX 64

// Structs
typedef struct {
    int operation;
    int key;
    char value[MAX];
    struct hlist_node node;
} params;

struct h_node {
    int data;
    char name[MAX];
    struct hlist_node node;
};

struct my_device {
	struct cdev cdev;
};

// Declaration
static params perform_ht_operations(params *data);
static void netlink_receive(struct sk_buff *skb);
static u32 hash_func(u32 key);


struct sock *netlink_socket;

//init
DECLARE_HASHTABLE(ht, 3);
static int __init my_init_module(void) {

    struct netlink_kernel_cfg cfg = {
        .input = netlink_receive,
    };
    netlink_socket = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
    printk("Entering: %s, protocol family = %d \n",__FUNCTION__, NETLINK_USER);
    if(!netlink_socket) {
        printk(KERN_ALERT "Error creating socket.\n");
        return -ENOMEM;
    }
	 // Initialize the hashtable.
    printk(KERN_INFO "Device loaded\n");
    hash_init(ht);
    printk(KERN_INFO "Hashtable initialized\n");
    return 0;
}

//cleanup
static void __exit my_cleanup_module(void) {

    struct h_node *node, *tmp;
    int i;
    struct socket *sock;
    struct sockaddr_in server_addr;
    int ret;

    // Create a new TCP socket
    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (ret < 0) {
        printk(KERN_ERR "Error creating socket\n");
    }

    // Set the server address and port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = in_aton(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    ret = sock->ops->connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr), 0);
    if (ret < 0) {
        printk(KERN_ERR "Error connecting to server\n");
        sock_release(sock);
    }
    printk(KERN_INFO "LETS START DUMPING!!!!!!!");
    // Remove all elements from the hashtable.'
    params data;
    hash_for_each(ht, i, tmp, node) {
        struct msghdr msg = {.msg_name = NULL,
                         .msg_namelen = 0,
                         .msg_control = NULL,
                         .msg_controllen = 0,
                         .msg_flags = 0};
        data.key = tmp->data;
        data.operation = DUMP;
        strcpy(data.value, tmp->name);
        struct kvec iov = { &data, sizeof(data) };
        printk(KERN_INFO "sending: value: %s, key: %d, operation: %d\n", data.value, data.key, data.operation);
        ret = kernel_sendmsg(sock, &msg, &iov, 1, sizeof(params));
        if (ret < 0) {
            printk(KERN_ERR "Error sending data over socket\n");
        }
        char recv_str[1024];
        struct msghdr recv_msg = {.msg_name = NULL,
                            .msg_namelen = 0,
                            .msg_control = NULL,
                            .msg_controllen = 0,
                            .msg_flags = 0};
        struct kvec recv_iov = { &recv_str, sizeof(recv_str) };
        ret = kernel_recvmsg(sock, &recv_msg, &recv_iov, 1, sizeof(recv_str), 0);
        if (ret < 0) {
            printk(KERN_ERR "Error receiving string over socket\n");
        } else {
            printk(KERN_INFO "Received string: %s\n", recv_str);
        }
    }
    hash_for_each(ht, i, tmp, node) {
        hash_del(&tmp->node);
    }
    // Free the memory used by the hashtable.
    kfree(ht);
    
    sock_release(sock);
    netlink_kernel_release(netlink_socket);
    printk(KERN_INFO "Device unloaded\n");
}

// receive socket
static void netlink_receive(struct sk_buff *skb) {

    struct nlmsghdr *nlh;
    int pid;
    struct sk_buff *skb_out;
    params *data, resp;
    int msg_size;
    int res;

    printk(KERN_INFO "Entering: %s\n", __FUNCTION__);
    nlh = (struct nlmsghdr*)skb->data;
    data = (params*)NLMSG_DATA(nlh);
    resp = perform_ht_operations(data);
    pid = nlh->nlmsg_pid; /* pid of sending process */

    printk(KERN_INFO "Received data: field1=%d, field2=%s\n", data->key, data->value);
 
    msg_size = sizeof(params);
    skb_out = nlmsg_new(msg_size,0);

    if(!skb_out) {
        printk(KERN_ERR "Failed to allocate new skb\n");
        return;
    }

    nlh = nlmsg_put(skb_out,0,0,NLMSG_DONE,msg_size,0);

    NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */

    memcpy(nlmsg_data(nlh), &resp, msg_size);
    printk(KERN_INFO "RETURNING: value: %s, key: %d\n", resp.value, resp.key);
    res = nlmsg_unicast(netlink_socket, skb_out, pid);

    if(res<0) {
        printk(KERN_INFO "Error while sending back to user\n");
    }
}


static params perform_ht_operations(params *data) {
    
    params resp;
    u32 key = data->key;
    struct h_node *node, *cur;

    // Allocate memory for the node.
    node = kmalloc(sizeof(struct h_node), GFP_KERNEL);
    if (!node) {
        pr_err("myhashtable: failed to allocate memory for node\n");
        return resp;
    }

    switch (data->operation){

        case INSERT:  
            node->data = data->key;
            resp.key = data->key;
            resp.operation = INSERT;
            strcpy(resp.value, data->value);
            strcpy(node->name, data->value);
            hash_for_each_possible(ht, cur, node, hash_func(data->key)) {
                if (cur->data == data->key) {
                    resp.key = -1;
                    resp.operation = INSERT;
                    strcpy(resp.value, "Tried to insert duplicate key");
                    printk(KERN_INFO "Trying to insert duplicate key, continuing without inserting\n");
                    return resp;
                }
            }
            hash_add(ht, &node->node, hash_func(key));
            return resp;

        case REMOVE:
            printk(KERN_INFO "Doing remove\n");
            resp.key = data->key;
            resp.operation = REMOVE;
            strcpy(resp.value, data->value);

            // Get the element with the specified key.
            hash_for_each_possible(ht, cur, node, hash_func(data->key)) {
                if (cur->data == data->key) {
                    
                    hash_del(&cur->node);
                    pr_info("Removed element: data = %d, name = %s", cur->data, cur->name);
                    kfree(cur);
                    break;
                }
            }
            
            return resp;

        case LOOKUP:
            printk(KERN_INFO "Doing lookup\n");

            // Get the element with the specified key.
            hash_for_each_possible(ht, cur, node, hash_func(data->key)) {
                if (cur->data == data->key) {
                    pr_info("LOOKUP myhashtable: element found! data = %d, name = %s\n",
                            cur->data, cur->name);
                    resp.key = cur->data;
                    strcpy(resp.value, cur->name);
                    resp.operation = LOOKUP;
                    return resp;
                }
            }
            resp.key = -1;
            strcpy(resp.value, "No value with that key");
            break;
    }
    return resp;
}

static u32 hash_func(u32 key) {
    u32 hash = key;
    // Perform several rounds of mixing to produce a good hash value
    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;
    return hash;
}



module_init(my_init_module);
module_exit(my_cleanup_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mathias Hallberg, Gustaf Söderlund\n");
MODULE_DESCRIPTION("MODULE");
