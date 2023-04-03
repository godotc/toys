use bracket_lib::prelude::*;

use crate::constant::SCREEN_HEIGHT;

use super::player::Player;

pub struct Obstacle {
    pub x: i32, // world x
    gap_y: i32,
    size: i32,
}

impl Obstacle {
    pub fn new(x: i32, score: i32) -> Self {
        let mut random = RandomNumberGenerator::new();
        Obstacle {
            x,
            gap_y: random.range(10, 40),
            size: i32::max(2, 20 - score),
        }
    }

    pub fn render(&mut self, ctx: &mut BTerm, player_x: i32) {
        // TODO: render issue
        self.x -= player_x;
        let screen_x = self.x;
        let half_size = self.size / 2;

        for y in 0..(half_size - self.gap_y) {
            ctx.set(screen_x, y, RED, BLACK, to_cp437('|'));
        }
        for y in (half_size + self.gap_y)..SCREEN_HEIGHT {
            ctx.set(screen_x, y, RED, BLACK, to_cp437('|'));
        }
    }

    pub fn hit_obstacle(&self, player: &Player) -> bool {
        // TODO: hit issue
        let half_size = self.size / 2;
        let dose_x_match = player.x == self.x;
        let player_above_gap = player.x < (half_size - self.gap_y);
        let player_below_gap = player.x > (self.gap_y + half_size);

        dose_x_match & (player_above_gap || player_below_gap)
    }
}
