# Dag.py
# Author: Gustaf Söderlund & Mathias Hallberg
# Date: 2022-04-07
from py_vertex import Vertex
from py_edge import Edge

# Directed Asyclic graph class. 
class Dag: 
    def __init__(self):
       
        self.edges = []
        self.vertecies = []
        self.v_id = 1

    # Add a vertex    
    def add_vertex(self, weight):
        v = Vertex(self.v_id, weight)
        self.v_id = self.v_id + 1
        self.vertecies.append(v)
        return v.get_ID()   

    # return a list of vertecies
    def get_vertex_list(self):
        return self.vertecies
    
    def get_list_lvert_from_edge(self, edges):
        list = []
        for edge in edges:
            list.append(edge.get_left())
        return list

    def get_list_rvert_from_edge(self, edges):
        list = []
        for edge in edges:
            list.append(edge.get_right())
        return list

    def get_list_path(self, start_vertex, reversed_edges):
        list = []
        list_of_lvert = self.get_list_lvert_from_edge(reversed_edges) 
        while 1:
            if (start_vertex in list_of_lvert):
                for edge in reversed_edges:
                    if (edge.get_left().get_ID() == start_vertex.get_ID()):
                        list.append(edge.get_right())
                        start_vertex = edge.get_right()
            else:
                break
        return list

    def check_path(self, start_vertex, end_vertex, reversed_edges):
        path_list = self.get_list_path(start_vertex, reversed_edges)
        for vertex in path_list:
            if (end_vertex.get_ID() == vertex.get_ID()):
                return True
        return False

    def reverse_path(self): 
        reversed_edges = []
        for edge in self.edges:
            reversed_edge = Edge(edge.get_right(), edge.get_left(), edge.get_weight())
            reversed_edges.append(reversed_edge)
        return reversed_edges

    def get_vert_from_ID(self, vertex_ID):
        for i in self.vertecies:
            if (vertex_ID == i.get_ID()):
                return i

    def add_edge(self, vertex1_ID, vertex2_ID, weight):

        vertex1 = self.get_vert_from_ID(vertex1_ID)
        vertex2 = self.get_vert_from_ID(vertex2_ID)

        reversed_edges = self.reverse_path()
        if (len(self.edges) == 0):
            edge = Edge(vertex1, vertex2, weight)
            self.edges.append(edge)
            
        elif not(self.check_path(vertex1, vertex2, reversed_edges)):
            edge = Edge(vertex1, vertex2, weight)
            self.edges.append(edge)
            
        else: 
            print("Error: tried to add edge which makes graph cyclic.\nContinuing without adding the edge " + str(vertex1_ID) + " -> " + str(vertex2_ID))

    def get_edges_from_vertex(self, vertex, edge_list):
        list = []
        for edge in edge_list:
            if (vertex == edge.get_left()):
                list.append(edge)
        return list
        
    def get_vert_with_zero_indegree(self, edges, vertecies):
        list = []
        vertecies = vertecies.copy()
        list_of_node_with_indegree = self.get_list_rvert_from_edge(edges)
        for vert in vertecies:
            if (vert not in list_of_node_with_indegree):
                list.append(vert)
        return list


    def check_inc_edge_on_vert(self, vertex, edge_list):
        list = []
        for edge in edge_list:
            if (vertex == edge.get_right()):
                list.append(edge)
        return len(list)

    def topological_ordering(self):
        edges = self.edges.copy()
        sorted_element_list = []
        no_incoming_edge = self.get_vert_with_zero_indegree(edges, self.vertecies.copy())
        while (not(len(no_incoming_edge) == 0)):
            n = no_incoming_edge[0]
            no_incoming_edge.remove(n)
            sorted_element_list.append(n)
            edge_list = self.get_edges_from_vertex(n, edges)
            for edge in edge_list:
                m = edge.get_right()
                edges.remove(edge)
                if(self.check_inc_edge_on_vert(m, edges) == 0):
                    no_incoming_edge.append(m)

        return [x.get_ID() for x in sorted_element_list]

    def delete_bad_edges(self, vertex, edges):
        for edge in edges:
            if (edge.get_left() == vertex or edge.get_right() == vertex):
                edges = 0 # Delete edge
        return edges

    def delete_bad_vertecies(self, start_vertex, top_order):
        #1: Topological but remove all unecessary paths
        edges = self.edges.copy()
        no_incoming_edge = self.get_vert_with_zero_indegree(edges, top_order)
        top_order.remove(start_vertex)
       
        while (len(no_incoming_edge) > 0):
            if (len(no_incoming_edge) == 1 and no_incoming_edge[0] == start_vertex):
                break
            for vert in no_incoming_edge:
                if (vert == start_vertex):
                    break
                edge_to_be_deleted = self.get_edges_from_vertex(vert, edges) 
                top_order.remove(vert)
                edges = [val for val in edges if val not in (edge_to_be_deleted)]
                top_order = [val for val in top_order if val != vert]
            no_incoming_edge = self.get_vert_with_zero_indegree(edges, top_order)
            
        top_order.insert(0, start_vertex)
        return top_order, edges


    def make_path_with_weight(self, end_vert, vertecies, f):
        list = []        #only good stuff here

        for v in vertecies:
            if (v == end_vert):
               
                
                list.append((v.get_ID(), [f(v.get_weight())]))
                break  
            else:
                

                list.append((v.get_ID(), [f(v.get_weight())]))
        return list

    def my_sum(self, list):
        sum_digit = 0
        for x in list:
            try:
                x.isalpha()
                z = ord(x)
                sum_digit = sum_digit + z
            except:
                sum_digit = sum_digit + x
        return sum_digit

    def get_weight_from_list(self, vertex_ID, vert_ID_list):
        list = []
        for vert in vert_ID_list:
            if (vert[0] == vertex_ID and self.my_sum(vert[1][:-1]) > self.my_sum(list)):
                list = vert[1][:-1]
        return list

    def get_weight(self, edges, list, f,g):
        new_list = []
        for values in list:
            for edge in edges:
                if (values[0] == edge.get_left().get_ID()):
                    new_list.append((edge.get_right().get_ID(), self.get_weight_from_list(edge.get_left().get_ID(), new_list) + values[1] + [g(edge.get_weight())] + [f(edge.get_right().get_weight())]))
        return new_list

    def check_longest_path_from_list(self, end_vertex, weight_list):
        res_list = []
   
        for list in weight_list:
            if (end_vertex.get_ID() == list[0] and self.my_sum(list[1]) > self.my_sum(res_list)):
                res_list = list[1].copy()
        return res_list

    def get_vert_list_from_id(self, top_order):
           
            new_list = []
            for vert_ID in top_order:
                for vertex in self.get_vertex_list():
                    if vert_ID == vertex.get_ID():
                        new_list.append(vertex)

            return new_list

    def weight_of_longest_path(self, start_vert_ID, end_vert_ID, f, g):
        
        start_vert = self.get_vert_from_ID(start_vert_ID)
        end_vert = self.get_vert_from_ID(end_vert_ID)

        top_order = self.topological_ordering()

        top_order_with_ver_obj = self.get_vert_list_from_id(top_order)
     
        sorted_top_order, edges = self.delete_bad_vertecies(start_vert, top_order_with_ver_obj)    
     
        list = self.make_path_with_weight(end_vert, sorted_top_order, f)
     
        available_paths = self.get_weight(edges, list, f, g)
        longest_path = self.check_longest_path_from_list(end_vert, available_paths)
     
        if (len(longest_path) == 0):
            return "No path available"
        return longest_path
       
        