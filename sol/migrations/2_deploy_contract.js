
const Hello = artifacts.require("../contracts/hello.sol");


module.exports = function (deployer) {
	deployer.deploy(Hello);
};

