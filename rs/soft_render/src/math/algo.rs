use std::ops::{Add, Mul, Sub};
use super::vector::*;
use super::matrix::*;

#[rustfmt::skip]
pub fn create_translate(offset: &Vec3) -> Mat4 {
    Mat4::from_row(&[
        1.0, 0.0, 0.0, offset.x,
        0.0, 1.0, 0.0, offset.y,
        0.0, 0.0, 1.0, offset.z,
        0.0, 0.0, 0.0, 1.0,
    ])
}

#[rustfmt::skip]
pub fn create_scale(scale: &Vec3) -> Mat4 {
    Mat4::from_row(&[
        scale.x, 0.0, 0.0, 0.0,
        0.0, scale.y, 0.0, 0.0,
        0.0, 0.0, scale.z, 0.0,
        0.0, 0.0, 0.0, 1.0,
    ])
}

#[rustfmt::skip]
pub fn create_eular_rotate_x(angle: f32) -> Mat4 {
    let c = angle.cos();
    let s = angle.sin();
    Mat4::from_row(&[
        1.0, 0.0, 0.0, 0.0,
        0.0, c, -s, 0.0,
        0.0, s, c, 0.0,
        0.0, 0.0, 0.0, 1.0,
    ])
}

#[rustfmt::skip]
pub fn create_eular_rotate_y(angle: f32) -> Mat4 {
    let c = angle.cos();
    let s = angle.sin();
    Mat4::from_row(&[
        c, 0.0, -s, 0.0,
        0.0, 1.0, 0.0, 0.0,
        s, 0.0, c, 0.0,
        0.0, 0.0, 0.0, 1.0,
    ])
}

#[rustfmt::skip]
pub fn create_eular_rotate_z(angle: f32) -> Mat4 {
    let c = angle.cos();
    let s = angle.sin();
    Mat4::from_row(&[
        c, -s, 0.0, 0.0,
        s, c, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0,
    ])
}

pub fn create_eular_rotate_xyz(rotation: &Vec3) -> Mat4 {
    create_eular_rotate_z(rotation.z)
        * create_eular_rotate_y(rotation.y)
        * create_eular_rotate_x(rotation.x)
}

/// axis must be normalized
pub fn rotate_by_axis_rodrigues(rotation: f32, v: &Vec3, axis: &Vec3) -> Vec3 {
    let c = rotation.cos();
    let s = rotation.sin();

    c * *v + axis.dot(v) * *axis * (1.0 - c) + s * axis.cross(v)
}


pub fn lerp<T, F>(a: T, b: T, f: F) -> T
    where
        T: Clone + Add<T, Output=T> + Sub<T, Output=T> + Mul<F, Output=T>,
{
    a.clone() + (b - a) * f
}

