#ifndef GAME_H
#define GAME_H
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <cstring>
#include <iostream>

#ifdef EMSCRIPTEN
    #include <emscripten.h>
#endif 

#include "game_defs.h"


using namespace std;


int StartSDL(SDL_Window **window, SDL_Renderer **renderer);

SDL_Texture* GetSDLTexture(SDL_Renderer *renderer, SDL_Window *window, string textureLocation);

Texture InitTexture(SDL_Texture *sdlTexture, int x, int y);

TexturePart InitTexturePart(SDL_Texture *sdlTexture,
        TexturePart *texPart,
        int xPos, 
        int yPos,
        int x,
        int y,
        int w,
        int h);
void RenderTexture(SDL_Renderer *renderer, Texture tex);

void RenderTexturePart(SDL_Renderer *renderer, TexturePart tex);

void RemoveTextureWhiteSpace(SDL_Texture *texture);

AudioClip InitAudio(string filepath);

void PlayAudio(SDL_AudioDeviceID audioDevice, AudioClip clip);

SDL_Point RotatePointByOtherPoint(int inX,
                                  int inY,
                                  int centerX,
                                  int centerY,
                                  int degrees);


#endif // GAME_H
 
