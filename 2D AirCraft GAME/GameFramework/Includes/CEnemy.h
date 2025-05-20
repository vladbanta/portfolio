#pragma once
#include "Main.h"
#include "Sprite.h"

class CBullet;

class CEnemy
{
public:
	CEnemy();
	CEnemy(const BackBuffer* pBackBuffer);
	CEnemy(const BackBuffer* pBackBuffer, Vec2, Vec2);
	virtual ~CEnemy();

	void Draw();
	void Tick(float);
	void SetPosition(float, float);
//	void SaveObject(FILE* file);
//	void LoadObject(FILE* file);
	CBullet* CreateBullet(BackBuffer*);

	Sprite* m_pSprite;
	Vec2 m_pSpeed;
};

