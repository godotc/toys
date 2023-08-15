use super::vector::*;
use std::ops::{Add, Div, Mul, Neg, Sub};

macro_rules! declare_mat {
    ($name:ident, $dim:expr) => {
        #[derive(Debug, Clone, Copy)]
        pub struct $name {
            data: [f32; $dim * $dim],
        }

        impl $name {
            pub fn from_row(data: &[f32; $dim * $dim]) -> $name {
                $name { data: data.clone() }
            }

            pub fn from_col(data: &[f32; $dim * $dim]) -> $name {
                let mut mat = $name::zeros();
                for x in 0..$dim {
                    for y in 0..$dim {
                        mat.set(x, y, data[y + $dim * x]);
                    }
                }
                mat
            }

            pub fn zeros() -> $name {
                $name {
                    data: [0.; $dim * $dim],
                }
            }

            pub fn ones() -> $name {
                $name {
                    data: [1.; $dim * $dim],
                }
            }

            pub fn identity() -> $name {
                let mut result = $name::zeros();
                for i in 0..$dim {
                    result.set(i, i, 1.0);
                }
                result
            }

            pub fn get(&self, x: usize, y: usize) -> f32 {
                self.data[x + y * $dim]
            }

            pub fn set(&mut self, x: usize, y: usize, value: f32) {
                self.data[x + y * $dim] = value;
            }

            pub fn transpose(&self) -> $name {
                let mut result = $name::identity();
                for x in 0..$dim {
                    for y in 0..$dim {
                        result.set(y, x, self.get(x, y));
                    }
                }
                result
            }
        }

        impl Mul for $name {
            type Output = Self;

            fn mul(self, rhs: Self) -> Self::Output {
                let mut result = $name {
                    data: [0.0; $dim * $dim],
                };
                for i in 0..$dim {
                    for j in 0..$dim {
                        let mut sum = 0.0;
                        for k in 0..$dim {
                            sum += self.get(k, i) * rhs.get(j, k);
                        }
                        result.set(j, i, sum);
                    }
                }
                result
            }
        }

        impl Mul<f32> for $name {
            type Output = Self;

            fn mul(self, rhs: f32) -> Self::Output {
                let mut result = $name::zeros();
                for x in 0..$dim {
                    for y in 0..$dim {
                        result.set(x, y, self.get(x, y) * rhs);
                    }
                }
                result
            }
        }

        impl Div<f32> for $name {
            type Output = Self;

            fn div(self, rhs: f32) -> Self::Output {
                self * (1.0 / rhs)
            }
        }

        impl PartialEq for $name {
            fn eq(&self, other: &Self) -> bool {
                self.data == other.data
            }
        }
    };
}

declare_mat!(Mat2, 2);
declare_mat!(Mat3, 3);
declare_mat!(Mat4, 4);

impl Mul<Vec2> for Mat2 {
    type Output = Vec2;

    fn mul(self, rhs: Vec2) -> Self::Output {
        Vec2::new(
            self.get(0, 0) * rhs.x + self.get(1, 0) * rhs.y,
            self.get(0, 1) * rhs.x + self.get(1, 1) * rhs.y,
        )
    }
}

impl Mul<Vec3> for Mat3 {
    type Output = Vec3;

    fn mul(self, rhs: Vec3) -> Self::Output {
        Vec3::new(
            self.get(0, 0) * rhs.x + self.get(1, 0) * rhs.y + self.get(2, 0) * rhs.z,
            self.get(0, 1) * rhs.x + self.get(1, 1) * rhs.y + self.get(2, 1) * rhs.z,
            self.get(0, 2) * rhs.x + self.get(1, 2) * rhs.y + self.get(2, 2) * rhs.z,
        )
    }
}

impl Mul<Vec4> for Mat4 {
    type Output = Vec4;

    fn mul(self, rhs: Vec4) -> Self::Output {
        Vec4::new(
            self.get(0, 0) * rhs.x
                + self.get(1, 0) * rhs.y
                + self.get(2, 0) * rhs.z
                + self.get(3, 0) * rhs.w,
            self.get(0, 1) * rhs.x
                + self.get(1, 1) * rhs.y
                + self.get(2, 1) * rhs.z
                + self.get(3, 1) * rhs.w,
            self.get(0, 2) * rhs.x
                + self.get(1, 2) * rhs.y
                + self.get(2, 2) * rhs.z
                + self.get(3, 2) * rhs.w,
            self.get(0, 3) * rhs.x
                + self.get(1, 3) * rhs.y
                + self.get(2, 3) * rhs.z
                + self.get(3, 3) * rhs.w,
        )
    }
}

