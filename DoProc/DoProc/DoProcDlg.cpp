
// DoProcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DoProc.h"
#include "DoProcDlg.h"
#include "afxdialogex.h"
#include <map>
#include <string>
using namespace std;


#include "Poco/StringTokenizer.h"
using Poco::StringTokenizer;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDoProcDlg dialog




CDoProcDlg::CDoProcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDoProcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDoProcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDoProcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CDoProcDlg::OnBnClickedBtnBrowse)
END_MESSAGE_MAP()


// CDoProcDlg message handlers

BOOL CDoProcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDoProcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDoProcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDoProcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


typedef struct typeRules 
{
	string key;
	string value;
}RULES,*PRULES;

void CDoProcDlg::OnBnClickedBtnBrowse()
{
	//get open name
	CString cstrPath;
	CFileDialog dlg(TRUE,TEXT(".txt"),TEXT("input"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_EXPLORER ,
		TEXT("TXT Files(*.txt)|*.txt|All Files(*.*)|*.*||"));///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if(dlg.DoModal() == IDOK)
		cstrPath = dlg.GetPathName();
	else
		return;

	//read line one by one
	FILE* fp = NULL;
	fp = fopen(cstrPath.GetBuffer(),"rt");
	if ( fp == NULL )
	{
		AfxMessageBox("open config file failed");
		return;
	}
	char buf[100] = {'\0'};
	vector<RULES> rules;
	RULES rulesTmp;
	vector< vector<string> > vvBuf;
	vector< string> vBuf;
	bool bSkip = false;
	bool bCalc = false;
	float fResult = 0.0,fNumber1 = 0.0;
	while ( 0 == feof(fp) )
	{
		if ( NULL == fgets(buf,100,fp) )
		{
			AfxMessageBox(TEXT("Get info failed"));
			break;
		}
		if ( strlen(buf) < 5 )
		{
			bSkip = true;
			continue;
		}
		//info of calc 
		if ( bSkip )
		{
			vBuf.clear();
			StringTokenizer tokenizer(buf, " ", StringTokenizer::TOK_TRIM);
			for (StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end(); ++it)
			{
				vBuf.push_back(*it);
			}
			vvBuf.push_back(vBuf);
			
		}else//info of rule
		{
			StringTokenizer tokenizer(buf, " ", StringTokenizer::TOK_TRIM);
			rulesTmp.key = tokenizer[1];
			rulesTmp.value = tokenizer[3];
			rules.push_back(rulesTmp);
			if ( rulesTmp.key == "foot" )
			{
				rulesTmp.key = "feet";
				rules.push_back(rulesTmp);

			}
		}
		
	}
	fclose(fp);

	string strPath = cstrPath.Left(cstrPath.ReverseFind('\\') + 1).GetBuffer();
	strPath += "output.txt";
	fp = fopen(strPath.c_str(),"w+t");
	if ( fp == NULL )
	{
		AfxMessageBox("open config file failed");
		return;
	}
	string strTmp = "ascomtohom@126.com\n\n";
	int iRet = fwrite(strTmp.c_str(),1,strTmp.size(),fp);
	if ( iRet != strTmp.size() )
	{
		AfxMessageBox("write header failed\n");
		return;
	}
	float value = 0;
	int iCount = 0,lineCount = 2;
	char bufTmp[100] = {'\0'};
	//0 is +,1 is -,2 is *,3 is /
	int iSign = -1;
	for ( int i = 0; i < vvBuf.size(); ++i )
	{
		vBuf = vvBuf[i];
		for ( int n = 0; n < vBuf.size(); ++n )
		{
			if ( vBuf[n] == "+" || vBuf[n] == "-" )
			{
				++iCount;
			}
		}
		for ( int m = 0; m < vBuf.size();  )
		{
			if ( iCount && bCalc && (-1 == iSign) )
			{
				if ( vBuf[m] == "+" )
				{
					iSign = 0;
					++m;
					continue;
				}else if ( vBuf[m] == "-" )
				{
					iSign = 1;
					++m;
					continue;
				}else if ( vBuf[m] == "*" )
				{
					iSign = 2;
					++m;
					continue;
				}else if ( vBuf[m] == "/" )
				{
					iSign = 3;
					++m;
					continue;
				}else
				{
					AfxMessageBox("unknown sign");
					return;
				}
			}
			
			for (int j = 0; j < rules.size(); ++j )
			{
				
				if ( 0 == memcmp(vBuf[m + 1].c_str(),rules[j].key.c_str(),4) )
				{
					if ( iCount )
					{
						if ( false == bCalc )
						{
							//need calc
							fNumber1 = atof(rules[j].value.c_str()) * atof(vBuf[m].c_str());
							bCalc = true;
						}else
						{
							switch ( iSign )
							{
							case 0:
								fResult = fNumber1 + (atof(rules[j].value.c_str()) * atof(vBuf[m].c_str()) );
								break;
							case 1:
								fResult = fNumber1 - (atof(rules[j].value.c_str()) * atof(vBuf[m].c_str()) );
								break;
							case 2:
								fResult = fNumber1 * (atof(rules[j].value.c_str()) * atof(vBuf[m].c_str()) );
								break;
							case 3:
								fResult = fNumber1 / (atof(rules[j].value.c_str()) * atof(vBuf[m].c_str()) );
								break;
							default:
								AfxMessageBox("not supported now.");
							}
							--iCount;
							iSign = -1;
							fNumber1 = fResult;
							if ( 0 == iCount )
							{
								fNumber1 = 0.0;
								bCalc = false;
								goto skip_result;
							}
						}
						
						break;
					}else
					{
						//only transform
						fResult = atof(vBuf[m].c_str()) * atof(rules[j].value.c_str());
skip_result:
						++lineCount;
						if ( lineCount == 12 )
						{
							sprintf( bufTmp, "%.02f m", fResult ); //for float use %f
						}else
						{
							sprintf( bufTmp, "%.02f m\n", fResult ); //for float use %f
						}
						strTmp = &bufTmp[0];
						iRet = fwrite(strTmp.c_str(),1,strTmp.size(),fp);
						if ( iRet != strTmp.size() )
						{
							sprintf(bufTmp,"write failed at %d",j);
							AfxMessageBox(&bufTmp[0]);
							return;
						}
						break;
					}
					break;
				}
			}
			m += 2;
			
		}
		
		
	}
	fclose(fp);

}
