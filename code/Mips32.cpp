#include"Mips32.h"
HDC	g_hdc = NULL, g_mdc = NULL;       //ȫ���豸�������
HBITMAP g_hBackGround = NULL;  //����λͼ���
wchar_t sz[100];
HWND edit_ip;
HWND Start_Button;//������ҳ��ʼ��ť
HWND Help_Button;//������ҳ������ť
HWND End_Button;//������ҳ������ť


vector<char> Instruction_Memory;//ָ����ڴ�
int Data_Memory[256];//���ݶ��ڴ�
					 //ָ�����ݷֿ���� �����Դ�������
string Register_name[32];//�Ĵ�����
struct reg {
	int num;		//�Ĵ�������



	int used;		//�Ĵ����Ƿ�����ʹ��
}Register[32];//�Ĵ���
int reg_result[32];//�������������⣬�����ͣ��������Ĵ���
int clock_cycle = 0;//ʱ������
int PC = 0;//���������
int PC_Next = 0;//��Ϊ�����޿���������⣬������Ԥ���ַ
int PC_Len = 0;//���ָ������
int regis_init = 0;//��������ǰ�Ƿ��ʼ���Ĵ������ڴ�

				   //�������洢����ʾ��ͼ�ν���
string instruction_out;
string rt;
string rs;
char outtype;
int outimmediate;
int special = 0;
Pipeline P;
//-----------------------------------��BackGround_Paint( )������-------------------------------------
//	��������ʼ������������һЩ�򵥵ĳ�ʼ��
//------------------------------------------------------------------------------------------------

BOOL BackGround_Paint(HWND hwnd)
{
	g_hdc = GetDC(hwnd);
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"1.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);   																											   //-----��λͼ�����Ĳ���֮������������DC��-----
	g_mdc = CreateCompatibleDC(g_hdc);
	SelectObject(g_mdc, g_hBackGround);
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
	Register_display(hwnd);
	instruction_display(hwnd);
	FiveStage_assemblyline(hwnd);
	instructionID_display(hwnd);
	return TRUE;
}

//-----------------------------------��BackGround_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//---------------------------------------------------------------------------------------------------
BOOL BackGround_CleanUp(HWND hwnd)
{
	//�ͷ���Դ����
	DeleteObject(g_hBackGround);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}


//-----------------------------------�����ֺ������֡�-------------------------------------
void dialogue(HWND hwnd, wchar_t string[], int posX, int posY)//int charcter_num
{
	HFONT hFont;
	g_hdc = GetDC(hwnd);
	//g_mdc = CreateCompatibleDC(g_hdc);
	hFont = CreateFont
	(
		20, 0,    //�߶�20, ��ȡ0��ʾ��ϵͳѡ�����ֵ
		0, 0,    //�ı���б����������б��Ϊ0
		FW_HEAVY,    //����
		0, 0, 0,        //��б�壬���»��ߣ����л���
		GB2312_CHARSET,    //�ַ���
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,        //һϵ�е�Ĭ��ֵ
		DEFAULT_PITCH | FF_DONTCARE,
		L"����"    //��������
	);
	SetTextColor(g_hdc, RGB(255, 255, 255));
	SetBkMode(g_hdc, TRANSPARENT);
	SelectObject(g_hdc, hFont);
	TextOut(g_hdc, posX, posY, string, lstrlen(string));
	DeleteDC(g_hdc);
	DeleteObject(hFont);
}

//----------------------------------------------------------------------------------------
//------------------------------------�Ĵ�����ʼ��---------------------------------------
//---------------------------------------------------------------------------------------
void Register_init()
{
	for (int i = 0; i < 32; i++)
	{
		Register[i].num = 0;
		Register[i].used = 0;
	}
}

