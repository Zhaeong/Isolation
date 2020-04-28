#include "main.h"


GameState GS;


#ifndef EMSCRIPTEN
const int FPS = 60;
//How many miliseconds per frame
const int FrameDelay = 1000 / FPS;
#endif 

bool ShowDebug = true;
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
                switch(event.key.keysym.sym)
                {
                    case SDLK_0:
                        {
                            if(ShowDebug)
                            {
                                ShowDebug = false;
                            }
                            else
                            {
                                ShowDebug = true;
                            }
                            break;
                        }
                    case SDLK_d:
                        {
                            cout << "dd\n"; 
                            GS.PlayerState = "RIGHT";
                            break;
                        }
                    case SDLK_a:
                        {
                            cout << "aa\n"; 
                            GS.PlayerState = "LEFT";
                            break;
                        }
                }
                break;
            case SDL_KEYUP:
                //cout << "pressed\n"; 
                {
                    GS.PlayerState = "IDLE";
                    break;
                }
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

    if(GS.PlayerState == "LEFT")
    {
        GS.manTexArray[0].mX -= 1;
    }
    else if(GS.PlayerState == "RIGHT")
    {
        GS.manTexArray[0].mX += 1;
    }
    ProcessTexturePartArray(GS.manTexArray, ManTexArraySize);
    //GS.manTexArray[0].mRotation += 1;
    //GS.manTexArray[1].mRotation -= 1;


    //Render Code
    RenderTexturePartArray(GS.renderer, GS.manTexArray, ManTexArraySize);


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
    RemoveTextureWhiteSpace(GS.man);
    GS.State = "START";
    GS.PlayerState = "IDLE";
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
            63,
            4,
            42,
            48 
            ); 
    //Body
    GS.manTexArray[1] = InitTexturePart(GS.man,
            &GS.manTexArray[0], 
            0,
            0,
            -5,
            42,
            62,
            79,
            58,
            88 
            ); 

    //left thigh 
    GS.manTexArray[2] = InitTexturePart(GS.man,
            &GS.manTexArray[1], 
            0,
            0,
            13,
            80,
            54,
            177,
            34,
            71 
            ); 

    GS.manTexArray[2].mEnableRotation = true;
    GS.manTexArray[2].mRotState = 1;
    GS.manTexArray[2].mRotMax = 20;
    GS.manTexArray[2].mRotMin = -20;

    GS.manTexArray[2].mCenter.x = 17;
    GS.manTexArray[2].mCenter.y = 0;

    //left leg
    GS.manTexArray[3] = InitTexturePart(GS.man,
            &GS.manTexArray[2], 
            0,
            0,
            9,
            55,
            58,
            259,
            27,
            62 
            ); 

    GS.manTexArray[3].mXrenderOffset = -11;
    GS.manTexArray[3].mRotation = 20;

    //right thigh 
    GS.manTexArray[4] = InitTexturePart(GS.man,
            &GS.manTexArray[1], 
            0,
            0,
            13,
            80,
            103,
            173,
            33,
            67 
            ); 

    GS.manTexArray[4].mEnableRotation = true;
    GS.manTexArray[4].mRotState = 2;
    GS.manTexArray[4].mRotMax = 20;
    GS.manTexArray[4].mRotMin = -20;

    GS.manTexArray[4].mCenter.x = 17;
    GS.manTexArray[4].mCenter.y = 0;

    //right leg
    GS.manTexArray[5] = InitTexturePart(GS.man,
            &GS.manTexArray[4], 
            0,
            0,
            9,
            55,
            114,
            254,
            26,
            60 
            ); 

    GS.manTexArray[5].mXrenderOffset = -11;
    GS.manTexArray[5].mRotation = 20;
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

