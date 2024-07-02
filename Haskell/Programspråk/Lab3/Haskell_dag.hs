module Haskell_dag (Vertex, Edge, DAG, add_vertex, add_edge, topological_order, weight_of_longest_path) where
import Data.Map
import Data.List hiding (sum)
import Data.Char (chr, ord)


class (Eq a) => Num a where
    (+) :: a -> a -> a
    (-) :: a -> a -> a
    sum :: [a] -> a
instance Haskell_dag.Num Integer where
    i1 + i2 = i1 Prelude.+ i2
    i1 - i2 = i1 Prelude.- i2
    sum i1 = Prelude.sum i1
instance Haskell_dag.Num Char where
    c1 + c2 = chr (ord c1 Prelude.+ ord c2)
    c1 - c2 = chr (ord c1 Prelude.- ord c2)
    sum c3 = Data.List.foldl (Haskell_dag.+) '\0' c3
instance Prelude.Num Char where
    c1 + c2 = c1 -- SILENCE WARNINGS, NEVER USED
    c1 * c2 = c1 
    abs c1 = c1
    signum c1 = c1
    negate c1 = c1
    c1 - c2 = c1
    -- Only one necessary to convert char to actual integer for comparison
    fromInteger = chr.fromInteger

data Vertex a = Vertex Integer a deriving (Show, Ord, Eq, Read)
data Edge a = Edge (Vertex a) (Vertex a) a deriving (Show, Ord, Eq, Read)
data DAG a = DAG [Edge a] [Vertex a] deriving (Show, Ord, Eq, Read)

-- Add vertex to DAG
add_vertex :: DAG a -> a -> (Integer, DAG a)
add_vertex (DAG edges vertecies) weight
    | length vertecies == 0 = (1, DAG edges [Vertex 1 weight])
    | otherwise = (succ $ get_vertex_ID (last vertecies), (DAG edges (vertecies ++ [Vertex (succ $ get_vertex_ID(last vertecies)) weight])))

-- Adds edge to DAG if it doesnt make the graph cyclic
add_edge :: (Eq a) => Integer -> Integer -> a -> DAG a -> DAG a
add_edge vertex1_ID vertex2_ID weight (DAG edges vertecies) 
    | not (check_if_path_available vertex1 vertex2 [] (reverse_path $ get_list_of_edges dag)) = add_edge_to_DAG dag (Edge vertex1 vertex2 weight)
    | otherwise = error "Tried to enter edge which makes DAG cyclic"
    where
        dag = (DAG edges vertecies)
        vertex1 = find_vertex_from_ID vertex1_ID vertecies
        vertex2 = find_vertex_from_ID vertex2_ID vertecies

-- Finds vertex from from the vertex identifier
find_vertex_from_ID :: Integer -> [Vertex a] -> (Vertex a)
find_vertex_from_ID vertex_ID [] = error "Tried making an edge with a vertex that doesn't exist"
find_vertex_from_ID vertex_ID (x:xs)
    | get_vertex_ID x == vertex_ID = x
    | otherwise = find_vertex_from_ID vertex_ID xs

-- Checks if there is a path available from one vertex to another based on a list of vertecies and edges
check_if_path_available :: (Eq a) => Vertex a -> Vertex a -> [Vertex a] -> [Edge a] -> Bool
check_if_path_available _ _ [] [] = False
check_if_path_available vertex1 vertex2 visited edges
    | get_vertex_ID vertex1 == get_vertex_ID vertex2 = True
    | (get_vertex_ID vertex1) `elem` get_list_of_vertex_ID visited = False
    | otherwise = any (\nxt -> check_if_path_available nxt vertex2 (vertex1 : visited) edges) (list_of_succ vertex1 (get_list_of_paths edges))

