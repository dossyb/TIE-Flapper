#include "splashkit.h"

using namespace std;

#define FLAP_SPEED -5
#define PIPE_WIDTH 60
#define PIPE_GAP 225
#define PLAYER_HEIGHT 90
#define PLAYER_WIDTH 90

bool end_game()
{
    bool quit;
    draw_text("Game Over!", COLOR_WHITE, "game-font", 48, 150, 400);
    quit = true;
    return quit;
}

int main()
{
    // player variables
    double playerX;
    double playerY;
    double playerVel = 0;
    double playerAcc = 0.1;

    playerX = 50;
    playerY = 50;

    // pipe variables
    double pipe1X;
    double pipe1Y;
    double pipe2X;
    double pipe2Y;

    pipe1X = 600;
    pipe1Y = 200;
    pipe2X = 1000;
    pipe2Y = 400;

    double backgroundX;
    double backgroundY;

    // score variables
    int score;
    int high_score;
    bool score_flag;

    score = 0;
    high_score = 0;
    score_flag = false;

    backgroundX = -1900;
    backgroundY = -600;

    bool quit;
    quit = false;

    open_window("TIE Flapper", 600, 800);
    clear_screen(COLOR_BLACK);
    load_bitmap("player", "tie-fighter.png");
    load_bitmap("background", "background.jpg");
    load_font("game-font", "Space Crusaders.ttf");
    do
    {
        process_events();
        
        draw_bitmap("background", backgroundX, backgroundY);
        draw_bitmap("player", playerX, playerY);
        fill_rectangle(COLOR_GRAY, pipe1X, 0, PIPE_WIDTH, pipe1Y);
        fill_rectangle(COLOR_GRAY, pipe1X, pipe1Y + PIPE_GAP, PIPE_WIDTH, screen_height() - pipe1Y);
        fill_rectangle(COLOR_GRAY, pipe2X, 0, PIPE_WIDTH, pipe2Y);
        fill_rectangle(COLOR_GRAY, pipe2X, pipe2Y + PIPE_GAP, PIPE_WIDTH, screen_height() - pipe2Y);
        draw_text("Score: " + to_string(score), COLOR_WHITE, "game-font", 42, 20, 750);
        draw_text("Best: " + to_string(high_score), COLOR_WHITE, "game-font", 42, 400, 750);
        // Background Scrolling (TODO)
        // backgroundX -= 2;
        // if ( backgroundX < -3240){
        //     backgroundX = 0;
        // }

        pipe1X -= 1.5;
        pipe2X -= 1.5;
        playerY += playerVel;

        if ( key_typed(SPACE_KEY) )
        {
            playerVel = FLAP_SPEED;
        }
        else
        {
            playerVel += playerAcc;
        }

        if ( pipe1X < -200 )
        {
            pipe1X = 600;
            do {
                pipe1Y = rnd(500);
            }
            while ( pipe1Y < 100 );
        }

        if ( pipe2X < -200 )
        {
            pipe2X = 600;
            do {
                pipe2Y = rnd(500);
            }
            while ( pipe2Y < 100 );
        }

        if ( (playerX > (pipe1X + PIPE_WIDTH) or playerX > (pipe2X + PIPE_WIDTH)) and score_flag == false )
        {
            score++;
            score_flag = true;
        }

        if ( playerX < (pipe1X + PIPE_WIDTH) and playerX < (pipe2X + PIPE_WIDTH) )
        {
            score_flag = false;
        }

        if ( score >= high_score )
        {
            high_score = score;
        }

        if ( playerY + PLAYER_HEIGHT > screen_height() || playerY < 0 )
        {
            quit = end_game();
        }

        if ( playerX + PLAYER_WIDTH > pipe1X + 2 && playerX < pipe1X + PIPE_WIDTH - 2 && playerY < pipe1Y - 2 )
        {
            quit = end_game();
        }
        if ( playerX + PLAYER_WIDTH > pipe1X + 2 && playerX < pipe1X + PIPE_WIDTH - 2 && playerY > pipe1Y + PIPE_GAP - PLAYER_HEIGHT + 2 )
        {
            quit = end_game();
        }
        if ( playerX + PLAYER_WIDTH > pipe2X + 2 && playerX < pipe2X + PIPE_WIDTH - 2 && playerY < pipe2Y - 2 )
        {
            quit = end_game();
        }
        if ( playerX + PLAYER_WIDTH > pipe2X + 2 && playerX < pipe2X + PIPE_WIDTH - 2 && playerY > pipe2Y + PIPE_GAP - PLAYER_HEIGHT + 2 )
        {
            quit = end_game();
        }

        refresh_screen(60);
    } while ( not quit );
    delay(5000);
    
    return 0;
}