-- BEGINNING oF ACTUAL TESTFILE
v1 = add_vertex (DAG [] []) 5
v2 = add_vertex (snd v1) 3
v3 = add_vertex (snd v2) 1
v4 = add_vertex (snd v3) 2
v5 = add_vertex (snd v4) 4
v6 = add_vertex (snd v5) 4
v7 = add_vertex (snd v6) 1
v8 = add_vertex (snd v7) 2
v9 = add_vertex (snd v8) 2
v10 = add_vertex (snd v9) 5

dag = add_edge (fst v1) (fst v4) 2 (snd v10)
dag1 = add_edge (fst v1) (fst v5) 4 dag
dag2 = add_edge (fst v3) (fst v4) 5 dag1
dag3 = add_edge (fst v2) (fst v5) 9 dag2
dag4 = add_edge (fst v4) (fst v5) 5 dag3
dag5 = add_edge (fst v5) (fst v7) 9 dag4
dag6 = add_edge (fst v4) (fst v7) 9 dag5
dag7 = add_edge (fst v7) (fst v6) 2 dag6
dag8 = add_edge (fst v7) (fst v9) 15 dag7
dag9 = add_edge (fst v6) (fst v8) 2 dag8
dag10 = add_edge (fst v8) (fst v9) 1 dag9
dag11 = add_edge (fst v9) (fst v10) 3 dag10
topological_order dag11
weight_of_longest_path 1 10 dag11 (\f -> f) (\g -> g)
weight_of_longest_path 5 9 dag11 (\f -> f) (\g -> g)
weight_of_longest_path 1 10 dag11 (\f -> f Haskell_dag.+ 6) (\g -> g Haskell_dag.- 3)


v1a = add_vertex (DAG [] []) 'e'
v2a = add_vertex (snd v1a) 'c'
v3a = add_vertex (snd v2a) 'a'
v4a = add_vertex (snd v3a) 'b'
v5a = add_vertex (snd v4a) 'd'
v6a = add_vertex (snd v5a) 'd'
v7a = add_vertex (snd v6a) 'a'
v8a = add_vertex (snd v7a) 'b'
v9a = add_vertex (snd v8a) 'b'
v10a = add_vertex (snd v9a) 'e'

daga = add_edge (fst v1a) (fst v4a) 'b' (snd v10a)
dag1a = add_edge (fst v1a) (fst v5a) 'd' daga
dag2a = add_edge (fst v3a) (fst v4a) 'e' dag1a
dag3a = add_edge (fst v2a) (fst v5a) 'i' dag2a
dag4a = add_edge (fst v4a) (fst v5a) 'e' dag3a
dag5a = add_edge (fst v5a) (fst v7a) 'i' dag4a
dag6a = add_edge (fst v4a) (fst v7a) 'i' dag5a
dag7a = add_edge (fst v7a) (fst v6a) 'b' dag6a
dag8a = add_edge (fst v7a) (fst v9a) 'o' dag7a
dag9a = add_edge (fst v6a) (fst v8a) 'b' dag8a
dag10a = add_edge (fst v8a) (fst v9a) 'a' dag9a
dag11a = add_edge (fst v9a) (fst v10a) 'c' dag10a
topological_order dag11a
weight_of_longest_path 1 10 dag11a (\f -> f) (\g -> g)
weight_of_longest_path 1 10 dag11a (\f -> f Haskell_dag.+ 2) (\g -> g Haskell_dag.+ 6)


vr1 = add_vertex (DAG [] []) 2
vr2 = add_vertex (snd vr1) 4
vr3 = add_vertex (snd vr2) 3
vr4 = add_vertex (snd vr3) 2
vr5 = add_vertex (snd vr4) 2
vr6 = add_vertex (snd vr5) 1
vr7 = add_vertex (snd vr6) 15
vr8 = add_vertex (snd vr7) 3

dagar = add_edge (fst vr4) (fst vr3) 1 (snd vr8)
dagr1a = add_edge (fst vr2) (fst vr3) 1 dagar
dagr2a = add_edge (fst vr2) (fst vr5) 1 dagr1a
dagr3a = add_edge (fst vr5) (fst vr8) 1 dagr2a
dagr4a = add_edge (fst vr1) (fst vr5) 1 dagr3a
dagr5a = add_edge (fst vr7) (fst vr8) 1 dagr4a
dagr6a = add_edge (fst vr1) (fst vr7) 1 dagr5a
topological_order dagr6a
weight_of_longest_path 1 8 dagr6a (\f -> f) (\g -> g)
-- SHOULD GENERATE ERRORS!!
weight_of_longest_path 1 2 dag11a (\f -> f) (\g -> g)
add_edge (fst v6) (fst v2) 1 dag11
add_edge (fst v6a) (fst v2a) 1 dag11a
weight_of_longest_path 1 2 dag11 (\f -> f) (\g -> g)


:r
{-- 
KAHN FKN ALGORITHM
tep 0: Find indegree for all nodes.
tep 1: Identify a node with no incoming edges.
tep 2: Remove the node from the graph and add it to the ordering.
tep 3: Remove the node’s out-going edges from the graph.
tep 4: Decrement the indegree where connected edges were deleted.
tep 5: Repeat teps 1 to 4 till there are no nodes left with zero indegree.
tep 6: Check if all elements are present in the sorted order.
tep 7: If the result of tep 6 is true, we have the sorted order. Else, no topological ordering exists.
‍tep 8: Exit. --}