use std::default::Default;
use std::f32::consts::PI;
use std::ops::{Add, AddAssign, Div, DivAssign, Mul, MulAssign, Neg, Sub, SubAssign};

pub const PI_DIV_2: f32 = std::f32::consts::FRAC_PI_2;
pub const PI_DIV_4: f32 = std::f32::consts::FRAC_PI_4;
pub const TWO_PI: f32 = PI * 2.0;
pub const PI_INV: f32 = 1.0 / PI;

/**
* type {
    x: self.x + other.x
    y: self.y + other.y
    ....
    }
 */
macro_rules! declare_vec_op {
    ($name:ident, $trait_name:ident, $func_name:ident, $op:tt, $($attr:ident),+) => {
        impl $trait_name for $name {
            type Output = Self;

            fn $func_name(self, rhs: Self) -> Self::Output {
                $name {
                    $(
                        $attr: self.$attr $op rhs.$attr,
                    )+
                }
            }
        }
    };
}


/**
self.x += other.x
self.y += other.y
....
 */

macro_rules! declare_vec_op_assign {
    ($name:ident, $trait_name:ident, $func_name:ident, $op:tt, $($attr:ident),+) => {
        impl $trait_name for $name {
            fn $func_name(&mut self, rhs: Self) {
                $(
                    self.$attr $op rhs.$attr;
                )+
            }
        }
    };
}

macro_rules! declare_vec {
    ($name:ident, $($attr:ident),+) => {
        #[derive(Debug, PartialEq, Copy, Clone, Default)]
        pub struct $name {
            $(
                pub $attr : f32,
            )+
        }

        impl $name {
            pub const fn new($( $attr: f32,)+) -> $name {
                $name {
                    $( $attr, )+
                }
            }

            pub fn zero() -> $name {
                $name {
                    $( $attr: 0f32, )+
                }
            }

            pub fn length_square(&self) -> f32 {
                $(
                    self.$attr * self.$attr +
                )+
                0.0
            }

            pub fn length(&self) -> f32 {
                self.length_square().sqrt()
            }

            pub fn normalize(&self) -> $name {
                *self / self.length()
            }

            pub fn dot(&self, rhs: &$name) -> f32 {
                $(
                    self.$attr * rhs.$attr +
                )+
                0.0
            }
        }

        declare_vec_op!($name, Add, add, + $(,$attr)+);
        declare_vec_op!($name, Sub, sub, - $(,$attr)+);
        declare_vec_op!($name, Mul, mul, * $(,$attr)+);
        declare_vec_op!($name, Div, div, / $(,$attr)+);

        impl Neg for $name {
            type Output = Self;

            fn neg(self) -> Self::Output {
                Self::new(
                    $(
                        -self.$attr,
                    )+
                )
            }
        }


        impl Mul<f32> for $name {
            type Output = $name;

            fn mul(self, rhs: f32) -> Self::Output {
                $name {
                    $(
                        $attr: self.$attr * rhs,
                    )+
                }
            }
        }

        impl Mul<$name> for f32 {
            type Output = $name;

            fn mul(self, rhs: $name) -> Self::Output {
                rhs * self
            }
        }

        impl Div<f32> for $name {
            type Output = $name;

            fn div(self, rhs: f32) -> Self::Output {
                $name {
                    $(
                        $attr: self.$attr / rhs,
                    )+
                }
            }
        }

        impl Div<$name> for f32 {
            type Output = $name;

            fn div(self, rhs: $name) -> Self::Output {
                $name {
                    $(
                        $attr: self / rhs.$attr,
                    )+
                }
            }
        }

        declare_vec_op_assign!($name, AddAssign, add_assign, += $(,$attr)+ );
        declare_vec_op_assign!($name, SubAssign, sub_assign, -= $(,$attr)+ );
        declare_vec_op_assign!($name, MulAssign, mul_assign, *= $(,$attr)+ );
        declare_vec_op_assign!($name, DivAssign, div_assign, /= $(,$attr)+ );


        impl MulAssign<f32> for $name {
            fn mul_assign(&mut self, rhs: f32) {
                $(
                    self.$attr *= rhs;
                )+
            }
        }

        impl DivAssign<f32> for $name {
            fn div_assign(&mut self, rhs: f32) {
                $(
                    self.$attr /= rhs;
                )+
            }
        }
    };
}

declare_vec!(Vec2, x, y);
declare_vec!(Vec3, x, y, z);
declare_vec!(Vec4, x, y, z, w);

impl Vec2 {
    pub fn cross(&self, rhs: &Vec2) -> f32 {
        self.x * rhs.y - self.y * rhs.x
    }

    pub fn x_axis() -> &'static Vec2 {
        const AXIS: Vec2 = Vec2::new(1.0, 0.0);
        &AXIS
    }

    pub fn y_axis() -> &'static Vec2 {
        const AXIS: Vec2 = Vec2::new(0.0, 1.0);
        &AXIS
    }
}

impl Vec3 {
    pub fn from_vec2(v: &Vec2, z: f32) -> Self {
        Self { x: v.x, y: v.y, z }
    }

    pub fn cross(&self, rhs: &Vec3) -> Vec3 {
        Vec3 {
            x: self.y * rhs.z - self.z * rhs.y,
            y: self.z * rhs.x - self.x * rhs.z,
            z: self.x * rhs.y - self.y * rhs.x,
        }
    }

    pub fn x_axis() -> &'static Vec3 {
        const AXIS: Vec3 = Vec3::new(1.0, 0.0, 0.0);
        &AXIS
    }

    pub fn y_axis() -> &'static Vec3 {
        const AXIS: Vec3 = Vec3::new(0.0, 1.0, 0.0);
        &AXIS
    }

    pub fn z_axis() -> &'static Vec3 {
        const AXIS: Vec3 = Vec3::new(0.0, 0.0, 1.0);
        &AXIS
    }
}

impl Vec4 {
    pub fn from_vec3(v: &Vec3, w: f32) -> Self {
        Self {
            x: v.x,
            y: v.y,
            z: v.z,
            w,
        }
    }

    pub fn truncated_to_vec3(&self) -> Vec3 {
        Vec3 {
            x: self.x,
            y: self.y,
            z: self.z,
        }
    }

    pub fn truncated_to_vec2(&self) -> Vec2 {
        Vec2 {
            x: self.x,
            y: self.y,
        }
    }
}

// calculates the reflected vec from (in_vec and normal_vec)
pub fn reflect(in_vec: &Vec3, normal: &Vec3) -> Vec3 {
    2.0 * (in_vec.dot(&normal)) * (*normal) - (*in_vec)
}
