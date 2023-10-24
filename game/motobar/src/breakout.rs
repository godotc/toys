use bevy::ecs::bundle::DynamicBundle;
use bevy::math::vec3;
use bevy::prelude::*;
use bevy::render::camera::ScalingMode::WindowSize;
use bevy::sprite::{MaterialMesh2dBundle, SpriteBatch};
use bevy::window;
use bevy::window::{WindowResized, WindowResolution};
use bevy_editor_pls::egui::ImageSource::Texture;
use bevy_editor_pls::egui::Shape::Rect;
use bevy_editor_pls::egui::SizeHint::Size;


pub struct BreakOutPlugin;

impl Plugin for BreakOutPlugin {
    fn build(&self, app: &mut App) {
        app.add_plugins(DefaultPlugins)
            .insert_resource(FixedTime::new_from_secs(1.0 / 60.0))
            .insert_resource(WindowSettings {
                width: WIN_W,
                height: WIN_H,
                clear_color: BACKGROUND_COLOR,
            })
            .add_systems(Startup, setup)
        // .add_systems(Startup, play_bgm)
        ;
    }

    fn ready(&self, _app: &App) -> bool { true }

    fn finish(&self, _app: &mut App) {}
    fn cleanup(&self, _app: &mut App) {}

    fn name(&self) -> &str { "Breakout" }
    fn is_unique(&self) -> bool { true }
}

const WIN_W: f32 = 800.0;
const WIN_H: f32 = 600.0;
const BACKGROUND_COLOR: Color = Color::rgb(0.3, 0.4, 0.5);

const WIN_BOTTOM: f32 = -WIN_H / 2.0;
const PADDLE_GAP_FROM_BOTTOM: f32 = 10.0;

#[derive(Resource)]
struct WindowSettings {
    width: f32,
    height: f32,
    clear_color: Color,
}

#[derive(Resource)]
struct CollisionSound {
    ping: Handle<AudioSource>,
    pong: Handle<AudioSource>,
}

#[derive(Resource)]
struct BGM(Handle<AudioSource>);

#[derive(Component)]
struct RectSize {
    x: f32,
    y: f32,
}

#[derive(Component)]
struct Paddle;

#[derive(Component)]
struct Collider;

#[derive(Component)]
struct Ball;

#[derive(Component)]
struct Velocity(f32);


fn setup(
    mut commands: Commands,
    mut windows: Query<&mut Window>,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<ColorMaterial>>,
    asset_server: Res<AssetServer>,
    window_settings: Res<WindowSettings>,
) {
    commands.insert_resource(ClearColor(window_settings.clear_color));

    // camera
    commands.spawn(Camera2dBundle::default());

    windows.single_mut()
        .resolution.set(window_settings.width, window_settings.height);

    let ping = asset_server.load("audio/ping.mp3");
    let pong = asset_server.load("audio/pong.wav");
    commands.insert_resource(CollisionSound { ping, pong });

    // let paddle_default_tex = asset_server.load("texture/paddle.png");
    let paddle_y = WIN_BOTTOM + PADDLE_GAP_FROM_BOTTOM;

    let paddle_size = RectSize { x: 120.0, y: 10.0 };

    // paddle
    let paddle_init_pos = vec3(0.0, paddle_y, 0.0);
    commands.spawn((
        SpriteBundle {
            transform: Transform {
                translation: paddle_init_pos,
                scale: vec3(paddle_size.x, paddle_size.y, 0.0),
                ..default()
            },
            sprite: Sprite {
                color: Color::GREEN,
                ..default()
            },
            // texture: paddle_default_tex,
            ..default()
        },
        Paddle,
        Collider,
        RectSize { x: paddle_size.x, y: paddle_size.y },
    ));


    // Ball
    let ball_init_transf = Transform::from_translation(
        // vec3(window_settings.width / 2.0, window_settings.height / 2.0, 0.0))
        vec3(0.0, 0.0, 0.0))
        .with_scale(vec3(10.0, 10.0, 10.0));

    commands.spawn((
        MaterialMesh2dBundle {
            mesh: meshes.add(shape::Circle::default().into()).into(),
            material: materials.add(ColorMaterial::from(Color::RED)),
            transform: ball_init_transf,
            ..default()
        },
        Ball,
        // Velocity((ball_init_transf.translation - paddle_init_pos).normalize() * 1.0),
        Velocity(1.0),
    ));


    // TODO: ScoreBoard HUD

    // TODO: Map
    // 1. walls
    // 2. bricks
}

fn play_bgm(
    mut cmds: Commands,
    bgm: Res<BGM>,
    pp: Res<CollisionSound>,
) {
    cmds.spawn(AudioBundle {
        source: bgm.0.clone(),
        settings: PlaybackSettings::LOOP,
    });
}