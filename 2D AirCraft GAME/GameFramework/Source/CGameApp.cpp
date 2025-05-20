//-----------------------------------------------------------------------------
// File: CGameApp.cpp
//
// Desc: Game Application class, this is the central hub for all app processing
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CGameApp Specific Includes
//-----------------------------------------------------------------------------
#include "CGameApp.h"

extern HINSTANCE g_hInst;

//-----------------------------------------------------------------------------
// CGameApp Member Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CGameApp () (Constructor)
// Desc : CGameApp Class Constructor
//-----------------------------------------------------------------------------
CGameApp::CGameApp()
{
	// Reset / Clear all required values
	m_hWnd			= NULL;
	m_hIcon			= NULL;
	m_hMenu			= NULL;
	m_pBBuffer		= NULL;
	m_pPlayer		= NULL;
	m_pPlane		= NULL;
	m_LastFrameRate = 0;
	m_iScore		= 0;
	m_iLastScore	= 0;
	m_fBackgroundOffset = 0.0f;
}

//-----------------------------------------------------------------------------
// Name : ~CGameApp () (Destructor)
// Desc : CGameApp Class Destructor
//-----------------------------------------------------------------------------
CGameApp::~CGameApp()
{
	// Shut the engine down
	ShutDown();
}

//-----------------------------------------------------------------------------
// Name : InitInstance ()
// Desc : Initialises the entire Engine here.
//-----------------------------------------------------------------------------
bool CGameApp::InitInstance( LPCTSTR lpCmdLine, int iCmdShow )
{
	// Create the primary display device
	if (!CreateDisplay()) { ShutDown(); return false; }

	// Build Objects
	if (!BuildObjects()) 
	{ 
		MessageBox( 0, _T("Failed to initialize properly. Reinstalling the application may solve this problem.\nIf the problem persists, please contact technical support."), _T("Fatal Error"), MB_OK | MB_ICONSTOP);
		ShutDown(); 
		return false; 
	}

	// Set up all required game states
	SetupGameState();

	// Success!
	return true;
}

//-----------------------------------------------------------------------------
// Name : CreateDisplay ()
// Desc : Create the display windows, devices etc, ready for rendering.
//-----------------------------------------------------------------------------
bool CGameApp::CreateDisplay()
{
	LPTSTR			WindowTitle		= _T("GameFramework");
	LPCSTR			WindowClass		= _T("GameFramework_Class");
	USHORT			Width			= 800;
	USHORT			Height			= 600;
	RECT			rc;
	WNDCLASSEX		wcex;


	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= CGameApp::StaticWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= g_hInst;
	wcex.hIcon			= LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= WindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));

	if(RegisterClassEx(&wcex)==0)
		return false;

	// Retrieve the final client size of the window
	::GetClientRect( m_hWnd, &rc );
	m_nViewX		= rc.left;
	m_nViewY		= rc.top;
	m_nViewWidth	= rc.right - rc.left;
	m_nViewHeight	= rc.bottom - rc.top;

	m_hWnd = CreateWindow(WindowClass, WindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, NULL, NULL, g_hInst, this);

	if (!m_hWnd)
		return false;

	// Show the window
	ShowWindow(m_hWnd, SW_SHOW);

	// Success!!
	return true;
}

//-----------------------------------------------------------------------------
// Name : BeginGame ()
// Desc : Signals the beginning of the physical post-initialisation stage.
//		From here on, the game engine has control over processing.
//-----------------------------------------------------------------------------
int CGameApp::BeginGame()
{
	MSG		msg;

	// Start main loop
	while(true) 
	{
		// Did we recieve a message, or are we idling ?
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		} 
		else 
		{
			// Advance Game Frame.
			FrameAdvance();

		} // End If messages waiting
	
	} // Until quit message is receieved

	return 0;
}

