#pragma once
//author:Ericam_
//csdn:https://blog.csdn.net/xjm850552586
//���˲���:https://850552586.github.io/
//github:https://github.com/850552586
//��ӭ����ѧϰ
#pragma once
#pragma once
#define head_h
//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
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
//-----------------------------------�����ļ��������֡�---------------------------------------
//	���������������������Ŀ��ļ�
//------------------------------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib")  //����PlaySound����������ļ�
#pragma  comment(lib,"Msimg32.lib")
//-----------------------------------���궨�岿�֡�--------------------------------------------
//	����������һЩ������
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	1040					//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	800				//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE	L"���ڷ����32λ�������������ʵ��"	//Ϊ���ڱ��ⶨ��ĺ�

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
//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//------------------------------------------------------------------------------------------------
extern HDC	g_hdc, g_mdc;       //ȫ���豸�������
extern HBITMAP g_hBackGround;  //����λͼ���
extern wchar_t sz[100];
extern HWND edit_ip;
extern HWND Start_Button;//������ҳ��ʼ��ť
extern HWND Help_Button;//������ҳ������ť
extern HWND End_Button;//������ҳ������ť
extern vector<char> Instruction_Memory;//ָ����ڴ�
extern int Data_Memory[256];//���ݶ��ڴ�
					 //ָ�����ݷֿ���� �����Դ�������
extern string Register_name[32];//�Ĵ�����
extern int reg_result[32];//�������������⣬�����ͣ��������Ĵ���
extern int clock_cycle;//ʱ������
extern int PC;//���������
extern int PC_Next;//��Ϊ�����޿���������⣬������Ԥ���ַ
extern int PC_Len;//���ָ������
extern int regis_init;//��������ǰ�Ƿ��ʼ���Ĵ������ڴ�

				   //�������洢����ʾ��ͼ�ν���
extern string instruction_out;
extern string rt;
extern string rs;
extern char outtype;
extern int outimmediate;
extern int special;
//-------------------------------------------------------------------------------------------
//--------------------------------------����������------------------------------------------
//------------------------------------------------------------------------------------------
class Mips32_instruction
{
public:
	void init();
	int IF();//ȡָ�׶�
	void ID();//����׶�
	void EX();//����׶�
	void MEM();//�ô�׶�
	void WB();//д�ؽ׶�
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
	int result;//������ݼ�����
	int add_result;//��ŵ�ַ������
	int rs_num = 0;//rs���
	int rt_num = 0;//rt���
	int rd_num = 0;//rd���
	int shamt_num = 0;//shamt��ֵ
	int immediate_num = 0;//��������ֵ
	int address_num = 0;//ת�ƺ�ָ���ַ
	int memory_address = 0;//���ݷô��ַ
	string order;
	int rs_data, rt_data;//ȡ��
};


class Pipeline
{
public:
	Mips32_instruction Task[5];
	int Task_Pro[5];
	int IF_Used = 0, ID_Used = 0, EX_Used = 0, MEM_Used = 0, WB_Used = 0;
	//
	void System_Run();//ȡָ�׶�
};
extern Pipeline P;
//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//------------------------------------------------------------------------------------------------

LRESULT CALLBACK	WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//���ڹ��̺���
BOOL	BackGround_Paint(HWND hwnd);			//�ڴ˺����н�����Դ�ĳ�ʼ��
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



