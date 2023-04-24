import detectEthereumProvider from '@metamask/detect-provider';
import { ethers } from "ethers"
import React from 'react';
import { render } from 'react-dom';

var config = require("../../../../artifacts/contracts/Token.sol/Token.json")

type ProviderType = 'mainnet' | 'ropsten' | 'rinkeby' | 'kovan';


// export default class DApp extends React.Component<{}, DAppState> {

// }