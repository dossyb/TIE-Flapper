#include "splashkit.h"
#include <iostream>
#include <fstream>

using namespace std;

#define FLAP_SPEED -5
#define PIPE_WIDTH 60
#define PIPE_HEIGHT 600
#define PIPE_GAP 225
#define PLAYER_HEIGHT 90
#define PLAYER_WIDTH 90
#define BG_WIDTH 3840

bool music_on;
bool sound_on;

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
        player_death = load_bitmap("player-death", "explosion.png");
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
    int high_score;
    music menu_music;
    music_on = music_on;
    sound_on = sound_on;

    load_bitmap("background-blurred", "background-blurred.jpg");
    load_bitmap("menu-logo", "tie-fighter.png");
    load_bitmap("start", "button.png");
    load_bitmap("info", "button.png");
    load_bitmap("exit", "button.png");
    load_bitmap("music-on", "musicOn.png");
    load_bitmap("music-off", "musicOff.png");
    load_bitmap("audio-on", "audioOn.png");
    load_bitmap("audio-off", "audioOff.png");
    high_score = get_high_score();
    load_sound_effect("tie", "tie.wav");
    menu_music = load_music("menu", "menu_music.mp3");


    do {
        process_events();

        draw_bitmap("background-blurred", -1900, -600);
        draw_text("TIE", COLOR_WHITE, "game-font", 100, 150, 200);
        draw_text("Flapper", COLOR_WHITE, "game-font", 64, 160, 300);

        if ( music_on )
        {
            draw_bitmap("music-on", 540, 10);
        }
        else
        {
            draw_bitmap("music-off", 540, 10);
            fade_music_out(1000);
        }
        if ( music_on && !music_playing() )
        {
            play_music(menu_music, 5, 0.25f);
        }
        if ( sound_on )
        {
            draw_bitmap("audio-on", 500, 10);
        }
        else
        {
            draw_bitmap("audio-off", 500, 10);
        }

        draw_bitmap("menu-logo", 320, 215);
        draw_bitmap("start", 220, 400);
        draw_bitmap("info", 220, 500);
        draw_bitmap("exit", 220, 600);
        draw_text("PLAY", COLOR_WHITE, "game-font", 32, 262, 410);
        draw_text("INFO", COLOR_WHITE, "game-font", 32, 262, 510);
        draw_text("EXIT", COLOR_WHITE, "game-font", 32, 262, 610);
        draw_text("High Score: " + to_string(high_score), COLOR_WHITE, "game-font", 42, 140, 750);
        if (mouse_clicked(LEFT_BUTTON) && mouse_x() > 540 && mouse_x() < 580 && mouse_y() > 20 && mouse_y() < 80)
        {
            music_on = !music_on;
        }
        if (mouse_clicked(LEFT_BUTTON) && mouse_x() > 500 && mouse_x() < 540 && mouse_y() > 20 && mouse_y() < 80)
        {
            sound_on = !sound_on;
        }
        if (mouse_clicked(LEFT_BUTTON) && mouse_x() > 220 && mouse_x() < 400 && mouse_y() > 410 && mouse_y() < 470)
        {
            stop_music();
            if ( sound_on )
            {
                play_sound_effect("tie");
            }
            status = 2;
        }
        if (mouse_clicked(LEFT_BUTTON) && mouse_x() > 220 && mouse_x() < 400 && mouse_y() > 510 && mouse_y() < 570)
        {
            status = 3;
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
    load_bitmap("back", "button.png");
    load_bitmap("arrow", "arrow.png");
    load_bitmap("tie", "tie-fighter.png");
    while( status == 3) 
    {
        process_events();
        draw_bitmap("background-blurred", -1900, -600);
        draw_bitmap("back", 220, 700);
        draw_text("Due to the Empire's ongoing financial", COLOR_WHITE, "game-font", 24, 40, 50);
        draw_text("troubles, Darth Vader has replaced", COLOR_WHITE, "game-font", 24, 40, 80);
        draw_text("your usual TIE Fighter with a TIE", COLOR_WHITE, "game-font", 24, 40, 110);
        draw_text("Flapper to save on fuel costs.", COLOR_WHITE, "game-font", 24, 40, 140);

        draw_text("Master your new ship by flying", COLOR_WHITE, "game-font", 24, 40, 200);
        draw_text("it through a series of obstacles.", COLOR_WHITE, "game-font", 24, 40, 230);
        draw_text("Don't ask who put them there.", COLOR_WHITE, "game-font", 24, 40, 260);

        draw_bitmap("tie", 100, 430);
        draw_bitmap("arrow", 200, 420);

        draw_text("SPACE", COLOR_WHITE, "game-font", 30, 300, 420);
        draw_text("or", COLOR_WHITE, "game-font", 24, 300, 465);
        draw_text("LEFT MOUSE", COLOR_WHITE, "game-font", 30, 300, 500);

        draw_text("Created by Haydon Boyd", COLOR_WHITE, "game-font", 24, 125, 660);
        
        draw_text("BACK", COLOR_WHITE, "game-font", 32, 255, 710);
        if (mouse_clicked(LEFT_BUTTON) && mouse_x() > 220 && mouse_x() < 400 && mouse_y() > 700 && mouse_y() < 770)
        {
            status = 0;
        }     
        refresh_screen(60);
    }
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
    music level_music;

    playerX = 50;
    playerY = 50;
    playerVel = 0;
    playerAcc = 0.1;

    // pipe variables
    bitmap pipe1U;
    bitmap pipe1L;
    bitmap pipe2U;
    bitmap pipe2L;
    double pipe1X;
    double pipe1Y = 0;
    double pipe2X;
    double pipe2Y = 0;

    pipe1X = 600;
    pipe1Y = reset_pipe(pipe1Y);
    pipe2X = 1000;
    pipe2Y = reset_pipe(pipe2Y);

    // difficulty variable (affects collision)
    int dif;
    dif = 4;

    // score variables
    int score;
    int high_score;
    bool score_flag;

    score = 0;
    high_score = get_high_score();
    score_flag = false;

    // background variables
    bitmap bg;
    int bg1X;
    int bgY;
    int bg2X;

    bg1X = -1900;
    bgY = -600;
    bg2X = (BG_WIDTH + bg1X);
    
    bg = load_bitmap("background", "background.jpg");
    player = load_bitmap("player", "tie-fighter.png");
    pipe1U = load_bitmap("pipe", "pipe.png");
    pipe1L = load_bitmap("pipe", "pipe.png");
    pipe2U = load_bitmap("pipe", "pipe.png");
    pipe2L = load_bitmap("pipe", "pipe.png");
    load_sound_effect("explosion", "explosion.wav");
    load_sound_effect("score-ding", "score.wav");
    load_sound_effect("whoosh", "whoosh.wav");
    level_music = load_music("level-music", "level_music.mp3");
    if ( music_on )
    {
        play_music(level_music, 5, 0.25f);
    }
    do
    {
        process_events();
        
        draw_bitmap(bg, bg1X, bgY);
        draw_bitmap(bg, bg2X, bgY);
        draw_bitmap(player, playerX, playerY);
        draw_bitmap(pipe1U, pipe1X, pipe1Y - PIPE_HEIGHT);
        draw_bitmap(pipe1L, pipe1X, pipe1Y + PIPE_GAP);
        draw_bitmap(pipe2U, pipe2X, pipe2Y - PIPE_HEIGHT);
        draw_bitmap(pipe2L, pipe2X, pipe2Y + PIPE_GAP);
        draw_text("Score: " + to_string(score), COLOR_WHITE, "game-font", 42, 20, 750);
        draw_text("Best: " + to_string(high_score), COLOR_WHITE, "game-font", 42, 400, 750);

        pipe1X -= 2;
        pipe2X -= 2;
        bg1X -= 1;
        bg2X -= 1;
        playerY += playerVel;

        if ( key_typed(SPACE_KEY) || (mouse_down(LEFT_BUTTON)))
        {
            playerVel = FLAP_SPEED;
            if ( sound_on )
            {
                play_sound_effect("whoosh", 0.25f);
            }
        }
        else
        {
            playerVel += playerAcc;
        }

        if (bg1X < -4000)
        {
            bg1X = bg2X + BG_WIDTH;
        }
        if (bg2X < -4000)
        {
            bg2X = bg1X + BG_WIDTH;
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
            if ( sound_on )
            {
                play_sound_effect("score-ding", 0.25f);
            }
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
        (playerX + PLAYER_WIDTH > pipe1X + dif && playerX < pipe1X + PIPE_WIDTH - dif && playerY < pipe1Y - dif) ||
        (playerX + PLAYER_WIDTH > pipe1X + dif && playerX < pipe1X + PIPE_WIDTH - dif && playerY > pipe1Y + PIPE_GAP - PLAYER_HEIGHT + dif) ||
        (playerX + PLAYER_WIDTH > pipe2X + dif && playerX < pipe2X + PIPE_WIDTH - dif && playerY < pipe2Y - dif) ||
        (playerX + PLAYER_WIDTH > pipe2X + dif && playerX < pipe2X + PIPE_WIDTH - dif && playerY > pipe2Y + PIPE_GAP - PLAYER_HEIGHT + dif) )
        {
            if ( sound_on )
            {
                play_sound_effect("explosion");
            }
            status = end_game(playerX, playerY, player, high_score);
            fade_music_out(1000);
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
    music_on = true;
    sound_on = true;

    while ( option != 1 )
    {
        switch (option)
        {
        case 2:
            delay(1500);
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
