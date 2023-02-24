import Control.Arrow (ArrowApply(app))
import Text.XHtml (applet)

-- max:: Ord a => a->a ->a
-- max:: Ord a => a->(a -> a)


xThree :: (Num a) => a->a->a->a
xThree x y z = x*y*z

--xThree' :: (Num a) => a->(a-> ( a->a ))
--(((xThree 2)3)5


compareWith10 :: (Ord a, Num a) => a -> Ordering
compareWith10 = compare 10


isUpperCase::Char->Bool
isUpperCase = (`elem` ['A'..'Z'])


apllyTwice:: (a->a)->a->a
apllyTwice f x = f(f x)


zipWith' :: (a->b->c) -> [a]->[b]->[c]
zipWith'_ [] _ = []
zipWith' _ _ [] = []
zipWith' f (x:xs) (y:ys) = f x y :zipWith' f xs ys


flip' :: (a->b->c) -> (b->a->c)
--flip' f = g where g x y = f y x
flip' g x y = g y x
