"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const app = (0, express_1.default)();
const port = 3000;
app.use('/public', express_1.default.static('public'));
app.get("/", (req, resp) => {
    resp.send({ msg: "Hello world" });
});
app.get("/arch", (x, y) => {
    y.send({ url: '/public/arch.png' });
});
app.listen(port, () => {
    console.log(`Listen in ${port}...`);
});
