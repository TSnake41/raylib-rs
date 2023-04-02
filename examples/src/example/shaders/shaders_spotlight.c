/*******************************************************************************************
*
*   raylib [shaders] example - Simple shader mask
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Example contributed by Chris Camacho (@codifies -  http://bedroomcoders.co.uk/) 
*   and reviewed by Ramon Santamaria (@raysan5)
*
*   Copyright (c) 2019 Chris Camacho (@codifies) and Ramon Santamaria (@raysan5)
*
********************************************************************************************
*
*   The shader makes alpha holes in the forground to give the apearance of a top
*   down look at a spotlight casting a pool of light...
* 
*   The right hand side of the screen there is just enough light to see whats
*   going on without the spot light, great for a stealth type game where you
*   have to avoid the spotlights.
* 
*   The left hand side of the screen is in pitch dark except for where the spotlights are.
* 
*   Although this example doesn't scale like the letterbox example, you could integrate
*   the two techniques, but by scaling the actual colour of the render texture rather
*   than using alpha as a mask.
*
********************************************************************************************/

use raylib::prelude::*;


#include <stddef.h>
#include <stdint.h>

#[cfg(not(target_arch = "wasm32"))]
const GLSL_VERSION: i32 = 330;
#[cfg(target_arch = "wasm32")]
const GLSL_VERSION: i32 = 100;


#define MAXSPOT 3 // NB must be the same as define in shader
const numStars 400 const
    // Spot data
    typedef struct
{
    Vector2 pos;
    Vector2 vel;
    float inner;
    float radius;

    // let locations
    unsigned int posLoc;
    unsigned int innerLoc;
    unsigned int radiusLoc;
} Spot;

// Stars in the star field have a position and velocity
typedef struct Star
{
    Vector2 pos;
    Vector2 vel;
} Star;

