// Highscore.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "resource.h"
#include "Highscore.h"
#include "helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CHighscore 


CHighscore::CHighscore(CWnd* pParent /*=NULL*/)
	: CDialog(CHighscore::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHighscore)
	highscoreText = _T("");
	scoreText = _T("");
	//}}AFX_DATA_INIT
}


void CHighscore::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHighscore)
	DDX_Control(pDX, IDOK, okButton);
	DDX_Control(pDX, IDC_SCORE, scoreStatic);
	DDX_Control(pDX, IDC_HIGHSCORE, highscoreStatic);
	DDX_Text(pDX, IDC_HIGHSCORE, highscoreText);
	DDX_Text(pDX, IDC_SCORE, scoreText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHighscore, CDialog)
	//{{AFX_MSG_MAP(CHighscore)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CHighscore 

BOOL CHighscore::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	font1.CreateFont(-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE,0,0,0,0,0,0, "Comic Sans MS");

	scoreStatic.SubclassDlgItem(IDC_SCORE, this);    
    scoreStatic.SetTextColor(RGB(0, 0, 255));
	scoreStatic.SetFont(&font1);

	highscoreStatic.SubclassDlgItem(IDC_HIGHSCORE, this);    
    highscoreStatic.SetTextColor(RGB(255, 0, 0));        
	highscoreStatic.SetFont(&font1);

	okButton.SetIcon(IDI_OK);
	okButton.SetFlat(FALSE);

	UpdateData(TRUE);

	if(score>highscore&&score>0)
	{
		highscore=score;

		highscoreText="New Highscore!";
	}
	else
		highscoreText="Highscore is: "+IntToStr(highscore);
	
	scoreText="Your score: "+IntToStr(score);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}
