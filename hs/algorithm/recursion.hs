fibonacci:: Integer -> Integer
fibonacci 0 = 0
fibonacci 1 = 1
fibonacci n = fibonacci(n-1)+fibonacci(n-2)



getMaxOfList::(Ord a) => [a] ->a
getMaxOfList [] = error "empty list"
getMaxOfList [x] = x
getMaxOfList (x:xs) 
    | x>maxTail =x
    | otherwise = maxTail
   where maxTail = getMaxOfList xs
    
getMaxOfList'::(Ord a) => [a] ->a
getMaxOfList' [] = error "empty list"
getMaxOfList' [x] = x
getMaxOfList' (x:xs) = max x (getMaxOfList xs)


--`Num` is not the child class of `Ord`
--Here we appoint 2 of constraints(约束)
replicate' ::(Num i, Ord i) => i->a->[a]
replicate' n x
    | n <=0  = []
    | otherwise = x:replicate' (n-1) x

--get fisrt n of xs
take' ::(Num i, Ord i) => i->[a]->[a]
----xs is null
take' n _ 
    |n<=0 = []          -- only concern about `n`, not about `xs`, if `n` <= 0, goto next match
----position is null
take' _ [] = [] 
take' n (x:xs) = x : take' (n-1) xs


reverse' ::[a] -> [a]
reverse' [] = []
reverse' (x:xs) = reverse' xs ++ [x]


zip' ::[a]->[b] ->[(a,b)]
zip' _ [] = []
zip' [] _ = []
zip' (x:xs) (y:ys) = (x,y): zip' xs ys



-- Is e the element of [e]
elem' ::(  Eq e ) => e->[e] -> Bool
elem' a [] = False
elem' a (x:xs)
    | a == x = True
    | otherwise =  elem' a xs




