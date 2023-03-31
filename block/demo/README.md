
1. 启动测试网络
```
ganache -D \
        -i=test_net \
        --logging.debug=true \
        --server.host/-h loaclhost/127.0.0.1 \
        --server.port/-p 8545
```

2. 启动 truffle console
```
- 更改 config.js 文件中的development为ganache测试网络的ip与port
$ truffl console
> compile
> migrate
> test
```