//----------------------------------------------------------------------------------------
//------------------------------------�Ĵ��� �ڴ�״��ͼ�λ���ʾ--------------------------------
//---------------------------------------------------------------------------------------
void Register_display(HWND hwnd)
{
	swprintf_s(sz, L"ʱ�����ڣ�%d", clock_cycle);
	dialogue(hwnd, sz, 680, 130);
	swprintf_s(sz, L"Register Data");
	dialogue(hwnd, sz, 80, 410);
	swprintf_s(sz, L"Data Memory");
	dialogue(hwnd, sz, 570, 200);
	for (int i = 0; i < 5; i++)
	{
		swprintf_s(sz, L"%S:%d", Register_name[6 * i].c_str(), Register[6 * i].num);
		dialogue(hwnd, sz, 80, 450 + 40 * i);
		swprintf_s(sz, L"%S:%d", Register_name[6 * i + 1].c_str(), Register[6 * i + 1].num);
		dialogue(hwnd, sz, 160, 450 + 40 * i);
		swprintf_s(sz, L"%S:%d", Register_name[6 * i + 2].c_str(), Register[6 * i + 2].num);
		dialogue(hwnd, sz, 220, 450 + 40 * i);
		swprintf_s(sz, L"%S:%d", Register_name[6 * i + 3].c_str(), Register[6 * i + 3].num);
		dialogue(hwnd, sz, 280, 450 + 40 * i);
		swprintf_s(sz, L"%S:%d", Register_name[6 * i + 4].c_str(), Register[6 * i + 4].num);
		dialogue(hwnd, sz, 340, 450 + 40 * i);
		swprintf_s(sz, L"%S:%d", Register_name[6 * i + 5].c_str(), Register[6 * i + 5].num);
		dialogue(hwnd, sz, 400, 450 + 40 * i);
	}
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			swprintf_s(sz, L"%d", Data_Memory[16 * i + j]);
			dialogue(hwnd, sz, 570 + 23 * j, 220 + 20 * i);
		}
	}
}

//----------------------------------------------------------------------------------------
//------------------------------------ָ��ͼ�λ���ʾ--------------------------------
//---------------------------------------------------------------------------------------
void instruction_display(HWND hwnd)
{
	swprintf_s(sz, L"��ǰָ��");
	dialogue(hwnd, sz, 600, 570);
	string temp;
	for (int i = 0; i < 32; i++)
		temp += P.Task[P.IF_Used - 1].instruction[31 - i];
	if (clock_cycle)
	{
		swprintf_s(sz, L"%d-%S", clock_cycle, temp.c_str());
		dialogue(hwnd, sz, 580, 600);
	}
}

//----------------------------------------------------------------------------------------
//------------------------------------�����ˮ��ͼ�λ���ʾ--------------------------------
//---------------------------------------------------------------------------------------
void FiveStage_assemblyline(HWND hwnd)
{
	swprintf_s(sz, L"ָ��ϵͳ");
	dialogue(hwnd, sz, 80, 100);
	swprintf_s(sz, L"IF:");
	dialogue(hwnd, sz, 70, 150);
	swprintf_s(sz, L"ID:");
	dialogue(hwnd, sz, 70, 200);
	swprintf_s(sz, L"EX:");
	dialogue(hwnd, sz, 70, 250);
	swprintf_s(sz, L"MEM:");
	dialogue(hwnd, sz, 70, 300);
	swprintf_s(sz, L"WB:");
	dialogue(hwnd, sz, 70, 350);
	if (P.IF_Used)
	{
		swprintf_s(sz, L"%S", P.Task[P.IF_Used - 1].instruction);
		dialogue(hwnd, sz, 110, 150);
	}
	if (P.ID_Used)
	{
		swprintf_s(sz, L"%S", P.Task[P.ID_Used - 1].instruction);
		dialogue(hwnd, sz, 110, 200);
	}
	if (P.EX_Used)
	{
		swprintf_s(sz, L"%S", P.Task[P.EX_Used - 1].instruction);
		dialogue(hwnd, sz, 110, 250);
	}
	if (P.MEM_Used)
	{
		swprintf_s(sz, L"%S", P.Task[P.MEM_Used - 1].instruction);
		dialogue(hwnd, sz, 110, 300);
	}
	if (P.WB_Used)
	{
		swprintf_s(sz, L"%S", P.Task[P.WB_Used - 1].instruction);
		dialogue(hwnd, sz, 110, 350);
		P.Task[P.WB_Used - 1].init();
	}
}

