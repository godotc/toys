import { Game } from "./src/game.js";

const Config = {
    tick_interval: 200,
    bLoop: false
}



export const main = async () => {
    let game = new Game(Config)
    console.log(game)
    await game.init()
    game.run()
}


await main()
