#pragma once

//***************单词
//文本共有8574行
//共4287个单词
struct Word
{
	//字母和注释意思
	CString letter;
	CString note;
};


//一个个的方格
struct Case
{
	int C_row;            //格子的行
	int C_list;           //格子的列
};

//食物
struct Food
{
	int F_row;            //食物所在的行
	int F_list;           //食物所在的列
	bool HaveEat;         //是否被吃了
	char L;               //字母
	Food()
	{
		HaveEat = false;
		L = 0;
	}
};
//*****************************自动寻路
struct AutoPath
{
	int Flag;             //-1表示障碍物
	CPoint Cur;        //当前点
	int F;                  //路径距离
	AutoPath()
	{
		F = 1000;
		Flag = 0;
	}
};

class CMySnake
{
public:
	CMySnake();
	~CMySnake();
public:
	int Snake_len;                //蛇长度
	Case Snake_body[1500];        //蛇身，假设最长为1500个格子
	int direction;                //蛇前进的方向
	//1.上 2.右 3.下 4.左
	Case ForwordUp();
	Case ForwordRight();
	Case ForwordDown();
	Case ForwordLeft();
};

