print("lua begin")

package.cpath = "/home/me/toys/game/lua/common_usage/build/linux/x86_64/debug/?.so" .. package.cpath


print(package.cpath)
local mylib = require "mylib"



-- 练习29.1：请使用C语言编写一个可变长参数函数summation，来计算数值类型参数的和：
print(mylib)
print(mylib.summation)
print(summation)
