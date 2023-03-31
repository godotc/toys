
var Faucet = artfacts.require('Faucet')

module.exports = function (deployer) {
	deployer.deploy(Faucet, 600, '0x3df186851464ad9dc8f5142cb29d30f24a38c8695399a8f43b5a327729e9d982');
}
