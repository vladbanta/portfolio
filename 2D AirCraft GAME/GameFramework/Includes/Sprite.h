// Sprite.h
// By Frank Luna
// August 24, 2004.
#ifndef SPRITE_H
#define SPRITE_H

#include "main.h"
#include "Vec2.h"
#include "BackBuffer.h"

class Sprite
{
public:
	Sprite(int imageID, int maskID);
	Sprite(const char *szImageFile, const char *szMaskFile);
	Sprite(const char *szImageFile, COLORREF crTransparentColor);

	virtual ~Sprite();

	virtual int width(){ return mImageBM.bmWidth; }
	virtual int height(){ return mImageBM.bmHeight; }
	void setScale(float, float);
	Vec2 getScale();
	void update(float dt);
	void SaveObject(FILE* file);
	void LoadObject(FILE* file);
	void SetImage(HBITMAP&);

	void setBackBuffer(const BackBuffer *pBackBuffer);
	virtual void draw();

public:
	// Keep these public because they need to be
	// modified externally frequently.
	Vec2 mPosition;
	Vec2 mVelocity;
private:
	// Make copy constructor and assignment operator private
	// so client cannot copy Sprites. We do this because
	// this class is not designed to be copied because it
	// is not efficient--copying bitmaps is slow (lots of memory).
	Sprite(const Sprite& rhs);
	Sprite& operator=(const Sprite& rhs);

protected:
	HBITMAP mhImage;
	HBITMAP mhMask;
	BITMAP mImageBM;
	BITMAP mMaskBM;

	HDC mhSpriteDC;
	const BackBuffer *mpBackBuffer;

	float mfScaleX;
	float mfScaleY;

	COLORREF mcTransparentColor;
	void drawTransparent();
	void drawMask();
};

// AnimatedSprite
// by Mihai Popescu
// April 5, 2008
class AnimatedSprite : public Sprite
{
public:
	//NOTE: The animation is on a single row.
	AnimatedSprite(const char *szImageFile, const char *szMaskFile, const RECT& rcFirstFrame, int iFrameCount);
	virtual ~AnimatedSprite() { }

public:
	void SetFrame(int iIndex);
	int GetFrameCount() { return miFrameCount; }

	virtual void draw();
	
	int width() override { return mImageBM.bmWidth / miFramesPerRow; }
	int height() override { return mImageBM.bmHeight / miFramesPerRow; }

protected:
	POINT mptFrameStartCrop;// first point of the frame (upper-left corner)
	POINT mptFrameCrop;		// crop point of frame
	int miFrameWidth;		// width
	int miFrameHeight;		// height
	int miFrameCount;		// number of frames
	int miFramesPerRow;
};



#endif // SPRITE_H

