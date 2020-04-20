#include "main.h"


GameState GS;


#ifndef EMSCRIPTEN
const int FPS = 60;
//How many miliseconds per frame
const int FrameDelay = 1000 / FPS;
#endif 

SDL_AudioDeviceID audioDevice;

void gameloop() 
{
    Uint32 frameStart;

    frameStart = SDL_GetTicks();  

    //The color at which the screen will be if alpha = 0 on all textures
    SDL_SetRenderDrawColor(GS.renderer, GS.screenColor.r, GS.screenColor.g, GS.screenColor.b, GS.screenColor.a);


    SDL_RenderClear(GS.renderer);


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
                GS.manTexArray[0].mX += 1;
                GS.manTexArray[0].mY += 1;
                break;
            case SDL_MOUSEBUTTONUP:
                //cout <<  "MOUSE_UP\n";
                SDL_GetMouseState(&xMouse, &yMouse);
                cout << "mouseX: " << xMouse << " mouseY: " << yMouse << "\n";
                //SDL_Point mousePoint = {xMouse, yMouse};

                break;
            case SDL_QUIT:
                exit(0);
                break;
        }
    }

    ProcessTexturePartArray(GS.manTexArray, ManTexArraySize);
    GS.manTexArray[0].mRotation += 1;
    GS.manTexArray[1].mRotation -= 1;


    //Render Code

    RenderTexturePart(GS.renderer, GS.manTexArray[0]);
    RenderTexturePart(GS.renderer, GS.manTexArray[1]);
    RenderTexturePart(GS.renderer, GS.manTexArray[2]);

    ////////////////////////////////////////////////////////////////////////
    //End of main game code
    ////////////////////////////////////////////////////////////////////////

#ifndef EMSCRIPTEN
    Uint32 frameTime;
    frameTime = SDL_GetTicks() - frameStart;

    if (FrameDelay > frameTime)
    {
        SDL_Delay(FrameDelay - frameTime);
    }
#endif 
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
    StartSDL(&(GS.window), &(GS.renderer));
    GS.man = GetSDLTexture(GS.renderer, GS.window, "./res/png/man.png");
    //RemoveTextureWhiteSpace(GS.man);
    GS.State = "START";
    GS.screenColor.r = 0;
    GS.screenColor.g = 0;
    GS.screenColor.b = 0;
    GS.screenColor.a = 255;

    //Head
    GS.manTexArray[0] = InitTexturePart(GS.man,
            NULL,
            0,
            0,
            0,
            0,
            20,
            20,
            140,
            140
            ); 
    //Body
    GS.manTexArray[1] = InitTexturePart(GS.man,
            &GS.manTexArray[0], 
            0,
            0,
            10,
            140,
            190,
            20,
            180,
            240
            ); 

    //Leg
    GS.manTexArray[2] = InitTexturePart(GS.man,
            &GS.manTexArray[1], 
            0,
            0,
            10,
            140,
            400,
            30,
            55,
            180 
            ); 
    //GS.manTexArray[0].mRotation = 30;


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

