#pragma once
#include "Main.h"
#include "Sprite.h"

class CSyringe
{
public:
	CSyringe();
	CSyringe(const BackBuffer* pBackBuffer);
	CSyringe(const BackBuffer* pBackBuffer, Vec2);
	virtual ~CSyringe();

	Vec2& Position();
	Vec2& Velocity();
	Vec2  Size();

	void Draw();
	void Tick(float);
	void SetPosition(float, float);
	/*	void SaveObject(FILE* file);
	void LoadObject(FILE* file);*/
	bool IsOutside();

	Sprite* m_pSprite;
	Vec2 m_pSpeed;
	int m_iFrame;
};