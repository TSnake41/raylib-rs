/*******************************************************************************************
*
*   raylib [shaders] example - Apply a shader to some shape or texture
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3), to test this example
*         on OpenGL ES 2.0 platforms (Android, Raspberry Pi, HTML5), use #version 100 shaders
*         raylib comes with shaders ready for both versions, check raylib/shaders install folder
*
*   This example has been created using raylib 1.7 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

use raylib::prelude::*;

#[cfg(not(target_arch = "wasm32"))]
const GLSL_VERSION: i32 = 330;
#[cfg(target_arch = "wasm32")]
const GLSL_VERSION: i32 = 100;


pub fn run(rl
           : &mut RaylibHandle, thread
           : &RaylibThread)
    ->crate::SampleOut
{
    // Initialization
    //--------------------------------------------------------------------------------------
    let screen_width = 800;
    let screen_height = 450;

    rl.set_window_size(screen_width, screen_height);
    rl.set_window_title(thread, "raylib [shaders] example - shapes and texture shaders");


    let fudesumi = rl.load_texture(thread, "original/shaders/resources/fudesumi.png");

    // Load shader to be used on some parts drawing
    // NOTE 1: Using GLSL 330 shader version, on OpenGL ES 2.0 use GLSL 100 shader version
    // NOTE 2: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
    let shader = rl.load_shader(thread,0, &format!("original/shaders/resources/shaders/glsl{}/grayscale.fs", GLSL_VERSION));

    rl.set_target_fps(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    return Box::new(move |rl: &mut RaylibHandle, thread: &RaylibThread| -> () // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        let mut d = rl.begin_drawing(thread);

        d.clear_background(Color::RAYWHITE);

        // Start drawing with default shader

        d.draw_text("USING DEFAULT SHADER", 20, 40, 10,Color::RED);

        d.draw_circle(80, 120, 35, Color::DARKBLUE);
        DrawCircleGradient(80, 220, 60, Color::GREEN, Color::SKYBLUE);
        DrawCircleLines(80, 340, 80, Color::DARKBLUE);

        // Activate our custom shader to be applied on next shapes/textures drawings
        let mut d = d.begin_shader_mode(&shader);

        d.draw_text("USING CUSTOM SHADER", 190, 40, 10,Color::RED);

        d.draw_rectangle(250 - 60, 90, 120, 60,Color::RED);
        d.draw_rectangleGradientH(250 - 90, 170, 180, 130, Color::MAROON, Color::GOLD);
        d.draw_rectangle_lines(250 - 40, 320, 80, 60, Color::ORANGE);

        // Activate our default shader for next drawings
        EndShaderMode();

        d.draw_text("USING DEFAULT SHADER", 370, 40, 10,Color::RED);

        d.draw_triangle(rvec2(430,  80),
                     rvec2(430 - 60,  150),
                     rvec2(430 + 60,  150), Color::VIOLET);

        d.draw_triangleLines(rvec2(430, 160),
                          rvec2(430 - 20,  230),
                          rvec2(430 + 20,  230), Color::DARKBLUE);

        DrawPoly(rvec2(430, 320), 6, 80, 0, Color::BROWN);

        // Activate our custom shader to be applied on next shapes/textures drawings
        let mut d = d.begin_shader_mode(&shader);

        d.draw_texture(fudesumi, 500, -30, Color::WHITE); // Using custom shader

        // Activate our default shader for next drawings
        EndShaderMode();

        d.draw_text("(c) Fudesumi sprite by Eiden Marsal", 380, screen_height - 20, 10, Color::GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(shader);    // Unload shader
    UnloadTexture(fudesumi); // Unload texture

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}