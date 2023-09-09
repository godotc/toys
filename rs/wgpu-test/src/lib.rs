mod state;
mod vertex;

#[cfg(target_arch = "wasm32")]
use wasm_bindgen::prelude::*;

use crate::state::State;
use winit::{
    event::*,
    event_loop::{self, ControlFlow, EventLoop},
    window::{self, Window, WindowBuilder, WindowId},
};

#[cfg_attr(target_arch = "wasm32", wasm_bindgen(start))]
pub async fn run() {
    cfg_if::cfg_if! {
        if #[cfg(target_arch = "wasm32")]{
            std::panic::set_hook(Box::new(console_error_panic_hook::hook));
            console_log::init_with_level(log::Level::Warn).expect("Couldn't initialize logger");
        }else{
            env_logger::init();
        }
    }

    let event_loop = EventLoop::new();
    let window = WindowBuilder::new().build(&event_loop).unwrap();

    #[cfg(target_arch = "wasm32")]
    {
        // Winit prevents sizing with CSS, so we have to set
        // the size manually when on web.
        use winit::dpi::PhysicalSize;
        window.set_inner_size(PhysicalSize::new(450, 400));

        use winit::platform::web::WindowExtWebSys;
        web_sys::window()
            .and_then(|win| win.document())
            .and_then(|doc| {
                let dst = doc.get_element_by_id("wgpu_test")?;
                let canvas = web_sys::Element::from(window.canvas());
                dst.append_child(&canvas).ok()?;
                Some(())
            })
            .expect("Couldn't append canvas to document body.");
    }

    main_loop(event_loop, window).await;
}

async fn main_loop(event_loop: EventLoop<()>, window: winit::window::Window) {
    let mut state = State::new(&window).await;

    event_loop.run(move |event, _, control_flow| match event {
        Event::RedrawRequested(window_id) if window_id == window.id() => {
            state.update();
            match state.render() {
                Ok(_) => {}
                // lost surfce context
                Err(wgpu::SurfaceError::Lost) => state.resize(state.size),
                // exit when out of memory
                Err(wgpu::SurfaceError::OutOfMemory) => *control_flow = ControlFlow::Exit,
                Err(e) => println!("{:?}", e),
            }
            {}
        }

        Event::WindowEvent {
            event: ref window_event,
            window_id,
        } => handle_window_event(&mut state, &window, control_flow, window_event, window_id),

        Event::MainEventsCleared => {
            window.request_redraw();
        }

        _ => {}
    });
}

fn handle_window_event(
    state: &mut State,
    window: &Window,
    control_flow: &mut ControlFlow,
    window_event: &winit::event::WindowEvent,
    window_id: WindowId,
) {
    if window_id != window.id() {
        return;
    }
    if state.input(window_event) {
        return;
    }

    match window_event {
        WindowEvent::Resized(physical_size) => {
            state.resize(*physical_size);
        }

        WindowEvent::ScaleFactorChanged { new_inner_size, .. } => {
            state.resize(**new_inner_size);
        }

        WindowEvent::CursorMoved {
            device_id: _,
            position,
            modifiers: _,
        } => {
            let r = position.x / state.size.width as f64;
            let g = position.y / state.size.height as f64;
            let b = r / g;
            state.clear_color = wgpu::Color {
                r,
                g,
                b,
                a: state.clear_color.a,
            };
        }

        WindowEvent::MouseWheel {
            device_id: _, // DeviceId(Wayland(DeviceId))
            delta,        // LineDelta(0.0, -1.0)
            phase: _,     // Moved
            modifiers: _, // (empty) or SHIFT somehow... And DEPERCTED Now
        } => {
            let weight = 0.03
                * match delta {
                    MouseScrollDelta::LineDelta(_, y) => y,
                    _ => &0.0,
                };
            state.clear_color.a += weight as f64;

            println!("New apparent is {}", state.clear_color.a);
        }

        WindowEvent::CloseRequested
        | WindowEvent::KeyboardInput {
            input:
                KeyboardInput {
                    state: ElementState::Pressed,
                    virtual_keycode: Some(VirtualKeyCode::Escape),
                    ..
                },
            ..
        } => *control_flow = ControlFlow::Exit,

        WindowEvent::KeyboardInput {
            input:
                KeyboardInput {
                    state: ElementState::Pressed,
                    virtual_keycode: Some(VirtualKeyCode::Space),
                    ..
                },
            ..
        } => {
            println!("Pressed space bar...");
            state.pipeline_index = (state.pipeline_index + 1) % state.render_pipelines.len();
        }

        _ => {}
    }
}
