#include "splashkit.h"
#include <iostream>
#include <fstream>

using namespace std;

#define FLAP_SPEED -5

enum menu_options
{
    MAIN,
    GAME,
    HELP,
    EXIT
};

struct pipe_data
{
    int width = 60;
    int height = 600;
    int gap = 225;
    bitmap upper;
    bitmap lower;
    int x;
    int y;
};

struct player_data
{
    int height = 90;
    int width = 90;
    double x;
    double y;
    double vel;
    double acc;
    bitmap bmp;
};

struct bg_data
{
    int x;
    int y;
    bitmap bmp;
    int width;
};

pipe_data new_pipe(int x, int y)
{
    pipe_data pipe;
    pipe.x = x;
    pipe.y = rnd(550);
    pipe.upper = load_bitmap("pipe", "pipe.png");
    pipe.lower = load_bitmap("pipe", "pipe.png");

    return pipe;
}

player_data new_player()
{
    player_data player;

    player.x = 50;
    player.y = 50;
    player.vel = 0;
    player.acc = 0.1;
    player.bmp = load_bitmap("player", "tie-fighter.png");

    return player;
}

bg_data new_bg(int x, int y)
{
    bg_data bg;

    bg.x = x;
    bg.y = y;
    bg.bmp = load_bitmap("background", "background.jpg");
    bg.width = 3840;

    return bg;
}

void button_hover(int x_left, int x_right, int y_top, int y_bottom, string text)
{
    if (mouse_x() > x_left && mouse_x() < x_right && mouse_y() > y_top && mouse_y() < y_bottom)
    {
        draw_text(text, COLOR_GRAY, "game-font", 32, x_left + 42, y_top + 10);
    }
}

void button_clicked(int x_left, int x_right, int y_top, int y_bottom, menu_options &menu, int index)
{
    if (mouse_clicked(LEFT_BUTTON) && mouse_x() > x_left && mouse_x() < x_right && mouse_y() > y_top && mouse_y() < y_bottom)
    {
        menu = static_cast<menu_options>(index);
    }
}

void button_clicked(int x_left, int x_right, int y_top, int y_bottom, bool &set)
{
    if (mouse_clicked(LEFT_BUTTON) && mouse_x() > x_left && mouse_x() < x_right && mouse_y() > y_top && mouse_y() < y_bottom)
    {
        set = !set;
    }
}

double reset_pipe(double y)
{
    do {
        y = rnd(550);
    } while ( y < 50 );
    return y;
}

bool collision(const player_data &player, const pipe_data &pipe_a, const pipe_data &pipe_b, const bool &sound_set, const int &dif)
{
    if ((player.y + player.height > screen_height() || player.y < 0) ||
    (player.x + player.width > pipe_a.x + dif && player.x < pipe_a.x + pipe_a.width - dif && player.y < pipe_a.y - dif) ||
    (player.x + player.width > pipe_a.x + dif && player.x < pipe_a.x + pipe_a.width - dif && player.y > pipe_a.y + pipe_a.gap - player.height + dif) ||
    (player.x + player.width > pipe_b.x + dif && player.x < pipe_b.x + pipe_b.width - dif && player.y < pipe_b.y - dif) ||
    (player.x + player.width > pipe_b.x + dif && player.x < pipe_b.x + pipe_b.width - dif && player.y > pipe_b.y + pipe_b.gap - player.height + dif))
    {
        if ( sound_set )
        {
            play_sound_effect("explosion");
        }
        fade_music_out(2000);
        return true;
    }
    return false;
}

void boundary_exceeded(int &pipe_x, int &pipe_y)
{
    if (pipe_x < - 200)
    {
        pipe_x = 600;
        pipe_y = reset_pipe(pipe_y);
    }
}

void flap(double &vel, const bool &sound_set)
{
    vel = FLAP_SPEED;
    if ( sound_set ) play_sound_effect("whoosh", 0.25f);
}

