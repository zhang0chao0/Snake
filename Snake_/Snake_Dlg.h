
// Snake_Dlg.h : 头文件
//

#pragma once

#include "MySnake.h"
#include "afxwin.h"
#include<stack>

// CSnake_Dlg 对话框
class CSnake_Dlg : public CDialogEx
{
// 构造
public:
	CSnake_Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SNAKE__DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool m_start;
	//*********************开始暂停
	int StartPause;
	afx_msg void OnStart();
	//文件路径
	CString m_path;
	//添加蛇
	CMySnake m_snake;
	//添加食物,不止一个食物
	Food* m_bean;
	//生成的豆子是否在蛇内
	bool IsInBody(int i);
	//吃到豆子了
	bool EatBean();
	//游戏结束
	bool GameOver();
	//撞倒了自己
	bool HeadBody();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// 分数
	int m_mark;
	int m_markCopy;
	int m_highest;
	int m_life;
	// 游戏难度
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	int m_rank;
	//**********************存单词
	Word* m_word;
	afx_msg void OnClose();
	//**********************豆子的长度
	int Bean_len;
	CString Letter;
	//******************豆子的坐标互不相同
	bool Bean_same(int i);
	CString m_FoodLetter;
	CString m_FoodNote;
	// 关卡
	int m_pass;
	//*********************过关
	bool Pass();
	//******************标记下标，是否吃错豆子
	int m_index;
	int TempDex;
	//********************是否吃到了正确的豆子
	bool EatRight();
	//********************蛇的拷贝，保存一条蛇
	CMySnake m_snakeCopy;
	//*********************清空界面并画豆子
	void ClearAndBean();
	afx_msg void OnMusic();
	//************************播放音乐的进程
	HANDLE hThread;
	DWORD ThreadID;
	afx_msg void OnEndMusic();
	//*********************自动寻路的函数
	void FindPath();
	BOOL m_AutoPath;
	void Voice(CString&);
};
//函数的定义
void ThreadFunc();