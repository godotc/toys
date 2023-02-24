solc transfer_test/contracts/CIVIL3.sol --abi -o CIVIL_abi --overwrite

# –abi表示abi文件名
# –pkg表示生成文件的所属包
# –type表示生成数据结构的名称，不填就默认是包名
# –out表示生成的文件名
abigen --abi CIVIL_abi/CIVIL3.abi --pkg civil3  --type Civil --out civil3/civil3.go
