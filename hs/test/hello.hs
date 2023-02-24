main = do 
  putStrLn "hello world"
  putStr "input: "
  name <-getLine
  putStrLn ("hello" ++ name)


