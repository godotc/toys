

factorial::(Integral a)=> a -> a
factorial 0 = 1
factorial n = n* factorial(n-1)

addPairs::(Num a) => (a,a)->(a,a) ->(a,a)
addPairs (x1,y1)(x2,y2) = (x1+x2,y1+y2)

first::(a,b,c) -> a
first(x,_,_) = x

second::(a,b,c) -> b
second(_,y,_) = y

third::(a,b,c) -> c
third(_,_,z) = z

head' ::[a]->a
head' [] = error "emply list"
head' (x:_) = x

echo::(Show a) => a ->String
echo = show 

say ::(Show a) => [a]->String
say[]=  error "empty list"
say(x:[]) = show x 
say(x:y:[]) = show x ++ show y


len' ::(Num b) => [a]->b
len' []=0
len' (_:xs)= 1+len' xs


capital::String -> String
capital ""="empty string"
captial  all@(x:xs) = "1st of " ++ all ++ ": "++[x]