//-----------------------------------------------------------------------------
// Name : ShutDown ()
// Desc : Shuts down the game engine, and frees up all resources.
//-----------------------------------------------------------------------------
bool CGameApp::ShutDown()
{
	// Release any previously built objects
	ReleaseObjects ( );
	
	// Destroy menu, it may not be attached
	if ( m_hMenu ) DestroyMenu( m_hMenu );
	m_hMenu		 = NULL;

	// Destroy the render window
	SetMenu( m_hWnd, NULL );
	if ( m_hWnd ) DestroyWindow( m_hWnd );
	m_hWnd		  = NULL;
	
	// Shutdown Success
	return true;
}

void CGameApp::GetWindowSize(int& width, int& height)
{
	RECT rect;
	if (GetWindowRect(m_hWnd, &rect))
	{
		width = rect.right - rect.left - 16;
		height = rect.bottom - rect.top - 36;
	}
}

//-----------------------------------------------------------------------------
// Name : StaticWndProc () (Static Callback)
// Desc : This is the main messge pump for ALL display devices, it captures
//		the appropriate messages, and routes them through to the application
//		class for which it was intended, therefore giving full class access.
// Note : It is VITALLY important that you should pass your 'this' pointer to
//		the lpParam parameter of the CreateWindow function if you wish to be
//		able to pass messages back to that app object.
//-----------------------------------------------------------------------------
LRESULT CALLBACK CGameApp::StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// If this is a create message, trap the 'this' pointer passed in and store it within the window.
	if ( Message == WM_CREATE ) SetWindowLong( hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);

	// Obtain the correct destination for this message
	CGameApp *Destination = (CGameApp*)GetWindowLong( hWnd, GWL_USERDATA );
	
	// If the hWnd has a related class, pass it through
	if (Destination) return Destination->DisplayWndProc( hWnd, Message, wParam, lParam );
	
	// No destination found, defer to system...
	return DefWindowProc( hWnd, Message, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name : DisplayWndProc ()
// Desc : The display devices internal WndProc function. All messages being
//		passed to this function are relative to the window it owns.
//-----------------------------------------------------------------------------
LRESULT CGameApp::DisplayWndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
	static UINT			fTimer;
	static UINT			fTimer2;

	// Determine message type
	switch (Message)
	{
		case WM_CREATE:
			break;
		
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		
		case WM_SIZE:
			if ( wParam == SIZE_MINIMIZED )
			{
				// App is inactive
				m_bActive = false;
			
			} // App has been minimized
			else
			{
				// App is active
				m_bActive = true;

				// Store new viewport sizes
				m_nViewWidth  = LOWORD( lParam );
				m_nViewHeight = HIWORD( lParam );
		
			
			} // End if !Minimized

			break;

		case WM_LBUTTONDOWN:
			// Capture the mouse
			SetCapture( m_hWnd );
			GetCursorPos( &m_OldCursorPos );
			break;

		case WM_LBUTTONUP:
			// Release the mouse
			ReleaseCapture( );
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
//			case VK_RETURN:
//				fTimer = SetTimer(m_hWnd, 1, 250, NULL);
//				m_pPlayer->Explode();
//				break;
			case 0x51:
				fTimer2 = SetTimer(m_hWnd, 2, 250, NULL);
				m_pPlane->Explode();
				break;
			case VK_SPACE:
			{
				if (m_pPlayer->IsExploding()) break;
				CBullet* bullet = m_pPlayer->CreateBullet(m_pBBuffer);
				m_bullets.push_back(bullet);
				break;
			}
			case VK_F4:
				Save();
				break;
			case VK_F5:
				Load();
				break;
			case VK_RETURN:
				SetTimer(m_hWnd, 4, 2500, NULL);
				break;
			}

		case WM_TIMER:
			switch (wParam)
			{
			case 1:
				if (!m_pPlayer->AdvanceExplosion())
					KillTimer(m_hWnd, 1);
				break;
			case 2:
				if (!m_pPlane->AdvanceExplosion())
					KillTimer(m_hWnd, 2);
				break;
			case 3:
				for (CAsteroid* asteroid : m_pAsteroids)
					asteroid->FrameAdvance();
				break;
			case 4:
			{
				CAsteroid* m_asteroid = new CAsteroid(m_pBBuffer, Vec2(0.0f, FRand(0.1f, 1.0f)));
				m_asteroid->SetPosition(FRand(0, 860), -100);
				m_pAsteroids.push_back(m_asteroid);
				if (rand() % 100 < 51)
				{
					CSyringe* m_syringe = new CSyringe(m_pBBuffer, Vec2(FRand(0, 860), FRand(0, 860)));
					m_pSyringes.push_back(m_syringe);
				}
				if (m_pEnemies.size() < 2)
				{
					float pos = -100.0f;
					if (rand() % 100 < 51) pos = 960.0f;
					CEnemy* m_enemy = new CEnemy(m_pBBuffer, Vec2(pos, 100.0f), Vec2(FRand(0.1, 1.0), 0.0f));
					m_pEnemies.push_back(m_enemy);
				}
				break;
			}
			}
			break;

		case WM_COMMAND:
			break;

		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);

	} // End Message Switch
	
	return 0;
}

