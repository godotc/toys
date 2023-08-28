use crate::math;
use std::collections::HashMap;

#[derive(Default)]
pub struct Uniforms {
    pub int: HashMap<u32, i32>,
    pub float: HashMap<u32, f32>,
    pub vec2: HashMap<u32, math::Vec2>,
    pub vec3: HashMap<u32, math::Vec3>,
    pub vec4: HashMap<u32, math::Vec4>,
    pub mat4: HashMap<u32, math::Mat4>,
    pub texture: HashMap<u32, u32>,
}

impl Uniforms {
    pub fn clear(&mut self) {
        self.int.clear();
        self.float.clear();
        self.vec2.clear();
        self.vec3.clear();
        self.vec4.clear();
        self.mat4.clear();
    }
}
