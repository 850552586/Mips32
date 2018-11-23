#include"Mips32.h"
//-----------------------------------��WinMain( )������--------------------------------------
//	������WindowsӦ�ó������ں��������ǵĳ�������￪ʼ
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
	wndClass.cbSize = sizeof(WNDCLASSEX);			//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;					//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra = 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra = 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
														//wndClass.hIcon=(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //ָ��������Ĺ������
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = L"Mips32_instruction";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

														//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	if (!RegisterClassEx(&wndClass))				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow(L"Mips32_instruction", WINDOW_TITLE,				//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd, 200, 20, WINDOW_WIDTH, WINDOW_HEIGHT, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�200,20����
	ShowWindow(hwnd, nShowCmd);    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

											//��Ϸ��Դ�ĳ�ʼ��������ʼ��ʧ�ܣ�����һ����Ϣ�򣬲�����FALSE
	if (!BackGround_Paint(hwnd))
	{
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0); //ʹ��MessageBox����������һ����Ϣ����
		return FALSE;
	}
	MSG msg = { 0 };				//���岢��ʼ��msg
	while (msg.message != WM_QUIT)		//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);			//�ַ�һ����Ϣ�����ڳ���
		}

		//else if (!BackRefresh)		//���δ�����ʼ��Ϸ������ҳѭ��ˢ��
		//{
		//	BackGround_CleanUp(hwnd);
		//	BackGround_Init(hwnd);
		//}
	}

	//��6���������ע��
	UnregisterClass(L"Mips32_instruction", wndClass.hInstance);  //����׼��������ע��������
	return 0;
}

//-----------------------------------��WndProc( )������--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)						//switch��俪ʼ
	{

	case WM_CREATE:
	{
		//InitRegAndMem();
		Memory_get();
		for (int i = 0; i < 5; i++)
			P.Task_Pro[i] = -1;
		break;
	}
	case WM_KEYDOWN:					// ���Ǽ��̰�����Ϣ
		switch (wParam)
		{
		case VK_ESCAPE: // ��������µļ���ESC
			BackGround_CleanUp(hwnd);								//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);		//X����
		int y = HIWORD(lParam);		//Y����
		if (x > 170 && x < 290 && y>710 && y < 800)
		{
			InitRegAndMem();
			BackGround_CleanUp(hwnd);
			BackGround_Paint(hwnd);
		}
		else if (x > 450 && x < 570 && y>710 && y < 800)
		{
			if (!regis_init)
				MessageBox(hwnd, L"���ȳ�ʼ���Ĵ������ڴ�", L"��ʾ", 0);
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
				MessageBox(hwnd, L"���ȳ�ʼ���Ĵ������ڴ�", L"��ʾ", 0);
			else
			{
				instruction_next();
				BackGround_CleanUp(hwnd);
				BackGround_Paint(hwnd);
			}
		}
		break;
	}
	case WM_DESTROY:					//���Ǵ���������Ϣ
		BackGround_CleanUp(hwnd);								//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage(0);			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;									//������switch���
	default:										//������case�����������ϣ���ִ�и�default���
		return DefWindowProc(hwnd, message, wParam, lParam);		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}

