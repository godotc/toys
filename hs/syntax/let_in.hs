val = let sqr x = x^2 in [(sqr 4, sqr 5)]



cylinerArea:: (RealFloat f) => f->f->f
cylinerArea r h =
    let sideArea = pi*2*r*h
        baseArea = pi*r^2
    in sideArea+ 2*baseArea

bmiApp ::(RealFloat f) => [(f,f)]->[f]
--bmiApp xs = [bmi w h| (w,h)<-xs]
--    where bmi weight height = weight/height ^2
bmiApp xs = [bmi|(w,h)<-xs,let bmi = w/h ^2]
