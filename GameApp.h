// GameApp.h: Schnittstelle für die Klasse CGameApp.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEAPP_H__905071B7_90E4_11D2_91E6_94190CC10E27__INCLUDED_)
#define AFX_GAMEAPP_H__905071B7_90E4_11D2_91E6_94190CC10E27__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class GameApp : public CWinApp  
{
public:
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL InitInstance();
	GameApp();
	virtual ~GameApp();

private:
	friend class GameWindow;
	GameWindow* gw;	
};

#endif // !defined(AFX_GAMEAPP_H__905071B7_90E4_11D2_91E6_94190CC10E27__INCLUDED_)
