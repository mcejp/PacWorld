// Thanks.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "resource.h"
#include "Thanks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CThanks 


CThanks::CThanks(CWnd* pParent /*=NULL*/)
	: CDialog(CThanks::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThanks)
	m_text = _T("");
	m_text1 = _T("");
	//}}AFX_DATA_INIT
}


void CThanks::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThanks)
	DDX_Text(pDX, IDC_TEXT, m_text);
	DDX_Text(pDX, IDC_TEXT2, m_text1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThanks, CDialog)
	//{{AFX_MSG_MAP(CThanks)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CThanks 

BOOL CThanks::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(correct)SetWindowText("Password correct");	
	else
		SetWindowText("Sorry, wrong password");	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}
