/*******************************************************************************************
*
*   raylib [shapes] example - collision area
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2019 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

use raylib::prelude::*;
#include <stdlib.h> // Required for abs()

pub fn run(rl
           : &mut RaylibHandle, thread
           : &RaylibThread)
    ->crate::SampleOut
{
    // Initialization
    //---------------------------------------------------------
    let screen_width = 800;
    let screen_height = 450;

    rl.set_window_size(screen_width, screen_height);
    rl.set_window_title(thread, "raylib [shapes] example - collision area");


    // Box A: Moving box
    Rectangle boxA = {10, Getscreen_height() / 2 - 50, 200, 100};
    int boxASpeedX = 4;

    // Box B: Mouse moved box
    Rectangle boxB = {Getscreen_width() / 2 - 30, Getscreen_height() / 2 - 30, 60, 60};

    Rectangle boxCollision = {0}; // Collision rectangle

    int screenUpperLimit = 40; // Top menu limits

    bool pause = false;     // Movement pause
    bool collision = false; // Collision detection

    rl.set_target_fps(60); // Set our game to run at 60 frames-per-second
    //----------------------------------------------------------

    // Main game loop
    return Box::new(move |rl: &mut RaylibHandle, thread: &RaylibThread| -> () // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------
        // Move box if not paused
        if (!pause)
            boxA.x += boxASpeedX;

        // Bounce box on x screen limits
        if (((boxA.x + boxA.width) >= Getscreen_width()) || (boxA.x <= 0))
            boxASpeedX *= -1;

        // Update player-controlled-box (box02)
        boxB.x = GetMouseX() - boxB.width / 2;
        boxB.y = GetMouseY() - boxB.height / 2;

        // Make sure Box B does not go out of move area limits
        if ((boxB.x + boxB.width) >= Getscreen_width())
            boxB.x = Getscreen_width() - boxB.width;
        else if (boxB.x <= 0)
            boxB.x = 0;

        if ((boxB.y + boxB.height) >= Getscreen_height())
            boxB.y = Getscreen_height() - boxB.height;
        else if (boxB.y <= screenUpperLimit)
            boxB.y = screenUpperLimit;

        // Check boxes collision
        collision = CheckCollisionRecs(boxA, boxB);

        // Get collision rectangle (only on collision)
        if (collision)
            boxCollision = GetCollisionRec(boxA, boxB);

        // Pause Box A movement
        if (IsKeyPressed(KEY_SPACE))
            pause = !pause;
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        let mut d = rl.begin_drawing(thread);

        d.clear_background(Color::RAYWHITE);

        d.draw_rectangle(0, 0, screen_width, screenUpperLimit, collision ? RED : Color::BLACK);

        d.draw_rectangleRec(boxA, Color::GOLD);
        d.draw_rectangleRec(boxB, Color::BLUE);

        if (collision)
        {
            // Draw collision area
            d.draw_rectangleRec(boxCollision, Color::LIME);

            // Draw collision message
            d.draw_text("COLLISION!", Getscreen_width() / 2 - MeasureText("COLLISION!", 20) / 2, screenUpperLimit / 2 - 10, 20, Color::BLACK);

            // Draw collision area
            d.draw_text(FormatText("Collision Area: %i", (int)boxCollision.width * (int)boxCollision.height), Getscreen_width() / 2 - 100, screenUpperLimit + 10, 20, Color::BLACK);
        }

        DrawFPS(10, 10);

        EndDrawing();
        //-----------------------------------------------------
    }

    // De-Initialization
    //---------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //----------------------------------------------------------

    return 0;
}