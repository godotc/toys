use super::vector::*;

#[derive(Clone, Copy)]
pub struct Plane {
    pub point: Vec3,
    pub normal: Vec3,
}

impl Plane {
    pub fn new(p: &Vec3, n: &Vec3) -> Self {
        Plane {
            point: *p,
            normal: *n,
        }
    }

    pub fn calculate_point(&self, p: &Vec3) -> f32 {
        self.normal.dot(&(*p - self.point))
    }
}
