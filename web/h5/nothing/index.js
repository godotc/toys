import { Game } from "./src/game.js";

let game = new Game()
await game.init()
game.run()