void increase_score(const player_data &player, const pipe_data &pipe_a, const pipe_data &pipe_b, int &score, int &high_score, bool &score_flag, const bool &sound_set)
{
    if ( (player.x > (pipe_a.x + pipe_a.width) or player.x > (pipe_b.x + pipe_b.width)) and score_flag == false )
    {
        if ( sound_set )
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

    if ( player.x < (pipe_a.x + pipe_a.width) and player.x < (pipe_b.x + pipe_b.width) ) score_flag = false;
}

void save_high_score(const int &high_score)
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

void load_resources()
{
    load_font("game-font", "Space Crusaders.ttf");
    load_bitmap("background-blurred", "background-blurred.jpg");
    load_bitmap("menu-logo", "tie-fighter.png");
    load_bitmap("start", "button.png");
    load_bitmap("info", "button.png");
    load_bitmap("exit", "button.png");
    load_bitmap("music-on", "musicOn.png");
    load_bitmap("music-off", "musicOff.png");
    load_bitmap("audio-on", "audioOn.png");
    load_bitmap("audio-off", "audioOff.png");
    load_sound_effect("tie", "tie.wav");
    load_bitmap("back", "button.png");
    load_bitmap("arrow", "arrow.png");
    load_bitmap("tie", "tie-fighter.png");
    load_sound_effect("explosion", "explosion.wav");
    load_sound_effect("score-ding", "score.wav");
    load_sound_effect("whoosh", "whoosh.wav");
}

void end_game(menu_options &menu, player_data &player, int &high_score)
{
    int timer = 180;
    process_events();
    while ( timer != 0)
    {
        save_high_score(high_score);
        player.bmp = load_bitmap("player-death", "explosion.png");
        draw_bitmap(player.bmp, player.x, player.y);
        draw_text("Game Over!", COLOR_WHITE, "game-font", 48, 150, 400);
        refresh_screen(60);
        timer -= 1;
    }
    menu = MAIN;
}

void main_menu(menu_options &menu, bool &music_set, bool &sound_set)
{
    int high_score;
    music menu_music;

    high_score = get_high_score();
    menu_music = load_music("menu", "menu_music.mp3");

    do {
        process_events();

        draw_bitmap("background-blurred", -1900, -600);
        draw_text("TIE", COLOR_WHITE, "game-font", 100, 150, 200);
        draw_text("Flapper", COLOR_WHITE, "game-font", 64, 160, 300);

        if ( music_set ) draw_bitmap("music-on", 540, 10);
        else
        {
            draw_bitmap("music-off", 540, 10);
            fade_music_out(1000);
        }
        if ( music_set && !music_playing() ) play_music(menu_music, 5, 0.25f);
        if ( sound_set ) draw_bitmap("audio-on", 500, 10);
        else draw_bitmap("audio-off", 500, 10);

        draw_bitmap("menu-logo", 320, 215);
        draw_bitmap("start", 220, 400);
        draw_bitmap("info", 220, 500);
        draw_bitmap("exit", 220, 600);
        draw_text("PLAY", COLOR_WHITE, "game-font", 32, 262, 410);
        button_hover(220, 380, 400, 460, "PLAY");
        draw_text("INFO", COLOR_WHITE, "game-font", 32, 262, 510);
        button_hover(220, 380, 500, 560, "INFO");
        draw_text("EXIT", COLOR_WHITE, "game-font", 32, 262, 610);
        button_hover(220, 380, 600, 660, "EXIT");
        draw_text("High Score: " + to_string(high_score), COLOR_WHITE, "game-font", 42, 140, 750);
        button_clicked(540, 580, 20, 80, music_set);
        button_clicked(500, 540, 20, 80, sound_set);
        button_clicked(220, 400, 410, 470, menu, 1);
        button_clicked(220, 400, 510, 570, menu, 2); 
        button_clicked(220, 400, 610, 670, menu, 3);
        if ( menu == GAME )
        {
            stop_music();
            if ( sound_set )
            {
                play_sound_effect("tie");
            }
        }
        refresh_screen(60);
    }
    while ( menu == MAIN );
}

void info_screen(menu_options &menu, bool &music_set, bool &sound_set)
{
    while( menu == HELP) 
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
    
        draw_text("BACK", COLOR_WHITE, "game-font", 32, 262, 710);
        button_hover(220, 380, 700, 760, "BACK");     
        button_clicked(220, 400, 700, 770, menu, 0);
        refresh_screen(60);
    }
}

