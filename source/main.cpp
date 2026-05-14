#include "Game.h"
#include <CEngine.h>

int main()
{
    Game* game = new Game();
    CEngine::Engine engine;
    engine.SetApplication(game);

    if (engine.Init(1280, 720))
    {
        engine.Run();
    }

    engine.Destroy();
    return 0;
}