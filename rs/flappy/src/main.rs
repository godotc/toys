use bracket_lib::prelude::*;
mod crate::base;
mod crate::constant;

use base::state::*;

fn main() -> BError {
    println!("Hello, world!");
    let context = BTermBuilder::simple80x50()
        .with_title("flappy bird")
        .build()?;

    main_loop(context, State::new())
}
