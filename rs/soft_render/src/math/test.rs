#[cfg(test)]
mod test {
    use super::super::vector::*;
    use super::super::matrix::*;
    use super::super::plane::*;
    use super::super::quaternion::*;
    use super::super::algo::*;

    #[test]
    fn vector_new() {
        let v2 = Vec2::new(1.0, 2.0);
        assert_eq!(v2.x, 1.0);
        assert_eq!(v2.y, 2.0);

        let v3 = Vec3::new(1.0, 2.0, 3.0);
        assert_eq!(v3.x, 1.0);
        assert_eq!(v3.y, 2.0);
        assert_eq!(v3.z, 3.0);

        let v4 = Vec4::new(1.0, 2.0, 3.0, 0.5);
        assert_eq!(v4.x, 1.0);
        assert_eq!(v4.y, 2.0);
        assert_eq!(v4.z, 3.0);
        assert_eq!(v4.w, 0.5);
    }

    #[test]
    fn vector_eq() {
        let v1 = Vec2::new(1.0, 2.0);
        let v2 = Vec2::new(2.0, 3.0);
        let v3 = Vec2::new(2.0, 3.0);
        assert_ne!(v1, v2);
        assert_eq!(v2, v3);
    }

    #[test]
    fn vector2_math_algorithm() {
        let v1 = Vec2::new(1.0, 2.0);
        let v2 = Vec2::new(4.0, 6.0);

        assert_eq!(v1 + v2, Vec2::new(5.0, 8.0));
        assert_eq!(v1 - v2, Vec2::new(-3.0, -4.0));
        assert_eq!(v1 * v2, Vec2::new(4.0, 12.0));
        assert_eq!(v1 / v2, Vec2::new(0.25, 2.0 / 6.0));
        assert_eq!(v1 * 3.0, Vec2::new(3.0, 6.0));
        assert_eq!(v1 / 2.0, Vec2::new(0.5, 1.0));
        assert_eq!(3.0 * v1, Vec2::new(3.0, 6.0));
        assert_eq!(v1.length_square(), 5.0);
        assert_eq!(v1.length(), 5.0f32.sqrt());
        assert_eq!(v1.cross(&v2), -2.0);
        assert_eq!(v1.dot(&v2), 16.0);
        assert_eq!(v1.normalize(), v1 / v1.length());
    }

    #[test]
    fn vector3_math_algorithm() {
        let v1 = Vec3::new(1.0, 2.0, 3.0);
        let v2 = Vec3::new(4.0, 6.0, 8.0);

        assert_eq!(v1 + v2, Vec3::new(5.0, 8.0, 11.0));
        assert_eq!(v1 - v2, Vec3::new(-3.0, -4.0, -5.0));
        assert_eq!(v1 * v2, Vec3::new(4.0, 12.0, 24.0));
        assert_eq!(v1 / v2, Vec3::new(0.25, 2.0 / 6.0, 3.0 / 8.0));
        assert_eq!(v1 * 3.0, Vec3::new(3.0, 6.0, 9.0));
        assert_eq!(v1 / 2.0, Vec3::new(0.5, 1.0, 1.5));
        assert_eq!(3.0 * v1, Vec3::new(3.0, 6.0, 9.0));
        assert_eq!(v1.length_square(), 14.0);
        assert_eq!(v1.length(), v1.length_square().sqrt());
        assert_eq!(v1.cross(&v2), Vec3::new(-2.0, 4.0, -2.0));
        assert_eq!(v1.dot(&v2), 40.0);
        assert_eq!(v1.normalize(), v1 / v1.length());
    }

    #[test]
    fn vector4_math_algorithm() {
        let v1 = Vec4::new(1.0, 2.0, 3.0, 2.0);
        let v2 = Vec4::new(4.0, 6.0, 8.0, 3.0);

        assert_eq!(v1 + v2, Vec4::new(5.0, 8.0, 11.0, 5.0));
        assert_eq!(v1 - v2, Vec4::new(-3.0, -4.0, -5.0, -1.0));
        assert_eq!(v1 * v2, Vec4::new(4.0, 12.0, 24.0, 6.0));
        assert_eq!(v1 / v2, Vec4::new(0.25, 2.0 / 6.0, 3.0 / 8.0, 2.0 / 3.0));
        assert_eq!(v1 * 3.0, Vec4::new(3.0, 6.0, 9.0, 6.0));
        assert_eq!(v1 / 2.0, Vec4::new(0.5, 1.0, 1.5, 1.0));
        assert_eq!(3.0 * v1, Vec4::new(3.0, 6.0, 9.0, 6.0));
        assert_eq!(v1.length_square(), 18.0);
        assert_eq!(v1.length(), v1.length_square().sqrt());
        assert_eq!(v1.dot(&v2), 46.0);
        assert_eq!(v1.normalize(), v1 / v1.length());
    }

    #[test]
    fn mat_math_algorithm() {
        let m1 = Mat4::from_row(&[
            1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.,
        ]);
        let m2 = Mat4::from_row(&[
            2., 1., 4., 3., 8., 7., 1., 9., 0., 6., 5., 2., 8., 9., 4., 3.,
        ]);

        let result = m1 * m2;

        let check_result = Mat4::from_row(&[
            50., 69., 37., 39., 122., 161., 93., 107., 194., 253., 149., 175., 266., 345., 205.,
            243.,
        ]);
        assert_eq!(result, check_result);
    }
}
