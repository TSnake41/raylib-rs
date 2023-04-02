/*******************************************************************************************
*
*   raylib [shaders] example - Raymarching shapes generation
*
*   NOTE: This example requires raylib OpenGL 3.3 for shaders support and only #version 330
*         is currently supported. OpenGL ES 2.0 platforms are not supported at the moment.
*
*   This example has been created using raylib 2.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2018 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

use raylib::prelude::*;

#[cfg(not(target_arch = "wasm32"))]
const GLSL_VERSION: i32 = 330;
#[cfg(target_arch = "wasm32")] -> Not supported at this moment
const GLSL_VERSION: i32 = 100;


pub fn run(rl
           : &mut RaylibHandle, thread
           : &RaylibThread)
    ->crate::SampleOut
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screen_width = 800;
    int screen_height = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    rl.set_window_size(screen_width, screen_height);
    rl.set_window_title(thread, "raylib [shaders] example - raymarching shapes");


    let mut camera = Camera3D::perspective(
    rvec3(2.5, 2.5, 3.0), // Camera position
    rvec3(0.0, 0.0, 0.7),   // Camera looking at point
    rvec3(0.0, 1.0, 0.0),       // Camera up vector (rotation towards target)
    65.0,                           // Camera field-of-view Y

    rl.set_camera_mode(&camera, raylib::consts::CameraMode::CAMERA_FREE); // Set camera mode

    // Load raymarching shader
    // NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
    let shader = rl.load_shader(thread,0, &format!("original/shaders/resources/shaders/glsl{}/raymarching.fs", GLSL_VERSION));

    // Get shader locations for required uniforms
    int viewEyeLoc = shader.get_shader_location( "viewEye");
    int viewCenterLoc = shader.get_shader_location( "viewCenter");
    int runTimeLoc = shader.get_shader_location( "runTime");
    int resolutionLoc = shader.get_shader_location( "resolution");

    float resolution[2] = {(float)screen_width, (float)screen_height};
    shader.set_shader_value( resolutionLoc, resolution, UNIFORM_VEC2);

    float runTime = 0.0;

    rl.set_target_fps(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    return Box::new(move |rl: &mut RaylibHandle, thread: &RaylibThread| -> () // Detect window close button or ESC key
    {
        // Check if screen is resized
        //----------------------------------------------------------------------------------
        if IsWindowResized()
        {
            screen_width = rl.get_screen_width();
            screen_height = rl.get_screen_height();
            float resolution[2] = {(float)screen_width, (float)screen_height};
            shader.set_shader_value( resolutionLoc, resolution, UNIFORM_VEC2);
        }

        // Update
        //----------------------------------------------------------------------------------
        rl.update_camera(&mut camera); // Update camera

        float cameraPos[3] = {camera.position.x, camera.position.y, camera.position.z};
        float cameraTarget[3] = {camera.target.x, camera.target.y, camera.target.z};

        float deltaTime = GetFrameTime();
        runTime += deltaTime;

        // Set shader required uniform values
        shader.set_shader_value( viewEyeLoc, cameraPos, UNIFORM_VEC3);
        shader.set_shader_value( viewCenterLoc, cameraTarget, UNIFORM_VEC3);
        shader.set_shader_value( runTimeLoc, &runTime, UNIFORM_FLOAT);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        let mut d = rl.begin_drawing(thread);

        d.clear_background(Color::RAYWHITE);

        // We only draw a white full-screen rectangle,
        // frame is generated in shader using raymarching
        let mut d = d.begin_shader_mode(&shader);
        d.draw_rectangle(0, 0, screen_width, screen_height, Color::WHITE);
        EndShaderMode();

        d.draw_text("(c) Raymarching shader by Iñigo Quilez. MIT License.", screen_width - 280, screen_height - 20, 10, Color::BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(shader); // Unload shader

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