-- Returns list of succesor from Vertex a and list of edges in vertecies 
list_of_succ :: (Eq a) => Vertex a -> [(Vertex a, Vertex a)] -> [Vertex a]
list_of_succ x [] = []
list_of_succ x ((y,z):list) 
    | get_vertex_ID x == get_vertex_ID y = z : list_of_succ x list
    | otherwise = list_of_succ x list

-- Returns the list of edges from DAG
get_list_of_edges :: DAG weight -> [Edge weight]
get_list_of_edges (DAG edges _) = edges

-- Returns the list of the path in vertecies from list of edges
get_list_of_paths :: [Edge weight] -> [(Vertex weight, Vertex weight)]
get_list_of_paths [] = []
get_list_of_paths (x:xs) = (get_left x, get_right x): get_list_of_paths xs

-- Reverses the path, i.e (Edge v1 v2 w) becomes (Edge v2 v1 w)
reverse_path :: [Edge weight] -> [Edge weight]
reverse_path [] = []
reverse_path ((Edge v1 v2 w):xs) =
    (Edge v2 v1 w): reverse_path xs

-- Adds an edge to the DAG
add_edge_to_DAG :: DAG weight -> Edge weight -> DAG weight
add_edge_to_DAG (DAG edges vertecies) newEdge = (DAG (edges++[newEdge]) vertecies)

-- Returns left Vertex from edge
get_left :: Edge weight -> Vertex weight
get_left (Edge v1 _ _) = v1

-- Return right Vertex from edge
get_right :: Edge weight -> Vertex weight
get_right (Edge _ v2 _) = v2

-- Return ID from vertex
get_vertex_ID :: Vertex weight -> Integer
get_vertex_ID (Vertex int _) = int

-- Get a list of ID's from list of vertecies
get_list_of_vertex_ID :: [Vertex a] -> [Integer]
get_list_of_vertex_ID [] = []
get_list_of_vertex_ID (x:xs) = 
    [get_vertex_ID x] ++ get_list_of_vertex_ID xs

-- Makes the DAG to a topological order
topological_order :: (Ord a) => DAG a -> [Integer]
topological_order dag = topological_order_helper ((find_indegree_of_vertecies dag dag)) dag

-- Makes list of ids with the amount of incoming edges, while being sorted
find_indegree_of_vertecies :: (Ord a, Eq a) => DAG a -> DAG a -> [(Integer, Integer)]
find_indegree_of_vertecies (DAG org_edges []) org_dag = []
find_indegree_of_vertecies (DAG [] (x:xs)) (DAG [] vertecies) = combine_indegree_with_vertices([(get_vertex_ID x,0)]) ++ find_indegree_of_vertecies (DAG [] (x:xs)) (DAG [] vertecies)
find_indegree_of_vertecies (DAG [] (x:xs)) (DAG org_edges org_vertecies) = combine_indegree_with_vertices (find_indegree_of_vertecies (DAG org_edges (xs)) (DAG org_edges (xs)))
find_indegree_of_vertecies (DAG (edge:xs) (vertex:ys)) org_dag 
    | get_vertex_ID vertex == get_vertex_ID(get_right edge) = combine_indegree_with_vertices ([(get_vertex_ID vertex, 1)] ++ find_indegree_of_vertecies (DAG xs (vertex:ys)) org_dag)
    | otherwise = combine_indegree_with_vertices ([(get_vertex_ID vertex,0)] ++ find_indegree_of_vertecies (DAG xs (vertex:ys)) org_dag)

combine_indegree_with_vertices :: [(Integer, Integer)] -> [(Integer, Integer)]
combine_indegree_with_vertices list = toList $ fromListWith (Prelude.+) [(k, v) | (k, v) <- list]

-- Lists the topological order by taking the one that has 0 indegrees first, deletes everything connected to that Vertex, 
-- finds the new indegree and repeat recursively 
topological_order_helper :: (Ord a, Eq a) => [(Integer, Integer)] -> DAG a -> [Integer]
topological_order_helper [] _ = []
topological_order_helper (x:xs) (DAG edges vertecies)
    | snd x == 0 = [fst x] ++ topological_order_helper (find_indegree_of_vertecies modified_dag modified_dag) modified_dag
    | otherwise = topological_order_helper xs (DAG edges vertecies)
        where
            modified_dag = (DAG (remove_edge edges edges (fst x)) (remove_vertex vertecies vertecies (fst x)))

