#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "dlist.h"

/*
 * Implementation of a generic, graph for the
 * "Datastructures and algorithms" courses at the Department of
 * Computing Science, Umea University.
 *
 * Authors: Mathias Hallberg    (maha6129@student.umu.se)
 *	        Gustaf Söderlund    (gusa0038@student.umu.se)
 *
 *
 * Version information:
 *   2019-04-06: v1.0, first public version.
 */

// ===========INTERNAL DATA TYPES============

/*
 * The nodes are inserting into a directed list with another list
 * as neighbours which understandably is the neighbours. The bool for seen is
 * whether the node has been seen or not and the value is the nodes name.
 */

struct graph {
    dlist *entries;
    int edges_number;
};

struct node {
    dlist *nghbrs;
    bool seen;
    char *value;

};

// =================== NODE COMPARISON FUNCTION ======================

/**
 * nodes_are_equal() - Check whether two nodes are equal.
 * @n1: Pointer to node 1.
 * @n2: Pointer to node 2.
 *
 * Returns: true if the nodes are considered equal, otherwise false.
 */
bool nodes_are_equal(const node *n1, const node *n2) {

    // Checking if the nodes are equal and returning true if thats the case otherwise false
    if (n1->value == n2->value) {
        return true;
    }
    else {
        return false;
    }
}

// =================== GRAPH STRUCTURE INTERFACE ======================

/**
 * graph_empty() - Create an empty graph.
 * @max_nodes: The maximum number of nodes the graph can hold.
 *
 * Returns: A pointer to the new graph.
 */
graph* graph_empty(int max_nodes) {

    // ALlocate memory for the graph
    graph *g = malloc(1*sizeof(graph));

    // g->entries set to a directed list
    g->entries = dlist_empty(free);

    return g;
}

/**
 * graph_is_empty() - Check if a graph is empty, i.e. has no nodes.
 * @g: Graph to check.
 *
 * Returns: True if graph is empty, otherwise false.
 */
bool graph_is_empty(const graph *g) {

    return (g->entries == NULL);

}

/**
 * graph_insert_node() - Inserts a node with the given name into the graph.
 * @g: Graph to manipulate.
 * @s: Node name.
 *
 * Creates a new node with a copy of the given name and puts it into
 * the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_node(graph *g, const char *s) {

    // Node not found return the same graph
    if (graph_find_node(g, s) != NULL) {
        return g;
    }

    // Allocating memory for a struct node
    struct node *n = calloc(1,sizeof(struct node));

    // Allocating memory for the value of the node
    n->value = malloc(40* sizeof(char));

    // Making a directed list from the node to store the neighbours
    n->nghbrs = dlist_empty(NULL);

    // Copying the input s to n->value and storing it in the struct
    n->value = strcpy(n->value, s);

    // Inserting the node in g->entries on first empty position
    dlist_insert(g->entries, n, dlist_first(g->entries));

    // Setting the node to not have been seen
    graph_node_set_seen(g, n, false);

    return g;
}

/**
 * graph_find_node() - Find a node stored in the graph.
 * @g: Graph to manipulate.
 * @s: Node identifier, e.g. a char *.
 *
 * Returns: A pointer to the found node, or NULL.
 */
node *graph_find_node(const graph *g, const char *s) {

    // inspecting the first position in the directed list and storing the that pos to pos
    dlist_pos pos = dlist_first(g->entries);

    while (!dlist_is_end(g->entries, pos)) {

        // Inspecting the list with given pos in g->entries and saving it as a node
        struct node *n = dlist_inspect(g->entries, pos);

        // Comparing input s and n->value and if they are the same return the node
        if (strcmp(s, n->value) == 0) {
            return n;
        }

    // Move to next pos if node was not found
    pos = dlist_next(g->entries, pos);
    }

    // Returning NULL if no node was found
    return NULL;
}

/**
 * graph_insert_edge() - Insert an edge into the graph.
 * @g: Graph to manipulate.
 * @n1: Source node (pointer) for the edge.
 * @n2: Destination node (pointer) for the edge.
 *
 * NOTE: Undefined unless both nodes are already in the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_edge(graph *g, node *n1, node *n2) {

    // Inserting n2 to n1 directed list as a neighbour on first given pos
    dlist_insert(n1->nghbrs, n2, dlist_first(n1->nghbrs));
    return g;
}

/**
 * graph_has_edges() - Check if a graph has any edges.
 * @g: Graph to check.
 *
 * Returns: True if graph has any edges, otherwise false.
 */
bool graph_has_edges(const graph *g) {

    // If the graph has edges return true otherwise false
    if (g->edges_number > 0) {
        return true;
    }
    return false;
}

/**
 * graph_node_is_seen() - Return the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to return seen status for.
 *
 * Returns: The seen status for the node.
 */
bool graph_node_is_seen(const graph *g, const node *n) {

    // Return true if n has been seen otherwise false
    return n->seen;
}
/**
 * graph_node_set_seen() - Set the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to set seen status for.
 * @s: Status to set.
 *
 * Returns: The modified graph.
 */
graph *graph_node_set_seen(graph *g, node *n, bool seen) {

    // Setting the node to either true or false depending on the parameter seen
    n->seen = seen;
    return g;
}

/**
 * graph_reset_seen() - Reset the seen status on all nodes in the graph.
 * @g: Graph to modify.
 *
 * Returns: The modified graph.
 */
graph *graph_reset_seen(graph *g) {

    // Simply iterating through the list and reseting all the node to not have been seen
    dlist_pos pos = dlist_first(g->entries);

    while(!dlist_is_end(g->entries, pos)) {
        struct node *n = dlist_inspect(g->entries, pos);
        n->seen = false;
        pos = dlist_next(g->entries, pos);
    }
    return g;
}

/**
 * *() - Return a list of neighbour nodes.
 * @g: Graph to inspect.
 * @n: Node to get neighbours for.
 *
 * Returns: A pointer to a list of nodes. Note: The list must be
 * dlist_kill()-ed after use.
 */
dlist *graph_neighbours(const graph *g, const node *n) {

    // Making a list of neighbours to return and storing the n->neighbours first pos to pos
    dlist *nghbrs_list = dlist_empty(NULL);
    dlist_pos pos = dlist_first(n->nghbrs);

    while(!dlist_is_end(n->nghbrs, pos)) {

        // Inspect n's neighbours and storing it to the new directed list on the first arbitrary position
        dlist_insert(nghbrs_list, dlist_inspect(n->nghbrs, pos), dlist_first(nghbrs_list));
        pos = dlist_next(n->nghbrs, pos);
    }

    return nghbrs_list;
}

/**
 * graph_kill() - Destroy a given graph.
 * @g: Graph to destroy.
 *
 * Return all dynamic memory used by the graph.
 *
 * Returns: Nothing.
 */
void graph_kill(graph *g) {

    // Storing first pos of g->entries
    dlist_pos pos = dlist_first(g->entries);

    while(!dlist_is_end(g->entries, pos)) {

        // Inspecting g->entries on pos and storing it to n
        struct node *n = dlist_inspect(g->entries, pos);

        // Clean up the neighbours
        dlist_kill(n->nghbrs);
        free(n->value);

        // Iterate through the whole list
        pos = dlist_next(g->entries, pos);
    }

    // Clean up the original list
    dlist_kill(g->entries);
    free(g);
}
