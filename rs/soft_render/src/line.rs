use crate::shader::{attribute::interp_attributes_v0, vertex::Vertex};

pub(crate) struct Line {
    pub start: Vertex,
    pub end: Vertex,
    pub step: Vertex,
}

impl Line {
    pub fn new(start: Vertex, end: Vertex) -> Self {
        let dx = (end.position.x - start.position.x).abs();
        let dy = (end.position.y - start.position.y).abs();
        let t = if dx >= dy { 1.0 / dx } else { 1.0 / dy };

        Self {
            start,
            end,
            step: Vertex {
                position: (end.position - start.position) * t,
                attributes: interp_attributes_v0(
                    &start.attributes,
                    &end.attributes,
                    |v1, v2, t| (v1 - v2) * t,
                    t,
                ),
            },
        }
    }
}
