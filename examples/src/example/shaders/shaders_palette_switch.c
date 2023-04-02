/*******************************************************************************************
*
*   raylib [shaders] example - Color palette switch
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3), to test this example
*         on OpenGL ES 2.0 platforms (Android, Raspberry Pi, HTML5), use #version 100 shaders
*         raylib comes with shaders ready for both versions, check raylib/shaders install folder
*
*   This example has been created using raylib 2.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Example contributed by Marco Lizza (@MarcoLizza) and reviewed by Ramon Santamaria (@raysan5)
*
*   Copyright (c) 2019 Marco Lizza (@MarcoLizza) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

use raylib::prelude::*;

#[cfg(not(target_arch = "wasm32"))]
const GLSL_VERSION 330
#[cfg(target_arch = "wasm32")]
const GLSL_VERSION 100


    const MAX_PALETTES 3 const COLORS_PER_PALETTE 8 const VALUES_PER_COLOR 3

    static let palettes[MAX_PALETTES][COLORS_PER_PALETTE * VALUES_PER_COLOR] = {
        {
            // 3-BIT RGB
            0,
            0,
            0,
            255,
            0,
            0,
            0,
            255,
            0,
            0,
            0,
            255,
            0,
            255,
            255,
            255,
            0,
            255,
            255,
            255,
            0,
            255,
            255,
            255,
        },
        {
            // AMMO-8 (GameBoy-like)
            4,
            12,
            6,
            17,
            35,
            24,
            30,
            58,
            41,
            48,
            93,
            66,
            77,
            128,
            97,
            137,
            162,
            87,
            190,
            220,
            127,
            238,
            255,
            204,
        },
        {
            // RKBV (2-strip film)
            21,
            25,
            26,
            138,
            76,
            88,
            217,
            98,
            117,
            230,
            184,
            193,
            69,
            107,
            115,
            75,
            151,
            166,
            165,
            189,
            194,
            255,
            245,
            247,
        }};

static const char *paletteText[] = {
    "3-BIT RGB",
    "AMMO-8 (GameBoy-like)",
    "RKBV (2-strip film)"};

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
    rl.set_window_title(thread, "raylib [shaders] example - color palette switch");


    // Load shader to be used on some parts drawing
    // NOTE 1: Using GLSL 330 shader version, on OpenGL ES 2.0 use GLSL 100 shader version
    // NOTE 2: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
    let shader = rl.load_shader(thread,0, &format!("original/shaders/resources/shaders/glsl{}/palette_switch.fs", GLSL_VERSION));

    // Get variable (uniform) location on the shader to connect with the program
    // NOTE: If uniform variable could not be found in the shader, function returns -1
    int paletteLoc = shader.get_shader_location( "palette");

    int currentPalette = 0;
    int lineHeight = screen_height / COLORS_PER_PALETTE;

    rl.set_target_fps(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    return Box::new(move |rl: &mut RaylibHandle, thread: &RaylibThread| -> () // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if rl.is_key_pressed(raylib::consts::KeyboardKey::KEY_RIGHT)
            currentPalette+=1;
        else if rl.is_key_pressed(raylib::consts::KeyboardKey::KEY_LEFT)
            currentPalette-=1;

        if currentPalette >= MAX_PALETTES
            currentPalette = 0;
        else if currentPalette < 0
            currentPalette = MAX_PALETTES - 1;

        // Send new value to the shader to be used on drawing.
        // NOTE: We are sending RGB triplets w/o the alpha channel
        SetShaderValueV(shader, paletteLoc, palettes[currentPalette], UNIFORM_IVEC3, COLORS_PER_PALETTE);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        let mut d = rl.begin_drawing(thread);

        d.clear_background(Color::RAYWHITE);

        let mut d = d.begin_shader_mode(&shader);

        for (int i = 0; i < COLORS_PER_PALETTE; i+=1)
        {
            // Draw horizontal screen-wide rectangles with increasing "palette index"
            // The used palette index is encoded in the RGB components of the pixel
            d.draw_rectangle(0, lineHeight * i, rl.get_screen_width(), lineHeight, (Color){i, i, i, 255});
        }

        EndShaderMode();

        d.draw_text("< >", 10, 10, 30, Color::DARKBLUE);
        d.draw_text("CURRENT PALETTE:", 60, 15, 20, Color::RAYWHITE);
        d.draw_text(paletteText[currentPalette], 300, 15, 20,Color::RED);

        d.draw_fps(700, 15);

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
