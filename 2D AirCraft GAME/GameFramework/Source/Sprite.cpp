#include "Sprite.h"
#include <cmath>

extern HINSTANCE g_hInst;

Sprite::Sprite(int imageID, int maskID)
{
	// Load the bitmap resources.
	mhImage = LoadBitmap(g_hInst, MAKEINTRESOURCE(imageID));
	mhMask = LoadBitmap(g_hInst, MAKEINTRESOURCE(maskID));

	mfScaleX = 1.0f;
	mfScaleY = 1.0f;
	// Get the BITMAP structure for each of the bitmaps.
	GetObject(mhImage, sizeof(BITMAP), &mImageBM);
	GetObject(mhMask, sizeof(BITMAP), &mMaskBM);

	// Image and Mask should be the same dimensions.
	assert(mImageBM.bmWidth == mMaskBM.bmWidth);
	assert(mImageBM.bmHeight == mMaskBM.bmHeight);	

	mcTransparentColor = 0;
	mhSpriteDC = 0;
}

Sprite::Sprite(const char *szImageFile, const char *szMaskFile)
{
	mhImage = (HBITMAP)LoadImage(g_hInst, szImageFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	mhMask = (HBITMAP)LoadImage(g_hInst, szMaskFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	mfScaleX = 1.0f;
	mfScaleY = 1.0f;
	// Get the BITMAP structure for each of the bitmaps.
	GetObject(mhImage, sizeof(BITMAP), &mImageBM);
	GetObject(mhMask, sizeof(BITMAP), &mMaskBM);

	// Image and Mask should be the same dimensions.
	assert(mImageBM.bmWidth == mMaskBM.bmWidth);
	assert(mImageBM.bmHeight == mMaskBM.bmHeight);

	mcTransparentColor = 0;
	mhSpriteDC = 0;
}

Sprite::Sprite(const char *szImageFile, COLORREF crTransparentColor)
{
	mhImage = (HBITMAP)LoadImage(g_hInst, szImageFile, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	mfScaleX = 1.0f;
	mfScaleY = 1.0f;

	mhMask = 0;
	mhSpriteDC = 0;
	mcTransparentColor = crTransparentColor;

	// Get the BITMAP structure for the bitmap.
	GetObject(mhImage, sizeof(BITMAP), &mImageBM);
}

Sprite::~Sprite()
{
	// Free the resources we created in the constructor.
	DeleteObject(mhImage);
	DeleteObject(mhMask);

	DeleteDC(mhSpriteDC);
}

void Sprite::update(float dt)
{
	// Update the sprites position.

	mPosition += mVelocity * dt;
	// Update bounding rectangle/circle
}

void Sprite::setBackBuffer(const BackBuffer *pBackBuffer)
{
	mpBackBuffer = pBackBuffer;
	if(mpBackBuffer)
	{
		DeleteDC(mhSpriteDC);
		mhSpriteDC = CreateCompatibleDC(mpBackBuffer->getDC());
	}
}

void Sprite::setScale(float scaleX, float scaleY)
{
	mfScaleX = scaleX;
	mfScaleY = scaleY;
}

Vec2 Sprite::getScale()
{
	return Vec2(mfScaleX, mfScaleY);
}

void Sprite::draw()
{
	if( mhMask != 0 )
		drawMask();
	else
		drawTransparent();
}

void Sprite::drawMask()
{
	if( mpBackBuffer == NULL )
		return;

	HDC hBackBufferDC = mpBackBuffer->getDC();

	// The position BitBlt wants is not the sprite's center
	// position; rather, it wants the upper-left position,
	// so compute that.
	int fW = width();
	int fH = height();

	// Upper-left corner.
	int x = (int)mPosition.x - (fW / 2);
	int y = (int)mPosition.y - (fH / 2);

	// Note: For this masking technique to work, it is assumed
	// the backbuffer bitmap has been cleared to some
	// non-zero value.
	// Select the mask bitmap.
	HGDIOBJ oldObj = SelectObject(mhSpriteDC, mhMask);

	// Draw the mask to the backbuffer with SRCAND. This
	// only draws the black pixels in the mask to the backbuffer,
	// thereby marking the pixels we want to draw the sprite
	// image onto.
	BitBlt(hBackBufferDC, x, y, fW, fH, mhSpriteDC, 0, 0, SRCAND);

	// Now select the image bitmap.
	SelectObject(mhSpriteDC, mhImage);

	// Draw the image to the backbuffer with SRCPAINT. This
	// will only draw the image onto the pixels that where previously
	// marked black by the mask.
	BitBlt(hBackBufferDC, x, y, fW, fH, mhSpriteDC, 0, 0, SRCPAINT);

	// Restore the original bitmap object.
	SelectObject(mhSpriteDC, oldObj);
}

void Sprite::drawTransparent()
{
	if( mpBackBuffer == NULL )
		return;

	HDC hBackBuffer = mpBackBuffer->getDC();

	int w = width();
	int h = height();

	float sx = mfScaleX;
	float sy = mfScaleY;

	// Upper-left corner.
	int x = (int)mPosition.x - (w / 2);
	int y = (int)mPosition.y - (h / 2);

	COLORREF crOldBack = SetBkColor(hBackBuffer, RGB(255, 255, 255));
	COLORREF crOldText = SetTextColor(hBackBuffer, RGB(0, 0, 0));
	HDC dcImage, dcTrans;

	// Create two memory dcs for the image and the mask
	dcImage=CreateCompatibleDC(hBackBuffer);
	dcTrans=CreateCompatibleDC(hBackBuffer);

	// Select the image into the appropriate dc
	SelectObject(dcImage, mhImage);

	// Create the mask bitmap
	BITMAP bitmap;
	GetObject(mhImage, sizeof(BITMAP), &bitmap);
	HBITMAP bitmapTrans = CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, 1, NULL);

	// Select the mask bitmap into the appropriate dc
	SelectObject(dcTrans, bitmapTrans);

	float bltsx = round(bitmap.bmWidth * sx);
	float bltsy = round(bitmap.bmHeight * sy);

	// Build mask based on transparent color
	SetBkColor(dcImage, mcTransparentColor);
	BitBlt(dcTrans, 0, 0, bitmap.bmWidth, bitmap.bmHeight, dcImage, 0, 0, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display

	StretchBlt(hBackBuffer, x, y, bltsx, bltsy, dcImage, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCINVERT);
	StretchBlt(hBackBuffer, x, y, bltsx, bltsy, dcTrans, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCAND);
	StretchBlt(hBackBuffer, x, y, bltsx, bltsy, dcImage, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCINVERT);

	// free memory	
	DeleteDC(dcImage);
	DeleteDC(dcTrans);
	DeleteObject(bitmapTrans);

	// Restore settings
	SetBkColor(hBackBuffer, crOldBack);
	SetTextColor(hBackBuffer, crOldText);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

AnimatedSprite::AnimatedSprite(const char *szImageFile, const char *szMaskFile, const RECT& rcFirstFrame, int iFrameCount) 
			: Sprite (szImageFile, szMaskFile)
{
	mptFrameCrop.x = rcFirstFrame.left;
	mptFrameCrop.y = rcFirstFrame.top;
	mptFrameStartCrop = mptFrameCrop;
	miFrameWidth = rcFirstFrame.right - rcFirstFrame.left;
	miFrameHeight = rcFirstFrame.bottom - rcFirstFrame.top;
	miFrameCount = iFrameCount;
	miFramesPerRow = ceil(sqrt(miFrameCount));
}

void AnimatedSprite::SetFrame(int iIndex)
{
	// index must be in range
	assert(iIndex >= 0 && iIndex < miFrameCount && "AnimatedSprite frame Index must be in range!");

	mptFrameCrop.x = mptFrameStartCrop.x + iIndex%miFramesPerRow*miFrameWidth;
	mptFrameCrop.y = mptFrameStartCrop.y + iIndex/miFramesPerRow*miFrameHeight;
}

void AnimatedSprite::draw()
{
	if( mpBackBuffer == NULL )
		return;

	// The position BitBlt wants is not the sprite's center
	// position; rather, it wants the upper-left position,
	// so compute that.
	int fW = miFrameWidth;
	int fH = miFrameHeight;

	HDC hBackBufferDC = mpBackBuffer->getDC();

	// Upper-left corner.
	int x = (int)mPosition.x - (fW / 2);
	int y = (int)mPosition.y - (fH / 2);

	// Note: For this masking technique to work, it is assumed
	// the backbuffer bitmap has been cleared to some
	// non-zero value.
	// Select the mask bitmap.
	HGDIOBJ oldObj = SelectObject(mhSpriteDC, mhMask);

	// Draw the mask to the backbuffer with SRCAND. This
	// only draws the black pixels in the mask to the backbuffer,
	// thereby marking the pixels we want to draw the sprite
	// image onto.
	BitBlt(hBackBufferDC, x, y, fW, fH, mhSpriteDC, mptFrameCrop.x, mptFrameCrop.y, SRCAND);

	// Now select the image bitmap.
	SelectObject(mhSpriteDC, mhImage);

	// Draw the image to the backbuffer with SRCPAINT. This
	// will only draw the image onto the pixels that where previously
	// marked black by the mask.
	BitBlt(hBackBufferDC, x, y, fW, fH, mhSpriteDC, mptFrameCrop.x, mptFrameCrop.y, SRCPAINT);

	// Restore the original bitmap object.
	SelectObject(mhSpriteDC, oldObj);
}

void Sprite::SetImage(HBITMAP& image)
{
	mhImage = image;
	GetObject(mhImage, sizeof(BITMAP), &mImageBM);
}

void Sprite::SaveObject(FILE* file)
{
	fwrite(&mPosition, sizeof(double), 2, file);
	fwrite(&mVelocity, sizeof(double), 2, file);
	fwrite(&mfScaleX, sizeof(mfScaleX), 1, file);
	fwrite(&mfScaleY, sizeof(mfScaleY), 1, file);
}

void Sprite::LoadObject(FILE* file)
{
	fread(&mPosition, sizeof(double), 2, file);
	fread(&mVelocity, sizeof(double), 2, file);
	fread(&mfScaleX, sizeof(mfScaleX), 1, file);
	fread(&mfScaleY, sizeof(mfScaleY), 1, file);
}