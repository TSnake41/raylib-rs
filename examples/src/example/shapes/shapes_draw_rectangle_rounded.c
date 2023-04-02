/*******************************************************************************************
*
*   raylib [shapes] example - draw rectangle rounded (with gui options)
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Example contributed by Vlad Adrian (@demizdor) and reviewed by Ramon Santamaria (@raysan5)
*
*   Copyright (c) 2018 Vlad Adrian (@demizdor) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <raylib.h>

const RAYGUI_IMPLEMENTATION
#include "raygui.h" // Required for GUI controls

pub fn run(rl: &mut RaylibHandle, thread: &RaylibThread) -> crate::SampleOut {
    // Initialization
    //--------------------------------------------------------------------------------------
    let screen_width = 800;
    let screen_height = 450;

    rl.set_window_size(screen_width, screen_height);
    rl.set_window_title(thread, "raylib [shapes] example - draw rectangle rounded");


    float roundness = 0.2;
    int width = 200;
    int height = 100;
    int segments = 0;
    int lineThick = 1;

    bool drawRect = false;
    bool drawRoundedRect = true;
    bool drawRoundedLines = false;

    rl.set_target_fps(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    return Box::new(move |rl: &mut RaylibHandle, thread: &RaylibThread| -> () // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        let rec  = rrect((rl.get_screen_width() - width - 250) / 2,  (rl.get_screen_height() - height) / 2,  width,  height);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        let mut d = rl.begin_drawing(thread);

        d.clear_background(Color::RAYWHITE);

        DrawLine(560, 0, 560, rl.get_screen_height(), Fade(Color::LIGHTGRAY, 0.6f));
        d.draw_rectangle(560, 0, rl.get_screen_width() - 500, rl.get_screen_height(), Fade(Color::LIGHTGRAY, 0.3));

        if drawRect
            d.draw_rectangle_rec(rec, Color::GOLD.fade(0.6));
        if drawRoundedRect
            d.draw_rectangleRounded(rec, roundness, segments, Color::MAROON.fade(0.2));
        if drawRoundedLines
            d.draw_rectangleRoundedLines(rec, roundness, segments, lineThick, Color::MAROON.fade(0.4));

        // Draw GUI controls
        //------------------------------------------------------------------------------
        width = GuiSliderBar(rrect(640, 40, 105, 20), "Width", width, 0, rl.get_screen_width() - 300, true);
        height = GuiSliderBar(rrect(640, 70, 105, 20), "Height", height, 0, rl.get_screen_height() - 50, true);
        roundness = GuiSliderBar(rrect(640, 140, 105, 20), "Roundness", roundness, 0.0, 1.0, true);
        lineThick = GuiSliderBar(rrect(640, 170, 105, 20), "Thickness", lineThick, 0, 20, true);
        segments = GuiSliderBar(rrect(640, 240, 105, 20), "Segments", segments, 0, 60, true);

        drawRoundedRect = GuiCheckBox(rrect(640, 320, 20, 20), "DrawRoundedRect", drawRoundedRect);
        drawRoundedLines = GuiCheckBox(rrect(640, 350, 20, 20), "DrawRoundedLines", drawRoundedLines);
        drawRect = GuiCheckBox(rrect(640, 380, 20, 20), "DrawRect", drawRect);
        //------------------------------------------------------------------------------

        d.draw_text(&format!("MODE: %s", (segments >= 4) ? "MANUAL" : "AUTO"), 640, 280, 10, (segments >= 4) ? Color::MAROON : Color::DARKGRAY);

        d.draw_fps(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
