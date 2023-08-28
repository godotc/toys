use std::mem::swap;
use crate::math::Vec2;
use crate::render::bresenham::cohen_sutherland::cohen_sutherland_line_clip;

pub(crate) struct Bresenham {
    final_x: i32,
    x: i32,
    y: i32,
    steep: i32,
    step: i32,
    e: i32,
    // step in x/y
    sy: i32,
    sx: i32,
    desc: i32,
}

impl Bresenham {
    pub fn new(
        p1: &Vec2,
        p2: &Vec2,
        min: &Vec2,
        max: &Vec2,
    ) -> Option<Self> {
        let clip_result = cohen_sutherland_line_clip(p1, p2, min, max);

        if let Some((v1, v2)) = clip_result {
            let (x0, y0) = v1.into();
            let (x1, y1) = v2.into();


            let mut dx = (x1 - x0).abs();
            let mut dy = (y1 - y0).abs();

            let mut sx = if x1 >= x0 { 1 } else { -1 };
            let mut sy = if y1 >= y0 { 1 } else { -1 };

            let mut x = x0;
            let mut y = y0;

            // steep (陡峭)
            let steep = if dx < dy { 1 } else { -1 };

            let final_x = if dx < dy { y1 } else { x1 };

            if dx < dy {
                swap(&mut dx, &mut dy);
                swap(&mut x, &mut y);
                swap(&mut sx, &mut sy);
            }

            let e = -dx;
            let step = 2 * dy as i32;
            let desc = -2 * dx as i32;


            Some(Self {
                final_x: final_x as i32,
                x: x as i32,
                y: y as i32,
                steep,
                e: e as i32,
                sy,
                sx,
                desc,
                step,
            })
        } else {
            None
        }
    }
}

impl Iterator for Bresenham {
    type Item = (i32, i32);

    fn next(&mut self) -> Option<Self::Item> {
        if self.x == self.final_x { return None; }

        let result = if self.steep > 0 {
            (self.y, self.x)
        } else {
            (self.x, self.y)
        };

        self.e += self.steep;
        if self.e >= 0 {
            self.y += self.sy;
            self.e += self.desc;
        }
        self.x += self.sx;


        Some(result)
    }
}

mod cohen_sutherland {
    use crate::math;

    const INSIDE: u8 = 0b000;
    const LEFT: u8 = 0b001;
    const RIGHT: u8 = 0b010;
    const BOTTOM: u8 = 0b100;
    const TOP: u8 = 0b0100;

    pub fn cohen_sutherland_line_clip(
        p1: &math::Vec2,
        p2: &math::Vec2,
        rect_min: &math::Vec2,
        rect_max: &math::Vec2,
    ) -> Option<(math::Vec2, math::Vec2)> {
        let mut pt1 = *p1;
        let mut pt2 = *p2;

        let mut outcode1 = compute_outcode(&pt1, rect_min, rect_max);
        let mut outcode2 = compute_outcode(&pt2, rect_min, rect_max);

        loop {
            // all outside
            if outcode1 & outcode2 != 0x00 {
                return None;
            }
            // all inner
            if outcode1 | outcode2 == 0x00 {
                return Some((pt1, pt2));
            }

            let mut p = math::Vec2::zero();

            let out_code =
                if outcode2 > outcode1 {
                    outcode2
                } else {
                    outcode1
                };

            // clip on top edge
            if out_code & TOP != 0 {
                p.x = p1.x + (pt2.x - pt1.x) * (rect_max.y - pt1.y) / (pt2.y - pt1.y);
                p.y = rect_max.y;
            } else if out_code & BOTTOM != 0 {
                p.x = p1.x + (pt2.x - pt1.x) * (rect_min.y - pt1.y) / (pt2.y - pt1.y);
                p.y = rect_min.y;
            } else if out_code & RIGHT != 0 {
                p.y = p1.y + (pt2.y - pt1.y) * (rect_max.x - pt1.x) / (pt2.x - pt1.x);
                p.x = rect_max.x;
            } else if out_code & LEFT != 0 {
                p.y = p1.y + (pt2.y - pt1.y) * (rect_min.x - pt1.x) / (pt2.x - pt1.x);
                p.x = rect_min.x;
            }

            if outcode1 == outcode2 {
                pt1 = p;
                outcode1 = compute_outcode(&pt1, rect_min, rect_max)
            } else {
                pt2 = p;
                outcode2 = compute_outcode(&pt2, rect_min, rect_max)
            }
        }
    }

    fn compute_outcode(p: &math::Vec2, rect_min: &math::Vec2, rect_max: &math::Vec2) -> u8 {
        (
            if p.x < rect_min.x {
                LEFT
            } else if p.x > rect_max.x {
                RIGHT
            } else { INSIDE }
                |
                if p.y > rect_max.y {
                    TOP
                } else if p.y < rect_min.y {
                    BOTTOM
                } else {
                    INSIDE
                }
        )
    }
}
