use std::ops::{Add, Div, Mul, Neg, Sub};
use super::vector::*;

// Quaternion
pub struct Quaternion {
    pub s: f32,
    pub v: Vec3,
}

impl Mul<f32> for Quaternion {
    type Output = Self;

    fn mul(self, rhs: f32) -> Self::Output {
        Self {
            s: rhs * self.s,
            v: rhs * self.v,
        }
    }
}

impl Div<f32> for Quaternion {
    type Output = Self;

    fn div(self, rhs: f32) -> Self::Output {
        self * (1.0 / rhs)
    }
}

impl Mul<Quaternion> for f32 {
    type Output = Quaternion;

    fn mul(self, rhs: Quaternion) -> Self::Output {
        rhs * self
    }
}

impl Add for Quaternion {
    type Output = Self;

    fn add(self, rhs: Self) -> Self::Output {
        Self {
            s: self.s + rhs.s,
            v: self.v + rhs.v,
        }
    }
}

impl Sub for Quaternion {
    type Output = Self;

    fn sub(self, rhs: Self) -> Self::Output {
        self + (-rhs)
    }
}

impl Neg for Quaternion {
    type Output = Self;

    fn neg(self) -> Self::Output {
        Self {
            s: -self.s,
            v: -self.v,
        }
    }
}

impl Quaternion {
    pub fn length_square(&self) -> f32 {
        self.s * self.s + self.v.length_square()
    }

    pub fn length(&self) -> f32 {
        self.length_square().sqrt()
    }

    pub fn conjugate(&self) -> Quaternion {
        Quaternion {
            s: self.s,
            v: -self.v,
        }
    }

    // Hamilton product
    pub fn mul(&self, rhs: &Quaternion) -> Quaternion {
        Quaternion {
            s: self.s * rhs.s - self.v.dot(&rhs.v),
            v: self.s * rhs.v + self.v * rhs.s + self.v.cross(&rhs.v),
        }
    }

    pub fn inverse(&self) -> Quaternion {
        self.conjugate() / self.length()
    }
}
