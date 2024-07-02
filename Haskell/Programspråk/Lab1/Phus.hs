module Phus where

--Imports

import Data.List 
import Data.Map
import Data.Ord
import qualified Data.Map as Map  

--Type declatarions
type Parking' = (String,Bool,(Integer,Integer))
type PTime' = (String,(Integer,Integer))
type Time' = (Integer,Integer)

--main 
phus :: [(String, Bool, (Integer, Integer))] -> (String, [(String, (Integer, Integer))])
phus day =  getLongestParkedCar(mergeTime(mergeDuplicates(getVisitedCars day)))

-------------------------------------------------
-- Get the car that has parked the longest time -
-------------------------------------------------

--Function used to make the signarute correct. 
getLongestParkedCar :: [PTime'] -> (String, [PTime'])
getLongestParkedCar listOfCars = (getCarName ("",(0,0)) listOfCars , listOfCars)

-- Calculate wich car has spent most time in the garage. 
getCarName :: PTime' -> [PTime'] -> String
getCarName car [] = regName' car
getCarName car (head:tail)  = if( getTime' car < getTime' head )
                                then getCarName head tail 
                            else getCarName car tail

-- Get the total time spent in garage. Helps getCar func to decide wich 
-- car that has spent most time in the garage
getTime' :: PTime' -> Integer
getTime' (_,(h,m)) =  h*60 + m

-- get the car regname and print it.     
regName' :: PTime' -> String
regName' (name, _) =  name


--------------------------------------------
-- Merge the duplicates and merge the time -
--------------------------------------------

--merge the duplicates in the list. 
mergeDuplicates :: [PTime'] -> [(String, [Time'])]
mergeDuplicates carList = toList $ fromListWith (++) [(k, [v])| (k , v) <- carList] 
   
--merge the times and make correct signature   
mergeTime :: [(String, [Time'])] -> [PTime']
mergeTime [] = []
mergeTime (head:tail) = sumTime head : mergeTime tail

-- merges the list of timestamps to total time. 
sumTime :: (String, [Time']) -> PTime'
sumTime(carname, time) = (carname, convertBack (sum (convertToMinutes time)))

-- Convert the minutes to (Hour,Minutes) using divison and modulus. 
convertBack :: Integer -> Time'
convertBack time =  (div time 60, time `mod` 60)

-- Convert the list of tuples to minutes to easier calculate the total time. 
convertToMinutes :: [Time'] -> [Integer]
convertToMinutes [] = []
convertToMinutes ((h,m):tail) = (h * 60 + m) : convertToMinutes tail


------------------------------------
-- Get all the cars and their time -
------------------------------------


-- check how long car has been parked using recursion
-- The list that is returned will have duplicates. 
getVisitedCars :: [Parking'] -> [PTime']
getVisitedCars [] = []
getVisitedCars (head:tail) = 
                        if(carInHouse head)
                            then carParkedTime head tail : getVisitedCars tail 
                        else getVisitedCars tail
                    
-- Get the car name and the time the car spent in the garage during the visit. 
carParkedTime :: Parking' -> [Parking'] -> PTime'
carParkedTime car (head:tail) = 
                        if (carInHouse car == False)
                            then ("",(0,0))
                        else 
                            if(regName car == regName head)
                                then (regName car ,timeSpent (getTime car, getTime head))
                            else carParkedTime car tail

-- get the timespent in phus for one visit. 
timeSpent :: (Time',Time') -> (Integer,Integer)
timeSpent ((a,b),(c,d)) = 
                        if( d-b < 0) 
                            then (c-a-1, d-b+60)
                        else (c-a, d-b)

-- is car still in house? Use the bool to check. 
--True = car entered house, false = left house 
carInHouse :: Parking' -> Bool
carInHouse (_,bool,_) = bool

-- get the car regname    
regName :: Parking' -> String
regName (name, _, _) = name

-- get time for a car. 
getTime :: Parking' -> Time'
getTime (_,_,time) = time