//-----------------------------------------------------------------------------
// Name : BuildObjects ()
// Desc : Build our demonstration meshes, and the objects that instance them
//-----------------------------------------------------------------------------
bool CGameApp::BuildObjects()
{
	m_pBBuffer = new BackBuffer(m_hWnd, m_nViewWidth, m_nViewHeight);
	m_pPlayer = new CPlayer(m_pBBuffer);
	m_pPlayer->Init(m_pBBuffer);
	m_pPlane = new CPlayer(m_pBBuffer);
	CAsteroid* m_asteroid = new CAsteroid(m_pBBuffer);
	m_pAsteroids.push_back(m_asteroid);
	CSyringe* m_syringe = new CSyringe(m_pBBuffer);
	m_syringe->SetPosition(700, 300);
	m_pSyringes.push_back(m_syringe);
	CEnemy* m_enemy = new CEnemy(m_pBBuffer, { 700, 100 }, {1.0f, 0.1f});
	m_pEnemies.push_back(m_enemy);

	if(!m_imgBackground.LoadBitmapFromFile("data/BackgroundBig.bmp", GetDC(m_hWnd)))
		return false;

	// Success!
	return true;
}

//-----------------------------------------------------------------------------
// Name : SetupGameState ()
// Desc : Sets up all the initial states required by the game.
//-----------------------------------------------------------------------------
void CGameApp::SetupGameState()
{
	m_pPlayer->Position() = Vec2(100, 400);
	m_pPlane->Position() = Vec2(400, 250);
	SetTimer(m_hWnd, 3, 60, NULL);
}

//-----------------------------------------------------------------------------
// Name : ReleaseObjects ()
// Desc : Releases our objects and their associated memory so that we can
//		rebuild them, if required, during our applications life-time.
//-----------------------------------------------------------------------------
void CGameApp::ReleaseObjects( )
{
	if(m_pPlayer != NULL)
	{
		delete m_pPlayer;
		m_pPlayer = NULL;
	}

	if (m_pPlane != NULL)
	{
		delete m_pPlane;
		m_pPlane = NULL;
	}

	if(m_pBBuffer != NULL)
	{
		delete m_pBBuffer;
		m_pBBuffer = NULL;
	}
}

