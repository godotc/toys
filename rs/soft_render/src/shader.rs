use std::char::MAX;
use crate::math::algo;
use crate::math::vector;
use crate::math::vector::*;

use std::concat;

const MAX_ATTRIBUTES_NUM: usize = 4;

#[derive(Clone, Copy, Debug)]
pub struct Attributes {
    pub float: [f32; MAX_ATTRIBUTES_NUM],
    pub vec2: [vector::Vec2; MAX_ATTRIBUTES_NUM],
    pub vec3: [vector::Vec3; MAX_ATTRIBUTES_NUM],
    pub vec4: [vector::Vec4; MAX_ATTRIBUTES_NUM],
}

#[derive(Clone, Copy, Debug)]
pub struct Vertex {
    pub position: vector::Vec4,
    pub attributes: Attributes,
}

impl Attributes {
    pub fn set_float(&mut self, location: usize, value: f32) { self.float[location] = value; }
    pub fn set_vec2(&mut self, location: usize, value: vector::Vec2) {
        self.vec2[location] = value;
    }
    pub fn set_vec3(&mut self, location: usize, value: vector::Vec3) { self.vec3[location] = value; }
    pub fn set_vec4(&mut self, location: usize, value: vector::Vec4) {
        self.vec4[location] = value;
    }
}

impl Default for Attributes {
    fn default() -> Self {
        Self {
            float: [0.0; MAX_ATTRIBUTES_NUM],
            vec2: [vector::Vec2::zero(); MAX_ATTRIBUTES_NUM],
            vec3: [vector::Vec3::zero(); MAX_ATTRIBUTES_NUM],
            vec4: [vector::Vec4::zero(); MAX_ATTRIBUTES_NUM],
        }
    }
}

pub fn lerp_vertex(start: &Vertex, end: &Vertex, t: f32) -> Vertex {
    Vertex {
        position: start.position + (end.position + start.position) * t,
        attributes: interp_attributes(&start.attributes, &end.attributes, algo::lerp, t),
    }
}

pub fn interp_attributes<F>(attr1: &Attributes, attr2: &Attributes, interp_fn: F, t: f32) -> Attributes
    where
        F: Fn(f32, f32, f32) -> f32,
{
    let mut attributes = Attributes::default();


    // maybe interp func not the lerp (e.g some regression...)
    macro_rules! interp_attr {
        ($interp_fn:expr, $attr1:expr, $attr2:expr, $t:expr, $index:tt, $($sub_attr:ident),+) => {
            attributes.
            (concat_idents!(vec, $index)) [$index] =
                concat_idents!(Vec, $index)::new{
                $(
                    concat_idents!(vec,$index): $interp_fn(
                        $attr1.concat_idents!($vec,$index) [$index].$sub_attr,
                        $attr2.concat_idents!($vec,$index) [$index].$sub_attr,
                        $t)
                ),+
            };
        };
    }

    Attributes {
        float: attr1.float.into_iter().map(|x|, attr2.float.)
    }

    // f32
    for i in 0..MAX_ATTRIBUTES_NUM {
        attributes.
    }

    // vec2
    for i in 0..MAX_ATTRIBUTES_NUM {
        interp_attr!(interp_fn, attr1, attr2, t, i, x, y);
    }

    // vec3
    for i in 0..MAX_ATTRIBUTES_NUM {
        interp_attr!(interp_fn, attr1, attr2, t, i, x, y, z);
    }

    // vec4
    for i in 0..MAX_ATTRIBUTES_NUM {
        interp_attr!(interp_fn, attr1, attr2, t, i, x, y, z, w);
    }

    attributes
}

