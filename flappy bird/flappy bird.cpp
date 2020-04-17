#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<Windows.h>
#include<graphics.h>
#include<conio.h>
#include<mmsystem.h>
//// 引用 Windows Multimedia API
#pragma comment(lib,"Winmm.lib")
#pragma comment( lib, "MSIMG32.LIB")
#define PI 3.1415926
IMAGE image_bk,image_bk2,button,button2,img_welcome, img_start;//背景
IMAGE image_bird1, image_bird2,image_bird3, image_bird4;//鸟
IMAGE image_barup1, image_barup2, image_bardown1, image_bardown2;//柱
int height = 600,width = 350;;//边框尺寸
int i,j,l;
int k=0;
char input;//输入字符
int bird_x,bird_y;//鸟的位置
int bar_x, barup_y,bardown_y;//上下墙的位置
int length;//小孔长度
int hole_y;//孔的位置
int score;//得分
int state = 1;//游戏状态，1代表进行
int light1, light2;//鼠标放在上面，字变色
int wel = 1;
TCHAR b[5];
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	// 使用 Windows GDI 函数实现透明位图
	TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, transparentcolor);
}
void welcome()
{
	loadimage(&img_welcome, _T("..\\flappy bird图片音乐素材\\背景.png"));
	loadimage(&img_start, _T("..\\flappy bird图片音乐素材\\开始.png"));
	putimage(0, 0, &img_welcome);
	transparentimage(NULL, 25, 395, &img_start,BLACK);

	settextstyle(40, 0, _T("Blackoak Std"));
	settextcolor(RGB(0, 191, 255));
	TCHAR s[] = _T("FLAPPY");
	TCHAR c[] = _T("BIRD");
	outtextxy(15, 100, s);
	outtextxy(70, 200, c);

	FlushBatchDraw();
	mciSendString(_T("open \"..\\flappy bird图片音乐素材\\开始.mp3\" alias welmusic"), NULL, 0, NULL);//打开背景音乐
	mciSendString(_T("play welmusic repeat"), NULL, 0, NULL);  // 循环播放
	MOUSEMSG m;
	while (MouseHit())
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 25 && m.y >= 395 && m.x <= 325 && m.y <= 514)
		{
			wel = 0;
			mciSendString(_T("open \"..\\flappy bird图片音乐素材\\click.mp3\" alias clickmusic"), NULL, 0, NULL);//打开背景音乐
			mciSendString(_T("play clickmusic"), NULL, 0, NULL);  // 循环播放
			mciSendString(_T("close welmusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
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
{//初始位置
	length = 200;
	bird_x = 50;
	bird_y = height / 2;
	bar_x = width;
	hole_y = rand() % (height - length);
	barup_y = hole_y - 600;
	bardown_y = hole_y + length;
	score = 0;
	loadimage(&image_bk, _T("..\\flappy bird图片音乐素材\\background.jpg"));
	loadimage(&image_bk2, _T("..\\flappy bird图片音乐素材\\background_副本.jpg"));
	loadimage(&image_bird1, _T("..\\flappy bird图片音乐素材\\bird1.jpg"));
	loadimage(&image_bird2, _T("..\\flappy bird图片音乐素材\\bird2.jpg"));
	loadimage(&image_bird3, _T("..\\flappy bird图片音乐素材\\bird1 - 副本.jpg"));
	loadimage(&image_bird4, _T("..\\flappy bird图片音乐素材\\bird2 - 副本.jpg"));
	loadimage(&image_barup1, _T("..\\flappy bird图片音乐素材\\bar_up1.gif"));
	loadimage(&image_barup2, _T("..\\flappy bird图片音乐素材\\bar_up2.gif"));
	loadimage(&image_bardown1, _T("..\\flappy bird图片音乐素材\\bar_down1.gif"));
	loadimage(&image_bardown2, _T("..\\flappy bird图片音乐素材\\bar_down2.gif"));
	loadimage(&button2, _T("..\\flappy bird图片音乐素材\\按键.png"));
	mciSendString(_T("open \"..\\flappy bird图片音乐素材\\bgm.mp3\" alias bkmusic"), NULL, 0, NULL);//打开背景音乐
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);  // 循环播放

	light1 = light2 = 0;
}
void lose()//失败界面
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
	_stprintf_s(b, _T("%d"), score);		// 高版本 VC 推荐使用 _stprintf_s 函数
	settextstyle(60, 0, _T("Blackoak Std"));
	settextcolor(RGB(255, 69, 0));
	outtextxy(180, 160, b);
	transparentimage(NULL, 25,395, &button2, BLACK);
	transparentimage(NULL, 25,495, &button2, BLACK);
	settextstyle(60, 0, _T("方正粗黑宋简体"));
	settextcolor(RGB(220, 220, 220));
	TCHAR c[] = _T("从头再来");
	outtextxy(80, 400, c); 
	TCHAR d[] = _T("含泪离去");
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
			mciSendString(_T("close losemusic"), NULL, 0, NULL); // 仅播放一次
			mciSendString(_T("close clickmusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
			mciSendString(_T("open \"..\\flappy bird图片音乐素材\\click.mp3\" alias clickmusic"), NULL, 0, NULL); // 打开跳动音乐
			mciSendString(_T("play clickmusic"), NULL, 0, NULL); // 仅播放一次
			state = 1;
		}
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 25 && m.y >= 495 && m.x <= 325 && m.y <= 565)
		{
			mciSendString(_T("close losemusic"), NULL, 0, NULL); // 仅播放一次
			mciSendString(_T("close clickmusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
			mciSendString(_T("open \"..\\flappy bird图片音乐素材\\click.mp3\" alias clickmusic"), NULL, 0, NULL); // 打开跳动音乐
			mciSendString(_T("play clickmusic"), NULL, 0, NULL); // 仅播放一次
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
	//输出数值 1024，先将数字格式化输出为字符串 (VC2008 / VC2010 / VC2012)
	_stprintf_s(b, _T("%d"), score);// 高版本 VC 推荐使用 _stprintf_s 函数
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
			mciSendString(_T("close jpmusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
			mciSendString(_T("open \"..\\flappy bird图片音乐素材\\Jump.mp3\" alias jpmusic"), NULL, 0, NULL); // 打开跳动音乐
			mciSendString(_T("play jpmusic"), NULL, 0, NULL); // 仅播放一次
		}
	}
}
void updateWithoutInput()
{
	if (bird_y < height-24)
			bird_y++;
	Sleep(3);
	if (bar_x > -140)//向左移动
		bar_x -= 1;
	else
	{//移动到左壁后，从最右边重新开始
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
			mciSendString(_T("close bombmusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
			mciSendString(_T("open \"..\\flappy bird图片音乐素材\\撞击.mp3\" alias bombmusic"), NULL, 0, NULL); // 打开跳动音乐
			mciSendString(_T("play bombmusic"), NULL, 0, NULL); // 仅播放一次
			state = 0;
			mciSendString(_T("close bkmusic"), NULL, 0, NULL);
			mciSendString(_T("open \"..\\flappy bird图片音乐素材\\失败.mp3\" alias losemusic"), NULL, 0, NULL);//打开背景音乐
			mciSendString(_T("play losemusic"), NULL, 0, NULL); // 仅播放一次
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
	startup();//初始化参数
	while(1)
	{
		show();//显示
		updateWithInput();//和输入有关的更新
		updateWithoutInput();//和输入无关的更新
	}
	EndBatchDraw();
	_getch();
	closegraph();
}
