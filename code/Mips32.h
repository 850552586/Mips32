#pragma once
//author:Ericam_
//csdn:https://blog.csdn.net/xjm850552586
//个人博客:https://850552586.github.io/
//github:https://github.com/850552586
//欢迎交流学习
#pragma once
#pragma once
#define head_h
//-----------------------------------【头文件包含部分】---------------------------------------
//	描述：包含程序所依赖的头文件
//------------------------------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include<tchar.h>
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<math.h>
using namespace std;
//-----------------------------------【库文件包含部分】---------------------------------------
//	描述：包含程序所依赖的库文件
//------------------------------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib")  //调用PlaySound函数所需库文件
#pragma  comment(lib,"Msimg32.lib")
//-----------------------------------【宏定义部分】--------------------------------------------
//	描述：定义一些辅助宏
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	1040					//为窗口宽度定义的宏，以方便在此处修改窗口宽度
#define WINDOW_HEIGHT	800				//为窗口高度定义的宏，以方便在此处修改窗口高度
#define WINDOW_TITLE	L"基于仿真的32位虚拟计算机设计与实现"	//为窗口标题定义的宏

#define $zero 0
#define $at 1
#define $v0 2 
#define $v1 3
#define $a0 4
#define $a1 5
#define $a2 6
#define $a3 7
#define $t0 8
#define $t1 9
#define $t2 10
#define $t3 11
#define $t4 12
#define $t5 13
#define $t6 14
#define $t7 15
#define $s0 16
#define $s1 17
#define $s2 18
#define $s3 19
#define $s4 20
#define $s5 21
#define $s6 22
#define $s7 23
#define $t8 24
#define $t9 25
#define $k0 26
#define $k1 27
#define $gp 28
#define $sp 29
#define $fp 30
#define $ra 31
//-----------------------------------【全局变量声明部分】-------------------------------------
//	描述：全局变量的声明
//------------------------------------------------------------------------------------------------
extern HDC	g_hdc, g_mdc;       //全局设备环境句柄
extern HBITMAP g_hBackGround;  //定义位图句柄
extern wchar_t sz[100];
extern HWND edit_ip;
extern HWND Start_Button;//定义主页开始按钮
extern HWND Help_Button;//定义主页帮助按钮
extern HWND End_Button;//定义主页结束按钮
extern vector<char> Instruction_Memory;//指令段内存
extern int Data_Memory[256];//数据段内存
					 //指令数据分开存放 解决资源相关问题
extern string Register_name[32];//寄存器名
extern int reg_result[32];//解决数据相关问题，定向传送，结果缓冲寄存器
extern int clock_cycle;//时钟周期
extern int PC;//程序计数器
extern int PC_Next;//若为零则无控制相关问题，否则存放预测地址
extern int PC_Len;//存放指令条数
extern int regis_init;//程序运行前是否初始化寄存器和内存

				   //译码结果存储，显示在图形界面
extern string instruction_out;
extern string rt;
extern string rs;
extern char outtype;
extern int outimmediate;
extern int special;
//-------------------------------------------------------------------------------------------
//--------------------------------------【类声明】------------------------------------------
//------------------------------------------------------------------------------------------
class Mips32_instruction
{
public:
	void init();
	int IF();//取指阶段
	void ID();//译码阶段
	void EX();//计算阶段
	void MEM();//访存阶段
	void WB();//写回阶段
	char instruction[33];
	string op;//6
	string rs;//5
	string rt;//5
	string rd;//5
	string shamt;//5
	string func;//6
	string immediate;//16
	string address;//26
	char type;
	int result;//存放数据计算结果
	int add_result;//存放地址计算结果
	int rs_num = 0;//rs编号
	int rt_num = 0;//rt编号
	int rd_num = 0;//rd编号
	int shamt_num = 0;//shamt数值
	int immediate_num = 0;//立即数数值
	int address_num = 0;//转移后指令地址
	int memory_address = 0;//数据访存地址
	string order;
	int rs_data, rt_data;//取数
};


class Pipeline
{
public:
	Mips32_instruction Task[5];
	int Task_Pro[5];
	int IF_Used = 0, ID_Used = 0, EX_Used = 0, MEM_Used = 0, WB_Used = 0;
	//
	void System_Run();//取指阶段
};
extern Pipeline P;
//-----------------------------------【全局函数声明部分】-------------------------------------
//	描述：全局函数声明，防止“未声明的标识”系列错误
//------------------------------------------------------------------------------------------------

LRESULT CALLBACK	WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//窗口过程函数
BOOL	BackGround_Paint(HWND hwnd);			//在此函数中进行资源的初始化
BOOL BackGround_CleanUp(HWND hwnd);
void dialogue(HWND hwnd, wchar_t string[], int posX, int posY);
void Register_init();
void Register_display(HWND hwnd);
void instruction_display(HWND hwnd);
void FiveStage_assemblyline(HWND hwnd);
void instructionID_display(HWND hwnd);
bool Memory_get();
void InitRegAndMem();
void instruction_read();
void instruction_next();