-- Removes a edges from a list based on the integer input
remove_edge :: (Eq a) => [Edge a] -> [Edge a] -> Integer -> [Edge a]
remove_edge [] original_edges id = original_edges
remove_edge (x:xs) original_edges id 
    | get_vertex_ID (get_left x) == id || get_vertex_ID (get_right x) == id = remove_edge xs (remove_specific_edge original_edges x) id
    | otherwise = remove_edge xs original_edges id

-- Removes a specifiv edge from a list and returns a new list without that edge
remove_specific_edge :: (Eq a) => [Edge a] -> Edge a -> [Edge a]
remove_specific_edge (x:xs) edge
    | get_vertex_ID (get_left x) == get_vertex_ID (get_left edge) && get_vertex_ID (get_right x) == get_vertex_ID (get_right edge) = xs
    | otherwise = remove_specific_edge (xs++[x]) edge

-- Removes a vertecies from a list based on the integer input
remove_vertex :: (Eq a) => [Vertex a] -> [Vertex a] -> Integer -> [Vertex a]
remove_vertex [] orignial_vertecies id = orignial_vertecies
remove_vertex (x:xs) orignial_vertecies id
    | id == get_vertex_ID x = remove_vertex xs (remove_specific_vertex orignial_vertecies x) id
    | otherwise = remove_vertex xs orignial_vertecies id

-- Removes a specifiv vertex from a list and returns a new list without that vertex
remove_specific_vertex :: (Eq a) => [Vertex a] -> Vertex a -> [Vertex a]
remove_specific_vertex (x:xs) vertex
    | get_vertex_ID x == get_vertex_ID vertex = xs
    | otherwise = remove_specific_vertex (xs++[x]) vertex

-- Returns the weight of a vertex while applying the function f to it
get_weight_of_vertex :: (Haskell_dag.Num a) => (a -> a) -> Vertex a -> a
get_weight_of_vertex f (Vertex _ v) = f v

-- Returns the weight of a edge while applying the function g to it
get_weight_of_edge :: (Haskell_dag.Num a) => (a -> a) -> Edge a -> a
get_weight_of_edge g (Edge _ _ v) = g v

-- Calculates which path is the longest and returns a list of the longest path
weight_of_longest_path :: (Haskell_dag.Num a, Prelude.Num a, Ord a) => Integer -> Integer -> DAG a -> (a -> a) -> (a -> a) -> [a]
weight_of_longest_path vertex1_ID vertex2_ID dag f g = 
    weight_of_longest_path_helper vertex1_ID vertex2_ID dag f g

-- Helping function for finding the longest path
weight_of_longest_path_helper :: (Haskell_dag.Num a, Prelude.Num a, Ord a) => Integer -> Integer -> DAG a -> (a -> a) -> (a -> a) -> [a]
weight_of_longest_path_helper start_vertex_ID end_vertex_ID (DAG edges vertecies) f g =
    get_top_order (find_vertex_from_ID start_vertex_ID vertecies) (find_vertex_from_ID end_vertex_ID vertecies) (DAG edges vertecies) f g
    
-- Get the top order of the dag
get_top_order :: (Haskell_dag.Num a, Prelude.Num a, Ord a) => Vertex a -> Vertex a -> DAG a -> (a -> a) -> (a -> a) -> [a]
get_top_order start_vertex end_vertex dag f g = 
    set_top_order_without_zero_indegree_exluding_start (topological_order dag) start_vertex end_vertex dag f g  

