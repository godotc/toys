

const Btn_ConnectWallet = document.getElementById('btn_ConnectWallet')
const Btn_Test = document.getElementById('btn_test')
const AccountToshow = document.querySelector('.AccountToShow')
const Btn_Transfer = document.getElementById('btn_transfer')
const Btn_GetAllAccounts = document.getElementById('btn_getAllAccounts')

Btn_ConnectWallet.addEventListener('click', () => {
    getAccount()
});

Btn_Test.onclick = function () {
    console.log("test..")
};


async function getAccount() {
    const accounts = await ethereum.request({ method: 'eth_requestAccounts' });
    const account = accounts[0];
    AccountToshow.innerHTML = account;
}

ethereum.on('accountsChanged', function (accounts) {
    AccountToshow.innerHTML = accounts[0];
});


Btn_Transfer.onclick = () => {
    let account01 = document.getElementById('account01').value;
    let account02 = document.getElementById('account02').value;
    console.log(account01, "--->", account02);
    transfer(account01, account02);
};



async function transfer(acc1, acc2) {
    ethereum.request({
        method: 'eth_sendTransaction',
        params: [
            {
                from: acc1,
                to: acc2,
                value: '0x0000000000000000',
                gasPrice: '0x00000',
                gas: '0x0000',
                chainId: '1337'
            },
        ],
    })
        .then((txHash) => console.log(txHash))
        .catch((error) => console.error(error));
}




Btn_GetAllAccounts.onclick = () => {
    web3.eth.getAccounts(console.log);
}


