
say::(Show a)=> [a]->String
say xs = case xs of 
    [] -> "Empty string"
    [x] -> "1 element: " ++ show x
    [x,y] -> "2 elemens: " ++ show x ++","++show y
    (x:y:_)  -> "first 2 elemens: " ++ show x ++","++show y

