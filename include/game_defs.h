#ifndef GAMEDEFS_H
#define GAMEDEFS_H

#include <string>
#include <SDL.h>
#include <SDL_image.h>


using namespace std;



#define GAMEWIDTH  700 
#define GAMEHEIGHT 700


#define PI 3.14159265

const Uint32 TEXTUREFORMAT = SDL_PIXELFORMAT_RGBA8888;

extern bool ShowDebug;
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
    SDL_Rect mSrcRect;

    int mX;
    int mY;

    int mXoffset;
    int mYoffset;

    //For use when rendering to subtract from position of offset parent
    int mXrenderOffset;
    int mYrenderOffset;

    double mRotation;
    int mAlpha;
    SDL_Point mCenter;
    SDL_RendererFlip mFlip;
    


    //Rotation specific variables

    bool mEnableRotation;
    //0 for still,
    //1 for left
    //2 for right
    int mRotState;

    //When to begin rotation
    int mParentRotAngleStart;

    double mRotSpeed;
    int mRotMax;
    int mRotMin;
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


const int ManTexArraySize = 10;
struct GameState
{
    string State;
    SDL_Color screenColor;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_AudioDeviceID audioDevice;

    SDL_Texture *man;
    TexturePart manTexArray[ManTexArraySize];

    string PlayerState;
};




#endif // GAMEDEFS_H

