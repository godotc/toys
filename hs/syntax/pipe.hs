import Text.XHtml (height)


--the height is 1.80 likes
bmiCounter::(RealFloat f) =>  f->f-> String
bmiCounter weight height                      --the height is 1.80 likes
    | weight/height ^2 <=18.5 = "underweight"
    | weight/height ^2 <=25.0 = "normal"
    | weight/height ^2 <=30 = "fat"
    | otherwise = "You're a whale"

-- optmized
bmiCounter'::(RealFloat f) =>  f->f-> String
bmiCounter' weight height                      
    | bmi<=18.5 = "underweight"
    | bmi <=25.0 = "normal"
    | bmi <=30 = "fat"
    | otherwise = "You're a whale"
    where bmi = weight/height ^2

max' ::(Ord o) => o-> o -> o
max' a b
    | a>b = a
    | otherwise = b


-- When call this, like a `mCompare` b, with `` around
mCompare ::(Ord a) => a->a -> Ordering
x `mCompare` y
    | x>y = GT
    | x==y = EQ
    | x<y =LT


    
-- calcualte the bmi value of echo 2 args
bmiApp ::(RealFloat f) => [(f,f)]->[f]
bmiApp xs = [bmi w h| (w,h)<-xs]
    where bmi weight height = weight/height ^2

