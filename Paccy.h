// Paccy.h: Schnittstelle für die Klasse CPaccy.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACCY_H__4C36BF20_B7BB_11D3_A5E4_CF4B0D7B1038__INCLUDED_)
#define AFX_PACCY_H__4C36BF20_B7BB_11D3_A5E4_CF4B0D7B1038__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CDX.H>
#include "cdxhpc.h"


class CPaccy  
{
private:
	CDXHPC pc1;
	int leftPassage, rightPassage;
	CDXMap* Map;

public:
	int locX, locY;
	void Kill();
	int lives;
	bool alive;	
	bool chase;
	bool doped;
	bool hasKey;
	int tile;
	void SetPassages(int y1, int y2);
	void SetStartPos(int x, int y);
	int moving;
	bool up, down, left, right;
	bool keyRight, keyLeft, keyUp, keyDown;
	void SetMap(CDXMap *map);
	void MovePaccy();
	int x, y, v;
	CPaccy();
	virtual ~CPaccy();

};

#endif // !defined(AFX_PACCY_H__4C36BF20_B7BB_11D3_A5E4_CF4B0D7B1038__INCLUDED_)