-- Set the top order but without the zero indregrees.  
set_top_order_without_zero_indegree_exluding_start :: (Haskell_dag.Num a, Prelude.Num a, Ord a) => [Integer] -> Vertex a -> Vertex a -> DAG a -> (a -> a) -> (a -> a) -> [a]
set_top_order_without_zero_indegree_exluding_start topological_order start_vertex end_vertex (DAG edges vertecies) f g =
    get_necessary_top_order (Data.List.map fst (sortBy sortHelp(delete_vertex_with_zeroindegree start_vertex (DAG edges vertecies)))) topological_order start_vertex end_vertex (DAG edges vertecies) f g

-- Get necessary top order for the dag, returns the longest path. 
get_necessary_top_order :: (Haskell_dag.Num a, Prelude.Num a, Ord a) => [Integer] -> [Integer] -> Vertex a -> Vertex a -> DAG a -> (a -> a) -> (a -> a) -> [a]
get_necessary_top_order topogical_order_without_zero topological_order start_vertex end_vertex dag f g =
    get_list_of_vertecies_with_weigth (topological_sort topological_order topogical_order_without_zero topological_order) start_vertex end_vertex dag f g

-- Get list of vertecies with the weight, return the longest path.  
get_list_of_vertecies_with_weigth :: (Haskell_dag.Num a, Prelude.Num a, Ord a) => [Integer] -> Vertex a -> Vertex a -> DAG a -> (a -> a) -> (a -> a) -> [a]
get_list_of_vertecies_with_weigth topological_order start_vertex end_vertex (DAG edges vertecies) f g =
    delete_unecessary_vertecies (list_top_order f vertecies start_vertex end_vertex topological_order) topological_order (DAG edges vertecies) f g

-- Deletes alls the uncessary edges for other functions to return the longest path.
delete_unecessary_vertecies :: (Haskell_dag.Num a, Prelude.Num a, Ord a) => [(Integer, a)] -> [Integer] -> DAG a -> (a -> a) -> (a -> a) -> [a]
delete_unecessary_vertecies vertecies_with_weigth topological_order dag f g = 
    set_all_necessary_edges (topological_order Data.List.\\ Data.List.map fst vertecies_with_weigth) vertecies_with_weigth dag f g

-- Sets all the necessary edges for other functions to return the longest path
set_all_necessary_edges :: (Haskell_dag.Num a, Prelude.Num a, Ord a) => [Integer] -> [(Integer, a)] -> DAG a -> (a -> a) -> (a -> a) -> [a]
set_all_necessary_edges vert_to_be_deleted_from_edges vertecies_with_weigth (DAG edges vertecies) f g =
    get_longest_paths_list (get_list_of_edges (dag_modifier (zip vert_to_be_deleted_from_edges (replicate (length vert_to_be_deleted_from_edges) 0)) (DAG edges vertecies))) vertecies_with_weigth f g

-- Takes edges and a list of vertecies id with weigth and the higher order functions and returns a list of the longest path
get_longest_paths_list :: (Prelude.Num a, Haskell_dag.Num a, Ord a) => [Edge a] -> [(Integer, a)] -> (a -> a) -> (a -> a) -> [a]
get_longest_paths_list edges vertecies_with_weigth f g = 
    get_longest_path (get_weight f g (make_weight_list vertecies_with_weigth) (make_weight_list vertecies_with_weigth) edges) 

-- Get the longest path from a list of paths
get_longest_path :: (Haskell_dag.Num a, Prelude.Num a, Ord a) => [(Integer, [a])] -> [a]
get_longest_path longest_path_list = 
    reverse (snd(head longest_path_list))

-- Makes the polymorphic type a to a list
make_weight_list :: [(Integer, a)] -> [(Integer, [a])]
make_weight_list [] = []
make_weight_list list = Data.List.map (\(x,y) -> (x, [y])) list

