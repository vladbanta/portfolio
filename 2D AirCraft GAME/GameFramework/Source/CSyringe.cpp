#include "CSyringe.h"

CSyringe::CSyringe()
{

}

CSyringe::~CSyringe()
{
	delete m_pSprite;
}

CSyringe::CSyringe(const BackBuffer* pBackBuffer)
{
	m_pSprite = new Sprite("data/Syringe.bmp", RGB(0xff, 0xff, 0xff));
	m_pSprite->setBackBuffer(pBackBuffer);
}

CSyringe::CSyringe(const BackBuffer* pBackBuffer, Vec2 pos)
{
	m_pSprite = new Sprite("data/Syringe.bmp", RGB(0xff, 0xff, 0xff));
	m_pSprite->setBackBuffer(pBackBuffer);
	m_pSprite->mPosition = pos;
}

Vec2& CSyringe::Position()
{
	return m_pSprite->mPosition;
}

Vec2& CSyringe::Velocity()
{
	return m_pSprite->mVelocity;
}

Vec2 CSyringe::Size()
{
	return Vec2(m_pSprite->width(), m_pSprite->height());
}

void CSyringe::Draw()
{
	m_pSprite->draw();
}

void CSyringe::Tick(float delta)
{
	m_pSprite->mPosition.x += m_pSpeed.x;
	m_pSprite->mPosition.y += m_pSpeed.y;
}

void CSyringe::SetPosition(float x, float y)
{
	m_pSprite->mPosition.x = x;
	m_pSprite->mPosition.y = y;
}