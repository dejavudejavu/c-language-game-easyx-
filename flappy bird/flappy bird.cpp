#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<Windows.h>
#include<graphics.h>
#include<conio.h>
#include<mmsystem.h>
//// ���� Windows Multimedia API
#pragma comment(lib,"Winmm.lib")
#pragma comment( lib, "MSIMG32.LIB")
#define PI 3.1415926
IMAGE image_bk,image_bk2,button,button2,img_welcome, img_start;//����
IMAGE image_bird1, image_bird2,image_bird3, image_bird4;//��
IMAGE image_barup1, image_barup2, image_bardown1, image_bardown2;//��
int height = 600,width = 350;;//�߿�ߴ�
int i,j,l;
int k=0;
char input;//�����ַ�
int bird_x,bird_y;//���λ��
int bar_x, barup_y,bardown_y;//����ǽ��λ��
int length;//С�׳���
int hole_y;//�׵�λ��
int score;//�÷�
int state = 1;//��Ϸ״̬��1�������
int light1, light2;//���������棬�ֱ�ɫ
int wel = 1;
TCHAR b[5];
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	// ʹ�� Windows GDI ����ʵ��͸��λͼ
	TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, transparentcolor);
}
void welcome()
{
	loadimage(&img_welcome, _T("..\\flappy birdͼƬ�����ز�\\����.png"));
	loadimage(&img_start, _T("..\\flappy birdͼƬ�����ز�\\��ʼ.png"));
	putimage(0, 0, &img_welcome);
	transparentimage(NULL, 25, 395, &img_start,BLACK);

	settextstyle(40, 0, _T("Blackoak Std"));
	settextcolor(RGB(0, 191, 255));
	TCHAR s[] = _T("FLAPPY");
	TCHAR c[] = _T("BIRD");
	outtextxy(15, 100, s);
	outtextxy(70, 200, c);

	FlushBatchDraw();
	mciSendString(_T("open \"..\\flappy birdͼƬ�����ز�\\��ʼ.mp3\" alias welmusic"), NULL, 0, NULL);//�򿪱�������
	mciSendString(_T("play welmusic repeat"), NULL, 0, NULL);  // ѭ������
	MOUSEMSG m;
	while (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 25 && m.y >= 395 && m.x <= 325 && m.y <= 514)
		{
			wel = 0;
			mciSendString(_T("open \"..\\flappy birdͼƬ�����ز�\\click.mp3\" alias clickmusic"), NULL, 0, NULL);//�򿪱�������
			mciSendString(_T("play clickmusic"), NULL, 0, NULL);  // ѭ������
			mciSendString(_T("close welmusic"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
		}

	}
}
void fall()
{
		while (1)
		{
			cleardevice();
			putimage(0, 0, &image_bk2);
			//transparentimage(NULL, bird_x, bird_y, &image_bird4, WHITE);
			putimage(bird_x, bird_y, &image_bird3, NOTSRCERASE);
			putimage(bird_x, bird_y, &image_bird4, SRCINVERT);
			putimage(bar_x, barup_y, &image_barup1, NOTSRCERASE);
			putimage(bar_x, barup_y, &image_barup2, SRCINVERT);
			putimage(bar_x, bardown_y, &image_bardown1, NOTSRCERASE);
			putimage(bar_x, bardown_y, &image_bardown2, SRCINVERT);
			if (bird_x == bar_x - 35)
			{
				if (bird_y <= height - 24)
					bird_y++;
				else
					break;
			}
			else
			{
				if (bird_y <= hole_y + length - 24)
					bird_y++;
				else
					break;
			}
			FlushBatchDraw();
			Sleep(1);
		}

}
void startup()
{//��ʼλ��
	length = 200;
	bird_x = 50;
	bird_y = height / 2;
	bar_x = width;
	hole_y = rand() % (height - length);
	barup_y = hole_y - 600;
	bardown_y = hole_y + length;
	score = 0;
	loadimage(&image_bk, _T("..\\flappy birdͼƬ�����ز�\\background.jpg"));
	loadimage(&image_bk2, _T("..\\flappy birdͼƬ�����ز�\\background_����.jpg"));
	loadimage(&image_bird1, _T("..\\flappy birdͼƬ�����ز�\\bird1.jpg"));
	loadimage(&image_bird2, _T("..\\flappy birdͼƬ�����ز�\\bird2.jpg"));
	loadimage(&image_bird3, _T("..\\flappy birdͼƬ�����ز�\\bird1 - ����.jpg"));
	loadimage(&image_bird4, _T("..\\flappy birdͼƬ�����ز�\\bird2 - ����.jpg"));
	loadimage(&image_barup1, _T("..\\flappy birdͼƬ�����ز�\\bar_up1.gif"));
	loadimage(&image_barup2, _T("..\\flappy birdͼƬ�����ز�\\bar_up2.gif"));
	loadimage(&image_bardown1, _T("..\\flappy birdͼƬ�����ز�\\bar_down1.gif"));
	loadimage(&image_bardown2, _T("..\\flappy birdͼƬ�����ز�\\bar_down2.gif"));
	loadimage(&button2, _T("..\\flappy birdͼƬ�����ز�\\����.png"));
	mciSendString(_T("open \"..\\flappy birdͼƬ�����ز�\\bgm.mp3\" alias bkmusic"), NULL, 0, NULL);//�򿪱�������
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);  // ѭ������

	light1 = light2 = 0;
}
void lose()//ʧ�ܽ���
{
	fall();
	settextstyle(30, 0, _T("Blackoak Std"));
	settextcolor(WHITE);
	TCHAR s[] = _T("GAME OVER");
	outtextxy(0, 100, s);
	settextstyle(30, 0, _T("Arial Black"));
	settextcolor(BLACK);
	TCHAR a[] = _T("PNTS");
	outtextxy(100, 180, a);
	_stprintf_s(b, _T("%d"), score);		// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
	settextstyle(60, 0, _T("Blackoak Std"));
	settextcolor(RGB(255, 69, 0));
	outtextxy(180, 160, b);
	transparentimage(NULL, 25,395, &button2, BLACK);
	transparentimage(NULL, 25,495, &button2, BLACK);
	settextstyle(60, 0, _T("�����ֺ��μ���"));
	settextcolor(RGB(220, 220, 220));
	TCHAR c[] = _T("��ͷ����");
	outtextxy(80, 400, c); 
	TCHAR d[] = _T("������ȥ");
	outtextxy(80, 500, d);
	if (light1 == 1)
	{
		settextcolor(RGB(255, 255, 0));
		outtextxy(80, 400, c);
	}
	if (light2 == 1)
	{
		settextcolor(RGB(255, 255, 0));
		outtextxy(80, 500, d);
	}
	FlushBatchDraw();
	MOUSEMSG m;
	while (MouseHit())
	{
		m = GetMouseMsg();
		if (m.x >= 25 && m.y >= 395 && m.x <= 325 && m.y <= 465)
			light1 = 1;
		else
			light1 = 0;
		if (m.x >= 25 && m.y >= 495 && m.x <= 325 && m.y <= 565)
			light2 = 1;
		else
			light2 = 0;

		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 25 && m.y >= 395 && m.x <= 325 && m.y <= 465)//
		{
			startup();
			mciSendString(_T("close losemusic"), NULL, 0, NULL); // ������һ��
			mciSendString(_T("close clickmusic"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
			mciSendString(_T("open \"..\\flappy birdͼƬ�����ز�\\click.mp3\" alias clickmusic"), NULL, 0, NULL); // ����������
			mciSendString(_T("play clickmusic"), NULL, 0, NULL); // ������һ��
			state = 1;
		}
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 25 && m.y >= 495 && m.x <= 325 && m.y <= 565)
		{
			mciSendString(_T("close losemusic"), NULL, 0, NULL); // ������һ��
			mciSendString(_T("close clickmusic"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
			mciSendString(_T("open \"..\\flappy birdͼƬ�����ز�\\click.mp3\" alias clickmusic"), NULL, 0, NULL); // ����������
			mciSendString(_T("play clickmusic"), NULL, 0, NULL); // ������һ��
			Sleep(500);
			exit(0);
		}
	}
}
void show()
{
	while (state == 0)
	{
		lose();
	}
	putimage(0, 0, &image_bk);
	setbkcolor(BLACK);
	settextstyle(60, 0, _T("Broadway"));
	putimage(bird_x, bird_y, &image_bird1, NOTSRCERASE);
	putimage(bird_x, bird_y, &image_bird2, SRCINVERT);
	putimage(bar_x, barup_y, &image_barup1, NOTSRCERASE);
	putimage(bar_x, barup_y, &image_barup2, SRCINVERT);
	putimage(bar_x, bardown_y, &image_bardown1, NOTSRCERASE);
	putimage(bar_x, bardown_y, &image_bardown2, SRCINVERT);
	//putimage(0, 0, &button, NOTSRCERASE);
	//putimage(0, 0, &button2, SRCINVERT);
	//�����ֵ 1024���Ƚ����ָ�ʽ�����Ϊ�ַ��� (VC2008 / VC2010 / VC2012)
	_stprintf_s(b, _T("%d"), score);// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
	settextcolor(WHITE);
	outtextxy(250, 30, b);
	FlushBatchDraw();
}
void updateWithInput()
{
	if (_kbhit())
	{
		input =_getch();
		if (input == ' ' && bird_y > 1)
		{
			bird_y-=80;
			mciSendString(_T("close jpmusic"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
			mciSendString(_T("open \"..\\flappy birdͼƬ�����ز�\\Jump.mp3\" alias jpmusic"), NULL, 0, NULL); // ����������
			mciSendString(_T("play jpmusic"), NULL, 0, NULL); // ������һ��
		}
	}
}
void updateWithoutInput()
{
	if (bird_y < height-24)
			bird_y++;
	Sleep(3);
	if (bar_x > -140)//�����ƶ�
		bar_x -= 1;
	else
	{//�ƶ�����ں󣬴����ұ����¿�ʼ
		bar_x= width;
		hole_y= rand() % (height - length);
	}
	k = 0;
	if (bird_x == bar_x+140)
		score++;
	barup_y = hole_y - 600;
	bardown_y = hole_y + length;
	if (bar_x - 35 <= bird_x && bird_x <= bar_x + 140)
		if (bird_y <= hole_y || bird_y + 24 >= hole_y + length)
		{
			mciSendString(_T("close bombmusic"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
			mciSendString(_T("open \"..\\flappy birdͼƬ�����ز�\\ײ��.mp3\" alias bombmusic"), NULL, 0, NULL); // ����������
			mciSendString(_T("play bombmusic"), NULL, 0, NULL); // ������һ��
			state = 0;
			mciSendString(_T("close bkmusic"), NULL, 0, NULL);
			mciSendString(_T("open \"..\\flappy birdͼƬ�����ز�\\ʧ��.mp3\" alias losemusic"), NULL, 0, NULL);//�򿪱�������
			mciSendString(_T("play losemusic"), NULL, 0, NULL); // ������һ��
		}
}
int main()
{
	initgraph(width, height);
	BeginBatchDraw();
	setbkmode(TRANSPARENT);
	while (wel == 1)
	{
		welcome();
	}
	startup();//��ʼ������
	while(1)
	{
		show();//��ʾ
		updateWithInput();//�������йصĸ���
		updateWithoutInput();//�������޹صĸ���
	}
	EndBatchDraw();
	_getch();
	closegraph();
}
