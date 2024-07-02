#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>
#include "graph.h"
#include "queue.h"

/**
 * This is a test program for a graph, checking if there is a way from one
 * node to another. The nodes are and graph are built from an input file
 * with the number of edges and and names of the nodes. The graph is iterated
 * with a breadth-first search
 *
 * Authors: Mathias Hallberg    (maha6129@student.umu.se)
 *          Gustaf Söderlund    (gusa0038@student.umu.se)
 *
 * Version: v.1.0 2020-04-06
 */

#define BUFSIZE 40

/**
 * first_non_white_space() - Finds the position of the first non white space
 *  @s: The string to be check
 *
 * Returns: Position to where to first non space is found.
 */
int first_non_white_space(const char *s) {
    int i = 0;
    while (s[i] && isspace(s[i])) {
        i++;
    }
    if (s[i]) {
        return i;
    }
    else {
        return -1;
    }
}

/**
 * line_is_blank() - Checks whether a line is blank and returns a bool depending on the result
 *  @s: The string to be check
 *
 * Returns: True if s only conatins whitespace
 */
bool line_is_blank(const char *s) {
    return first_non_white_space(s) < 0;
}

/**
 * line_is_comment() - Checks whether a line is a comment and returns a bool depending on the result
 *  @s: The string to be check
 *
 * Returns: True if s is a comment line, i.e first non-whitespace char is '#'
 */
bool line_is_comment(const char *s) {
    int i = first_non_white_space(s);
    return (i >= 0 && s[i] == '#');
}

/**
 * line_is_number() - Check if the line consists of just a number
 *  @s: The string to be check
 *
 * Returns: True if s is a number, otherwise false
 */
bool line_is_number(const char *s) {
    return isdigit(s[0]);
}

/* find_path() - checks if there is a path in the given graph
 * @g: The given graph to inspect
 * @src: The source where we begin
 * @dest: The destination to look if there exists a path to
 *
 * Returns: True if there is a path and false if not.
 */
bool find_path(graph *g, node *src, node *dest) {

    // Quick check if the nodes are the same and if, they should return true
    if (nodes_are_equal(src, dest)) {
        return true;
    }

    dlist *nghbrs;

    // Initializing a queue to use breadth-first search
    queue *q = queue_empty(NULL);

    // Setting src node to seen and enqueueing it
    graph_node_set_seen(g, src, true);
    queue_enqueue(q, src);

    while(!queue_is_empty(q)) {

        /* Inserting the front of the queue to a variable and dequeueing it
         * while inserting it to a directed list
         */
        node *front = queue_front(q);
        q = queue_dequeue(q);
        nghbrs = graph_neighbours(g, front);
        dlist_pos pos = dlist_first(nghbrs);

        while (!dlist_is_end(nghbrs, pos)) {

            // Inspecting the neighbours and inserting it to a node to compare with
            node *cmp_node = dlist_inspect(nghbrs, pos);

            if(nodes_are_equal(dest, cmp_node)) {

                // Path found, clean up
                dlist_kill(nghbrs);
                queue_kill(q);
                return true;
            }

            // Check if the node we are working with hasn't been seen before
            if (!graph_node_is_seen(g, cmp_node)) {

                // Set it to seen and dequeing it.
                graph_node_set_seen(g, cmp_node, true);
                queue_enqueue(q, cmp_node);

            }

            // Move on to the next position and repeat
            pos = dlist_next(nghbrs, pos);
        }

        // Path have not been found, clean up the temporary directed list
        dlist_kill(nghbrs);
    }

    // Path Was not found, clean up and return false
    queue_kill(q);
    return false;
}
/**
 * build_graph() - Returns a graph from the file that has been given
 * @fp: File to inspect
 *
 * Returns: A complete graph built from the input file
 */
