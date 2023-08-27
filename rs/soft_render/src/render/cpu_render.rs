use fltk::app::sleep;
use fltk::enums::Font;
use crate::{camera, math};
use crate::camera::Camera;
use crate::image::{ColorAttachment, DepthAttachment};
use crate::math::{Mat4, Vec4};
use crate::render::algo::should_cull;
use crate::render::cpu_render::RasterizeResult::Discard;
use crate::render::interface::RenderInterface;
use crate::render::viewport;
use crate::render::viewport::{FaceCull, FrontFace, Viewport};
use crate::shader::{Shader, Uniforms};
use crate::shader::vertex::Vertex;
use crate::texture::TextureManager;

pub struct Renderer {
    color_attachment: ColorAttachment,
    depth_attachment: DepthAttachment,
    camera: camera::Camera,
    viewport: Viewport,
    shader: Shader,
    uniforms: Uniforms,
    front_face: FrontFace,
    cull: FaceCull,

    clipped_triangles: Vec<Vertex>,
    enable_framework: bool,
}

enum RasterizeResult {
    Ok,
    Discard,
    GenerateNewFace,
}

impl RenderInterface for Renderer {
    fn clear(&mut self, color: &Vec4) {
        self.color_attachment.clear(color);
    }

    fn clear_depth(&mut self) {
        self.depth_attachment.clear(f32::MIN); //right hand
    }

    fn get_canvas_width(&mut self) -> u32 {
        self.color_attachment.width()
    }

    fn get_canvas_height(&mut self) -> u32 {
        self.color_attachment.height()
    }

    fn draw_triangle(&mut self, model: &Mat4, vertices: &[Vertex], texture_manager: &TextureManager) {
        for i in 0..vertices.len() / 3usize {
            let vertices = [vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]];

            match self.raster {}
        }
    }

    fn get_rendered_iamge(&self) -> &[u8] {
        todo!()
    }

    fn get_rendered_image(&self) -> &[u8] {
        todo!()
    }

    fn get_shader(&mut self) -> &mut Shader {
        todo!()
    }

    fn get_uniforms(&mut self) -> &mut Uniforms {
        todo!()
    }

    fn get_camera(&mut self) -> &mut Camera {
        &mut self.camera
    }

    fn set_camera(&mut self, camera: Camera) {
        todo!()
    }

    fn set_front_face(&mut self, front_face: FrontFace) {
        todo!()
    }

    fn get_front_face(&self) -> FrontFace {
        todo!()
    }

    fn set_face_cull(&mut self, cull: FaceCull) {
        todo!()
    }

    fn get_face_cull(&self) -> FaceCull {
        todo!()
    }

    fn enable_framework(&mut self) {
        todo!()
    }

    fn disable_framework(&mut self) {
        todo!()
    }

    fn toggle_framework(&mut self) {
        todo!()
    }
}

impl Renderer {
    pub fn new(w: u32, h: u32, camera: camera::Camera) -> Self {
        Self {
            color_attachment: ColorAttachment::new(w, h),
            depth_attachment: DepthAttachment::new(w, h),
            camera,
            viewport: Viewport { x: 0, y: 0, w, h },
            shader: Default::default(),
            uniforms: Default::default(),
            front_face: FrontFace::CW,
            cull: FaceCull::None,
            clipped_triangles: Vec::new(),
            enable_framework: false,
        }
    }

    fn rasterize_triangle(
        &mut self,
        model: &math::Mat4,
        mut vertices: [Vertex; 3],
        texture_storage: &TextureManager,
    ) -> RasterizeResult
    {
        // change vertex positon
        for v in &mut vertices {
            *v = self.shader.call_vertex_change(v, &self.uniforms, texture_storage);
        }

        // model transf
        for v in &mut vertices {
            v.position = *model * v.position;
        }

        // face cull
        if should_cull(
            &vertices.map(|v| v.position.truncated_to_vec3()),
            self.camera.view_dir(),
            self.front_face,
            self.cull,
        ) {
            return RasterizeResult::Discard;
        }

        // view transf
        for v in &mut vertices {
            v.position = *self.get_camera().view_mat() * v.position;
        }


        // frustum clip
        if vertices.iter().all(|v| {
            !self.camera.get_frustum().contain(&v.position.truncated_to_vec3())
        }) {
            return Discard;
        }

        // near plane clip
        if vertices.iter().any(|v| v.position.z > self.camera.get_frustum().near()) {
            let (face1, face2) =
        }

        return Discard;
    }
}
