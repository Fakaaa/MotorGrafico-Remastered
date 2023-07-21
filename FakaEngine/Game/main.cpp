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
            game->InitGame();
            game->UpdateEngine();
            game->DestroyGame();
            game->DestroyEngine();
            delete game;
            game = NULL;
        }
    }
    return 0;
}