-- Returns the list of relevant ID's sorted by the orignial topological order
topological_sort :: [Integer] -> [Integer] -> [Integer] -> [Integer]
topological_sort [] [] org = []
topological_sort (x:xs) [] org = topological_sort (x:xs) org org
topological_sort (x:xs) (y:ys) org
    | x == y = [x] ++ topological_sort xs ys org
    | x `notElem` (y:ys) = topological_sort xs (y:ys) org
    | otherwise = topological_sort (x:xs) (ys++[y]) org

sortHelp (w1, ww1) (w2, ww2)
    | ww1 > ww2 = GT 
    | ww1 < ww2 = LT
    | otherwise = EQ

-- takes a function f, list of vertecies, the first and last vertex which you are searching the longest path for and a list of ID's
-- and transforms it to a list with the ID and the weight according to it
list_top_order :: (Prelude.Num a, Haskell_dag.Num a, Eq a) => (a -> a) -> [Vertex a] -> Vertex a -> Vertex a -> [Integer] -> [(Integer, a)]
list_top_order f vertecies first_vert end_vert [] = []
list_top_order f vertecies first_vert end_vert (x:xs)
    | get_vertex_ID first_vert == x = topological_helper f vertecies end_vert (x:xs)
    | otherwise = list_top_order f vertecies first_vert end_vert xs

-- takes a function f, list of vertecies, the last vertex which you are searching the longest path for and a list of ID's
-- and transforms it to a list with the ID and the weight according to it
topological_helper :: (Prelude.Num a, Haskell_dag.Num a, Eq a) => (a -> a) -> [Vertex a] -> Vertex a -> [Integer] -> [(Integer, a)]
topological_helper f vertecies end_vert [] = error "ERROR: tried getting longest path on nonexisting path"
topological_helper f [] end_vert top = []
topological_helper f vertecies end_vert (x:xs)
    | get_vertex_ID end_vert /= x = [(x, (get_weight_from_vert f x vertecies))] ++ topological_helper f vertecies end_vert xs
    | otherwise = [(x, (get_weight_from_vert f x vertecies))]

-- Finds any possible path from the parameter Vertex weight, deletes the rest
delete_vertex_with_zeroindegree :: (Ord a, Eq a) => Vertex a -> DAG a -> [(Integer, Integer)]
delete_vertex_with_zeroindegree start_vert dag 
    | 0 `notElem` delete_vertex_from_list start_vert (find_indegree_of_vertecies dag dag) = find_indegree_of_vertecies dag dag
    | otherwise = delete_vertex_with_zeroindegree start_vert (dag_modifier (Data.List.filter (\x -> (snd x == 0) && not ((fst x) == (get_vertex_ID start_vert))) (find_indegree_of_vertecies dag dag)) dag)

-- Deletes vertex from list with the amount of indegrees every vertex has

delete_vertex_from_list :: Vertex weight -> [(Integer, Integer)] -> [Integer]
delete_vertex_from_list vert [] = []
delete_vertex_from_list vert (x:xs)
    | get_vertex_ID vert == fst x = Data.List.map snd xs
    | otherwise = delete_vertex_from_list vert (xs++[x])

-- Removes anything from the dag that is contained in the list [(Integer, Integer)] the
-- first is the ID and second is second is the amount of indegrees
dag_modifier :: (Ord a, Eq a) => [(Integer, Integer)] -> DAG a -> DAG a
dag_modifier [] dag = dag
dag_modifier (x:xs) (DAG edges vertecies)
    | snd x == 0 = dag_modifier xs (DAG (remove_edge edges edges (fst x)) (remove_vertex vertecies vertecies (fst x)))
    | otherwise = dag_modifier xs (DAG edges vertecies)

-- takes function f, g, topological list with weight to ID twice to check the original list and a list of edges. 
get_weight :: (Haskell_dag.Num a, Prelude.Num a, Ord a) => (a -> a) -> (a -> a) -> [(Integer, [a])] -> [(Integer, [a])] -> [Edge a] -> [(Integer, [a])]
get_weight f g top [] [] = top
get_weight f g (x:xs) orgtop edge
    | length (x:xs) == 1 = (x:xs)
    | otherwise = get_weight f g (check_all_edges_helper g (fst x) (x:xs) orgtop edge) orgtop edge 

