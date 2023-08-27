pub(super) struct Viewport {
    pub(crate) x: u32,
    pub(crate) y: u32,
    pub(crate) w: u32,
    pub(crate) h: u32,
}

#[derive(PartialEq, Eq)]
pub(super) enum FaceCull {
    Front,
    Back,
    None,
}

pub(super) enum FrontFace {
    CW,
    CCW,
}

pub(super) enum TopologyType {
    LineList,
    TriangleList,
}


