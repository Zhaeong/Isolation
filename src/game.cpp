#include "game.h"

int StartSDL(SDL_Window **window, SDL_Renderer **renderer)
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Couldn't initialize SDL: %s",
                SDL_GetError());
    }

    //Init Audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Couldn't initialize SDL: %s",
                SDL_GetError());
    }

    //Uint32 windowType = SDL_WINDOW_FULLSCREEN;
    Uint32 windowType = SDL_WINDOW_RESIZABLE;

    //SDL_WINDOW_FULLSCREEN
    //SDL_WINDOW_RESIZABLE
    if (SDL_CreateWindowAndRenderer(GAMEWIDTH,
                GAMEHEIGHT,
                windowType,
                window,
                renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Couldn't create window and renderer: %s",
                SDL_GetError());
    }

    return 0;
}

SDL_Texture *GetSDLTexture(SDL_Renderer *renderer, SDL_Window *window, string textureLocation)
{
    //Make sure to initialize texture to null or else SDL_DestroyTexture will crash program
    SDL_Texture *texture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(textureLocation.c_str());

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n",
                textureLocation.c_str(),
                IMG_GetError());
    }
    else
    {
        //Convert surface to display format

        //Note have to use SDL_PIXELFORMAT_ARGB8888 due to window using SDL_PIXELFORMAT_RGB888
        //Which doesn't have alpha channel

        //SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface,
        //                                                          SDL_GetWindowPixelFormat( window ),
        //                                                          0 );

        //cout << "Loading Texture: " << textureLocation << "\n";
        SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface,
                TEXTUREFORMAT,
                0);

        if (formattedSurface == NULL)
        {
            printf("Unable to convert loaded surface to display format! SDL Error: %s\n",
                    SDL_GetError());
        }
        else
        {
            //Create blank streamable texture

            //texture = SDL_CreateTexture( renderer,
            //                             SDL_GetWindowPixelFormat( window ),
            //                             SDL_TEXTUREACCESS_STREAMING,
            //                             formattedSurface->w,
            //                             formattedSurface->h );

            texture = SDL_CreateTexture(renderer,
                    TEXTUREFORMAT,
                    SDL_TEXTUREACCESS_STREAMING,
                    formattedSurface->w,
                    formattedSurface->h);

            //texture = SDL_CreateTextureFromSurface( renderer, formattedSurface );
            if (texture == NULL)
            {
                printf("Unable to create blank texture! SDL Error: %s\n",
                        SDL_GetError());
            }
            else
            {
                void *mPixels;
                int mPitch;

                SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

                //Lock texture for manipulation
                if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
                {
                    printf("Unable to lock texture! %s\n", SDL_GetError());
                }

                //Copy loaded/formatted surface pixels
                memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

                //Unlock texture to update
                SDL_UnlockTexture(texture);

                mPixels = NULL;

                //Get image dimensions
                //mWidth = formattedSurface->w;
                //mHeight = formattedSurface->h;
            }

            //Get rid of old formatted surface
            SDL_FreeSurface(formattedSurface);
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return texture;
}

void RemoveTextureWhiteSpace(SDL_Texture *texture)
{
    void *mPixels;
    int mPitch;

    if (texture == NULL)
    {
        printf("Input Texture is null in RemoveTextureWhiteSpace! SDL Error: %s\n",
                SDL_GetError());
    }

    if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
    {
        printf("Unable to lock texture! %s\n", SDL_GetError());
    }
    else
    {
        //Allocate format from window
        //Uint32 format = SDL_GetWindowPixelFormat( window );

        SDL_PixelFormat *mappingFormat = SDL_AllocFormat(TEXTUREFORMAT);

        int texWidth = 0;
        int texHeight = 0;

        Uint32 texFormat;
        SDL_QueryTexture(texture, &texFormat, NULL, &texWidth, &texHeight);

        //Get pixel data
        Uint32 *pixels = (Uint32 *)mPixels;
        int pixelCount = (mPitch / 4) * texHeight;

        //cout << "texformat:" << texFormat << "\n";
        //cout << "texWidth:" << texWidth << "\n";
        //cout << "texHeight:" << texHeight << "\n";
        //cout << "mPitch:" << mPitch << "\n";
        //cout << "pixelCount:" << pixelCount << "\n";

        //Map colors

        Uint32 colorKeyWhite = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0xFF);
        Uint32 colorKeyGray = SDL_MapRGBA(mappingFormat, 182, 182, 182, 0xFF);

        Uint32 transparent = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0);

        //Color key pixels
        for (int i = 0; i < pixelCount; ++i)
        {
            //cout << "col:" << pixels[i] << "\n";
            //cout << "key:" << colorKey << "\n";

            if (pixels[i] == colorKeyWhite || pixels[i] == colorKeyGray)
            {
                pixels[i] = transparent;
            }
        }

        SDL_UnlockTexture(texture);
        //mPixels = NULL;
        //Free format
        SDL_FreeFormat(mappingFormat);
    }
}

