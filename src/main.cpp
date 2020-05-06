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
    int mouseX = 0;
    int mouseY = 0;
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
                        //Debug keysym
                    case SDLK_p:
                        {
                            cout << "==========INFO: " << GS.curSelectedTexturePart << "========\n";
                            cout << "Rotation: " << GS.manTexArray[GS.curSelectedTexturePart].mRotation << "\n"; 
                            cout << "RotMax: " << GS.manTexArray[GS.curSelectedTexturePart].mRotMax << "\n"; 
                            cout << "RotMin: " << GS.manTexArray[GS.curSelectedTexturePart].mRotMin << "\n"; 
                            cout << "mParentRotAngleMax: " << GS.manTexArray[GS.curSelectedTexturePart].mParentRotAngleMax << "\n"; 
                            cout << "mParentRotAngleMin: " << GS.manTexArray[GS.curSelectedTexturePart].mParentRotAngleMin << "\n"; 
                            cout << "=========================\n";
                            break;
                        }
                    case SDLK_1:
                        {
                            GS.manTexArray[GS.curSelectedTexturePart].mRotMax += 1;
                            break;
                        }
                    case SDLK_2:
                        {
                            GS.manTexArray[GS.curSelectedTexturePart].mRotMax -= 1;
                            break;
                        }
                    case SDLK_3:
                        {
                            GS.manTexArray[GS.curSelectedTexturePart].mRotMin += 1;
                            break;
                        }
                    case SDLK_4:
                        {
                            GS.manTexArray[GS.curSelectedTexturePart].mRotMin -= 1;
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
                SDL_GetMouseState(&mouseX, &mouseY);
                cout << "mouseX: " << mouseX << " mouseY: " << mouseY << "\n";
                //SDL_Point mousePoint = {xMouse, yMouse};
                GS.curSelectedTexturePart = TexturePartMouseCollision(GS.manTexArray, ManTexArraySize, mouseX, mouseY);
                cout << "CurTexture: " << GS.curSelectedTexturePart << "\n";
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

    GS.curSelectedTexturePart = 0;

    //Head
    GS.manTexArray[HEAD] = InitTexturePart(GS.man,
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
    GS.manTexArray[BODY] = InitTexturePart(GS.man,
            &GS.manTexArray[HEAD], 
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
    GS.manTexArray[LEFT_THIGH] = InitTexturePart(GS.man,
            &GS.manTexArray[BODY], 
            0,
            0,
            13,
            80,
            54,
            177,
            34,
            71 
            ); 

    GS.manTexArray[LEFT_THIGH].mRotState = 1;
    GS.manTexArray[LEFT_THIGH].mRotMax = 20;
    GS.manTexArray[LEFT_THIGH].mRotMin = -30;

    GS.manTexArray[LEFT_THIGH].mCenter.x = 17;
    GS.manTexArray[LEFT_THIGH].mCenter.y = 0;

    //left leg
    GS.manTexArray[LEFT_LEG] = InitTexturePart(GS.man,
            &GS.manTexArray[LEFT_THIGH], 
            0,
            0,
            15,
            55,
            58,
            259,
            27,
            62 
            ); 

    GS.manTexArray[LEFT_LEG].mXrenderOffset = -10;
    GS.manTexArray[LEFT_LEG].mRotation = 20;
    GS.manTexArray[LEFT_LEG].mRotState = 6;
    GS.manTexArray[LEFT_LEG].mParentRotAngleMax = 20;
    GS.manTexArray[LEFT_LEG].mParentRotAngleMin = -20;
    GS.manTexArray[LEFT_LEG].mRotMax = 30;
    GS.manTexArray[LEFT_LEG].mRotMin = 0;
    GS.manTexArray[LEFT_LEG].mCenter.x = 13;
    GS.manTexArray[LEFT_LEG].mCenter.y = 0;

    //left foot
    GS.manTexArray[LEFT_FOOT] = InitTexturePart(GS.man,
            &GS.manTexArray[LEFT_LEG], 
            0,
            0,
            -5,
            60,
            47,
            334,
            36,
            26 
            ); 
    GS.manTexArray[LEFT_FOOT].mYrenderOffset = -10;

    //right thigh 
    GS.manTexArray[RIGHT_THIGH] = InitTexturePart(GS.man,
            &GS.manTexArray[BODY], 
            0,
            0,
            13,
            80,
            54,
            177,
            34,
            71 
            ); 

    GS.manTexArray[RIGHT_THIGH].mRotState = 2;
    GS.manTexArray[RIGHT_THIGH].mRotMax = 20;
    GS.manTexArray[RIGHT_THIGH].mRotMin = -30;

    GS.manTexArray[RIGHT_THIGH].mCenter.x = 17;
    GS.manTexArray[RIGHT_THIGH].mCenter.y = 0;

    //right leg
    GS.manTexArray[RIGHT_LEG] = InitTexturePart(GS.man,
            &GS.manTexArray[RIGHT_THIGH], 
            0,
            0,
            15,
            55,
            58,
            259,
            27,
            62 
            ); 

    GS.manTexArray[RIGHT_LEG].mXrenderOffset = -10;
    GS.manTexArray[RIGHT_LEG].mRotation = 20;
    GS.manTexArray[RIGHT_LEG].mRotState = 6;
    GS.manTexArray[RIGHT_LEG].mParentRotAngleMax = 20;
    GS.manTexArray[RIGHT_LEG].mParentRotAngleMin = -20;
    GS.manTexArray[RIGHT_LEG].mRotMax = 30;
    GS.manTexArray[RIGHT_LEG].mRotMin = 0;
    GS.manTexArray[RIGHT_LEG].mCenter.x = 13;
    GS.manTexArray[RIGHT_LEG].mCenter.y = 0;

    //right foot
    GS.manTexArray[RIGHT_FOOT] = InitTexturePart(GS.man,
            &GS.manTexArray[RIGHT_LEG], 
            0,
            0,
            -5,
            60,
            47,
            334,
            36,
            26 
            ); 
    GS.manTexArray[RIGHT_FOOT].mYrenderOffset = -10;

    //left upper Arm
    GS.manTexArray[LEFT_UARM] = InitTexturePart(GS.man,
            &GS.manTexArray[BODY], 
            0,
            0,
            15,
            10,
            6,
            81,
            23,
            52 
            ); 
    GS.manTexArray[LEFT_UARM].mRotState = 0;
    GS.manTexArray[LEFT_UARM].mRotMax = 30;
    GS.manTexArray[LEFT_UARM].mRotMin = -30;
    GS.manTexArray[LEFT_UARM].mCenter.x = 11;
    GS.manTexArray[LEFT_UARM].mCenter.y = 0;

    //left Arm
    GS.manTexArray[LEFT_ARM] = InitTexturePart(GS.man,
            &GS.manTexArray[LEFT_UARM], 
            0,
            0,
            12,
            45,
            12,
            142,
            19,
            37 
            ); 
    GS.manTexArray[LEFT_ARM].mXrenderOffset = -2;
    GS.manTexArray[LEFT_ARM].mRotation = 0;
    GS.manTexArray[LEFT_ARM].mRotState = 0;
    GS.manTexArray[LEFT_ARM].mParentRotAngleMax = 20;
    GS.manTexArray[LEFT_ARM].mParentRotAngleMin = -20;
    GS.manTexArray[LEFT_ARM].mRotMax = 20;
    GS.manTexArray[LEFT_ARM].mRotMin = -20;
    GS.manTexArray[LEFT_ARM].mCenter.x = 11;
    GS.manTexArray[LEFT_ARM].mCenter.y = 0;
    //left hand
    GS.manTexArray[LEFT_HAND] = InitTexturePart(GS.man,
            &GS.manTexArray[LEFT_ARM], 
            0,
            0,
            -2,
            24,
            4,
            192,
            24,
            26 
            ); 

    //right upper Arm
    GS.manTexArray[RIGHT_UARM] = InitTexturePart(GS.man,
            &GS.manTexArray[BODY], 
            0,
            0,
            15,
            10,
            6,
            81,
            23,
            52 
            ); 
    GS.manTexArray[RIGHT_UARM].mRotState = 0;
    GS.manTexArray[RIGHT_UARM].mRotMax = 20;
    GS.manTexArray[RIGHT_UARM].mRotMin = -20;
    GS.manTexArray[RIGHT_UARM].mCenter.x = 11;
    GS.manTexArray[RIGHT_UARM].mCenter.y = 0;

    //right Arm
    GS.manTexArray[RIGHT_ARM] = InitTexturePart(GS.man,
            &GS.manTexArray[RIGHT_UARM], 
            0,
            0,
            12,
            45,
            12,
            142,
            19,
            37 
            ); 
    GS.manTexArray[RIGHT_ARM].mXrenderOffset = -2;
    GS.manTexArray[RIGHT_ARM].mRotation = -40;

    //right hand
    GS.manTexArray[RIGHT_HAND] = InitTexturePart(GS.man,
            &GS.manTexArray[RIGHT_ARM], 
            0,
            0,
            -2,
            24,
            4,
            192,
            24,
            26 
            ); 



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

