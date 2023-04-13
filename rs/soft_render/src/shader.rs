use crate::math;

const MAX_ATTRIBUTES_NUM: usize = 4;

#[derive(Clone, Copy, Debug)]
pub struct Attributes {
    pub float: [f32; MAX_ATTRIBUTES_NUM],
    pub vec2: [math::Vec2; MAX_ATTRIBUTES_NUM],
    pub vec3: [math::Vec3; MAX_ATTRIBUTES_NUM],
    pub vec4: [math::Vec4; MAX_ATTRIBUTES_NUM],
}

impl Attributes {
    pub fn set_float(&mut self, location: usize, value: f32) {
        self.float[location] = value;
    }
    pub fn set_vec2(&mut self, location: usize, value: math::Vec2) {
        self.vec2[location] = value;
    }
    pub fn set_vec3(&mut self, location: usize, value: math::Vec3) {
        self.vec3[location] = value;
    }
    pub fn set_vec4(&mut self, location: usize, value: math::Vec4) {
        self.vec4[location] = value;
    }
}

impl Default for Attributes {
    fn default() -> Self {
        Self {
            float: [0.0; MAX_ATTRIBUTES_NUM],
            vec2: [math::Vec2::zero(); MAX_ATTRIBUTES_NUM],
            vec3: [math::Vec3::zero(); MAX_ATTRIBUTES_NUM],
            vec4: [math::Vec4::zero(); MAX_ATTRIBUTES_NUM],
        }
    }
}

#[derive(Clone, Copy, Debug)]
pub struct Vertex {
    pub position: math::Vec4,
    pub attributes: Attributes,
}
