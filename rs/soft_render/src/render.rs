use crate::{image::*, math, shader};
use std::{array, mem::swap, ptr::null};

use self::cohen_sutherland::cohen_sutherland_line_clip;

pub struct Viewport {
    x: f32,
    y: f32,
    w: f32,
    h: f32,
}

#[derive(PartialEq, Eq)]
pub enum FaceCull {
    Front,
    Back,
    None,
}

pub enum FrontFace {
    CW,
    CCW,
}

pub enum TopologyType {
    LineList,
    TriangleList,
}

pub struct Render {
    //camera: Box<dyn camera::Camera>,
    viewport: Viewport,

    color_attachment: ColorAttachment,
    //depth_attachment: DepthAttachment,
    face_cull: FaceCull,
    front_face: FrontFace,
    //render_type: RenderType,

    //render_list: Vec<TopologyData>,
    //generated_faces: Vec<TopologyData>,
    //   pub uniforms: shader::Uniforms,
    //   pub pixel_shading: shader::PixelShading,
    //   pub vertex_changing: shader::VertexChanging,
    //   pub lights: Vec<light::LightType>,
}

impl Render {
    pub fn new(
        w: u32,
        h: u32,
        //vertex_changing: shader::VertexChanging,
        //pixel_shading: shader::PixelShading,
    ) -> Self {
        Self {
            viewport: Viewport {
                x: 0.0,
                y: 0.0,
                w: w as f32,
                h: h as f32,
            },
            color_attachment: ColorAttachment::new(w, h),
            face_cull: todo!(),
            front_face: todo!(),
        }
    }
    pub fn draw_line(&mut self, p1: &math::Vec2, p2: &math::Vec2, color: &math::Vec4) {
        let clip_result = cohen_sutherland_line_clip(
            p1,
            p2,
            &math::Vec2::zero(),
            &math::Vec2::new(
                self.color_attachment.width() as f32 - 1.0,
                self.color_attachment.width() as f32 - 1.0,
            ),
        );

        if let Some((p1, p2)) = clip_result {
            self.draw_line_without_clip(p1.x as i32, p1.y as i32, p2.x as i32, p2.y as i32, color);
        }
    }

    fn draw_line_without_clip(&mut self, x0: i32, y0: i32, x1: i32, y1: i32, color: &math::Vec4) {
        let mut dx: i32 = x1 - x0;
        let mut dy: i32 = y1 - y0;
        let mut sx = if x1 > x0 { 1 } else { -1 };
        let mut sy = if y1 > y0 { 1 } else { -1 };
        let mut x: i32 = x0;
        let mut y = y0;
        let steep: bool = if dx < dy { true } else { false };

        let final_x = if dx < dy { y1 } else { x1 };

        if dx < dy {
            std::mem::swap(&mut dx, &mut dy);
            std::mem::swap(&mut x, &mut y);
            std::mem::swap(&mut sx, &mut sy)
        }

        let mut e = -dy;
        let step = 2 * dy;
        let desc = -2 * dx;

        while x != final_x {
            if steep {
                self.color_attachment.set(y as u32, x as u32, color);
            } else {
                self.color_attachment.set(x as u32, y as u32, color);
            }

            e += step;
            if e >= 0 {
                y += sy;
                e += desc;
            }
            x += sx;
        }
    }
}

mod cohen_sutherland {

    use super::math;

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
            if outcode1 & outcode2 != 0x00 {
                return None;
            }
            if outcode1 | outcode2 == 0x00 {
                return Some((pt1, pt2));
            }

            let mut p = math::Vec2::zero();

            let outcode = if outcode2 > outcode1 {
                outcode2
            } else {
                outcode1
            };

            // clip on top edge
            if outcode & TOP != 0 {
                p.x = p1.x + (pt2.x - pt1.x) * (rect_max.y - pt1.y) / (pt2.y - pt1.y);
                p.y = rect_max.y;
            } else if outcode & BOTTOM != 0 {
                p.x = p1.x + (pt2.x - pt1.x) * (rect_min.y - pt1.y) / (pt2.y - pt1.y);
                p.y = rect_min.y;
            } else if outcode & RIGHT != 0 {
                p.y = p1.y + (pt2.y - pt1.y) * (rect_max.x - pt1.x) / (pt2.x - pt1.x);
                p.x = rect_max.x;
            } else if outcode & LEFT != 0 {
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
        if p.x < rect_min.x {
            LEFT
        } else if p.x > rect_max.x {
            RIGHT
        } else if p.y > rect_max.y {
            TOP
        } else if p.y < rect_min.y {
            BOTTOM
        } else {
            INSIDE
        }
    }
}