Texture InitTexture(SDL_Texture *sdlTexture, int x, int y)
{
    Texture outTex;
    outTex.mX = x;
    outTex.mY = y;
    SDL_QueryTexture(sdlTexture, NULL, NULL, &outTex.mW, &outTex.mH);
    outTex.mRotation = 0;
    outTex.mAlpha = 255;
    outTex.mTexture = sdlTexture;
    outTex.mCenter = NULL;
    outTex.mFlip = SDL_FLIP_NONE;
    return outTex;
}

TexturePart InitTexturePart(SDL_Texture *sdlTexture,
        TexturePart *texPart,
        int xPos, 
        int yPos,
        int xOffset,
        int yOffset,
        int x,
        int y,
        int w,
        int h)
{
    TexturePart outTex;

    outTex.mTexture = sdlTexture;
    outTex.mReferenceTexture = texPart;

    outTex.mSrcRect.x = x;
    outTex.mSrcRect.y = y;
    outTex.mSrcRect.w = w;
    outTex.mSrcRect.h = h;

    outTex.mX = xPos;
    outTex.mY = yPos;

    outTex.mXoffset = xOffset;
    outTex.mYoffset = yOffset;

    outTex.mRotation = 0;
    outTex.mAlpha = 255;

    outTex.mCenter.x = w/2;
    outTex.mCenter.y = h/2;

    outTex.mFlip = SDL_FLIP_NONE;

    //Rotation
    outTex.mEnableRotation = false;
    outTex.mRotState = 0;
    outTex.mParentRotAngleStart = 0;
    outTex.mRotSpeed = 0.5;
    outTex.mRotMax = 0;
    outTex.mRotMin = 0;

    return outTex;
}


void RenderTexture(SDL_Renderer *renderer, Texture tex)
{
    //Don't render if the alpha is 0
    if(tex.mAlpha > 0)
    {
        SDL_SetTextureBlendMode(tex.mTexture, SDL_BLENDMODE_BLEND);

        SDL_SetTextureAlphaMod(tex.mTexture, tex.mAlpha);

        SDL_Rect srcRect;
        SDL_Rect dstRect;

        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.h = tex.mH;
        srcRect.w = tex.mW;

        int xPos = tex.mX;
        int yPos = tex.mY;

        dstRect.x = xPos;
        dstRect.y = yPos; 
        dstRect.h = tex.mH;
        dstRect.w = tex.mW;

        SDL_RenderCopyEx(renderer, tex.mTexture, &srcRect, &dstRect, tex.mRotation, tex.mCenter, tex.mFlip);
    }
}



void RenderTexturePart(SDL_Renderer *renderer, TexturePart tex)
{
    //Don't render if the alpha is 0
    if(tex.mAlpha > 0)
    {
        SDL_SetTextureBlendMode(tex.mTexture, SDL_BLENDMODE_BLEND);

        SDL_SetTextureAlphaMod(tex.mTexture, tex.mAlpha);


        int xPos = tex.mX;
        int yPos = tex.mY;
        if(tex.mReferenceTexture != NULL)
        {
            xPos = tex.mReferenceTexture->mX + tex.mXoffset;
            yPos = tex.mReferenceTexture->mY + tex.mYoffset;

            //If the parent texture has a rotation, we need to rotate the anchor point by the 
            //mid point of parent texture so that it aligns
            if(tex.mReferenceTexture->mRotation != 0)
            {
                int midX = tex.mReferenceTexture->mX + tex.mReferenceTexture->mCenter.x;
                int midY = tex.mReferenceTexture->mY + tex.mReferenceTexture->mCenter.y;

                SDL_Point rotatedPoint = RotatePointByOtherPoint(xPos, yPos, midX, midY, tex.mReferenceTexture->mRotation);
                xPos = rotatedPoint.x;
                yPos = rotatedPoint.y;
            }

        }

        SDL_Rect dstRect;
        dstRect.x = xPos;
        dstRect.y = yPos; 

        //divided by to to shrink size of texture by 50%
        dstRect.h = tex.mSrcRect.h;
        dstRect.w = tex.mSrcRect.w;


        SDL_RenderCopyEx(renderer, tex.mTexture, &tex.mSrcRect, &dstRect, tex.mRotation, &tex.mCenter, tex.mFlip);
#ifdef DEBUG 
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

        int midX = xPos + tex.mCenter.x;
        int midY = yPos + tex.mCenter.y;

        SDL_RenderDrawLine(renderer,
                xPos,
                yPos,
                midX,
                midY);

        SDL_RenderDrawLine(renderer,
                xPos,
                yPos,
                xPos,
                yPos + tex.mSrcRect.h);

        //Draw line of rotated start and end points

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_Point startPoint = RotatePointByOtherPoint(xPos, yPos, midX, midY, tex.mRotation);
        SDL_Point endPoint = RotatePointByOtherPoint(xPos, yPos + tex.mSrcRect.h, midX, midY, tex.mRotation); 
        SDL_RenderDrawLine(renderer,
                startPoint.x,
                startPoint.y,
                midX,
                midY);
        SDL_RenderDrawLine(renderer,
                startPoint.x,
                startPoint.y,
                endPoint.x,
                endPoint.y);

        //cout << "inix: " << xPos << "iniy:" <<yPos + tex.mSrcRect.h<< "\n";
        //cout << "endx: " << endPoint.x << "endy:" << endPoint.y << "\n";

#endif
    }
}