//----------------------------------------------------------------------------------------
//------------------------------------������ͼ�λ���ʾ--------------------------------
//---------------------------------------------------------------------------------------
void instructionID_display(HWND hwnd)
{
	swprintf_s(sz, L"%s", L"������");
	dialogue(hwnd, sz, 600, 620);
	if (outtype == 'L')
	{
		if (!special)
		{
			swprintf_s(sz, L"%d-%S%d", clock_cycle - 1, instruction_out.c_str(), outimmediate);
			dialogue(hwnd, sz, 640, 650);
		}
		else if (special == 1)
		{
			swprintf_s(sz, L"%d-lw %S%d(%S)", clock_cycle - 1, rt.c_str(), outimmediate, outimmediate, rs.c_str());
			dialogue(hwnd, sz, 640, 650);
			special = 0;
		}
		else if (special == 2)
		{
			swprintf_s(sz, L"%d-sw %S%d(%S)", clock_cycle - 1, rt.c_str(), outimmediate, outimmediate, rs.c_str());
			dialogue(hwnd, sz, 640, 650);
			special = 0;
		}
	}
	else if (outtype == 'J')
	{
		swprintf_s(sz, L"%d-%S%d", clock_cycle - 1, instruction_out.c_str(), outimmediate);
		dialogue(hwnd, sz, 640, 650);
	}
	else if (outtype == 'R')
	{
		if (!special)
		{
			swprintf_s(sz, L"%d-%S", clock_cycle - 1, instruction_out.c_str());
			dialogue(hwnd, sz, 640, 650);
		}
		else if (special == 1)
		{
			swprintf_s(sz, L"%d-%S%d", clock_cycle - 1, instruction_out.c_str(), outimmediate);
			dialogue(hwnd, sz, 640, 650);
			special = 0;
		}
	}
}
//----------------------------------------------------------------------------------------
//------------------------------------��ȡָ��--------------------------------
//---------------------------------------------------------------------------------------
void instruction_read()
{
	if (!clock_cycle)
		P.System_Run();
}

void instruction_next()
{
	if (clock_cycle)
		P.System_Run();
}


//�������ļ��ж�ȡָ��ڴ�
bool Memory_get()
{
	ifstream f("input.txt", ios::in);
	char buffer[33];
	while (!f.eof())
	{
		f.getline(buffer, 33, '\n');

		if (buffer[0] == '\0')
			break;
		for (int i = 0; i<32; i++)
			Instruction_Memory.push_back(buffer[i]);
		PC_Len++;
	}
	f.close();
	return true;
}

//-------------------------------------------------------------------------------------------
//---------------------------------��ʼ���ڴ�ͼĴ���----------------------------------------
//-------------------------------------------------------------------------------------------
void InitRegAndMem()
{
	regis_init = 1;
	for (int i = 0; i++; i < 32)
	{
		Register[i].num = 0;
		Register[i].used = 0;
	}
	for (int j = 0; j++; j < 256)
	{
		Data_Memory[j] = 0;
	}
	Register_name[0] = "$zero";
	Register_name[1] = "$at";
	Register_name[2] = "$v0";
	Register_name[3] = "$v1";
	Register_name[4] = "$a0";
	Register_name[5] = "$a1";
	Register_name[6] = "$a2";
	Register_name[7] = "$a3";
	Register_name[8] = "$t0";
	Register_name[9] = "$t1";
	Register_name[10] = "$t2";
	Register_name[11] = "$t3";
	Register_name[12] = "$t4";
	Register_name[13] = "$t5";
	Register_name[14] = "$t6";
	Register_name[15] = "$t7";
	Register_name[16] = "$s0";
	Register_name[17] = "$s1";
	Register_name[18] = "$s2";
	Register_name[19] = "$s3";
	Register_name[20] = "$s4";
	Register_name[21] = "$s5";
	Register_name[22] = "$s6";
	Register_name[23] = "$s7";
	Register_name[24] = "$t8";
	Register_name[25] = "$t9";
	Register_name[26] = "$k0";
	Register_name[27] = "$k1";
	Register_name[28] = "$gp";
	Register_name[29] = "$sp";
	Register_name[30] = "$fp";
	Register_name[31] = "$ra";
}
//------------------------------------------------------------------------------------------
//----------------------------------��ʼ���׶�-------------------------------------------------
//-------------------------------------------------------------------------------------------
void Mips32_instruction::init()
{
	op = "";//6
	rs = "";//5
	rt = "";//5
	rd = "";//5
	shamt = "";//5
	func = "";//6
	immediate = "";//16
	address = "";//26
	result = 0;//������ݼ�����
	add_result = 0;//��ŵ�ַ������
	rs_num = 0;//rs���
	rt_num = 0;//rt���
	rd_num = 0;//rd���
	shamt_num = 0;//�ͻ�amt��ֵ
	immediate_num = 0;//��������ֵ
	address_num = 0;//ת�ƺ�ָ���ַ
	memory_address = 0;//���ݷô��ַ
	rs_data = 0;
	rt_data = 0;
}


