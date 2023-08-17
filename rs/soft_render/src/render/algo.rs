use criterion::criterion_group;
use crate::image::{ColorAttachment, DepthAttachment};
use crate::math;
use crate::render::viewport::{FaceCull, FrontFace};
use crate::texture::{Texture, TextureManager};
use crate::line::*;
use crate::shader;


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
    texure_storage: &TextureManager,
    color_attachment: &mut ColorAttachment,
    depth_attachment: &mut DepthAttachment,
) {
    let mut

}