
// Snake_Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Snake_.h"
#include "Snake_Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <Windows.h>
#include "Resource.h"
#include<fstream>
#include <Vfw.H>
#include<cmath>
#include <mmsystem.h>
#include "sapi.h"
#include "sphelper.h"
#pragma comment(lib, "sapi.lib")
#pragma comment(lib, "WINMM.LIB") 
using namespace std;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSnake_Dlg 对话框


//构造函数
CSnake_Dlg::CSnake_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSnake_Dlg::IDD, pParent)
	, m_mark(0)
	, m_life(4)
	, m_rank(1)
	, m_pass(1)
	, m_index(0)
	, m_markCopy(0)
	, m_AutoPath(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_start = false;
	//蛇的初始化
	m_snake.Snake_len = 4;       //长4
	m_snake.direction = 2;       //向右
	m_snake.Snake_body[0].C_row = 2; m_snake.Snake_body[0].C_list = 8;
	m_snake.Snake_body[1].C_row = 2; m_snake.Snake_body[1].C_list = 7;
	m_snake.Snake_body[2].C_row = 2; m_snake.Snake_body[2].C_list = 6;
	m_snake.Snake_body[3].C_row = 2; m_snake.Snake_body[3].C_list = 5;
	m_snakeCopy = m_snake;
	//**********记录
	ifstream fin("贪吃蛇素材\\记录.txt", ios::in);
	int t;
	fin >> t;
	m_highest = t;
	fin.close();
	//***************************读单词呢
	m_word = new Word[4287];
	ifstream FileIn("贪吃蛇素材\\2010年大学英语四级词汇.txt",ios::in);
	char a[100] = {0};
	char b[100] = {0};
	int i = 0;
	while (!FileIn.eof())
	{
		FileIn.getline(a,100);
		FileIn.getline(b, 100);
		m_word[i].letter = a;
		m_word[i].note = b;
		i++;
	}
	FileIn.close();
	//************************************
	srand(time(NULL));
	int R = rand() % 4287;        //随机找到单词下标
    Letter = m_word[R].letter;
	int len = Letter.GetLength();
	//***************************显示在编辑框
	m_FoodLetter = Letter;
	m_FoodNote = m_word[R].note;
	//************************保存一下豆子
	Bean_len = len;
	m_bean = new Food[len];
	//************************初始化豆子的坐标
	for (int i = 0; i < len; i++)
	{
		do
		{
			m_bean[i].L = Letter[i];        //保存单词
			m_bean[i].F_row = rand() % 25;
			m_bean[i].F_list = rand() % 25;
		} while (this->IsInBody(i)||this->Bean_same(i));
	}
}

void CSnake_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_mark);
	DDX_Text(pDX, IDC_EDIT2, m_highest);
	DDX_Text(pDX, IDC_EDIT3, m_life);
	DDX_Radio(pDX, IDC_RADIO1, m_rank);
	DDX_Text(pDX, IDC_EDIT4, m_FoodLetter);
	DDX_Text(pDX, IDC_EDIT5, m_FoodNote);
	DDX_Text(pDX, IDC_EDIT6, m_pass);
	DDX_Check(pDX, IDC_CHECK1, m_AutoPath);
}

BEGIN_MESSAGE_MAP(CSnake_Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSnake_Dlg::OnStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO1, &CSnake_Dlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CSnake_Dlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CSnake_Dlg::OnBnClickedRadio3)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CSnake_Dlg::OnMusic)
	ON_BN_CLICKED(IDC_BUTTON3, &CSnake_Dlg::OnEndMusic)
END_MESSAGE_MAP()


// CSnake_Dlg 消息处理程序

