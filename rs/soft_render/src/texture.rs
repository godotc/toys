use std::collections::HashMap;

use image::GenericImageView;

use crate::math;

pub struct Texture {
    image: image::DynamicImage,
    id: u32,
    name: String,
}

#[derive(Default)]
pub struct TextureManager {
    cur_id: u32,
    images: HashMap<u32, Texture>,
    name_2_id: HashMap<String, u32>,
}

impl Texture {
    pub fn id(&self) -> u32 {
        self.id
    }
    pub fn width(&self) -> u32 {
        self.image.width()
    }
    pub fn height(&self) -> u32 {
        self.image.height()
    }
    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn get(&self, x: u32, y: u32) -> math::Vec4 {
        let data = &self.image.get_pixel(x, y).0;

        math::Vec4 {
            x: data[0] as f32 / 255.0,
            y: data[1] as f32 / 255.0,
            z: data[2] as f32 / 255.0,
            w: data[3] as f32 / 255.0,
        }
    }

    fn load(filename: &str, id: u32, name: &str) -> image::ImageResult<Texture> {
        let image = image::open(filename)
            .expect(&format!("{}: no such file!", filename))
            .flipv();

        Ok(Self {
            image,
            id,
            name: name.to_string(),
        })
    }
}

impl TextureManager {
    pub fn load(&mut self, filename: &str, name: &str) -> image::ImageResult<u32> {
        let id = self.cur_id;
        self.cur_id += 1;
        self.images.insert(id, Texture::load(filename, id, name)?);
        self.name_2_id.insert(name.to_string(), id);
        Ok(id)
    }

    pub fn get_by_id(&self, id: u32) -> Option<&Texture> {
        self.images.get(&id)
    }
    pub fn get_by_name(&self, name: &str) -> Option<&Texture> {
        self.images.get(self.name_2_id.get(name)?)
    }
    pub fn get_id(&self, name: &str) -> Option<&u32> {
        self.name_2_id.get(name)
    }
}
