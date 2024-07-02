module Huffman where

--Imports
import Data.List 
import Data.Map 
import Data.Ord hiding(compare)
import Prelude hiding(compare)
import qualified Data.Map as Map  

----------------- datatyper ------------------------------
data Htree = Leaf Char | Branch Htree Htree deriving Show
data WeightedTree = WL Integer Char | WB Integer WeightedTree WeightedTree deriving Show
data Ftree = FLeaf Char [Integer] | Node Ftree Ftree deriving Show
----------------- Deluppgift 1 ------------------------------

statistics :: String -> [(Integer, Char)]
statistics [] = [] --basecase
statistics input = sort(reverseFunc(toList $ fromListWith (+) [(k,1)| (k) <- input]))

--use map to reverse the tuples in the list
reverseFunc :: [(Char,Integer)] -> [(Integer,Char)] 
reverseFunc [] = []
reverseFunc  (x:xs) = reversetuple x: reverseFunc xs

--reverse the tuple to match the output. 
reversetuple :: (Char, Integer) -> (Integer, Char)
reversetuple (c,i) = (i,c)

----------------- Deluppgift 2 ------------------------------
maketree :: [(Integer, Char)] -> Htree
maketree input =  getTree( head( makeBranchedTree(checkDups( makeLeafs(input)))))

--------------make leafs------------
-- create a list of leafs---
makeLeafs :: [(Integer, Char)] -> [WeightedTree]
makeLeafs [] = []
makeLeafs (x:xs) =  createL x : makeLeafs xs

-- set WL ----
createL :: (Integer,Char) -> WeightedTree
createL (fst,snd) = WL fst snd

--This function is used if the input string has only 1 letter
-- for example "aaaaaaaaaa", If so i want to duplicate the leaf
-- in order to be able to create the branch. 
checkDups :: [WeightedTree] -> [WeightedTree]
checkDups [] = []
checkDups (x:xs) =   if (length (x:xs) < 2)
                        then x:(x:xs) 
                    else (x:xs)

------------Begin branching---------

--B Integer WeightedTree WeightedTree deriving Show
makeBranchedTree :: [WeightedTree] -> [WeightedTree]
makeBranchedTree [] = []
makeBranchedTree (a:[]) = [a]
makeBranchedTree (a:b:[]) = [WB (getWeight(a) + getWeight( b)) a b]
makeBranchedTree (a:b:c) = makeBranchedTree( sortList(WB (getWeight(a) + getWeight(b)) a b : sortList c))

-- get the weight
getWeight :: WeightedTree -> Integer 
getWeight (WL weight _ ) = weight
getWeight (WB weight _ _ ) = weight 

-- sort the tree, the leaf with least weight is first in list
-- the leaf with highest weight is last in the list. 
sortList :: [WeightedTree] -> [WeightedTree]
sortList tree = sortBy compareWeight tree

-- Compare weight
compareWeight :: WeightedTree -> WeightedTree -> Ordering
compareWeight x y 
    | getWeight x < getWeight y  = LT
    | getWeight x > getWeight y  = GT
    | getWeight x == getWeight y = EQ

-- When there's only one WeightedTree in the list, get the head and remove the list. 
rmList :: [WeightedTree] -> WeightedTree
rmList x = head x

-- traverse the WeightedTree and create Htree recursive.
-- w1 is left branch and w2 is right branch
getTree ::  WeightedTree -> Htree
getTree (WL _ c) = Leaf c
getTree (WB _ w1 w2) = Branch (getTree w1) (getTree w2)


--------Deluppgift 3---------------
encode :: String -> (Htree, [Integer])
encode [] = (Leaf ' ',[])
encode input =
    let y = (createPaths (maketree (statistics input)) [])
        x = (extractLeafs input y y)
    in (maketree (statistics input), setBits input x x )

-- Create the paths for each leaf with a list of integers where the integers are 0 and 1.
-- 0 = left 1 = right in the tree. 
createPaths :: Htree  -> [Integer] -> Ftree
createPaths (Leaf c) x  = FLeaf c x
createPaths (Branch h1 h2) x = Node (createPaths h1 (x++[0])) (createPaths h2 (x++[1]))

-- Take out all the leafs from the tree and get a list of tuples with 
-- the char and path from the leaf
extractLeafs :: String -> Ftree-> Ftree -> [(Char,[Integer])]
extractLeafs [] _ _ = []
extractLeafs string tree (Node f1 f2) = (extractLeafs string tree f1) ++ (extractLeafs string tree f2)
extractLeafs (x:xs) tree (FLeaf c path) = [(c, path)]

-- Create a list of all the paths. compare the input string with the list of char and integers
-- when the head of the string is equal to the char in the list, set the path in the
-- output list. 
setBits :: String -> [(Char,[Integer])] -> [(Char,[Integer])] -> [Integer]
setBits [] _ _  = []
setBits (x:xs) org (y:ys) 
                            | x == (getchar y) = (getPath y) ++ setBits xs org org
                            | x /= (getchar y) = (setBits (x:xs) org ys)

--Get the path from the tuple. 
getPath :: (Char,[Integer]) -> [Integer]
getPath (_,path) = path

--Take the char from the tuple.
getchar :: (Char,[Integer]) -> Char
getchar (c,_) = c

--------Deluppgift 4---------------

decode :: Htree -> [Integer] -> String 
decode _ [] = []
decode htree bitSekvens =  traverseTree htree htree bitSekvens

--Traverse the tree using the list of integers t.ex. [0,1,1,0,1,1,1,1]... 
-- 0 means left 1 right. Do this until a Leaf is reached, get the char and do it again until the list of integers is empty. 
-- takes 2 Htrees in to the function to "reset" when a char is found in the leafs. 
traverseTree :: Htree -> Htree -> [Integer] -> String
traverseTree (Branch h1 h2) _ [] = []  
traverseTree (Leaf c) tree [] = [c] ++ traverseTree tree tree []

traverseTree (Leaf c) tree list = [c] ++ traverseTree tree tree list

traverseTree (Branch h1 h2) tree (x:[]) 
                                        | x == 0 = traverseTree h1 tree (tail [x]) 
                                        | x == 1 = traverseTree h2 tree (tail [x])

traverseTree (Branch h1 h2) tree (x:xs)  
                                        | x == 0 = traverseTree h1 tree xs
                                        | x == 1 = traverseTree h2 tree xs

