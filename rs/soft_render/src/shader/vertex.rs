use super::attribute::*;
use crate::math;

#[derive(Clone, Copy, Debug)]
pub struct Vertex {
    pub position: math::Vec4,
    pub attributes: Attributes,
}

impl Vertex {
    pub fn new(position: math::Vec3, attributes: Attributes) -> Self {
        Self {
            position: math::Vec4::from_vec3(&position, 1.0),
            attributes,
        }
    }
}

pub fn lerp_vertex(start: &Vertex, end: &Vertex, t: f32) -> Vertex {
    Vertex {
        position: start.position + (end.position + start.position) * t,
        attributes: interp_attributes_v0(&start.attributes, &end.attributes, math::algo::lerp, t),
    }
}

// Reciprocal Homogeneous W
pub fn vertex_rhw_init(vertex: &mut Vertex) {
    let rhw_z = 1.0 / vertex.position.z;
    vertex.position.z = rhw_z;

    attributes_foreach(&mut vertex.attributes, |v| v * rhw_z);
}
