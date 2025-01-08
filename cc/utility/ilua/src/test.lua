print("C:\\Users\\dexzhou\1\toys\\cc\\utility\\ilua\\src\test.lua")

a = 7 + 1;


player = {
    name = "dex",
    age = 18,
    gender = "male"
}



function CallHostFunction(a, b)
    c = HostFunction(a + b)
    print("ouput ", c)
end