//------------------------------------------------------------------------------------------
//----------------------------------ȡָ�׶�-------------------------------------------------
//-------------------------------------------------------------------------------------------
int Mips32_instruction::IF()
{
	if (PC_Next)
	{
		PC = PC_Next;
		PC_Next = 0;

	}
	if (PC * 8 < PC_Len * 32)
	{
		for (int i = 0; i < 32; i++)
		{
			instruction[31 - i] = Instruction_Memory[PC * 8 + i];
		}
		instruction[32] = '\0';
		PC = PC + 4;
		return 1;
	}
	else
		return 0;
}

//-------------------------------------------------------------------------------------------
//----------------------------------����׶�-------------------------------------------------
//-------------------------------------------------------------------------------------------
void Mips32_instruction::ID()
{
	op = "";
	for (int i = 0; i < 6; i++)
		op.push_back(instruction[31 - i]);
	if (op == "000000")
		type = 'R';
	else if (op == "000010" || op == "000011")
		type = 'J';
	else
		type = 'L';
	//---------------------R-type--------------------
	ofstream f("output.txt", ios::app);
	if (type == 'R')
	{
		for (int i = 0; i < 6; i++)
			func.push_back(instruction[5 - i]);
		for (int i = 0; i < 5; i++)
		{
			rs.push_back(instruction[21 + i]);
			rs_num += (int(instruction[21 + i]) - 48)*pow(2, i);
			rt.push_back(instruction[16 + i]);
			rt_num += (int(instruction[16 + i]) - 48)*pow(2, i);
			rd.push_back(instruction[11 + i]);
			rd_num += (int(instruction[11 + i]) - 48)*pow(2, i);
			shamt.push_back(instruction[6 + i]);
			shamt_num += (int(instruction[6 + i]) - 48)*pow(2, i);
		}
		if (Register[rs_num].used == 1)
		{
			rs_data = reg_result[rs_num];
		}
		else
		{
			rs_data = Register[rs_num].num;
		}

		if (Register[rt_num].used == 1)
		{
			rt_data = reg_result[rt_num];
		}
		else
		{
			rt_data = Register[rt_num].num;
		}
		if (func == "100001")//addu
		{
			Register[rd_num].used = 1;
			f << "addu " << Register_name[rd_num] << "," << Register_name[rs_num] << "," << Register_name[rt_num] << endl;
			instruction_out = "addu" + Register_name[rd_num] + ',' + Register_name[rs_num] + ',' + Register_name[rt_num];
			outtype = 'R';
		}
		else if (func == "100011")//subu
		{
			Register[rd_num].used = 1;
			f << "subu " << Register_name[rd_num] << "," << Register_name[rs_num] << "," << Register_name[rt_num] << endl;
			instruction_out = "subu" + Register_name[rd_num] + ',' + Register_name[rs_num] + ',' + Register_name[rt_num];
			outtype = 'R';
		}
		else if (func == "100100")//and
		{
			Register[rd_num].used = 1;
			f << "and " << Register_name[rd_num] << "," << Register_name[rs_num] << "," << Register_name[rt_num] << endl;
			instruction_out = "and" + Register_name[rd_num] + ',' + Register_name[rs_num] + ',' + Register_name[rt_num];
			outtype = 'R';
		}
		else if (func == "100101")//or
		{
			Register[rd_num].used = 1;
			f << "or " << Register_name[rd_num] << "," << Register_name[rs_num] << "," << Register_name[rt_num] << endl;
			instruction_out = "or" + Register_name[rd_num] + ',' + Register_name[rs_num] + ',' + Register_name[rt_num];
			outtype = 'R';
		}
		else if (func == "100110")//xor
		{
			Register[rd_num].used = 1;
			f << "xor " << Register_name[rd_num] << "," << Register_name[rs_num] << "," << Register_name[rt_num] << endl;
			instruction_out = "xor" + Register_name[rd_num] + ',' + Register_name[rs_num] + ',' + Register_name[rt_num];
			outtype = 'R';
		}
		else if (func == "100111")//nor
		{
			Register[rd_num].used = 1;
			f << "nor " << Register_name[rd_num] << "," << Register_name[rs_num] << "," << Register_name[rt_num] << endl;
			instruction_out = "nor" + Register_name[rd_num] + ',' + Register_name[rs_num] + ',' + Register_name[rt_num];
			outtype = 'R';
		}
		else if (func == "000000")//sll
		{
			Register[rd_num].used = 1;
			f << "sll " << Register_name[rd_num] << "," << Register_name[rt_num] << "," << shamt_num << endl;
			instruction_out = "sll" + Register_name[rd_num] + ',' + Register_name[rt_num] + ',';
			outtype = 'R';
			special = 1;
			outimmediate = shamt_num;
		}
		else if (func == "000010")//srl
		{
			Register[rd_num].used = 1;
			f << "srl " << Register_name[rd_num] << "," << Register_name[rt_num] << "," << shamt_num << endl;
			instruction_out = "srl" + Register_name[rd_num] + ',' + Register_name[rt_num] + ',';
			outtype = 'R';
			special = 1;
			outimmediate = shamt_num;
		}
		else if (func == "001000")//jr
		{
			PC_Next = rs_data;
			f << "jr " << Register_name[rs_num] << endl;
			instruction_out = "jr" + Register_name[rs_num];
			outtype = 'R';
		}
	}
	//-----------------------------L-type-----------------------------------------------------------
	else if (type == 'L')
	{
		for (int i = 0; i < 5; i++)
		{
			rs.push_back(instruction[21 + i]);
			rs_num += (int(instruction[21 + i]) - 48)*pow(2, i);
			rt.push_back(instruction[16 + i]);
			rt_num += (int(instruction[16 + i]) - 48)*pow(2, i);
		}
		for (int i = 0; i < 16; i++)
		{
			immediate.push_back(instruction[i]);
			immediate_num += (int(instruction[i]) - 48)*pow(2, i);
		}

		if (Register[rs_num].used == 1)
		{
			rs_data = reg_result[rs_num];
		}
		else
		{
			rs_data = Register[rs_num].num;
		}

		if (op == "001000")//addi
		{
			Register[rt_num].used = 1;
			f << "addi " << Register_name[rt_num] << ',' << Register_name[rs_num] << ',' << immediate_num << endl;
			instruction_out = "addu" + Register_name[rt_num] + ',' + Register_name[rs_num] + ',';
			outtype = 'L';
			outimmediate = immediate_num;
		}
		else if (op == "001100")//andi
		{
			Register[rt_num].used = 1;
			f << "andi " << Register_name[rt_num] << ',' << Register_name[rs_num] << ',' << immediate_num << endl;
			instruction_out = "andi" + Register_name[rt_num] + ',' + Register_name[rs_num] + ',';
			outtype = 'L';
			outimmediate = immediate_num;
		}
		else if (op == "001101")//ori
		{
			Register[rt_num].used = 1;
			f << "ori " << Register_name[rt_num] << ',' << Register_name[rs_num] << ',' << immediate_num << endl;
			instruction_out = "ori" + Register_name[rt_num] + ',' + Register_name[rs_num] + ',';
			outtype = 'L';
			outimmediate = immediate_num;
		}
		else if (op == "001110")//xori
		{
			Register[rt_num].used = 1;
			f << "xori " << Register_name[rt_num] << ',' << Register_name[rs_num] << ',' << immediate_num << endl;
			instruction_out = "xori" + Register_name[rt_num] + ',' + Register_name[rs_num] + ',';
			outtype = 'L';
			outimmediate = immediate_num;
		}
		else if (op == "100011")//lw
		{
			Register[rt_num].used = 1;
			f << "lw " << Register_name[rt_num] << ',' << immediate_num << '(' << Register_name[rs_num] << ')' << endl;
			special = 1;
			rt = Register_name[rt_num];
			rs = Register_name[rs_num];
			outtype = 'L';
			outimmediate = immediate_num;

		}
		else if (op == "101011")//sw
		{
			if (Register[rt_num].used == 1)
			{
				rt_data = reg_result[rt_num];
			}
			else
			{
				rt_data = Register[rt_num].num;
			}
			f << "sw " << Register_name[rt_num] << ',' << immediate_num << '(' << Register_name[rs_num] << ')' << endl;
			special = 2;
			rt = Register_name[rt_num];
			rs = Register_name[rs_num];
			outtype = 'L';
			outimmediate = immediate_num;
		}
		else if (op == "000100")//beq
		{
			if (Register[rt_num].used == 1)
			{
				rt_data = reg_result[rt_num];
			}
			else
			{
				rt_data = Register[rt_num].num;
			}
			PC_Next = PC + immediate_num << 2;;
			f << "beq " << Register_name[rt_num] << ',' << Register_name[rs_num] << ',' << immediate_num << endl;
			instruction_out = "beq" + Register_name[rt_num] + ',' + Register_name[rs_num] + ',';
			outtype = 'L';
			outimmediate = immediate_num;
		}
	}
	//-----------------------------------------J-type----------------------------------------------
	else if (type == 'J')
	{
		for (int i = 0; i < 26; i++)
		{
			address.push_back(instruction[i]);
			address_num += (int(instruction[i]) - 48)*pow(2, i);
		}
		if (op == "000010")//j
		{
			PC_Next = address_num << 2;
			f << "j " << (address_num << 2) << endl;
			instruction_out = "j ";
			outtype = 'J';
			outimmediate = PC_Next;
		}
	}
	f.close();
}

