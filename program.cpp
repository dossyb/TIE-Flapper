#include "splashkit.h"

#define FLAP_SPEED -5
#define PLAYER_X 50

int main()
{
    double playerY;
    double playerVel = 0;
    double playerAcc = 0.1;

    double backgroundX;
    double backgroundY;

    playerY = 50;
    backgroundX = -1900;
    backgroundY = -600;

    open_window("TIE Flapper", 600, 800);
    clear_screen(COLOR_BLACK);
    load_bitmap("player", "tie-fighter.png");
    load_bitmap("background", "background.jpg");
    do
    {
        process_events();
        
        draw_bitmap("background", backgroundX, backgroundY);
        draw_bitmap("player", PLAYER_X, playerY);

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

        playerY += playerVel;

        if ( playerY > screen_height() )
        {
            playerY = 0;
        }

        refresh_screen(60);
    } while ( not quit_requested() );
    
    return 0;
}