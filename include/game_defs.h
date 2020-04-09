#ifndef GAMEDEFS_H
#define GAMEDEFS_H

#include <string>
#include <SDL.h>
#include <SDL_image.h>


using namespace std;



#define GAMEWIDTH  500 
#define GAMEHEIGHT 500


#define PI 3.14159265


const Uint32 TEXTUREFORMAT = SDL_PIXELFORMAT_RGBA8888;


struct Texture
{
    int mX;
    int mY;
    int mW;
    int mH;
    int mRotation;
    int mAlpha;
    SDL_Texture *mTexture;
    SDL_Point *mCenter;
    SDL_RendererFlip mFlip;
};

struct TexturePart
{
    SDL_Texture *mTexture;
    TexturePart *mReferenceTexture;
    SDL_Point mOffSet;
    SDL_Rect mSrcRect;

    int mX;
    int mY;

    int mRotation;
    int mAlpha;
    SDL_Point *mCenter;
    SDL_RendererFlip mFlip;
};
struct vect2
{
    float x;
    float y;
};

struct AudioClip
{
    string wavPath;
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
};

struct GameState
{
    string State;
    SDL_Color screenColor;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_AudioDeviceID audioDevice;

    SDL_Texture *man;
    TexturePart manTex;
};
#endif // GAMEDEFS_H

