/*******************************************************************************************
*
*   raylib example - particles blending
*
*   This example has been created using raylib 1.7 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2017 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

use raylib::prelude::*;

const MAX_PARTICLES 200

    // Particle structure with basic data
    typedef struct
{
    Vector2 position;
    Color color;
    float alpha;
    float size;
    float rotation;
    bool active; // NOTE: Use it to activate/deactive particle
} Particle;

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
    rl.set_window_title(thread, "raylib [textures] example - particles blending");


    // Particles pool, reuse them!
    Particle mouseTail[MAX_PARTICLES] = {0};

    // Initialize particles
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        mouseTail[i].position = (Vector2){0, 0};
        mouseTail[i].color = (Color){raylib::get_random_value(0, 255), raylib::get_random_value(0, 255), raylib::get_random_value(0, 255), 255};
        mouseTail[i].alpha = 1.0;
        mouseTail[i].size = (float)raylib::get_random_value(1, 30) / 20.0;
        mouseTail[i].rotation = (float)raylib::get_random_value(0, 360);
        mouseTail[i].active = false;
    }

    float gravity = 3.0;

    Texture2D smoke = LoadTexture("resources/smoke.png");

    int blending = BLEND_ALPHA;

    rl.set_target_fps(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    return Box::new(move |rl: &mut RaylibHandle, thread: &RaylibThread| -> () // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        // Activate one particle every frame and Update active particles
        // NOTE: Particles initial position should be mouse position when activated
        // NOTE: Particles fall down with gravity and rotation... and disappear after 2 seconds (alpha = 0)
        // NOTE: When a particle disappears, active = false and it can be reused.
        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            if (!mouseTail[i].active)
            {
                mouseTail[i].active = true;
                mouseTail[i].alpha = 1.0;
                mouseTail[i].position = GetMousePosition();
                i = MAX_PARTICLES;
            }
        }

        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            if (mouseTail[i].active)
            {
                mouseTail[i].position.y += gravity;
                mouseTail[i].alpha -= 0.01f;

                if (mouseTail[i].alpha <= 0.0)
                    mouseTail[i].active = false;

                mouseTail[i].rotation += 5.0;
            }
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            if (blending == BLEND_ALPHA)
                blending = BLEND_ADDITIVE;
            else
                blending = BLEND_ALPHA;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        let mut d = rl.begin_drawing(thread);

        d.clear_background(Color::DARKGRAY);

        BeginBlendMode(blending);

        // Draw active particles
        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            if (mouseTail[i].active)
                DrawTexturePro(smoke, (Rectangle){0.0, 0.0, (float)smoke.width, (float)smoke.height},
                               (Rectangle){mouseTail[i].position.x, mouseTail[i].position.y, smoke.width * mouseTail[i].size, smoke.height * mouseTail[i].size},
                               (Vector2){(float)(smoke.width * mouseTail[i].size / 2.0), (float)(smoke.height * mouseTail[i].size / 2.0)}, mouseTail[i].rotation,
                               Fade(mouseTail[i].color, mouseTail[i].alpha));
        }

        EndBlendMode();

        d.draw_text("PRESS SPACE to CHANGE BLENDING MODE", 180, 20, 20, Color::BLACK);

        if (blending == BLEND_ALPHA)
            d.draw_text("ALPHA BLENDING", 290, screen_height - 40, 20, Color::BLACK);
        else
            d.draw_text("ADDITIVE BLENDING", 280, screen_height - 40, 20, RAYWHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(smoke);

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}