//-----------------------------------------------------------------------------
// Name : FrameAdvance () (Private)
// Desc : Called to signal that we are now rendering the next frame.
//-----------------------------------------------------------------------------
void CGameApp::FrameAdvance()
{
	static TCHAR FrameRate[ 50 ];
	static TCHAR TitleBuffer[ 255 ];

	// Advance the timer
	m_Timer.Tick(0.0f);

	// Skip if app is inactive
	if ( !m_bActive ) return;
	
	// Get / Display the framerate
	if ( m_LastFrameRate != m_Timer.GetFrameRate() || m_iLastScore != m_iScore)
	{
		m_LastFrameRate = m_Timer.GetFrameRate( FrameRate, 50 );
		m_iLastScore = m_iScore;
		sprintf_s( TitleBuffer, _T("Game : %s | Score : %i"), FrameRate , m_iScore);
		SetWindowText( m_hWnd, TitleBuffer );
	} // End if Frame Rate Altered

	for (auto it = m_bullets.begin(); it != m_bullets.end(); )
	{
		CBullet& bullet = **it;
		bullet.Tick(0.0f);
		bool collided = false;
		if (bullet.GetType() == 0)
		{
			for (auto jt = m_pAsteroids.begin(); jt != m_pAsteroids.end(); )
			{
				if (bullet.Intersects((*jt)->m_pSprite))
				{
					delete* jt;
					jt = m_pAsteroids.erase(jt);
					m_iScore += 25;
					collided = true;
					break;
				}
				else
					++jt;
			}
			for (auto jt = m_pEnemies.begin(); jt != m_pEnemies.end(); )
			{
				if (bullet.Intersects((*jt)->m_pSprite))
				{
					delete* jt;
					jt = m_pEnemies.erase(jt);
					m_iScore += 100;
					collided = true;
					break;
				}
				else
					++jt;
			}
		}
		else
		{
			if (m_pPlayer->Intersects(bullet.m_pSprite) && !m_pPlayer->IsExploding())
			{
				SetTimer(m_hWnd, 1, 250, NULL);
				m_pPlayer->Explode();
				m_pPlayer->Position() = Vec2(100, 400);
				m_pPlayer->Velocity() = Vec2(0, 0);
				collided = true;
			}
		}
		if (bullet.IsOutside() || collided)
		{
			delete *it;
			it = m_bullets.erase(it);
		}
		else
			++it;
	}

	if (!m_pPlayer->IsExploding() && m_pPlayer->GetLives() < 1)
		ShutDown();

	for (auto it = m_pEnemies.begin(); it != m_pEnemies.end(); )
	{
		CEnemy& enemy = **it;
		enemy.Tick(0.0f);
		if (rand() % 250 < 1)
		{
			CBullet* bullet = enemy.CreateBullet(m_pBBuffer);
			m_bullets.push_back(bullet);
		}
		++it;
	}

	for (auto it = m_pAsteroids.begin(); it != m_pAsteroids.end(); )
	{
		CAsteroid& asteroid = **it;
		asteroid.Tick(0.0f);
		if (asteroid.IsOutside())
		{
			delete* it;
			it = m_pAsteroids.erase(it);
		}
		else
		{
			if (m_pPlayer->Intersects(asteroid.m_pSprite) && !m_pPlayer->IsExploding())
			{
				SetTimer(m_hWnd, 1, 250, NULL);
				m_pPlayer->Explode();
				m_pPlayer->Position() = Vec2(100, 400);
				m_pPlayer->Velocity() = Vec2(0, 0);
			}
			++it;
		}
	}
	
	for (auto it = m_pSyringes.begin(); it != m_pSyringes.end(); )
	{
		CSyringe& syringe = **it;
//		syringe.Tick(0.0f);
		if (m_pPlayer->Intersects(syringe.m_pSprite) && !m_pPlayer->IsExploding())
		{
			m_pPlayer->AddLife();
			delete* it;
			it = m_pSyringes.erase(it);
		}
		else
			++it;
	}

	// Poll & Process input devices
	ProcessInput();

	// Animate the game objects
	AnimateObjects();

	// Drawing the game objects
	DrawObjects();
}