void SetTextureColor(SDL_Texture *texture, int R, int G, int B, int A)
{
    void *mPixels;
    int mPitch;

    if (texture == NULL)
    {
        printf("Input Texture is null in RemoveTextureWhiteSpace! SDL Error: %s\n",
                SDL_GetError());
    }

    if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
    {
        printf("Unable to lock texture! %s\n", SDL_GetError());
    }
    else
    {
        SDL_PixelFormat *mappingFormat = SDL_AllocFormat(TEXTUREFORMAT);

        int texWidth = 0;
        int texHeight = 0;

        Uint32 texFormat;
        SDL_QueryTexture(texture, &texFormat, NULL, &texWidth, &texHeight);

        //Get pixel data
        Uint32 *pixels = (Uint32 *)mPixels;
        int pixelCount = (mPitch / 4) * texHeight;

        //Map colors

        Uint32 colorKey = SDL_MapRGBA(mappingFormat, R, G, B, A);

        Uint32 transparent = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0);

        //Color key pixels
        for (int i = 0; i < pixelCount; ++i)
        {

            if (pixels[i] != transparent)
            {
                pixels[i] = colorKey;
            }
        }

        SDL_UnlockTexture(texture);

        //Free format
        SDL_FreeFormat(mappingFormat);
    }
}

AudioClip InitAudio(string filepath)
{
    AudioClip clip;
    clip.wavPath = filepath;
    if (SDL_LoadWAV(clip.wavPath.c_str(), &clip.wavSpec, &clip.wavBuffer, &clip.wavLength) == NULL) 
    {
        fprintf(stderr, "Could not open wav %s: %s\n", filepath.c_str(), SDL_GetError());
    } 
    return clip;
}

void PlayAudio(SDL_AudioDeviceID audioDevice, AudioClip clip)
{
    int success = SDL_QueueAudio(audioDevice, clip.wavBuffer, clip.wavLength);
    if(success < 0)
    {
        printf("SDL_QueueAudio failed %s, err: %s", clip.wavPath.c_str(), SDL_GetError()); 
    }

}


bool TextureMouseCollisionSingle(Texture mTexture, int xPos, int yPos)
{

    if (xPos >= mTexture.mX 
            && xPos <= (mTexture.mX + mTexture.mW) 
            && yPos >= mTexture.mY
            && yPos <= (mTexture.mY + mTexture.mH) 
       )
    {
        return true;
    }

    return false;
}

SDL_Point RotatePointByOtherPoint(int inX,
        int inY,
        int centerX,
        int centerY,
        int degrees)
{
    SDL_Point retPoint;
    double rad = degrees * PI / (double)180.0;
    double cosVal = cos(rad);
    double sinVal = sin(rad);

    //Change offset points back to origin
    int startX = inX - centerX;
    int startY = inY - centerY;

    double rotateX = startX * cosVal - startY * sinVal;
    double rotateY = startX * sinVal + startY * cosVal;

    retPoint.x = rotateX + centerX; 
    retPoint.y = rotateY + centerY;

    return retPoint;
}

void ProcessTexturePartArray(TexturePart *texturePartArray, int size)
{
    for(int i = 0; i < size; i++)
    {
        TexturePart tex = texturePartArray[i];
        if(tex.mReferenceTexture != NULL)
        {
            int xPos = tex.mReferenceTexture->mX + tex.mXoffset;
            int yPos = tex.mReferenceTexture->mY + tex.mYoffset;

            //If the parent texture has a rotation, we need to rotate the anchor point by the 
            //mid point of parent texture so that it aligns
            if(tex.mReferenceTexture->mRotation != 0)
            {
                int midX = tex.mReferenceTexture->mX + tex.mReferenceTexture->mCenter.x;
                int midY = tex.mReferenceTexture->mY + tex.mReferenceTexture->mCenter.y;
                SDL_Point rotatedPoint = RotatePointByOtherPoint(xPos, yPos, midX, midY, tex.mReferenceTexture->mRotation);
                xPos = rotatedPoint.x;
                yPos = rotatedPoint.y;

            }
            tex.mX = xPos;
            tex.mY = yPos;

        }

        //Apply rotation based on struct vals
        if(tex.mEnableRotation)
        {
            if(tex.mRotState == 1)
            {
                if(tex.mRotation < tex.mRotMax)
                {
                    tex.mRotation += tex.mRotSpeed;
                }
                else
                {
                    tex.mRotState = 2;
                }
            }
            else if(tex.mRotState == 2)
            {
                if(tex.mRotation > tex.mRotMin)
                {
                    tex.mRotation -= tex.mRotSpeed;
                }
                else
                {
                    tex.mRotState = 1;
                }

            }
        }
        texturePartArray[i] = tex;
    }
}
