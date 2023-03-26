# 部署合约与转账测试

## 当前目录结构:
```
.
├── contracts // 合约目录
├── migrations // 部署脚本
├── package.json
├── package-lock.json
├── READEME.md
├── test // 测试脚本目录
└── truffle-config.js // truffle 配置文件
```

## 1. 环境搭建

1. 克隆我的玩具仓库到本地并进入测试目录
```sh
$ git clone https://github.com/godotc/toys.git
$ cd toys/sol/transfer_test
```

2. 安装依赖项目

> requirment: nodejs, npm

- 安装全局的测试网络与框架
> 可能在安装与使用truffle都需要root权限(不是root用户的话)
```sh
$ npm i -g ganache 
$ npm i -g truffle
```
- 安装合约依赖项(web3,openzeppelin)
```sh
$ npm install 
```

- 安装solc
```sh
$ pip install solc-select
$ solc install 0.8.17
$ solc use 0.8.17
```

- 启动 ganache 测试网络(另开一个终端)
```sh
$ ganache -D
```

## 2. 部署与测试
```sh
# 进入 truffle interactive console
$ truffle console

> compile # 编译
> migrate # 部署
> test # 测试(详见test目录下)
```

## 3. 交互式测试

- truffle console下其实就是一个js的终端
- 已经被注入了truffle, web3等依赖可以直接使用

```javascript
> let contracts = artifact.require('CIVIL3')
> let accounts = await web3.eth.getAccounts()
    
// 查看获取的数据
> accounts
> contracts

// 进行操作
...
```