//-------------------------------------------------------------------------------------------
//----------------------------------����׶�-------------------------------------------------
//-------------------------------------------------------------------------------------------
void Mips32_instruction::EX()
{
	//---------------------R-type--------------------
	if (type == 'R')
	{
		if (func == "100001")//addu
		{
			result = rs_data + rt_data;
			reg_result[rd_num] = result;
		}
		else if (func == "100011")//subu
		{
			result = rs_data - rt_data;
			reg_result[rd_num] = result;
		}
		else if (func == "100100")//and
		{
			result = rs_data & rt_data;
			reg_result[rd_num] = result;
		}
		else if (func == "100101")//or
		{
			result = rs_data | rt_data;
			reg_result[rd_num] = result;
		}
		else if (func == "100110")//xor
		{
			result = rs_data ^ rt_data;
			reg_result[rd_num] = result;
		}
		else if (func == "100111")//nor
		{
			result = ~(rs_data | rt_data);
			reg_result[rd_num] = result;
		}
		else if (func == "000000")//sll
		{
			result = rt_data << shamt_num;
			reg_result[rd_num] = result;
		}
		else if (func == "000010")//srl
		{
			result = rt_data >> shamt_num;
			reg_result[rd_num] = result;
		}
	}
	else if (type == 'L')
	{
		if (op == "001000")//addi
		{
			result = rs_data + immediate_num;
			reg_result[rt_num] = result;
		}
		else if (op == "001100")//andi
		{
			result = rs_data & immediate_num;
			reg_result[rt_num] = result;
		}
		else if (op == "001101")//ori
		{
			result = rs_data | immediate_num;
			reg_result[rt_num] = result;
		}
		else if (op == "001110")//xori
		{
			result = rs_data ^ immediate_num;
			reg_result[rt_num] = result;
		}
		else if (op == "100011")//lw
		{
			memory_address = rs_data + immediate_num;
			reg_result[rt_num] = Data_Memory[memory_address];
		}
		else if (op == "101011")//sw
		{
			memory_address = rs_data + immediate_num;
			result = rt_data;
		}
		else if (op == "000100")//beq
		{
			if (rs_data == rt_data)
			{
				add_result = PC + immediate_num << 2;
			}
		}
	}
	else if (type == 'J')
	{
		if (op == "000010")//j
		{
			add_result = address_num << 2;
		}
	}
}

