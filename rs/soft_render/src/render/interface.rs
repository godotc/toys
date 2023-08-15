use super::viewport::*;
use crate::shader::shader::*;
use crate::shader::uniform::*;
use crate::{math, shader::vertex::Vertex, texture::TextureManager};

pub trait RenderInterface {
    fn clear(&mut self, color: &math::Vec4);
    fn clear_depth(&mut self);

    fn get_canvas_width(&mut self) -> i32;
    fn get_canvas_height(&mut self) -> i32;

    fn draw_triangle(
        &mut self,
        model: &math::Mat4,
        vertices: &[Vertex],
        texture_manager: &TextureManager,
    );

    fn get_rendered_iamge(&self) -> &[u8];
    fn get_rendered_image(&self) -> &[u8];
    fn get_shader(&mut self) -> &mut Shader;
    fn get_uniforms(&mut self) -> &mut Uniforms;
    fn get_camera(&mut self) -> &mut Camera;
    fn set_camera(&mut self, camera: Camera);
    fn set_front_face(&mut self, front_face: FrontFace);
    fn get_front_face(&self) -> FrontFace;
    fn set_face_cull(&mut self, cull: FaceCull);
    fn get_face_cull(&self) -> FaceCull;
    fn enable_framework(&mut self);
    fn disable_framework(&mut self);
    fn toggle_framework(&mut self);
}
