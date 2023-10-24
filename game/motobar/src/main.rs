// use std::num::IntErrorKind::PosOverflow;
use ::bevy::prelude::*;
use bevy_editor_pls::EditorPlugin;
use motobar::breakout::BreakOutPlugin;

pub struct HelloPlugin;

impl Plugin for HelloPlugin {
    fn build(&self, app: &mut App) {
        println!("Hello Plugin building..");

        app.insert_resource(GreetTimer(Timer::from_seconds(2.0, TimerMode::Repeating)))
            .add_systems(Startup, add_people)
            .add_systems(Update, (add_people, greeting_people));
        // .add_systems(Update, print_position_system)
    }
}

#[derive(Component)]
struct Position {
    x: f32,
    y: f32,
    z: f32,
}

#[derive(Component)]
struct Person;

#[derive(Component)]
struct Name(String);

fn print_position_system(query: Query<&Position>) {
    for pos in &query {
        println!("postion: ({}, {}, {})", pos.x, pos.y, pos.z);
    }
}

fn add_people(mut commands: Commands) {
    commands.spawn((Person, Name("Jack".to_string()), Position { x: 0.0, y: 0.0, z: 0.0 }));
    commands.spawn((Person, Name("Muck".to_string()), Position { x: 0.0, y: 0.0, z: 0.0 }));
    commands.spawn((Person, Name("Shit".to_string()), Position { x: 0.0, y: 0.0, z: 0.0 }));
    commands.spawn((Person, Name("Occur".to_string()), Position { x: 0.0, y: 0.0, z: 0.0 }));
    commands.spawn((Person, Name("Greedy".to_string()), Position { x: 0.0, y: 0.0, z: 0.0 }));
    commands.spawn((Person, Name("Pad".to_string()), Position { x: 0.0, y: 0.0, z: 0.0 }));
}


#[derive(Resource)]
struct GreetTimer(Timer);

fn greeting_people(time: Res<Time>, mut timer: ResMut<GreetTimer>, q: Query<&Name, With<Person>>) {
    if timer.0.tick(time.delta()).just_finished() {
        for name in &q {
            println!("hello {}!", name.0);
        }
    }
}

fn hello_world() {
    println!("Hello, world!");
}

fn main() {
    // App::new()
    //     .add_plugins(DefaultPlugins)
    //     .add_plugins(HelloPlugin)
    //     // .add_plugins(UiPlugin)
    //     .run();


    // App::new()
    //     .add_plugins(DefaultPlugins)
    //     .add_plugins(EditorPlugin::default())
    //     .run();

    App::new()
        .add_plugins(BreakOutPlugin)
        .run();
}