//-----------------------------------------------------------------------------
// Name : ProcessInput () (Private)
// Desc : Simply polls the input devices and performs basic input operations
//-----------------------------------------------------------------------------
void CGameApp::ProcessInput( )
{
	static UCHAR pKeyBuffer[ 256 ];
	ULONG		Direction = 0;
	ULONG		Rotation = 0;
	POINT		CursorPos;
	float		X = 0.0f, Y = 0.0f;

	// Retrieve keyboard state
	if ( !GetKeyboardState( pKeyBuffer ) ) return;

	// Check the relevant keys
	if ( pKeyBuffer[ VK_UP	] & 0xF0 ) Direction |= CPlayer::DIR_FORWARD;
	if ( pKeyBuffer[ VK_DOWN  ] & 0xF0 ) Direction |= CPlayer::DIR_BACKWARD;
	if ( pKeyBuffer[ VK_LEFT  ] & 0xF0 ) Direction |= CPlayer::DIR_LEFT;
	if ( pKeyBuffer[ VK_RIGHT ] & 0xF0 ) Direction |= CPlayer::DIR_RIGHT;
	if ( pKeyBuffer[0x41] & 0xF0 ) Rotation |= CPlayer::ROT_LEFT;
	if ( pKeyBuffer[0x44] & 0xF0 ) Rotation |= CPlayer::ROT_RIGHT;
	if ( pKeyBuffer[0x53] & 0xF0 ) Rotation |= CPlayer::ROT_DOWN;
	if ( pKeyBuffer[0x57] & 0xF0 ) Rotation |= CPlayer::ROT_UP;
	
	
	// Move the player
	m_pPlayer->Move(Direction);
	m_pPlayer->Rotate(Rotation);
	m_pPlayer->Intersects(m_pPlane);

	// Now process the mouse (if the button is pressed)
	if ( GetCapture() == m_hWnd )
	{
		// Hide the mouse pointer
		SetCursor( NULL );

		// Retrieve the cursor position
		GetCursorPos( &CursorPos );

		// Reset our cursor position so we can keep going forever :)
		SetCursorPos( m_OldCursorPos.x, m_OldCursorPos.y );

	} // End if Captured
}

//-----------------------------------------------------------------------------
// Name : AnimateObjects () (Private)
// Desc : Animates the objects we currently have loaded.
//-----------------------------------------------------------------------------
void CGameApp::AnimateObjects()
{
	m_pPlayer->Update(m_Timer.GetTimeElapsed());
	m_pPlane->Update(m_Timer.GetTimeElapsed());
}

//-----------------------------------------------------------------------------
// Name : DrawObjects () (Private)
// Desc : Draws the game objects
//-----------------------------------------------------------------------------
void CGameApp::DrawObjects()
{
	m_pBBuffer->reset();

	if (m_fBackgroundOffset <= -1154.0f) m_fBackgroundOffset = 0.2f;
	m_imgBackground.Paint(m_pBBuffer->getDC(), 0, m_fBackgroundOffset -= 0.2f);

	m_pPlayer->Draw();

	m_pPlane->Draw();

	for (CBullet* bullet : m_bullets)
		bullet->Draw();

	for (CSyringe* syringe : m_pSyringes)
		syringe->Draw();

	for (CAsteroid* asteroid : m_pAsteroids)
		asteroid->Draw();

	for (CEnemy* enemy : m_pEnemies)
		enemy->Draw();

	m_pBBuffer->present();
}

void CGameApp::Save()
{
	FILE* file = fopen("save.sav", "wb");
	m_pPlayer->SaveObject(file);
	m_pPlane->SaveObject(file);
	size_t count = m_bullets.size();
	fwrite(&count, sizeof(count), 1, file);
	for (CBullet* bullet : m_bullets)
		bullet->SaveObject(file);
	fclose(file);
}

void CGameApp::Load()
{
	FILE* file = fopen("save.sav", "r");
	m_pPlayer->LoadObject(file);
	if(m_pPlayer->IsExploding())
		SetTimer(m_hWnd, 1, 250, NULL);
	m_pPlane->LoadObject(file);
	if (m_pPlane->IsExploding())
		SetTimer(m_hWnd, 2, 250, NULL);
	for (auto it = m_bullets.begin(); it != m_bullets.end(); )
	{
		delete* it;
		it = m_bullets.erase(it);
	}
	m_bullets.clear();
	size_t count = 0;
	fread(&count, sizeof(count), 1, file);
	for (int i = 0; i < count; i++)
	{
		CBullet* bullet = new CBullet(m_pBBuffer);
		bullet->LoadObject(file);
		m_bullets.push_back(bullet);
	}
	fclose(file);
}