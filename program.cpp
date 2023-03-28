#include "splashkit.h"
#include <iostream>
#include <fstream>

using namespace std;

#define FLAP_SPEED -5
#define PIPE_WIDTH 60
#define PIPE_GAP 225
#define PLAYER_HEIGHT 90
#define PLAYER_WIDTH 90

void save_high_score(int high_score)
{
    ofstream file;
    file.open("highscore.txt");
    file << high_score;
    file.close();
}

int get_high_score()
{
    int high_score;
    ifstream input_file("highscore.txt");
    input_file >> high_score;
    return high_score;
}

double reset_pipe(double y)
{
    do {
        y = rnd(500);
    } while ( y < 100 );
    return y;
}

int end_game(double x, double y, bitmap player, int high_score)
{
    int status;
    bitmap player_death;
    int timer;
    timer = 180;
    process_events();
    while ( timer != 0)
    {
        save_high_score(high_score);
        player_death = load_bitmap("player-death", "tie-fighter-destroyed.png");
        draw_bitmap(player_death, x, y);
        draw_text("Game Over!", COLOR_WHITE, "game-font", 48, 150, 400);
        refresh_screen(60);
        timer -= 1;
    }
    status = 0;
    return status;
}

int main_menu(int status)
{
    load_bitmap("background-blurred", "background-blurred.jpg");
    load_bitmap("menu-logo", "tie-fighter.png");
    load_bitmap("start", "button.png");
    load_bitmap("info", "button.png");
    load_bitmap("exit", "button.png");

    do {
        process_events();

        draw_bitmap("background-blurred", -1900, -600);
        draw_text("TIE", COLOR_WHITE, "game-font", 100, 150, 200);
        draw_text("Flapper", COLOR_WHITE, "game-font", 64, 160, 300);

        draw_bitmap("menu-logo", 320, 220);
        draw_bitmap("start", 220, 400);
        draw_bitmap("info", 220, 500);
        draw_bitmap("exit", 220, 600);
        draw_text("START", COLOR_WHITE, "game-font", 32, 250, 410);
        draw_text("INFO", COLOR_WHITE, "game-font", 32, 262, 510);
        draw_text("EXIT", COLOR_WHITE, "game-font", 32, 262, 610);
        if (mouse_clicked(LEFT_BUTTON) && mouse_x() > 220 && mouse_x() < 400 && mouse_y() > 410 && mouse_y() < 470)
        {
            status = 2;
        }
        if (mouse_clicked(LEFT_BUTTON) && mouse_x() > 220 && mouse_x() < 400 && mouse_y() > 610 && mouse_y() < 670)
        {
            status = 1;
        }
        refresh_screen(60);
    }
    while ( status == 0 );
    return status;
}

int info_screen(int status)
{
    // TODO
    delay(1000);
    status = 0;
    return status;
}

int game(int status)
{
    // player variables
    double playerX;
    double playerY;
    double playerVel;
    double playerAcc;
    bitmap player;

    playerX = 50;
    playerY = 50;
    playerVel = 0;
    playerAcc = 0.1;

    // pipe variables
    double pipe1X;
    double pipe1Y;
    double pipe2X;
    double pipe2Y;

    pipe1X = 600;
    pipe1Y = 200;
    pipe2X = 1000;
    pipe2Y = 400;

    // score variables
    int score;
    int high_score;
    bool score_flag;

    score = 0;
    high_score = get_high_score();
    score_flag = false;

    // backgroundX = -1900;
    // backgroundY = -600;
    
    load_bitmap("background", "background.jpg");
    player = load_bitmap("player", "tie-fighter.png");
    do
    {
        process_events();
        
        draw_bitmap("background", -1900, -600);
        draw_bitmap(player, playerX, playerY);
        fill_rectangle(COLOR_GRAY, pipe1X, 0, PIPE_WIDTH, pipe1Y);
        fill_rectangle(COLOR_GRAY, pipe1X, pipe1Y + PIPE_GAP, PIPE_WIDTH, screen_height() - pipe1Y);
        fill_rectangle(COLOR_GRAY, pipe2X, 0, PIPE_WIDTH, pipe2Y);
        fill_rectangle(COLOR_GRAY, pipe2X, pipe2Y + PIPE_GAP, PIPE_WIDTH, screen_height() - pipe2Y);
        draw_text("Score: " + to_string(score), COLOR_WHITE, "game-font", 42, 20, 750);
        draw_text("Best: " + to_string(high_score), COLOR_WHITE, "game-font", 42, 400, 750);

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

        if (pipe1X < - 200)
        {
            pipe1X = 600;
            pipe1Y = reset_pipe(pipe1Y);
        }
        if (pipe2X < - 200)
        {
            pipe2X = 600;
            pipe2Y = reset_pipe(pipe2Y);
        }

        if ( (playerX > (pipe1X + PIPE_WIDTH) or playerX > (pipe2X + PIPE_WIDTH)) and score_flag == false )
        {
            score++;
            score_flag = true;           
            if ( score >= high_score )
            {
                high_score = score;
            }
        }

        if ( playerX < (pipe1X + PIPE_WIDTH) and playerX < (pipe2X + PIPE_WIDTH) )
        {
            score_flag = false;
        }

        if ( (playerY + PLAYER_HEIGHT > screen_height() || playerY < 0) ||
        (playerX + PLAYER_WIDTH > pipe1X + 2 && playerX < pipe1X + PIPE_WIDTH - 2 && playerY < pipe1Y - 2) ||
        (playerX + PLAYER_WIDTH > pipe1X + 2 && playerX < pipe1X + PIPE_WIDTH - 2 && playerY > pipe1Y + PIPE_GAP - PLAYER_HEIGHT + 2) ||
        (playerX + PLAYER_WIDTH > pipe2X + 2 && playerX < pipe2X + PIPE_WIDTH - 2 && playerY < pipe2Y - 2) ||
        (playerX + PLAYER_WIDTH > pipe2X + 2 && playerX < pipe2X + PIPE_WIDTH - 2 && playerY > pipe2Y + PIPE_GAP - PLAYER_HEIGHT + 2) )
        {
            status = end_game(playerX, playerY, player, high_score);
        }
        refresh_screen(60);
    } while ( status == 2 );
    return status;
}

int main()
{
    int option;
    option = 0;
    open_window("TIE Flapper", 600, 800);
    clear_screen(COLOR_BLACK);
    load_font("game-font", "Space Crusaders.ttf");

    while ( option != 1 )
    {
        switch (option)
        {
        case 2:
            option = game(option);
            break;
        case 3:
            option = info_screen(option);
            break;
        default:
            option = main_menu(option);
            break;
        }
    }
    refresh_screen(60);
    return 0;
}