-- Takes the function g, ID to check path from, topological list with weight to ID twice to check the original list and a list of edges returns topological list with weight
check_all_edges_helper :: (Haskell_dag.Num a, Prelude.Num a, Ord a, Eq a) => (a -> a) -> Integer -> [(Integer, [a])] -> [(Integer, [a])] -> [Edge a] -> [(Integer, [a])]
check_all_edges_helper g weight toporder org_toporder [] = tail toporder
check_all_edges_helper g weight toporder org_toporder (x:xs)
    | weight == get_vertex_ID (get_left x) && compare_weight_of_edges_and_vertecies org_toporder toporder x g 
    = check_all_edges_helper g weight (add_weight_to_vertex toporder org_toporder (get_vertex_ID (get_right x)) ([get_weight_of_edge g x] ++ get_weight_from_left_edge toporder x)) org_toporder xs
    | otherwise = check_all_edges_helper g weight toporder org_toporder xs

-- Comapres the current weight of the edge with the one to be added later
compare_weight_of_edges_and_vertecies :: (Haskell_dag.Num a, Prelude.Num a, Ord a, Eq a) => [(Integer, [a])] -> [(Integer, [a])] -> Edge a -> (a -> a) -> Bool
compare_weight_of_edges_and_vertecies org_toporder toporder edge g
    | (Haskell_dag.sum (get_weight_from_right_edge toporder edge) < (Haskell_dag.sum(get_weight_from_left_edge toporder edge) Haskell_dag.+ get_weight_of_edge g edge Haskell_dag.+ (Haskell_dag.sum(get_weight_from_toplist (get_vertex_ID (get_right edge)) org_toporder)))) = True
    | otherwise = False

-- Get the weights from the right edge
get_weight_from_right_edge :: (Haskell_dag.Num a, Prelude.Num a, Ord a, Eq a) => [(Integer, [a])] -> Edge a -> [a]
get_weight_from_right_edge toplist edge = 
    get_weight_from_toplist (get_vertex_ID (get_right edge)) toplist

-- Get the weights from the left edge
get_weight_from_left_edge :: (Haskell_dag.Num a, Prelude.Num a, Ord a, Eq a) => [(Integer, [a])] -> Edge a -> [a]
get_weight_from_left_edge toplist edge = 
    get_weight_from_toplist (get_vertex_ID (get_left edge)) toplist

-- Takes in topological list with weight to ID twice to check the original list, ID to check, weight to add and list with added weights
add_weight_to_vertex :: (Prelude.Num a, Eq a) => [(Integer, [a])] -> [(Integer, [a])] -> Integer -> [a] -> [(Integer, [a])]
add_weight_to_vertex list [] _ _ = list
add_weight_to_vertex list org_list vert weight
    | vert == (fst (head list)) = [(fst (head list), get_weight_from_toplist vert org_list ++ weight)] ++ tail list
    | otherwise = [head list] ++ add_weight_to_vertex (tail list) org_list vert weight

-- Takes function f, ID to get weight from, list of vertecies and returns the weight
get_weight_from_vert :: (Prelude.Num a, Haskell_dag.Num a, Eq a) => (a -> a) -> Integer -> [Vertex a] -> a
get_weight_from_vert f id [] = 0
get_weight_from_vert f id (x:xs)
    | id == get_vertex_ID x = get_weight_of_vertex f x
    | otherwise = get_weight_from_vert f id xs

-- Takes a ID to get weight from and list to fetch it from, returns weight
get_weight_from_toplist :: (Prelude.Num a, Eq a) => Integer -> [(Integer, [a])] -> [a]
get_weight_from_toplist id [] = [0]
get_weight_from_toplist id (x:xs)
    | id == fst x = snd x
    | otherwise = get_weight_from_toplist id xs
