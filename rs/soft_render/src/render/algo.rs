use crate::image::{ColorAttachment, DepthAttachment};
use crate::math;
use crate::render::viewport::{FaceCull, FrontFace};
use crate::texture::{Texture, TextureManager};
use crate::line::*;
use crate::render::bresenham::Bresenham;
use crate::shader;
use crate::shader::attribute::{interp_attributes};


// texture( tex, texCoord)
pub fn texture_sample(texture: &Texture, texcoord: &math::Vec2) -> math::Vec4 {
    let x = texcoord.x * (texture.width() - 1) as f32;
    let y = texcoord.y * (texture.width() - 1) as f32;
    texture.get(x as u32, y as u32)
}

pub(crate) fn should_cull(
    positions: &[math::Vec3; 3],
    view_dir: &math::Vec3,
    face: FrontFace,
    cull: FaceCull,
) -> bool {
    if cull == FaceCull::None { return false; }

    let norm = (positions[1] - positions[0]).cross(&(positions[2] - positions[1]));
    let is_front_face = match face {
        FrontFace::CW => norm.dot(view_dir) > 0.0,
        FrontFace::CCW => norm.dot(view_dir) <= 0.0,
    };

    match cull {
        FaceCull::Front => is_front_face,
        FaceCull::Back => !is_front_face,
        FaceCull::None => false,
    }
}

pub(crate) fn rasterize_line(
    line: &mut Line,
    shading: &shader::OnPixelShaded,
    uniform: &shader::Uniforms,
    texture_storage: &TextureManager,
    color_attachment: &mut ColorAttachment,
    depth_attachment: &mut DepthAttachment,
) {
    let mut bresenham = Bresenham::new(
        &line.start.position.truncated_to_vec2(),
        &line.end.position.truncated_to_vec2(),
        &math::Vec2::zero(),
        &math::Vec2::new(
            color_attachment.width() as f32 - 1.0,
            color_attachment.height() as f32 - 1.0,
        ),
    );

    if let Some(iter) = &mut bresenham {
        let mut postion = iter.next();
        let mut vertex = line.start;

        while postion.is_some() {
            let (x, y) = postion.unwrap();

            let rhw = vertex.position.z;
            let z = 1.0 / rhw;

            let x = x as u32;
            let y = y as u32;

            if depth_attachment.get(x, y) <= z {
                let mut attr = vertex.attributes;
                shader::attribute::attributes_foreach(&mut attr, |value| value / rhw);
                let color = shading(&attr, uniform, texture_storage);
                color_attachment.set(x, y, &color);
                depth_attachment.set(x, y, z);
            }

            vertex.position += line.step.position;
            vertex.attributes = interp_attributes(
                &vertex.attributes,
                &line.step.attributes,
                |v1, v2, _| v1 + v2,
                0.0,
            );

            postion = iter.next();
        }
    }
}