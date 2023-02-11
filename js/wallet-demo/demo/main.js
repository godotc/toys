var web3 = require('web3')

console.log('hello world')


document.getElementById('btn_login').onclick =
    function () {
        console.log("login..")
        web3.personnal.sign()
    };

document.getElementById('btn_test').onclick =
    function () {
        console.log("test..")
        console.warn("test..")
        console.error("test..")
    };
