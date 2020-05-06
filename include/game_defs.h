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

//Player array posits
const int ManTexArraySize = 14;
//Denotes the render order
const int HEAD = 0;

const int RIGHT_UARM = 2;
const int RIGHT_ARM = 1;
const int RIGHT_HAND = 3;

const int BODY = 4;
const int LEFT_THIGH = 6;
const int LEFT_LEG = 5;
const int LEFT_FOOT = 7;
const int RIGHT_THIGH = 9;
const int RIGHT_LEG = 8;
const int RIGHT_FOOT = 10;
const int LEFT_UARM = 12;
const int LEFT_ARM = 11;
const int LEFT_HAND = 13;


//Declared as extern so that it's shared between files
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

    //0 for still, no rotation
    //1 for left
    //2 for right
    int mRotState;

    //When to begin rotation towards max or min
    double mParentRotAngleMax;
    double mParentRotAngleMin;
    bool mRotMaxStart;
    bool mRotMinStart;

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

    //Debugging
    int curSelectedTexturePart;
};




#endif // GAMEDEFS_H

