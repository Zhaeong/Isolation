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

void RenderTexture(SDL_Renderer *renderer, Texture tex);

void RemoveTextureWhiteSpace(SDL_Texture *texture);

AudioClip InitAudio(string filepath);

void PlayAudio(SDL_AudioDeviceID audioDevice, AudioClip clip);

#endif // GAME_H
 