use crate::math;

const MAX_ATTRIBUTES_NUM: usize = 4;

#[derive(Clone, Copy, Debug)]
pub struct Attributes {
    pub float: [f32; MAX_ATTRIBUTES_NUM],
    pub vec2: [math::Vec2; MAX_ATTRIBUTES_NUM],
    pub vec3: [math::Vec3; MAX_ATTRIBUTES_NUM],
    pub vec4: [math::Vec4; MAX_ATTRIBUTES_NUM],
}

impl Attributes {
    pub fn set_float(&mut self, location: usize, value: f32) {
        self.float[location] = value;
    }
    pub fn set_vec2(&mut self, location: usize, value: math::Vec2) {
        self.vec2[location] = value;
    }
    pub fn set_vec3(&mut self, location: usize, value: math::Vec3) {
        self.vec3[location] = value;
    }
    pub fn set_vec4(&mut self, location: usize, value: math::Vec4) {
        self.vec4[location] = value;
    }
}

impl Default for Attributes {
    fn default() -> Self {
        Self {
            float: [0.0; MAX_ATTRIBUTES_NUM],
            vec2: [math::Vec2::zero(); MAX_ATTRIBUTES_NUM],
            vec3: [math::Vec3::zero(); MAX_ATTRIBUTES_NUM],
            vec4: [math::Vec4::zero(); MAX_ATTRIBUTES_NUM],
        }
    }
}

pub fn interp_attributes_v0<F>(
    attr1: &Attributes,
    attr2: &Attributes,
    interp_fn: F,
    t: f32,
) -> Attributes
where
    F: Fn(f32, f32, f32) -> f32,
{
    let mut attributes = Attributes::default();

    let float = &mut attributes.float;
    let vec2 = &mut attributes.vec2;
    let vec3 = &mut attributes.vec3;
    let vec4 = &mut attributes.vec4;

    for i in 0..MAX_ATTRIBUTES_NUM {
        float[i] = interp_fn(attr1.float[i], attr2.float[i], t);
        vec2[i].x = interp_fn(attr1.vec2[i].x, attr2.vec2[i].x, t);
        vec2[i].y = interp_fn(attr1.vec2[i].y, attr2.vec2[i].y, t);
        vec3[i].x = interp_fn(attr1.vec3[i].x, attr2.vec3[i].x, t);
        vec3[i].y = interp_fn(attr1.vec3[i].y, attr2.vec3[i].y, t);
        vec3[i].z = interp_fn(attr1.vec3[i].z, attr2.vec3[i].z, t);
        vec4[i].x = interp_fn(attr1.vec4[i].x, attr2.vec4[i].x, t);
        vec4[i].y = interp_fn(attr1.vec4[i].y, attr2.vec4[i].y, t);
        vec4[i].z = interp_fn(attr1.vec4[i].z, attr2.vec4[i].z, t);
        vec4[i].w = interp_fn(attr1.vec4[i].w, attr2.vec4[i].w, t);
    }

    attributes
}

/*
TODO: make it use map/iter/collect
    pub fn interp_attributes_v1<F>(
        attr1: &Attributes,
        attr2: &Attributes,
        interp_fn: F,
        t: f32,
    ) -> Attributes
    where
        F: Fn(f32, f32, f32) -> f32,
    {

        let float = &mut attributes.float;
        let vec2 = &mut attributes.vec2;
        let vec3 = &mut attributes.vec3;
        let vec4 = &mut attributes.vec4;

        for i in 0..MAX_ATTRIBUTES_NUM {
            float[i] = interp_fn(attr1.float[i], attr2.float[i], t);
            vec2[i].x = interp_fn(attr1.vec2[i].x, attr2.vec2[i].x, t);
            vec2[i].y = interp_fn(attr1.vec2[i].y, attr2.vec2[i].y, t);
            vec3[i].x = interp_fn(attr1.vec3[i].x, attr2.vec3[i].x, t);
            vec3[i].y = interp_fn(attr1.vec3[i].y, attr2.vec3[i].y, t);
            vec3[i].z = interp_fn(attr1.vec3[i].z, attr2.vec3[i].z, t);
            vec4[i].x = interp_fn(attr1.vec4[i].x, attr2.vec4[i].x, t);
            vec4[i].y = interp_fn(attr1.vec4[i].y, attr2.vec4[i].y, t);
            vec4[i].z = interp_fn(attr1.vec4[i].z, attr2.vec4[i].z, t);
            vec4[i].w = interp_fn(attr1.vec4[i].w, attr2.vec4[i].w, t);
        }

        attributes
    }
*/

pub fn attributes_foreach<F>(attr: &mut Attributes, func: F)
where
    F: Fn(f32) -> f32,
{
    attr.float.iter_mut().for_each(|f| *f = func(*f));

    attr.vec2.iter_mut().for_each(|vec2| {
        vec2.x = func(vec2.x);
        vec2.y = func(vec2.y);
    });

    attr.vec3.iter_mut().for_each(|vec3| {
        vec3.x = func(vec3.x);
        vec3.y = func(vec3.y);
        vec3.z = func(vec3.z);
    });

    attr.vec4.iter_mut().for_each(|vec4| {
        vec4.x = func(vec4.x);
        vec4.y = func(vec4.y);
        vec4.z = func(vec4.z);
        vec4.w = func(vec4.w);
    });
}
