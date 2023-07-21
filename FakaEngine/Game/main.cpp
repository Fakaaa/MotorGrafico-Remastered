#include "Game.h"

int main()
{
    srand(time(NULL));
    Game* game = new Game();
    int result = game->InitEngine();

    if (result != INIT_ERROR)
    {
        if (game != NULL) 
        {
        }
    }
    return 0;
}