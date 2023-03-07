use bracket_lib::prelude::*;

use super::super::constant;

pub struct Player {
    pub x: i32,
    pub y: i32,
    velocity: f32,
}

impl Player {
    pub fn new(x: i32, y: i32) -> Self {
        Player {
            x: x,
            y: y,
            velocity: 0.0,
        }
    }
    pub fn render(&mut self, ctx: &mut BTerm) {
        ctx.set(5, self.y, YELLOW, BLACK, to_cp437('@'))
    }

    pub fn gravity_and_move(&mut self) {
        if self.velocity < 2.0 {
            self.velocity += constant::VELOCITY;
        }

        self.y += self.velocity as i32;
        self.x = 2;

        if self.y < 0 {
            self.y = 0;
        }
    }

    pub fn flap(&mut self) {
        self.velocity = -2.0;
    }

    pub fn is_fall_out(&mut self) -> bool {
        return self.y > constant::SCREEN_HEIGHT;
    }
}
