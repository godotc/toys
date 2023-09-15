import { Game } from "./src/game.js";

const Config = {
    tick_interval: 200,
    bLoop: true
}

const main = async () => {
    let game = new Game(Config)
    console.log(game)
    await game.init()
    game.run()
}


await main()
