//! Pong Tutorial 1

use amethyst::{
    prelude::*,
    renderer::{
        plugins::{RenderFlat2D, RenderToWindow},
        types::DefaultBackend,
        RenderingBundle,
    },
    utils::application_root_dir,
};

pub struct Pong;

impl SimpleState for Pong {}

fn main() -> amethyst::Result<()> {
    // Setup logger
    amethyst::start_logger(Default::default());
    // Setup display configuration from display.ron
    let app_root = application_root_dir()?;
    let display_config_path = app_root.join("config").join("display.ron");

    // Build default game data storage and render the window
    let game_data = GameDataBuilder::default()
        .with_bundle(
            RenderingBundle::<DefaultBackend>::new()
                // The RenderToWindow plugin provides everything for opening a window and drawing.
                .with_plugin(
                    RenderToWindow::from_config_path(display_config_path)?
                        .with_clear([0.00196, 0.23726, 0.21765, 1.0]),
                )
                // RenderFlat2D plugin is used to render entities with a `SpriteRender` component.
                .with_plugin(RenderFlat2D::default()),
        )?;

    let assets_dir = app_root.join("assets");
     // Create the Amethyst root application object which binds all systems together
    let mut game = Application::new(assets_dir, Pong, game_data)?;
    game.run(); // Start the game loop

    Ok(())
}