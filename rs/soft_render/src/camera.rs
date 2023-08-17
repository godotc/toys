use crate::math;

pub struct Frustum {
    near: f32,
    far: f32,
    aspect: f32,
    fov: f32,

    mat: math::Mat4,
}

pub struct Camera {
    frustum: Frustum,
    position: math::Vec3,
    rotation: math::Vec3,

    view_mat: math::Mat4,
    view_dir: math::Vec3,
}

impl Frustum {
    #[rustfmt::skip]
    pub fn new(near: f32, far: f32, aspect: f32, fov: f32) -> Self {
        let mat =
            // without far plane, clamp [x,y] into [-1,1]. z = near
            if cfg!(feature = "cpu") {
                let a = 1.0 / (near * fov.tan());
                math::Mat4::from_row(
                    &[a, 0.0, 0.0, 0.0,
                        0.0, aspect * a, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, -1.0 / near, 0.0, ]
                )
            }
            // use opengl matrix, with far plane and clamp [x,y,z] into [--1, 1]]
            else {
                let half_w = near * fov.tan();
                let half_h = half_w / aspect;
                let near = near.abs();
                let far = far.abs();

                let a11 = near / half_w;
                let a22 = a11.clone();
                let a33 = (far + near) / (near - far);
                let a34 = 2.0 * far * near / (near - far);
                let a43 = -1.0;

                math::Mat4::from_row(
                    &[a11, 0.0, 0.0, 0.0,
                        0.0, a22, 0.0, 0.0,
                        0.0, 0.0, a33, a34,
                        0.0, 0.0, a43, 0.0, ]
                )
            };

        Self { near, far, aspect, fov, mat }
    }


    pub fn get_mat(&self) -> &math::Mat4 {
        &self.mat
    }

    pub fn near(&self) -> f32 {
        self.near
    }


    // check a pt \in a frustum
    pub fn contain(&self, pt: &math::Vec3) -> bool {
        let half_h = self.near * self.fov.tan() / self.aspect;
        let h_fov_cos = self.fov.cos();
        let h_fov_sin = self.fov.sin();

        !(
            math::Vec3::new(h_fov_cos, 0.0, h_fov_sin).dot(pt) >= 0.0 || //  right plane
                math::Vec3::new(-h_fov_cos, 0.0, h_fov_sin).dot(pt) >= 0.0 || // left

                math::Vec3::new(0.0, self.near, half_h).dot(pt) >= 0.0 || // top
                math::Vec3::new(0.0, -self.near, half_h).dot(pt) >= 0.0 || // bottom

                pt.z >= -self.near || // near plane
                pt.z <= -self.far   // far plane
        )
    }
}


impl Camera {
    pub fn new(near: f32, far: f32, aspect: f32, fov: f32) -> Self {
        Self {
            frustum: Frustum::new(near, far, aspect, fov),
            position: math::Vec3::new(0.0, 0.0, 0.0),
            rotation: math::Vec3::zero(),
            view_mat: math::Mat4::identity(),
            view_dir: -*math::Vec3::z_axis(),
        }
    }
    // TODO: make something like command buffer to handle the recalculate at last (after move, rotate, look)
    pub fn move_to(&mut self, position: math::Vec3) {
        self.position = position;
        self.recalculate_view_mat();
    }

    pub fn move_offset(&mut self, offset: math::Vec3) {
        self.position += offset;
        self.recalculate_view_mat();
    }

    pub fn set_rotation(&mut self, rotation: math::Vec3) {
        self.rotation += rotation;
        self.recalculate_view_mat();
    }

    #[rustfmt::skip]
    pub fn look_at(&mut self, target: math::Vec3) {
        let back = (self.position - target).normalize();
        let up = math::Vec3::y_axis();
        let right = up.cross(&back).normalize();
        // now get the read upwards
        let up = back.cross(&right).normalize();

        self.view_mat = math::Mat4::from_row(&[
            right.x, right.y,    right.z,    -right.dot(&self.position),
            up.x,    up.y,       up.z,       -up.dot(&self.position),
            back.x,  back.y,     back.z,     -back.dot(&self.position),
            0.0,     0.0,        0.0,         1.0,
        ]);

        let dir = target -self.position;
        let x = math::Vec3::y_axis().dot(&math::Vec3::new(0.0,dir.y,dir.z).normalize()).acos();
        let y = math::Vec3::z_axis().dot(&math::Vec3::new(dir.x,0.0,dir.z).normalize()).acos();
        let z = math::Vec3::x_axis().dot(&math::Vec3::new(dir.x,dir.y,0.0).normalize()).acos();
        self.view_dir = -back;
        self.rotation = math::Vec3::new(x,y,z);
    }

    pub fn get_frustum(&self) -> &Frustum {
        &self.frustum
    }
    pub fn get_rotation(&self) -> &math::Vec3 {
        &self.rotation
    }
    pub fn view_mat(&self) -> &math::Mat4 {
        &self.view_mat
    }
    pub fn view_dir(&self) -> &math::Vec3 {
        &self.view_dir
    }
    pub fn position(&self) -> &math::Vec3 {
        &self.position
    }


    fn recalculate_view_mat(&mut self) {
        let rotation = math::create_eular_rotate_xyz(&-self.rotation);
        self.view_mat = rotation * math::create_translate(&-self.position);
        self.view_dir = (rotation * math::Vec4::new(0.0, 0.0, -1.0, 1.0)).truncated_to_vec3();
    }
}