void game(menu_options &menu, bool &music_set, bool &sound_set)
{
    // player variables
    player_data player;
    player = new_player();
    music level_music;

    // pipe variables
    pipe_data pipe_a = new_pipe(600, reset_pipe(0));
    pipe_data pipe_b = new_pipe(1000, reset_pipe(0));

    // score variables
    int score = 0;
    int high_score = get_high_score();
    bool score_flag = false;

    // background variables
    bg_data bg_a = new_bg(-1900, -600);
    bg_data bg_b = new_bg(bg_a.width + bg_a.x, -600);

    // difficulty variable
    int dif = 4;

    level_music = load_music("level-music", "level_music.mp3");
    if ( music_set ) play_music(level_music, 5, 0.25f);
    do
    {
        process_events();
        increase_score(player, pipe_a, pipe_b, score, high_score, score_flag, sound_set);
        
        draw_bitmap(bg_a.bmp, bg_a.x, bg_a.y);
        draw_bitmap(bg_b.bmp, bg_b.x, bg_b.y);
        draw_bitmap(player.bmp, player.x, player.y);
        draw_bitmap(pipe_a.upper, pipe_a.x, pipe_a.y - pipe_a.height);
        draw_bitmap(pipe_a.lower, pipe_a.x, pipe_a.y + pipe_a.gap);
        draw_bitmap(pipe_b.upper, pipe_b.x, pipe_b.y - pipe_b.height);
        draw_bitmap(pipe_b.lower, pipe_b.x, pipe_b.y + pipe_b.gap);
        draw_text("Score: " + to_string(score), COLOR_WHITE, "game-font", 42, 20, 750);
        draw_text("Best: " + to_string(high_score), COLOR_WHITE, "game-font", 42, 400, 750);

        pipe_a.x -= 2;
        pipe_b.x -= 2;
        boundary_exceeded(pipe_a.x, pipe_a.y);
        boundary_exceeded(pipe_b.x, pipe_b.y);
        bg_a.x -= 1;
        bg_b.x -= 1;
        if (bg_a.x < -4000) bg_a.x = bg_b.x + bg_b.width;
        if (bg_b.x < -4000) bg_b.x = bg_a.x + bg_a.width;
        player.y += player.vel;

        if ( key_typed(SPACE_KEY) || (mouse_down(LEFT_BUTTON))) flap(player.vel, sound_set);
        else player.vel += player.acc;

        if (collision(player, pipe_a, pipe_b, sound_set, dif)) end_game(menu, player, high_score);

        refresh_screen(60);
    } while ( menu == GAME );
}

int main()
{
    menu_options menu = MAIN;
    load_resources();
    bitmap icon = load_bitmap("icon", "tie-fighter.png");
    window game_window = open_window("TIE Flapper", 600, 800);
    window_set_icon(game_window, icon);
    clear_screen(COLOR_BLACK);    
    bool music_set = true;
    bool sound_set = true;

    while ( menu != EXIT )
    {
        switch (menu)
        {
        case GAME:
            delay(1500);
            game(menu, music_set, sound_set);
            break;
        case HELP:
            info_screen(menu, music_set, sound_set);
            break;
        default:
            main_menu(menu, music_set, sound_set);
            break;
        }      
    }
    refresh_screen(60);
    return 0;
}
