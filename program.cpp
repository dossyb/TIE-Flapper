#include "splashkit.h"

#define FLAP_SPEED -5
#define PLAYER_X 50
#define PIPE_WIDTH 60
#define PIPE_GAP 125
#define PLAYER_HEIGHT 50
#define PLAYER_WIDTH 60

int main()
{
    // player variables
    double playerY;
    double playerVel = 0;
    double playerAcc = 0.1;

    // pipe variables
    double pipe1X;
    double pipe1Y;
    double pipe2X;
    double pipe2Y;

    double backgroundX;
    double backgroundY;

    playerY = 50;
    pipe1X = 600;
    pipe1Y = 200;
    pipe2X = 1000;
    pipe2Y = 400;

    backgroundX = -1900;
    backgroundY = -600;

    bool quit;
    quit = false;

    open_window("TIE Flapper", 600, 800);
    clear_screen(COLOR_BLACK);
    load_bitmap("player", "tie-fighter.png");
    load_bitmap("background", "background.jpg");
    load_font("game-font", "caslon.ttf");
    do
    {
        process_events();
        
        draw_bitmap("background", backgroundX, backgroundY);
        draw_bitmap("player", PLAYER_X, playerY);
        fill_rectangle(COLOR_GRAY, pipe1X, -100, PIPE_WIDTH, pipe1Y);
        fill_rectangle(COLOR_GRAY, pipe1X, pipe1Y + PIPE_GAP, PIPE_WIDTH, screen_height() - pipe1Y);
        fill_rectangle(COLOR_GRAY, pipe2X, -100, PIPE_WIDTH, pipe2Y);
        fill_rectangle(COLOR_GRAY, pipe2X, pipe2Y + PIPE_GAP, PIPE_WIDTH, screen_height() - pipe2Y);
        // backgroundX -= 2;
        // if ( backgroundX < -3240){
        //     backgroundX = 0;
        // }
        if ( key_typed(SPACE_KEY) )
        {
            playerVel = FLAP_SPEED;
        }
        else
        {
            playerVel += playerAcc;
        }

        pipe1X -= 1.5;
        pipe2X -= 1.5;

        if ( pipe1X < -200 )
        {
            pipe1X = 600;
            pipe1Y = rnd(800);
        }
        if ( pipe2X < -200 )
        {
            pipe2X = 600;
            pipe2Y = rnd(800);
        }

        playerY += playerVel;

        if ( playerY > screen_height() + 50 )
        {
            draw_text("Game Over!", COLOR_WHITE, "game-font", 48, 180, 400);
            quit = true;
        }

        refresh_screen(60);
    } while ( not quit );
    delay(5000);
    
    return 0;
}