impl Mat2 {
    pub fn det(&self) -> f32 {
        self.get(0, 0) * self.get(1, 1) - self.get(1, 0) * self.get(0, 1)
    }

    #[rustfmt::skip]
    pub fn inverse(&self) -> Option<Self> {
        let d = self.det();
        if d.abs() <= f32::EPSILON {
            return None;
        }
        Some(Mat2::from_row(&[self.get(1, 1) / d, -self.get(1, 0) / d,
            -self.get(0, 1) / d, self.get(0, 0) / d]))
    }
}

impl Mat3 {
    #[rustfmt::skip]
    pub fn det(&self) -> f32 {
        self.get(0, 0) * self.get(1, 1) * self.get(2, 2)
            + self.get(2, 0) * self.get(0, 1) * self.get(1, 2)
            + self.get(1, 0) * self.get(2, 1) * self.get(0, 2)
            - (self.get(2, 0) * self.get(1, 1) * self.get(0, 2)
            + self.get(1, 0) * self.get(0, 1) * self.get(2, 2)
            + self.get(0, 0) * self.get(1, 2) * self.get(2, 1))
    }

    #[rustfmt::skip]
    pub fn inverse(&self) -> Option<Self> {
        let d = self.det();
        if d.abs() <= f32::EPSILON {
            return None;
        }
        Some(Mat3::from_row(&[
            self.get(1, 1) * self.get(2, 2) - self.get(2, 1) * self.get(1, 2),
            self.get(2, 0) * self.get(1, 2) - self.get(1, 0) * self.get(2, 2),
            self.get(1, 0) * self.get(2, 1) - self.get(2, 0) * self.get(1, 1),
            self.get(2, 1) * self.get(0, 2) - self.get(0, 1) * self.get(2, 2),
            self.get(0, 0) * self.get(2, 2) - self.get(2, 0) * self.get(0, 2),
            self.get(0, 1) * self.get(2, 1) - self.get(0, 0) * self.get(2, 0),
            self.get(0, 1) * self.get(1, 2) - self.get(1, 1) * self.get(0, 2),
            self.get(1, 0) * self.get(0, 2) - self.get(0, 0) * self.get(1, 2),
            self.get(0, 0) * self.get(1, 1) - self.get(1, 0) * self.get(0, 1),
        ]) / d)
    }
}

impl Mat4 {
    #[rustfmt::skip]
    pub fn truncated_to_mat3(&self) -> Mat3 {
        Mat3::from_row(&[
            self.get(0, 0), self.get(1, 0), self.get(2, 0),
            self.get(0, 1), self.get(1, 1), self.get(2, 1),
            self.get(0, 2), self.get(1, 2), self.get(2, 2),
        ])
    }

    pub fn get_algebraic_cofactor(&self, x: usize, y: usize) -> Mat3 {
        let mut result = Mat3::identity();
        for x_iter in 0..4 {
            if x_iter == x {
                continue;
            }
            for y_iter in 0..4 {
                if y_iter == y {
                    continue;
                }

                let real_x = if x_iter > x { x_iter - 1 } else { x_iter };
                let real_y = if y_iter > y { y_iter - 1 } else { y_iter };
                result.set(real_x, real_y, self.get(x_iter, y_iter));
            }
        }
        result
    }

    pub fn get_cofactor(&self, x: usize, y: usize) -> Mat3 {
        self.get_algebraic_cofactor(x, y) * if (x + y) % 2 == 0 { 1 } else { -1 } as f32
    }

    #[rustfmt::skip]
    pub fn det(&self) -> f32 {
        self.get_cofactor(0, 0).det() * self.get(0, 0)
            + self.get_cofactor(1, 0).det() * self.get(1, 0)
            + self.get_cofactor(2, 0).det() * self.get(2, 0)
            + self.get_cofactor(3, 0).det() * self.get(3, 0)
    }

    #[rustfmt::skip]
    pub fn inverse(&self) -> Option<Mat4> {
        let d = self.det();
        if d.abs() <= std::f32::EPSILON {
            return None;
        }

        let mut result = Mat4::identity();
        for x in 0..4 {
            for y in 0..4 {
                result.set(x, y, self.get_cofactor(x, y).det() / d);
            }
        }
        Some(result.transpose())
    }
}
