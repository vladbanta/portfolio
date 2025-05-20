//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "CPlayer.h"
#include "CGameApp.h"
#include "CBoundingBox.inl"

extern CGameApp g_App;
extern HINSTANCE g_hInst;

//-----------------------------------------------------------------------------
// Name : CPlayer () (Constructor)
// Desc : CPlayer Class Constructor
//-----------------------------------------------------------------------------
CPlayer::CPlayer(const BackBuffer *pBackBuffer)
{
	//m_pSprite = new Sprite("data/planeimg.bmp", "data/planemask.bmp");
	m_pSprite = new Sprite("data/planeimgandmask.bmp", RGB(0xff, 0x00, 0xff));
	m_pSprite->setBackBuffer(pBackBuffer);
	m_uRot = CPlayer::ROT_UP;
	m_pImage[0] = (HBITMAP)LoadImage(g_hInst, "data/planeimgandmask.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	m_pImage[1] = (HBITMAP)LoadImage(g_hInst, "data/planeimgandmask90.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	m_pImage[2] = (HBITMAP)LoadImage(g_hInst, "data/planeimgandmask180.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	m_pImage[3] = (HBITMAP)LoadImage(g_hInst, "data/planeimgandmask270.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	m_eSpeedState = SPEED_STOP;
	m_fTimer = 0;
	m_iLives = 0;

	// Animation frame crop rectangle
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 128;
	r.bottom = 128;

	m_pExplosionSprite	= new AnimatedSprite("data/explosion.bmp", "data/explosionmask.bmp", r, 16);
	m_pExplosionSprite->setBackBuffer( pBackBuffer );
	m_bExplosion		= false;
	m_iExplosionFrame	= 0;
}

//-----------------------------------------------------------------------------
// Name : ~CPlayer () (Destructor)
// Desc : CPlayer Class Destructor
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	delete m_pSprite;
	delete m_pExplosionSprite;
}

void CPlayer::Update(float dt)
{

	// Update sprite
	m_pSprite->update(dt);


	// Get velocity
	double v = m_pSprite->mVelocity.Magnitude();

	// NOTE: for each async sound played Windows creates a thread for you
	// but only one, so you cannot play multiple sounds at once.
	// This creation/destruction of threads also leads to bad performance
	// so this method is not recommanded to be used in complex projects.

	// update internal time counter used in sound handling (not to overlap sounds)
	m_fTimer += dt;

	// A FSM is used for sound manager 
	switch(m_eSpeedState)
	{
	case SPEED_STOP:
		if(v > 35.0f)
		{
			m_eSpeedState = SPEED_START;
			PlaySound("data/jet-start.wav", NULL, SND_FILENAME | SND_ASYNC);
			m_fTimer = 0;
		}
		break;
	case SPEED_START:
		if(v < 25.0f)
		{
			m_eSpeedState = SPEED_STOP;
			PlaySound("data/jet-stop.wav", NULL, SND_FILENAME | SND_ASYNC);
			m_fTimer = 0;
		}
		else
			if(m_fTimer > 1.f)
			{
				PlaySound("data/jet-cabin.wav", NULL, SND_FILENAME | SND_ASYNC);
				m_fTimer = 0;
			}
		break;
	}

	// NOTE: For sound you also can use MIDI but it's Win32 API it is a bit hard
	// see msdn reference: http://msdn.microsoft.com/en-us/library/ms711640.aspx
	// In this case you can use a C++ wrapper for it. See the following article:
	// http://www.codeproject.com/KB/audio-video/midiwrapper.aspx (with code also)
}

void CPlayer::Init(const BackBuffer* pBackBuffer)
{
	m_iLives = 3;
	m_pLifeImages[0] = new Sprite("data/Heart.bmp", RGB(0xff, 0xff, 0xff));
	m_pLifeImages[0]->setBackBuffer(pBackBuffer);
	m_pLifeImages[0]->mPosition.x = 180;
	m_pLifeImages[0]->mPosition.y = 250;
	m_pLifeImages[0]->setScale(0.1f, 0.1f);

	m_pLifeImages[1] = new Sprite("data/Heart.bmp", RGB(0xff, 0xff, 0xff));
	m_pLifeImages[1]->setBackBuffer(pBackBuffer);
	m_pLifeImages[1]->mPosition.x = 180 + m_pLifeImages[0]->width() * m_pLifeImages[0]->getScale().x;
	m_pLifeImages[1]->mPosition.y = 250;
	m_pLifeImages[1]->setScale(0.1f, 0.1f);

	m_pLifeImages[2] = new Sprite("data/Heart.bmp", RGB(0xff, 0xff, 0xff));
	m_pLifeImages[2]->setBackBuffer(pBackBuffer);
	m_pLifeImages[2]->mPosition.x = m_pLifeImages[1]->mPosition.x + m_pLifeImages[0]->width() * m_pLifeImages[0]->getScale().x;
	m_pLifeImages[2]->mPosition.y = 250;
	m_pLifeImages[2]->setScale(0.1f, 0.1f);
}

void CPlayer::Draw()
{
	for (int i = 0; i < m_iLives; i++)
		m_pLifeImages[i]->draw();
	if(!m_bExplosion)
		m_pSprite->draw();
	else
		m_pExplosionSprite->draw();
}

void CPlayer::Move(ULONG ulDirection)
{
	int width, height;
	g_App.GetWindowSize(width, height);
	if( ulDirection & CPlayer::DIR_LEFT )
		m_pSprite->mVelocity.x -= .1;
	double pos = m_pSprite->mPosition.x - m_pSprite->width()/2;
	if (pos < 0)
		m_pSprite->mPosition.x += 0 - pos;

	if( ulDirection & CPlayer::DIR_RIGHT )
		m_pSprite->mVelocity.x += .1;
	pos = m_pSprite->mPosition.x + m_pSprite->width() / 2;
	if (pos > width)
		m_pSprite->mPosition.x += width - pos;

	if( ulDirection & CPlayer::DIR_FORWARD )
		m_pSprite->mVelocity.y -= .1;
	pos = m_pSprite->mPosition.y - m_pSprite->height() / 2;
	if (pos < 0)
		m_pSprite->mPosition.y += 0 - pos;

	if( ulDirection & CPlayer::DIR_BACKWARD )
		m_pSprite->mVelocity.y += .1;
	pos = m_pSprite->mPosition.y + m_pSprite->height() / 2;
	if (pos > height)
		m_pSprite->mPosition.y += height - pos;
}

void CPlayer::Intersects(CPlayer* player)
{
	Vec2 pPosition = player->Position();
	Vec2 pSize = player->Size();

	Vec2& position = m_pSprite->mPosition;
	Vec2& velocity = m_pSprite->mVelocity;

	CBoundingBox box(m_pSprite->mPosition, m_pSprite->mPosition + Vec2(m_pSprite->width(), m_pSprite->height()));
	CBoundingBox playerBox(pPosition, pPosition + pSize);

	Vec2 center = box.GetCenter();
	Vec2 pCenter = playerBox.GetCenter();
	
	if (box.Intersects(playerBox))
	{
		Vec2 c = pCenter - center;
		Vec2 norm = c.Normalize();
		position -= norm;
	}
}

bool CPlayer::Intersects(Sprite* sprite)
{
	Vec2 pPosition = sprite->mPosition;
	Vec2 pSize = Vec2(sprite->width(), sprite->height());

	Vec2& position = m_pSprite->mPosition;
	Vec2& velocity = m_pSprite->mVelocity;

	CBoundingBox box(m_pSprite->mPosition, m_pSprite->mPosition + Vec2(m_pSprite->width(), m_pSprite->height()));
	CBoundingBox asteroidBox(pPosition, pPosition + pSize);

	if (box.Intersects(asteroidBox))
		return true;
	return false;
}

void CPlayer::Rotate(ULONG ulRotation)
{
	if(ulRotation != 0)
		m_uRot = ulRotation;
	if (ulRotation & CPlayer::ROT_UP)
		m_pSprite->SetImage(m_pImage[0]);
	else if (ulRotation & CPlayer::ROT_RIGHT)
		m_pSprite->SetImage(m_pImage[1]);
	else if (ulRotation & CPlayer::ROT_DOWN)
		m_pSprite->SetImage(m_pImage[2]);
	else if (ulRotation & CPlayer::ROT_LEFT)
		m_pSprite->SetImage(m_pImage[3]);
}

Vec2& CPlayer::Position()
{
	return m_pSprite->mPosition;
}

Vec2& CPlayer::Velocity()
{
	return m_pSprite->mVelocity;
}

Vec2 CPlayer::Size()
{
	return Vec2(m_pSprite->width(), m_pSprite->height());
}

void CPlayer::Explode()
{
	m_iLives -= 1;
	m_pExplosionSprite->mPosition = m_pSprite->mPosition;
	m_pExplosionSprite->SetFrame(0);
	PlaySound("data/explosion.wav", NULL, SND_FILENAME | SND_ASYNC);
	m_bExplosion = true;
}

bool CPlayer::AdvanceExplosion()
{
	if(m_bExplosion)
	{
		m_pExplosionSprite->SetFrame(m_iExplosionFrame++);
		if(m_iExplosionFrame==m_pExplosionSprite->GetFrameCount())
		{
			m_bExplosion = false;
			m_iExplosionFrame = 0;
			m_pSprite->mVelocity = Vec2(0,0);
			m_eSpeedState = SPEED_STOP;
			return false;
		}
	}

	return true;
}

bool CPlayer::IsExploding()
{
	return m_bExplosion;
}

int CPlayer::GetLives()
{
	return m_iLives;
}

void CPlayer::AddLife()
{
	if(m_iLives < 3)
		m_iLives++;
}

CBullet* CPlayer::CreateBullet(BackBuffer* buffer)
{
	CBullet* bullet = NULL;
	float posx = 0;
	float posy = 0;
	if (m_uRot & CPlayer::ROT_UP)
	{
		bullet = new CBullet(buffer, Vec2(0.0f, -1.0f));
		posx = m_pSprite->mPosition.x + m_pSprite->width() / 2 + (round(bullet->m_pSprite->width() * 0.1f) / 2);
		posy = m_pSprite->mPosition.y;
	}
	else if (m_uRot & CPlayer::ROT_RIGHT)
	{
		bullet = new CBullet(buffer, Vec2(1.0f, 0.0f));
		posx = m_pSprite->mPosition.x + m_pSprite->width() - (round(bullet->m_pSprite->width() * 0.1f) / 2);
		posy = m_pSprite->mPosition.y + m_pSprite->height() / 2 + (round(bullet->m_pSprite->height() * 0.1f) / 2);
	}
	else if (m_uRot & CPlayer::ROT_DOWN)
	{
		bullet = new CBullet(buffer, Vec2(0.0f, 1.0f));
		posx = m_pSprite->mPosition.x + m_pSprite->width() / 2 + (round(bullet->m_pSprite->width() * 0.1f) / 1.3);
		posy = m_pSprite->mPosition.y + m_pSprite->height() - (round(bullet->m_pSprite->height() * 0.1f) / 2);
	}
	else
	{
		bullet = new CBullet(buffer, Vec2(-1.0f, 0.0f));
		posx = m_pSprite->mPosition.x;
		posy = m_pSprite->mPosition.y + m_pSprite->height() / 2 + (round(bullet->m_pSprite->height() * 0.1f) / 1.3);
	}
	bullet->m_pSprite->setScale(0.1f, 0.1f);
	
	bullet->SetPosition(posx, posy);
	return bullet;
}

void CPlayer::SaveObject(FILE* file)
{
	m_pSprite->SaveObject(file);
	fwrite(&m_bExplosion, sizeof(m_bExplosion), 1, file);
	fwrite(&m_iExplosionFrame, sizeof(m_iExplosionFrame), 1, file);
	fwrite(&m_eSpeedState, sizeof(m_eSpeedState), 1, file);
	fwrite(&m_uRot, sizeof(m_uRot), 1, file);
}

void CPlayer::LoadObject(FILE* file)
{
	m_pSprite->LoadObject(file);
	fread(&m_bExplosion, sizeof(m_bExplosion), 1, file);
	fread(&m_iExplosionFrame, sizeof(m_iExplosionFrame), 1, file);
	fread(&m_eSpeedState, sizeof(m_eSpeedState), 1, file);
	fread(&m_uRot, sizeof(m_uRot), 1, file);
	Rotate(m_uRot);
}