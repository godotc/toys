use crate::math;

pub struct Frustum {
    near: f32,
    far: f32,
    aspect: f32,
    fov: f32,

    mat: math::Mat4,
}
impl Frustum {
    #[rustfmt::skip]
    pub fn new(near: f32, far: f32, aspect: f32, fov: f32) -> Self {
        Self {
            near,
            far,
            aspect,
            fov,
            mat: 
                // without far plane, clamp [x,y] into [-1,1]. z = near
                if cfg!(feature = "cpu") {
                    let a = 1.0 / (near * fov.tan());
                    math::Mat4::from_row(
                        &[ a    ,0.0        ,0.0       , 0.0 , 
                           0.0  ,aspect * a  ,0.0       , 0.0, 
                           0.0  , 0.0       , 1.0       , 0.0,
                           0.0  , 0.0      , -1.0/near , 0.0,  ]
                    )
                }
                // use opengl matrix, with far plane and clamp [x,y,z] into [--1, 1]]
                else{
                    let half_w = near* fov.tan();
                    let half_h =  half_w / aspect;
                    let near = near.abs();
                    let far = far.abs();

                    let a11 = near/half_w;
                    let a22 = a11.clone();
                    let a33 =  (far +near) / (near-far);
                    let a34 =  2.0 * far* near/ (near-far);
                    let a43 = -1.0;

                    math::Mat4::from_row(
                        &[ a11  ,0.0  ,0.0  ,0.0, 
                           0.0  ,a22  ,0.0  ,0.0, 
                           0.0  ,0.0  ,a33  ,a34,
                           0.0  ,0.0  ,a43  ,0.0,  ]
                    )
                },
        }
    }


    pub fn get_mat(&self) -> &math::Mat4{
        &self.mat
    }
    pub fn near(&self) -> f32{
        self.near
    }


    // check a pt \in a frustum
    pub fn contain(&self, pt: &math::Vec3) -> bool{
        let half_h = self.near * self.fov.tan() / self.aspect;
        let h_fov_cos = self.fov.cos();
        let h_fov_sin = self.fov.sin();

    }

}
