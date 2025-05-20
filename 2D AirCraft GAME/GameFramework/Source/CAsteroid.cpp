#include "CAsteroid.h"
#include "CGameApp.h"

extern CGameApp g_App;

CAsteroid::CAsteroid()
{

}

CAsteroid::~CAsteroid()
{
	delete m_pSprite;
}

CAsteroid::CAsteroid(const BackBuffer* pBackBuffer)
{
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 128;
	r.bottom = 128;

	m_pSprite = new AnimatedSprite("data/asteroid1.bmp", "data/asteroid1mask.bmp", r, 48);
	m_pSprite->setBackBuffer(pBackBuffer);
	m_pSprite->mPosition.x = 150;
	m_pSprite->mPosition.y = 100;
}

CAsteroid::CAsteroid(const BackBuffer* pBackBuffer, Vec2 speed)
{
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 128;
	r.bottom = 128;

	m_pSprite = new AnimatedSprite("data/asteroid1.bmp", "data/asteroid1mask.bmp", r, 48);
	m_pSprite->setBackBuffer(pBackBuffer);
	m_pSpeed = speed;
	m_pSprite->mPosition.x = 0;
	m_pSprite->mPosition.y = 0;
}

bool CAsteroid::FrameAdvance()
{
	m_pSprite->SetFrame(m_iFrame++);
	if (m_iFrame >= m_pSprite->GetFrameCount())
	{
		m_iFrame = 0;
		return false;
	}
	return true;
}

Vec2& CAsteroid::Position()
{
	return m_pSprite->mPosition;
}

Vec2& CAsteroid::Velocity()
{
	return m_pSprite->mVelocity;
}

Vec2 CAsteroid::Size()
{
	return Vec2(m_pSprite->width(), m_pSprite->height());
}

void CAsteroid::Draw()
{
	m_pSprite->draw();
}

void CAsteroid::Tick(float delta)
{
	m_pSprite->mPosition.x += m_pSpeed.x;
	m_pSprite->mPosition.y += m_pSpeed.y;
}

void CAsteroid::SetPosition(float x, float y)
{
	m_pSprite->mPosition.x = x;
	m_pSprite->mPosition.y = y;
}

bool CAsteroid::IsOutside()
{
	int width, height;
	g_App.GetWindowSize(width, height);
	Vec2 scale = m_pSprite->getScale();

	double pos = m_pSprite->mPosition.y + m_pSprite->height() * scale.y / 2;
	if (pos > (height + m_pSprite->width()))
		return true;

	return false;
}