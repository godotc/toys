use crate::math;
use crate::math::Vec4;

use super::attribute::*;
use super::uniform::*;
use super::vertex::*;

use crate::texture::*;

pub type OnVertexChanged = Box<dyn Fn(&Vertex, &Uniforms, &TextureManager) -> Vertex>;
pub type OnPixelShaded = Box<dyn Fn(&Attributes, &Uniforms, &TextureManager) -> math::Vec4>;

pub struct Shader {
    pub on_vertex_changed: OnVertexChanged,
    pub on_pixel_shaded: OnPixelShaded,

    pub unifrom: Uniforms,
}

impl Shader {
    // location
    pub fn call_vertex_change(
        &self,
        vertex: &Vertex,
        uniform: &Uniforms,
        texture_storage: &TextureManager,
    ) -> Vertex {
        (self.on_vertex_changed)(vertex, uniform, texture_storage)
    }

    // color
    pub fn call_pixel_shade(
        &self,
        attr: &Attributes,
        uniform: &Uniforms,
        texture_storage: &TextureManager,
    ) -> math::Vec4 {
        (self.on_pixel_shaded)(attr, uniform, texture_storage)
    }
}

impl Default for Shader {
    fn default() -> Self {
        Self {
            on_vertex_changed: Box::new(|vertex, _, _| *vertex),
            on_pixel_shaded: Box::new(|_, _, _| Vec4::new(0.0, 0.0, 0.0, 1.0)),
            unifrom: Uniforms::default(),
        }
    }
}