void UpdateStar(Star *s);
void ResetStar(Star *s);

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
    rl.set_window_title(thread, "raylib - shader spotlight");

    HideCursor();

    let texRay = rl.load_texture(thread, "original/resources/raysan.png");

    Star stars[numStars] = {0};

    for (int n = 0; n < numStars; n+=1)
        ResetStar(&stars[n]);

    // Progress all the stars on, so they don't all start in the centre
    for (int m = 0; m < screen_width / 2.0; m+=1)
    {
        for (int n = 0; n < numStars; n+=1)
            UpdateStar(&stars[n]);
    }

    int frameCounter = 0;

    // Use default vert shader
    let spotShader = rl.load_shader(thread,0, &format!("original/shaders/resources/shaders/glsl{}/spotlight.fs", GLSL_VERSION));

    // Get the locations of spots in the shader
    Spot spots[MAXSPOT];
    /*
    unsigned int posLoc;
    unsigned int innerLoc;
    unsigned int radiusLoc;
*/
    for (int i = 0; i < MAXSPOT; i+=1)
    {
        char posName[32] = "spots[x].pos\0";
        char innerName[32] = "spots[x].inner\0";
        char radiusName[32] = "spots[x].radius\0";

        posName[6] = '0' + i;
        innerName[6] = '0' + i;
        radiusName[6] = '0' + i;

        spots[i].posLoc = GetShaderLocation(spotShader, posName);
        spots[i].innerLoc = GetShaderLocation(spotShader, innerName);
        spots[i].radiusLoc = GetShaderLocation(spotShader, radiusName);
    }

    // tell the shader how wide the screen is so we can have
    // a pitch black half and a dimly lit half.
    {
        unsigned int wLoc = GetShaderLocation(spotShader, "screen_width");
        float sw = (float)rl.get_screen_width();
        SetShaderValue(spotShader, wLoc, &sw, UNIFORM_FLOAT);
    }

    // randomise the locations and velocities of the spotlights
    // and initialise the shader locations
    for (int i = 0; i < MAXSPOT; i+=1)
    {

        spots[i].pos.x = raylib::get_random_value::<i32>(64, screen_width - 64);
        spots[i].pos.y = raylib::get_random_value::<i32>(64, screen_height - 64);
        spots[i].vel = rvec2(0,  0);

        while ((fabs(spots[i].vel.x) + fabs(spots[i].vel.y)) < 2)
        {
            spots[i].vel.x = raylib::get_random_value::<i32>(-40, 40) / 10.0;
            spots[i].vel.y = raylib::get_random_value::<i32>(-40, 40) / 10.0;
        }

        spots[i].inner = 28 * (i + 1);
        spots[i].radius = 48 * (i + 1);

        SetShaderValue(spotShader, spots[i].posLoc, &spots[i].pos.x, UNIFORM_VEC2);
        SetShaderValue(spotShader, spots[i].innerLoc, &spots[i].inner, UNIFORM_FLOAT);
        SetShaderValue(spotShader, spots[i].radiusLoc, &spots[i].radius, UNIFORM_FLOAT);
    }

    rl.set_target_fps(60); // Set  to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    return Box::new(move |rl: &mut RaylibHandle, thread: &RaylibThread| -> () // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        frameCounter+=1;

        // Move the stars, resetting them if the go offscreen
        for (int n = 0; n < numStars; n+=1)
            UpdateStar(&stars[n]);

        // Update the spots, send them to the shader
        for (int i = 0; i < MAXSPOT; i+=1)
        {
            if i == 0
            {
                Vector2 mp = rl.get_mouse_position();
                spots[i].pos.x = mp.x;
                spots[i].pos.y = screen_height - mp.y;
            }
            else
            {
                spots[i].pos.x += spots[i].vel.x;
                spots[i].pos.y += spots[i].vel.y;

                if spots[i].pos.x < 64
                    spots[i].vel.x = -spots[i].vel.x;
                if spots[i].pos.x > screen_width - 64
                    spots[i].vel.x = -spots[i].vel.x;
                if spots[i].pos.y < 64
                    spots[i].vel.y = -spots[i].vel.y;
                if spots[i].pos.y > screen_height - 64
                    spots[i].vel.y = -spots[i].vel.y;
            }

            SetShaderValue(spotShader, spots[i].posLoc, &spots[i].pos.x, UNIFORM_VEC2);
        }

        // Draw
        //----------------------------------------------------------------------------------
        let mut d = rl.begin_drawing(thread);

        d.clear_background(Color::Color::DARKBLUE);

        // Draw stars and bobs
        for (int n = 0; n < numStars; n+=1)
        {
            // Single pixel is just too small these days!
            d.draw_rectangle(stars[n].pos.x, stars[n].pos.y, 2, 2, Color::WHITE);
        }

        for (int i = 0; i < 16; i+=1)
        {
            d.draw_texture(texRay,
                        (screen_width / 2.0) + cos((frameCounter + i * 8) / 51.45f) * (screen_width / 2.2) - 32,
                        (screen_height / 2.0) + sin((frameCounter + i * 8) / 17.87f) * (screen_height / 4.2),
                        Color::WHITE);
        }

        // Draw spot lights
        let mut d = d.begin_shader_mode(&spotShader);
        // instead of a blank rectangle you could render here
        // a render texture of the full screen used to do screen
        // scaling (slight adjustment to shader would be required
        // to actually pay attention to the colour!)
        d.draw_rectangle(0, 0, screen_width, screen_height, Color::WHITE);
        EndShaderMode();

        d.draw_fps(10, 10);

        d.draw_text("Move the mouse!", 10, 30, 20, Color::GREEN);
        d.draw_text("Pitch Black", screen_width * .2, screen_height / 2, 20, Color::GREEN);
        d.draw_text("Dark", screen_width * .66, screen_height / 2, 20, Color::GREEN);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texRay);

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void ResetStar(Star *s)
{
    s->pos = rvec2(rl.get_screen_width() / 2.0,  rl.get_screen_height() / 2.0);

    do
    {
        s->vel.x = (float)raylib::get_random_value::<i32>(-1000, 1000) / 100.0;
        s->vel.y = (float)raylib::get_random_value::<i32>(-1000, 1000) / 100.0;

    } while (!(fabs(s->vel.x) + fabs(s->vel.y) > 1));

    s->pos = Vector2Add(s->pos, Vector2MultiplyV(s->vel, rvec2(8,  8)));
}

void UpdateStar(Star *s)
{
    s->pos = Vector2Add(s->pos, s->vel);

    if s->pos.x < 0 || s->pos.x > rl.get_screen_width( ||
        s->pos.y < 0 || s->pos.y > rl.get_screen_height())
    {
        ResetStar(s);
    }
}
