getUppercase str= [a|a<-str,a `elem` ['A'..'Z']]

len xs = sum [1|_<-xs]

pingPong xs = [if x>10 then "ping" else "pong" | x<-xs,odd x] 

--rightTrianglesWith len  =  [(x,y,z) |x<-[1..len],y<-[1..x],z<-[1..y], x^2+y^2==z^2]
rightTrianglesWith' len sum  =  [(x,y,z) |z<-[1..len],y<-[1..z],x<-[1..y], x^2+y^2==z^2,x+y+z==sum]


