// Tutorial.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "resource.h"
#include "Tutorial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CTutorial 


CTutorial::CTutorial(CWnd* pParent /*=NULL*/)
	: CDialog(CTutorial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTutorial)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}


void CTutorial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTutorial)
	DDX_Control(pDX, IDC_EDIT1, m_editStatic);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Text(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTutorial, CDialog)
	//{{AFX_MSG_MAP(CTutorial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CTutorial 

BOOL CTutorial::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_OK);    

	m_editStatic.SubclassDlgItem(IDC_EDIT1, this);    
	m_editStatic.SetBkColor(RGB(255,255,230));
	m_editStatic.SetTextColor(RGB(0,0,255));	
	
	m_edit=
		"PacWorld - how to play\r\n*************************************************************\r\n\r\n"		
		"Steer Paccy through the maze with the cursor keys. Eat all the dots and avoid the four ghosts. If you eat one of the four big items in the corners of each level, the roles are reversed for a few seconds, that means "
		"Paccy can chase the ghosts. If he catches one, the ghost loses one of three lives and returns after a little while. If Paccy gets hit by a ghost, he loses one of three lives too. You can see how many lives Paccy and the ghost have on the status bar "
		"at the top of the main window. If Paccy lost all 3 lives, he has to start again in the first level!\r\nIn every level there is a closed up little room, for which you must first get the key to get in. If the door of the closet opens, you can see a diamond, that Paccy should also pick up. "
		"From time to time a fruit appears inside the maze (3 at most in each level). If Paccy manages to get it before it disappears again, he increases speed and can avoid the ghosts better for a little while. He then runs at the same speed "
		"as at the times when he is invincible by having eaten one of the four big items. Otherwise he is a bit slower than the ghosts, so he has to be careful! When Paccy is invincible or has eaten one of the fruits, a little blue gauge tells you when "
		"the magic item will lose its power (about 8 seconds). In every level there are ways that lead out of the labyrinth at the left and the right side of the labyrinth. By using them, Paccy can quickly move to the other side of the maze.\r\n\r\n"

		"The number of points Paccy gets depend on the number of lives (hearts) he has got:\r\n\r\n"

		"3 Lives: dot-3, ghost-75, fruit-45, diamond-200\r\n"
		"2 Lives: dot-2, ghost-50, fruit-30, diamond-100\r\n"
		"1 Life:   dot-1, ghost-25, fruit-15, diamond-50\r\n\r\n"		
		
		"When Paccy lost all his lives or when you finished the last level, a highscore window pops up, where you can see how you scored.\r\n"				
		"*************************************************************";			
	
	
	UpdateData(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}
