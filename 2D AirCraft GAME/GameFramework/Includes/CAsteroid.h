#pragma once
#include "Main.h"
#include "Sprite.h"

class CAsteroid {
public:
	CAsteroid();
	CAsteroid(const BackBuffer* pBackBuffer);
	CAsteroid(const BackBuffer* pBackBuffer, Vec2);
	virtual ~CAsteroid();

	Vec2& Position();
	Vec2& Velocity();
	Vec2  Size();

	void Draw();
	bool FrameAdvance();
	void Tick(float);
	void SetPosition(float, float);
	/*	void SaveObject(FILE* file);
	void LoadObject(FILE* file);*/
	bool IsOutside();

	AnimatedSprite* m_pSprite;
	Vec2 m_pSpeed;
	int m_iFrame;
};