// GameApp.cpp: Implementierung der Klasse CGameApp.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "GameApp.h"
#include "GameWindow.h"

//used to make OnIdle-function slower
const int DELAY=5;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

GameApp::GameApp()
{
	gw=0;
}

GameApp::~GameApp()
{

}

BOOL GameApp::InitInstance()
{
CBrush bkbrush;
bkbrush.CreateStockObject(WHITE_BRUSH);

LPCSTR cname= AfxRegisterWndClass(0,LoadStandardCursor(IDC_ARROW),bkbrush,LoadIcon(IDI_MAIN_ICON));
m_pMainWnd = new GameWindow(cname);
m_pMainWnd->ShowWindow(m_nCmdShow);
m_pMainWnd->UpdateWindow();

return TRUE;
}

GameApp GmApp;

BOOL GameApp::OnIdle(LONG lCount)
{

	gw->MoveGame();
	
	return TRUE;
}