//---------------------------------------------------------------------------------
//------------------------------------�ô�׶�-------------------------------------
//---------------------------------------------------------------------------------
void Mips32_instruction::MEM()
{
	if (type == 'L')
	{
		if (op == "100011")//lw
		{
			result = Data_Memory[memory_address];
		}
		else if (op == "101011")
		{
			Data_Memory[memory_address] = result;
		}
	}
}

//-------------------------------------------------------------
//-----------------------д�ؽ׶�------------------------------
//-------------------------------------------------------------

void Mips32_instruction::WB()
{
	//---------------------R-type--------------------
	if (type == 'R')
	{
		if (func == "100001")//addu
		{
			Register[rd_num].num = result;
			Register[rd_num].used = 0;
		}
		else if (func == "100011")//subu
		{
			Register[rd_num].num = result;
			Register[rd_num].used = 0;
		}
		else if (func == "100100")//and
		{
			Register[rd_num].num = result;
			Register[rd_num].used = 0;
		}
		else if (func == "100101")//or
		{
			Register[rd_num].num = result;
			Register[rd_num].used = 0;
		}
		else if (func == "100110")//xor
		{
			Register[rd_num].num = result;
			Register[rd_num].used = 0;
		}
		else if (func == "100111")//nor
		{
			Register[rd_num].num = result;
			Register[rd_num].used = 0;
		}
		else if (func == "000000")//sll
		{
			Register[rd_num].num = result;
			Register[rd_num].used = 0;
		}
		else if (func == "000010")//srl
		{
			Register[rd_num].num = result;
			Register[rd_num].used = 0;
		}
	}
	else if (type == 'L')
	{
		if (op == "001000")//addi
		{
			Register[rt_num].num = result;
			Register[rt_num].used = 0;
		}
		else if (op == "001100")//andi
		{
			Register[rt_num].num = result;
			Register[rt_num].used = 0;
		}
		else if (op == "001101")//ori
		{
			Register[rt_num].num = result;
			Register[rt_num].used = 0;
		}
		else if (op == "001110")//xori
		{
			Register[rt_num].num = result;
			Register[rt_num].used = 0;
		}
		else if (op == "100011")//lw
		{
			Register[rt_num].num = result;
			Register[rt_num].used = 0;
		}

	}
}
//-------------------------------------------------------------
//-------------------------��ˮ��------------------------------
//-------------------------------------------------------------
int start = 0;
void Pipeline::System_Run()
{
	instruction_out = "";
	int i, sign = 0;
	IF_Used = 0, ID_Used = 0, EX_Used = 0, MEM_Used = 0, WB_Used = 0;
	if (clock_cycle == 0)
		Task_Pro[0] = 0;
	if (clock_cycle == 1)
		Task_Pro[1] = 0;
	if (clock_cycle == 2)
		Task_Pro[2] = 0;
	if (clock_cycle == 3)
		Task_Pro[3] = 0;
	if (clock_cycle == 4)
		Task_Pro[4] = 0;
	for (int k = 0; k < 5; k++)
	{
		i = (start + k) % 5;
		switch (Task_Pro[i])
		{
		case 0://ȡָ
			if (IF_Used == 0)
			{
				if (Task[i].IF())
				{
					IF_Used = i + 1;
					Task_Pro[i] = (Task_Pro[i] + 1) % 5;
				}
			}
			break;
		case 1://����
			if (ID_Used == 0)
			{
				Task[i].ID();
				ID_Used = i + 1;
				Task_Pro[i] = (Task_Pro[i] + 1) % 5;
			}
			break;
		case 2://����
			if (EX_Used == 0)
			{
				Task[i].EX();
				EX_Used = i + 1;
				Task_Pro[i] = (Task_Pro[i] + 1) % 5;
			}
			break;
		case 3://�ô�
			if (MEM_Used == 0)
			{
				Task[i].MEM();
				MEM_Used = i + 1;
				Task_Pro[i] = (Task_Pro[i] + 1) % 5;
			}
			break;
		case 4://д��
			if (WB_Used == 0)
			{
				sign = 1;
				Task[i].WB();
				WB_Used = i + 1;
				Task_Pro[i] = (Task_Pro[i] + 1) % 5;
			}
			break;
		}
	}
	clock_cycle++;
	if (sign)
		start = (start + 1) % 5;
}
