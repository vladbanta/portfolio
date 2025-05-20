//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//	   such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _CPLAYER_H_
#define _CPLAYER_H_

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "Sprite.h"
#include "CBullet.h"
#include "CSyringe.h"

//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CPlayer (Class)
// Desc : Player class handles all player manipulation, update and management.
//-----------------------------------------------------------------------------
class CPlayer
{
public:
	//-------------------------------------------------------------------------
	// Enumerators
	//-------------------------------------------------------------------------
	enum DIRECTION 
	{ 
		DIR_FORWARD	 = 1 << 0, 
		DIR_BACKWARD = 1 << 1, 
		DIR_LEFT	 = 1 << 2, 
		DIR_RIGHT	 = 1 << 3,
	};

	enum Rotation
	{
		ROT_UP = 1 << 0,
		ROT_RIGHT = 1 << 1,
		ROT_DOWN = 1 << 2,
		ROT_LEFT = 1 << 3,
	};

	enum ESpeedStates
	{
		SPEED_START,
		SPEED_STOP
	};

	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
			 CPlayer(const BackBuffer *pBackBuffer);
	virtual ~CPlayer();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void					Update( float dt );
	void					Draw();
	void					Move(ULONG ulDirection);
	void					Rotate(ULONG ulRotation);
	Vec2&					Position();
	Vec2&					Velocity();
	Vec2					Size();

	void					Explode();
	bool					AdvanceExplosion();
	CBullet*				CreateBullet(BackBuffer*);

	void					Init(const BackBuffer* pBackBuffer);
	void					Intersects(CPlayer*);
	bool					Intersects(Sprite*);
	void					SaveObject(FILE* file);
	void					LoadObject(FILE* file);
	bool					IsExploding();
	int						GetLives();
	void					AddLife();

private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	Sprite*					m_pSprite;
	HBITMAP					m_pImage[4];
	Sprite*					m_pLifeImages[3];
	ESpeedStates			m_eSpeedState;
	float					m_fTimer;
	
	bool					m_bExplosion;
	AnimatedSprite*			m_pExplosionSprite;
	int						m_iExplosionFrame;
	ULONG					m_uRot;
	int						m_iLives;
};

#endif // _CPLAYER_H_