BOOL CSnake_Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_path = _T("贪吃蛇素材\\");
	StartPause = 1;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSnake_Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSnake_Dlg::OnPaint()
{
	//初始化游戏界面
	CPaintDC pDC(this);
	//画格子
	CBitmap Bitmap;
	Bitmap.LoadBitmapW(IDB_BITMAP2);
	CDC MemDC;
	MemDC.CreateCompatibleDC(&pDC);
	MemDC.SelectObject(&Bitmap);
	Bitmap.Detach();             //分离，便于下次使用
	//********************************画格子 20*20的格子
	for (int i = 0; i < 25; i++)
	for (int j = 0; j < 25; j++)
	{
		pDC.StretchBlt(20 * i, 20 * j, 20, 20, &MemDC, 0, 0, 48, 48, SRCCOPY);
	}
	//*******************************************画蛇头
	Bitmap.LoadBitmapW(IDB_BITMAP3);
	MemDC.SelectObject(&Bitmap);
	pDC.StretchBlt(20 * m_snake.Snake_body[0].C_list,20 * m_snake.Snake_body[0].C_row, 20, 20, &MemDC, 0, 0, 48, 48, SRCCOPY);
	Bitmap.Detach();
	//*******************************************画蛇身
	Bitmap.LoadBitmapW(IDB_BITMAP4);
	MemDC.SelectObject(&Bitmap);
	for (int i = 1; i < m_snake.Snake_len; i++)
		pDC.StretchBlt(20 * m_snake.Snake_body[i].C_list,20 * m_snake.Snake_body[i].C_row, 20, 20, &MemDC, 0, 0, 48, 48, SRCCOPY);
	Bitmap.Detach();
	//*******************************************画豆
	if (::LoadImage(NULL, m_path + L"a.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION) == false)
	{
		MessageBox(_T("未找到指定素材，请检查素材是否在指定位置！"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	//******************************************小判断一下
	//**********画出豆子
	for (int i = 0; i <Bean_len ; i++)
	{
		//***********************************************画了
		HBITMAP hbmp = (HBITMAP)::LoadImage(NULL, m_path + (CString)Letter[i] +L".bmp", 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Bitmap.Attach(hbmp);
		BITMAP temp;
		Bitmap.GetBitmap(&temp);
		MemDC.SelectObject(&Bitmap);
		pDC.StretchBlt(20 * m_bean[i].F_list, 20 * m_bean[i].F_row, 20, 20, &MemDC, 0, 0,temp.bmWidth, temp.bmHeight, SRCCOPY);
		Bitmap.Detach();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSnake_Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSnake_Dlg::OnStart()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (StartPause % 2 == 0)
		m_start = false;
	else
		m_start = true;
	StartPause++;
	//设置一个定时器
	if (m_rank==0)
		SetTimer(1, 200, NULL);
	else if (m_rank==1)
		SetTimer(1, 100, NULL);
	else
		SetTimer(1, 50, NULL);
	//this->Voice(Letter);
}

//*********************生成的豆子是否在蛇内,在豆子内也不行
bool CSnake_Dlg::IsInBody(int i)
{
	//豆子的行等于蛇身并且豆子的列等于蛇身,最好也不能在蛇头的前一格，
	for (int j = 0; j < m_snake.Snake_len; j++)
	if (m_bean[i].F_row == m_snake.Snake_body[j].C_row&&m_bean[i].F_list == m_snake.Snake_body[j].C_list)
	{
		return true;
	}
	if (m_bean[i].F_row == 2 && m_bean[i].F_list == 9)
		return true;
	return false;
}
//**********************在豆子内也不行
bool CSnake_Dlg::Bean_same(int i)
{
	for (int j = 0; j < i; j++)
	{
		if ((m_bean[i].F_list == m_bean[j].F_list&&m_bean[i].F_row == m_bean[j].F_row))
			return true;
	}
	return false;
}

//*********************************吃到豆子了，吃错豆子，生命-1
bool  CSnake_Dlg::EatBean()
{
	for (int i = 0; i < Bean_len;i++)
	if (m_snake.Snake_body[0].C_row == m_bean[i].F_row&&m_snake.Snake_body[0].C_list == m_bean[i].F_list&&m_bean[i].HaveEat==false)
	{
		//*******************保存i，当前吃到的是哪个豆子
		TempDex = i;
		return true;
	}
	return false;
}
//******************************吃到正确的豆子
bool CSnake_Dlg::EatRight()
{
	if (m_bean[TempDex].L==Letter[m_index])
	{
		m_bean[TempDex].HaveEat = true;
		m_index++;
		return true;
	}
	else
		return false;
}
//********************************游戏结束了
bool  CSnake_Dlg::GameOver()
{
	//注意墙是0到25个格子，撞墙了
	if (m_snake.Snake_body[0].C_row <0 || m_snake.Snake_body[0].C_row >=25
		|| m_snake.Snake_body[0].C_list <0 || m_snake.Snake_body[0].C_list >= 25)
		return true;
	//或者自己撞倒了自己
	if (HeadBody() == true)
		return true;
	return false;
}
//*********************************过关
bool  CSnake_Dlg::Pass()
{
	for (int i = 0; i < Bean_len; i++)
	if (m_bean[i].HaveEat == false)
		return false;
	return true;
}
//********************************自己撞到了自己
bool CSnake_Dlg::HeadBody()
{
	for (int i = 1; i < m_snake.Snake_len; i++)
	if (m_snake.Snake_body[0].C_row == m_snake.Snake_body[i].C_row&&m_snake.Snake_body[0].C_list == m_snake.Snake_body[i].C_list)
	{
		return true;
	}
	return false;
}

void CSnake_Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	UpdateData(true);
	if (m_start == true)
	{
		Case CaseTemp;
		CClientDC dc(this);
		CBitmap Bitmap;
		CDC dcMemory;
		dcMemory.CreateCompatibleDC(&dc);
		switch (m_snake.direction)
		{
		case 1:              //上
			CaseTemp = m_snake.ForwordUp();
			break;
		case 2:              //右
			CaseTemp = m_snake.ForwordRight();
			break;
		case 3:              //下
			CaseTemp = m_snake.ForwordDown();
			break;
		case 4:              //左
			CaseTemp = m_snake.ForwordLeft();
			break;
		}
		//********************
		//if (m_pass > 5)
		//{
		//	m_start = false;
		//	MessageBox(_T("已通关！"), _T("恭喜"), MB_OK);
		//	if (m_mark > m_highest)
		//	{
		//		MessageBox(_T("已刷新记录!"), _T("提示"), MB_OK);
		//		ofstream fout("贪吃蛇素材\\记录.txt", ios::out);
		//		fout << m_mark;
		//		fout.close();
		//		m_highest = m_mark;
		//	}
		//	UpdateData(false);
		//}
		if (GameOver() == true)
		{
			m_start = false;
			m_life--;
			UpdateData(false);
			//*****************生命值用完
			if (m_life <= 0)
			{
				if (MessageBox(_T("Game over!，是否继续游戏？"), _T("遗憾"), MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
				{
					CString str;
					str = "GAME OVER!!!";
					CFont font;
					font.CreateFont(60, 30, 0, 0, FW_THIN, 0, 0, 0, DEFAULT_CHARSET, OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, FIXED_PITCH | FF_MODERN, LPCTSTR(str));
					dc.SetTextColor(RGB(0, 200, 0));
					dc.SelectObject(&font);
					dc.TextOut(162, 200, str);
					GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
					return;
				}
				m_life += 4;
				UpdateData(false);
			}
			MessageBox(_T("生命减1，本局重新开始"), _T("提示"), MB_OK);
			m_index = 0;
			m_snake = m_snakeCopy;
			this->ClearAndBean();
		}
		//*******************************通关了！
		if (this->Pass() == true)
		{
			m_start = false;
			m_snake = m_snakeCopy;
			m_pass++;
			UpdateData(false);
			MessageBox(_T("恭喜过关！"), _T("提示"), MB_OK);
			//***********原来的豆子不要了
			delete[]m_bean;
			srand(time(NULL));
			int R = rand() % 4287;        //随机找到单词下标
			Letter = m_word[R].letter;
			int len = Letter.GetLength();
			//***************************显示在编辑框
			m_FoodLetter = Letter;
			m_FoodNote = m_word[R].note;
			UpdateData(false);
			//************************保存一下豆子的长度
			Bean_len = len;
			m_bean = new Food[len];
			//************************初始化豆子的坐标
			for (int i = 0; i < len; i++)
			{
				do
				{
					m_bean[i].L = Letter[i];
					m_bean[i].F_row = rand() % 25;
					m_bean[i].F_list = rand() % 25;
				} while (this->IsInBody(i) || this->Bean_same(i));
			}
			//**************************把画面清空一下
			m_index = 0;
			m_snake = m_snakeCopy;
			this->ClearAndBean();
		}
		//**************************自动寻路
		if (m_AutoPath==true)
			this->FindPath();
		//************************当蛇吃到豆子
		if (this->EatBean() == true)
		{
			if (EatRight() == true)
			{
				m_mark += 100;
				m_markCopy += 100;
				if (m_markCopy >= 1000)
				{
					m_start = false;
					MessageBox(_T("奖励生命一条！"), _T("提示"), MB_OK);
					m_life++;
					m_start = true;
					m_markCopy -= 1000;
				}
				//蛇身+1
				m_snake.Snake_len++;
				//身体变化
				m_snake.Snake_body[m_snake.Snake_len - 1].C_row = CaseTemp.C_row;
				m_snake.Snake_body[m_snake.Snake_len - 1].C_list = CaseTemp.C_list;
			}
			//********************************吃错豆子了,生命-1,本关重来
			else if (EatRight() == false)
			{
				m_life--;
				m_start = false;
				MessageBox(_T("吃错字母，生命值减1！本关重来！"), _T("提示"), MB_OK);
				m_index = 0;
				m_snake = m_snakeCopy;
				//**************************把画面清空一下
				this->ClearAndBean();
			}
			UpdateData(false);
		}
		//******************************更新蛇尾
		Bitmap.LoadBitmapW(IDB_BITMAP2);
		dcMemory.SelectObject(&Bitmap);
		dc.StretchBlt(20 * CaseTemp.C_list, 20 * CaseTemp.C_row, 20, 20, &dcMemory, 0, 0,48, 48, SRCCOPY);
		Bitmap.Detach();
		//********************************画蛇头
		Bitmap.LoadBitmapW(IDB_BITMAP3);
		dcMemory.SelectObject(&Bitmap);
		dc.StretchBlt(20 * m_snake.Snake_body[0].C_list, 20 * m_snake.Snake_body[0].C_row, 20, 20, &dcMemory, 0, 0, 48, 48, SRCCOPY);
		Bitmap.Detach();
		//**********************************蛇身
		Bitmap.LoadBitmapW(IDB_BITMAP4);
		dcMemory.SelectObject(&Bitmap);
		for (int i = 1; i < m_snake.Snake_len; i++)
			dc.StretchBlt(20 * m_snake.Snake_body[i].C_list,20 * m_snake.Snake_body[i].C_row, 20, 20, &dcMemory, 0, 0, 48, 48, SRCCOPY);
		Bitmap.Detach();
	}
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CSnake_Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (WM_KEYDOWN != pMsg->message)
		return CWnd::PreTranslateMessage(pMsg);
	if (pMsg->message==WM_KEYDOWN)
	{
		//*********开启自动寻路后就不用键盘了
		if (m_AutoPath == true)
			return true;
		if (m_start == false)
			return false;
		//向上 w
		if ((pMsg->wParam=='W') && m_snake.direction != 3)
		{
			m_snake.direction = 1;
		}
		//向下 s
		if ((pMsg->wParam=='S') && m_snake.direction != 1)
		{
			m_snake.direction = 3;
		}
		//向左 a
		if ((pMsg->wParam=='A') && m_snake.direction != 2)
		{
			m_snake.direction = 4;
		}
		//向右 d
		if ((pMsg->wParam=='D') && m_snake.direction != 4)
		{
			m_snake.direction = 2;
		}
	}
	return true;
}


void CSnake_Dlg::OnBnClickedRadio1()
{
	// TODO:  在此添加控件通知处理程序代码
	SetTimer(1, 200, NULL);
	m_rank = 0;
}


void CSnake_Dlg::OnBnClickedRadio2()
{
	// TODO:  在此添加控件通知处理程序代码
	SetTimer(1, 100, NULL);
	m_rank = 1;
}


void CSnake_Dlg::OnBnClickedRadio3()
{
	// TODO:  在此添加控件通知处理程序代码
	SetTimer(1, 50, NULL);
	m_rank = 2;
}

//***************************写析构函数
void CSnake_Dlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_mark > m_highest)
	{
		ofstream fout("贪吃蛇素材\\记录.txt", ios::out);
		fout << m_mark;
		fout.close();
	}
	delete[]m_word;
	delete[]m_bean;
	CDialogEx::OnClose();
}
//***************************清空界面并画豆子
void  CSnake_Dlg::ClearAndBean()
{
	//******************************重要
	for (int i = 0; i < Bean_len; i++)
		m_bean[i].HaveEat = false;
	CClientDC pDC(this);
	//画格子
	CBitmap Bitmap;
	Bitmap.LoadBitmapW(IDB_BITMAP2);
	CDC MemDC;
	MemDC.CreateCompatibleDC(&pDC);
	MemDC.SelectObject(&Bitmap);
	Bitmap.Detach();             //分离，便于下次使用
	for (int i = 0; i < 25; i++)
	for (int j = 0; j < 25; j++)
	{
		pDC.StretchBlt(20 * i, 20 * j, 20, 20, &MemDC, 0, 0, 48, 48, SRCCOPY);
	}
	Bitmap.Detach();
	this->OnPaint();
	//**********画出豆子
	for (int i = 0; i < Bean_len; i++)
	{
		//***********************************************画了
		HBITMAP hbmp = (HBITMAP)::LoadImage(NULL, m_path +
			(CString)Letter[i] + L".bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		Bitmap.Attach(hbmp);
		BITMAP temp;
		Bitmap.GetBitmap(&temp);
		MemDC.SelectObject(&Bitmap);
		pDC.StretchBlt(20 * m_bean[i].F_list, 20 * m_bean[i].F_row, 20, 20, &MemDC, 0, 0, temp.bmWidth, temp.bmHeight, SRCCOPY);
		Bitmap.Detach();
	}
}

void CSnake_Dlg::OnMusic()
{
	// TODO:  在此添加控件通知处理程序代码
	//创建进程
	hThread = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadFunc,
		NULL,
		0,
		&ThreadID);
}

void ThreadFunc()
{
	PlaySound(L"贪吃蛇素材\\音乐.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}


void CSnake_Dlg::OnEndMusic()
{
	// TODO:  在此添加控件通知处理程序代码
	PlaySound(NULL, NULL, NULL);
}
//****************************自动寻路的函数
void CSnake_Dlg::FindPath()
{
	AutoPath Point[25][25];       //界面的格子
	for (int i = 0; i < 25; i++)
	for (int j = 0; j < 25; j++)
	{
		Point[i][j].Cur = CPoint(i, j);
	}
	//*************蛇身都视为障碍物
	for (int i = 0; i < m_snake.Snake_len; i++)
	{
		Point[m_snake.Snake_body[i].C_list][m_snake.Snake_body[i].C_row].Flag = -1;
	}
	//**************起始点，终止点
	AutoPath P_start, P_end;
	P_start = Point[m_snake.Snake_body[0].C_list][m_snake.Snake_body[0].C_row];
	//***************关闭所有单词，只开启当前单词
	for (int i = 0; i < Letter.GetLength(); i++)
	{
		//************如果没有被吃，当障碍物处理
		if (m_bean[i].HaveEat==false)
			Point[m_bean[i].F_list][m_bean[i].F_row].Flag = -1;
	}
	Point[m_bean[m_index].F_list][m_bean[m_index].F_row].Flag = 0;
	P_end = Point[m_bean[m_index].F_list][m_bean[m_index].F_row];
	//*********************************************** 
	AutoPath* temp = &P_start;
	//************相邻的四个格子
	AutoPath* Other = new AutoPath[4];
	Other[0] = Point[temp->Cur.x][temp->Cur.y - 1];
	Other[1] = Point[temp->Cur.x][temp->Cur.y + 1];
	Other[2] = Point[temp->Cur.x - 1][temp->Cur.y];
	Other[3] = Point[temp->Cur.x + 1][temp->Cur.y];
	for (int i = 0; i < 4; i++)
	{
		if (Other[i].Flag == -1)
			continue;
		else
		{
			int t = abs(Other[i].Cur.x - P_end.Cur.x) + abs(Other[i].Cur.y - P_end.Cur.y);
			Other[i].F = t;
		}
	}
	//***********找出最满足的
	AutoPath Tem;
	for (int i = 0; i < 4; i++)
	if (Tem.F>Other[i].F)
		Tem = Other[i];
	CPoint p = Tem.Cur;
	//*************************找出路径
	if (m_snake.Snake_body[0].C_row+1== p.y)
		m_snake.direction = 3;       //向下
	else if (m_snake.Snake_body[0].C_row-1 == p.y)
		m_snake.direction = 1;        //向上
	else if (m_snake.Snake_body[0].C_list+1 == p.x)
		m_snake.direction = 2;        //向右
	else if (m_snake.Snake_body[0].C_list-1 == p.x)
		m_snake.direction = 4;        //向左
	delete[]Other;
}

//*************************发音
void CSnake_Dlg::Voice(CString&find)
{
	// TODO:  在此添加控件通知处理程序代码
	ISpVoice *pSpVoice;        // 重要COM接口
	::CoInitialize(NULL);         // COM初始化
	// 获取ISpVoice接口
	CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void**)&pSpVoice);
	pSpVoice->SetVolume(60);
	pSpVoice->Speak(find, SPF_DEFAULT, NULL);
	pSpVoice->Release();
	::CoUninitialize();
}