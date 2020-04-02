#include "main.h"


//Declarations for sdl window displaying
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

const int FPS = 60;
//How many miliseconds per frame
const int FrameDelay = 1000 / FPS;


SDL_AudioDeviceID audioDevice;

void gameloop() 
{
    Uint32 frameStart;
    Uint32 frameTime;
    frameStart = SDL_GetTicks();  

    //The color at which the screen will be if alpha = 0 on all textures
    //SDL_SetRenderDrawColor(renderer, screenColor.r, screenColor.g, screenColor.b, screenColor.a);

    SDL_RenderClear(renderer);

    ////////////////////////////////////////////////////////////////////////
    //Main Game Code
    ////////////////////////////////////////////////////////////////////////
    int xMouse = 0;
    int yMouse = 0;
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        switch (event.type) 
        {
            case SDL_KEYDOWN:
                break;
            case SDL_KEYUP:
                //cout << "pressed\n"; 
                break;
            case SDL_MOUSEBUTTONDOWN:
                //cout << "MOUSE_DOWN \n";
                break;
            case SDL_MOUSEBUTTONUP:
                {
                    //cout <<  "MOUSE_UP\n";
                    SDL_GetMouseState(&xMouse, &yMouse);

                    //SDL_Point mousePoint = {xMouse, yMouse};
                    

                  
                }
            case SDL_QUIT:
                exit(0);
                break;
        }
    }
          

    ////////////////////////////////////////////////////////////////////////
    //End of main game code
    ////////////////////////////////////////////////////////////////////////

    frameTime = SDL_GetTicks() - frameStart;

    if (FrameDelay > frameTime)
    {
        SDL_Delay(FrameDelay - frameTime);
    }

    //emscripten_cancel_main_loop();
}

//Need to use this main signature (int argv, char** args)
//Because SDL alters default program entry point resolving
//SDL calls your entry point expecting a specific signature,
//which is no longer subject to compiler interpretation
int main(int argv, char **args)
{
    //cout << "Starting Game\n";

    //Initiate SDL
    StartSDL(&window, &renderer);
 
/*
    audioDevice = SDL_OpenAudioDevice(NULL, 0, &ToddlerMus.wavSpec, NULL, 0);
    if (audioDevice == 0) {
        printf("Failed to open audio: %s", SDL_GetError());
    } 
*/

//    SDL_PauseAudioDevice(audioDevice, 0);


#ifdef EMSCRIPTEN
    emscripten_set_main_loop(gameloop, 0, 0);
#else
    while(1)
    {
        gameloop();
    }
#endif 
    return 0;

}
