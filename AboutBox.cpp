// AboutBox.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "resource.h"
#include "AboutBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld AboutBox 


AboutBox::AboutBox(CWnd* pParent /*=NULL*/)
	: CDialog(AboutBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(AboutBox)	
	//}}AFX_DATA_INIT
}


void AboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AboutBox)	
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_URL, m_url);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AboutBox, CDialog)
	//{{AFX_MSG_MAP(AboutBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten AboutBox 

BOOL AboutBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_OK);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}
