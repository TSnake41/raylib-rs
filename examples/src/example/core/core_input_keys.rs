/*******************************************************************************************
*
*   raylib [core] example - Keyboard input
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

use raylib::prelude::*;

pub fn run(rl: &mut RaylibHandle, thread: &RaylibThread) -> crate::SampleOut {
    // Initialization
    //--------------------------------------------------------------------------------------
    let screen_width = 800;
    let screen_height = 450;

    rl.set_window_size(screen_width, screen_height);
    rl.set_window_title(thread, "raylib [core] example - keyboard input");

    let mut ballPosition = rvec2(screen_width / 2, screen_height / 2);

    rl.set_target_fps(60); // Set our game to run at 60 frames-per-second
                           //--------------------------------------------------------------------------------------

    // Main game loop
    return Box::new(
        move |rl: &mut RaylibHandle, thread: &RaylibThread| -> () // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if rl.is_key_down(raylib::consts::KeyboardKey::KEY_RIGHT)
            {

                ballPosition.x += 2.0;
            }
        if rl.is_key_down(raylib::consts::KeyboardKey::KEY_LEFT)
            {

                ballPosition.x -= 2.0;
            }
        if rl.is_key_down(raylib::consts::KeyboardKey::KEY_UP)
            {

                ballPosition.y -= 2.0;
            }
        if rl.is_key_down(raylib::consts::KeyboardKey::KEY_DOWN)
            {

                ballPosition.y += 2.0;
            }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        let mut d = rl.begin_drawing(thread);

        d.clear_background(Color::RAYWHITE);

        d.draw_text("move the ball with arrow keys", 10, 10, 20, Color::DARKGRAY);

        d.draw_circle_v(ballPosition, 50.0, Color::MAROON);

        //----------------------------------------------------------------------------------
    },
    );
}
