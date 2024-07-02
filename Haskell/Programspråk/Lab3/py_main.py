from py_dag import Dag

def main():

    f = lambda int : int
    g = lambda int : int
    
    dag = Dag()                                                 # create dag object. 
    v1 = dag.add_vertex(5) 
    v2 = dag.add_vertex(3)  
    v3 = dag.add_vertex(1)
    v4 = dag.add_vertex(2)                                      #Add vertex with some weight.
    v5 = dag.add_vertex(4)
    v6 = dag.add_vertex(4)                                    
    v7 = dag.add_vertex(1)
    v8 = dag.add_vertex(2)
    v9 = dag.add_vertex(2)
    v10 = dag.add_vertex(5)

    dag.add_edge(v1, v4, 2)                                     #Add edge with some Vertecies and Weight. 
    dag.add_edge(v1, v5, 4)
    dag.add_edge(v2, v5, 9)
    dag.add_edge(v3, v4, 5)
    dag.add_edge(v4, v5, 5)
    dag.add_edge(v4, v7, 9)
    dag.add_edge(v5, v1, 9)
    dag.add_edge(v7, v6, 2)
    dag.add_edge(v6, v8, 2)
    dag.add_edge(v8, v9, 1)
    dag.add_edge(v9, v10, 3)
    dag.add_edge(v7, v9, 15)
    dag.add_edge(v6, v2, 3)
    
    topological_order_list = dag.topological_ordering()             # topological ordering
    print(topological_order_list)
    
    longest_path = dag.weight_of_longest_path(v1,v2,f,g)  
    print(longest_path)     
    longest_path = dag.weight_of_longest_path(v5,v9,f,g) 
    print(longest_path)

    f = lambda int : int+6 
    g = lambda int : int-3 

    longest_path = dag.weight_of_longest_path(v1,v10,f,g) 
    print(longest_path)
 

    #Using Characters
    f = lambda int : int
    g = lambda int : int
   
    dag = Dag()                                                 # create dag object. 
    v1 = dag.add_vertex('e') 
    v2 = dag.add_vertex('c')  
    v3 = dag.add_vertex('a')
    v4 = dag.add_vertex('b')                                      #Add vertex with some weight.
    v5 = dag.add_vertex('d')
    v6 = dag.add_vertex('d')                                    
    v7 = dag.add_vertex('a')
    v8 = dag.add_vertex('b')
    v9 = dag.add_vertex('b')
    v10 = dag.add_vertex('e')

    dag.add_edge(v1, v4, 'b')                                     #Add edge with some Vertecies and Weight. 
    dag.add_edge(v1, v5, 'd')
    dag.add_edge(v2, v5, 'i')
    dag.add_edge(v3, v4, 'e')
    dag.add_edge(v4, v5, 'e')
    dag.add_edge(v4, v7, 'i')
    dag.add_edge(v5, v7, 'i')
    dag.add_edge(v7, v6, 'b')
    dag.add_edge(v6, v8, 'b')
    dag.add_edge(v8, v9, 'a')
    dag.add_edge(v9, v10, 'c')
    dag.add_edge(v7, v9, 'o')
    
    topological_order_list = dag.topological_ordering()             # topological ordering
    print(topological_order_list)
    
    longest_path = dag.weight_of_longest_path(v1,v10,f,g)  
    print(longest_path)
main()


