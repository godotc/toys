
ms ::(Ord a) => [a] ->[a]
ms [] =[]
ms [x] = [x]
ms (x:xs) = 
  let smallX = ms [a| a<-xs, a<x]
      bigX = ms [a| a<-xs, a>x]
  in smallX ++ [x] ++ bigX


qs ::(Ord a) => [a] ->[a]
qs [] = []
qs [x] =[x]
qs (x:xs) let l = qs [a| ]






   
    
