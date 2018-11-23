#include"Mips32.h"
//-----------------------------------【WinMain( )函数】--------------------------------------
//	描述：Windows应用程序的入口函数，我们的程序从这里开始
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//【1】窗口创建四步曲之一：开始设计一个完整的窗口类
	WNDCLASSEX wndClass = { 0 };							//用WINDCLASSEX定义了一个窗口类
	wndClass.cbSize = sizeof(WNDCLASSEX);			//设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//设置窗口的样式
	wndClass.lpfnWndProc = WndProc;					//设置指向窗口过程函数的指针
	wndClass.cbClsExtra = 0;								//窗口类的附加内存，取0就可以了
	wndClass.cbWndExtra = 0;							//窗口的附加内存，依然取0就行了
	wndClass.hInstance = hInstance;						//指定包含窗口过程的程序的实例句柄。
														//wndClass.hIcon=(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //本地加载自定义ico图标
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //指定窗口类的光标句柄。
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //为hbrBackground成员指定一个白色画刷句柄	
	wndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
	wndClass.lpszClassName = L"Mips32_instruction";		//用一个以空终止的字符串，指定窗口类的名字。

														//【2】窗口创建四步曲之二：注册窗口类
	if (!RegisterClassEx(&wndClass))				//设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
		return -1;

	//【3】窗口创建四步曲之三：正式创建窗口
	HWND hwnd = CreateWindow(L"Mips32_instruction", WINDOW_TITLE,				//喜闻乐见的创建窗口函数CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//【4】窗口创建四步曲之四：窗口的移动、显示与更新
	MoveWindow(hwnd, 200, 20, WINDOW_WIDTH, WINDOW_HEIGHT, true);		//调整窗口显示时的位置，使窗口左上角位于（200,20）处
	ShowWindow(hwnd, nShowCmd);    //调用ShowWindow函数来显示窗口
	UpdateWindow(hwnd);						//对窗口进行更新，就像我们买了新房子要装修一样

											//游戏资源的初始化，若初始化失败，弹出一个消息框，并返回FALSE
	if (!BackGround_Paint(hwnd))
	{
		MessageBox(hwnd, L"资源初始化失败", L"消息窗口", 0); //使用MessageBox函数，创建一个消息窗口
		return FALSE;
	}
	MSG msg = { 0 };				//定义并初始化msg
	while (msg.message != WM_QUIT)		//使用while循环，如果消息不是WM_QUIT消息，就继续循环
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage(&msg);		//将虚拟键消息转换为字符消息
			DispatchMessage(&msg);			//分发一个消息给窗口程序。
		}

		//else if (!BackRefresh)		//如果未点击开始游戏，则主页循环刷新
		//{
		//	BackGround_CleanUp(hwnd);
		//	BackGround_Init(hwnd);
		//}
	}

	//【6】窗口类的注销
	UnregisterClass(L"Mips32_instruction", wndClass.hInstance);  //程序准备结束，注销窗口类
	return 0;
}

//-----------------------------------【WndProc( )函数】--------------------------------------
//	描述：窗口过程函数WndProc,对窗口消息进行处理
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)						//switch语句开始
	{

	case WM_CREATE:
	{
		//InitRegAndMem();
		Memory_get();
		for (int i = 0; i < 5; i++)
			P.Task_Pro[i] = -1;
		break;
	}
	case WM_KEYDOWN:					// 若是键盘按下消息
		switch (wParam)
		{
		case VK_ESCAPE: // 如果被按下的键是ESC
			BackGround_CleanUp(hwnd);								//调用自定义的资源清理函数Game_CleanUp（）进行退出前的资源清理
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);		//X坐标
		int y = HIWORD(lParam);		//Y坐标
		if (x > 170 && x < 290 && y>710 && y < 800)
		{
			InitRegAndMem();
			BackGround_CleanUp(hwnd);
			BackGround_Paint(hwnd);
		}
		else if (x > 450 && x < 570 && y>710 && y < 800)
		{
			if (!regis_init)
				MessageBox(hwnd, L"请先初始化寄存器和内存", L"提示", 0);
			else
			{
				instruction_read();
				BackGround_CleanUp(hwnd);
				BackGround_Paint(hwnd);
			}
		}
		else if (x > 750 && x < 870 && y>710 && y < 800)
		{
			if (!regis_init)
				MessageBox(hwnd, L"请先初始化寄存器和内存", L"提示", 0);
			else
			{
				instruction_next();
				BackGround_CleanUp(hwnd);
				BackGround_Paint(hwnd);
			}
		}
		break;
	}
	case WM_DESTROY:					//若是窗口销毁消息
		BackGround_CleanUp(hwnd);								//调用自定义的资源清理函数Game_CleanUp（）进行退出前的资源清理
		PostQuitMessage(0);			//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;									//跳出该switch语句
	default:										//若上述case条件都不符合，则执行该default语句
		return DefWindowProc(hwnd, message, wParam, lParam);		//调用缺省的窗口过程
	}

	return 0;									//正常退出
}

