#pragma once

#include "Main.h"
#include "Sprite.h"
#include "CBoundingBox.inl"

class CAsteroid;

class CBullet {
public:
	CBullet();
	CBullet(const BackBuffer* pBackBuffer);
	CBullet(const BackBuffer* pBackBuffer, Vec2);
	virtual ~CBullet();

	virtual int GetType() { return 0; }

	void Draw();
	void Tick(float);
	void SetPosition(float, float);
	void SaveObject(FILE* file);
	void LoadObject(FILE* file);
	bool IsOutside();
	bool Intersects(Sprite*);

	Sprite* m_pSprite;
	Vec2 m_pSpeed;
};

class CEnemyBullet : public CBullet {
public:
	CEnemyBullet(const BackBuffer* pBackBuffer, Vec2);
	int GetType() override { return 1; }
};