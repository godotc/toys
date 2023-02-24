
qs ::(Ord a) => [a] ->[a]
qs [] =[]
qs [x] = [x]
qs (x:xs) = 
  let smallX = qs [a| a<-xs, a<x]
      bigX = qs [a| a<-xs, a>x]
  in smallX ++ [x] ++ bigX

qs' ::(Ord a) => [a] ->[a]
qs' [] =[]
qs' [x] = [x]
qs' (x:xs) = 
  let smallX = qs' (filter(<x) xs)
      bigX = qs' (filter(>x) xs)
  in smallX ++ [x] ++ bigX


 -- TODO: merge sort
-- ms ::(Ord a) => [a] ->[a]
-- ms [] = []
-- ms [x] =[x]
-- ms forall[x::Nat] x.(Take n, Drop n) 



bigDiv :: Integer
bigDiv = head (filter p [1000,999..])
  where p x = x `mod` 321 == 0
    