graph *build_graph(FILE *fp) {

    graph *g;
    int edge, edge_check = 0;
    char line[BUFSIZE];

    // While there is a line thats not empty, run the loop
    while (fgets(line, BUFSIZE, fp) != NULL) {


        // Checking wheter the line is blank and a comment and ignoring it if it is
        if(!(line_is_blank(line) || line_is_comment(line))) {

            // Check if the line is a number, which will be our edges and save it to a variable
            if(line_is_number(line)) {

                edge = atoi(line);
                g = graph_empty(edge);
            }
            else {

                char n1[BUFSIZE], n2[BUFSIZE];

                // Scanning the nodes and saving it to variables
                int inputs = sscanf(line,"%40s %40s", n1, n2);

                // If the input while has less than one node show an error and exit
                if (inputs != 2) {
                    fprintf(stderr, "ERROR: input file is incorrect.\n\n");
                    graph_kill(g);
                    exit(EXIT_FAILURE);
                }

                // Inserting node to the graph
                g = graph_insert_node(g, n1);
                g = graph_insert_node(g, n2);

                // Making our strings to actual nodes
                node *node_1 = graph_find_node(g, n1);
                node *node_2 = graph_find_node(g, n2);

                // Adding +1 to edges to later check if the right amount of edges were correct
                edge_check++;

                // Inserting and edge between node1 and node2.
                g = graph_insert_edge(g, node_1, node_2);

                }
            }
        }

        if (edge_check != edge) {

            fprintf(stderr, "Bad input file: invalid amout of edges.\n\n");
            graph_kill(g);
            exit(EXIT_FAILURE);
    }

    return g;
}
/**
 * user_input() - Reads the input from the user and prints
 *                if there is a way from one node to another
 * @g: Graph to inspect.
 *
 * Returns: Nothing
 */
void user_input(graph *g) {

    char source_val[BUFSIZE] = "", dest_val[BUFSIZE] = "";
    char buffer_place[81];
    char exit_status[] = "quit";

    // Running the program while quit hasn't been entered by the user
    while (strcmp(exit_status, source_val) != 0) {


        printf("Enter origin and destination (quit to exit): ");

        // Saving the input to a buffer and then scanning it into 2 different variables
        fgets(buffer_place, 80, stdin);
        int usr_inp = sscanf(buffer_place, "%40s %40s", source_val, dest_val);

        // Comparing if the first input is quit and do a normal exit of the program if thats the case
        if (strcmp(exit_status, source_val) == 0) {
            printf("Normal exit.\n\n");
            graph_kill(g);
            exit(EXIT_SUCCESS);
        }

        // If the user hasn't entered two nodes give a warning sign
        if (usr_inp != 2) {
            printf("Your input was wrong, only two nodes and respectively max 40 characters per node\n\n");
            continue;
        }

        // Looking if the nodes entered was actually valid inputs
        else if (usr_inp == 2) {
            if (graph_find_node(g, source_val) == NULL || graph_find_node(g, dest_val) == NULL) {
                printf("That is not a valid airport, enter a valid airport.\n\n");
                continue;
            }
        }

        // Making the inputs to actual nodes
        node *source = graph_find_node(g, source_val);
        node *dest = graph_find_node(g, dest_val);

        // Calling the find_path function and if a path is found return it was a succes
        if (find_path(g, source, dest)) {
            printf("There is a path from %s to %s.\n\n", source_val, dest_val);
        }

        // Path not found, letting the user know
        else {
            printf("There is no path from %s to %s.\n\n", source_val, dest_val);
        }

        // Making the whole graph not seen
        g = graph_reset_seen(g);
    }
}

int main(int argc, char **argv){

    graph *g;
    FILE *fp = fopen(argv[1], "rb");

    // Checking if the file was opened correctly
    if (fp == NULL) {

        fprintf(stderr, "ERROR: failed to open file\n\n");
        exit(EXIT_FAILURE);
    }

    g = build_graph(fp);

    // close files before exit
    if (fclose(fp)) {
        graph_kill(g);
        fprintf(stderr, "ERROR: failed to close file.\n\n");
        exit(EXIT_FAILURE);
        return -1;
    }

    user_input(g);

    return 0;
}
