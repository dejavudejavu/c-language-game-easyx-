#include <graphics.h>		// EasyX_20190219(beta)
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include<Mmsystem.h>
#include<iostream>
// 引用该库才能使用 AlphaBlend 函数
#pragma comment( lib, "MSIMG32.LIB")
#pragma comment(lib,"Winmm.lib")

using namespace std;
char input;
int i, j;
int photo_x=0, photo_y=79;//人物图片左上角
int girl_x, girl_y;
int b[20][30];//方块位置
int direction;//0向左；1向右
int k,walkgap;//走数帧画面活动一次
int jump_state;//1在跳跃过程中，0不在跳跃过程中
int bar_left,bar_right;//左右移动是否有障碍，0无，1有
int jump_t;//跳跃间隔帧数
int lift_y;//电梯坐标
int lift_state;//电梯状态，1上，-1下
int lift_start;//电梯是否启动，1启动
int lift_t;//电梯冷却时间
int beng_state;//是否在蹦床上跳跃
int stone_x;//石头坐标
int push_t;//推石延迟
int panda1_x, panda2_x;//两个熊猫坐标
int plane_x;//飞机坐标
int plane_t;//飞机间隔帧数
int move_t;//移动间隔帧数
int solid_t1,solid_t2,solid_t3,solid_t4,solid_t5;//固定间隔帧数
int down_x, down_y;//来不及解释了
int turn1, turn2;//坏人方向，0左，1右
int fire1, fire2;//开火
int bullet_r_x[30], bullet_r_y[30];//人发出的子弹坐标
int bullet_p1_x[30], bullet_p2_x[30];//熊猫的子弹坐标
int bullet_s_x[30];//射手的子弹坐标
int pos1,pos2,pos3,pos4;//子弹在数组中的位置
int fire_t;//子弹冷却帧数
int kill_t1, kill_t2, kill_t3;//敌人子弹间隔帧数
int pea_x[30];//豌豆坐标
int lifebar;
int girl_s,bad_s[3];//存在状态，0为存在1不存在
int shottime[3];//敌人中弹数
int collect;
int wel = 1;
int light1, light2;
int winn,losing;

IMAGE src, bk, brick, fire, plane, bad1, bad2, bad3, bad4, peashooter, lift, bounce, stone, wc;
IMAGE pea, flash, bullet_l, bullet_r, burger, furu, garlic, hami, lemon,lifeup,lifedown,start;
IMAGE button,winning,guang,loser;

void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, int a, int b, int w, int h)//带透明信息
{//xy相对于背景的位置，ab贴图的起始显示位置，wh显示贴图的宽和高
	HDC dstDC = GetImageHDC(dstimg);	
	HDC srcDC = GetImageHDC(srcimg);
	// 结构体的第三个成员表示额外的透明度，0 表示全透明，255 表示不透明。
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	// 使用 Windows GDI 函数实现半透明位图
	AlphaBlend(dstDC, x, y, w, h, srcDC, a, b, w, h, bf);
}
void transparentimagesp(IMAGE* dstimg, int x, int y, IMAGE* srcimg)//简易透明贴图函数
{//NULL,xy相对于背景的位置
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();

	// 结构体的第三个成员表示额外的透明度，0 表示全透明，255 表示不透明。
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	// 使用 Windows GDI 函数实现半透明位图
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

void bricks()
{//0不输出，1输出砖块，2输出大蒜，3输出，4输出机关枪，5输出飞机,6输出火,8输出电梯,
	//9输出蹦床，10输出wc
	for (i = 0;i < 20;i++)
	{
		for (j = 0;j < 30;j++)
		{
			if (b[i][j] == 1 )
				putimage(j * 30, i * 30, &brick);
			if (b[i][j] == 3)
				transparentimagesp(NULL,j * 30, i * 30, &furu);
			if (b[i][j] == 4)
				transparentimagesp(NULL,j * 30, i * 30, &lemon);
			if (b[i][j] == 5)
				transparentimagesp(NULL,j * 30, i * 30, &garlic);
			if (b[i][j] == 6)
				transparentimagesp(NULL,j * 30, i * 30, &burger);
			if (b[i][j] == 7)
				transparentimagesp(NULL,j * 30, i * 30, &hami);
		}	
	}
}

void startup()
{
	mciSendString(_T("open \".\\素材\\die.wav\" alias die"), NULL, 0, NULL);
	mciSendString(_T("open \".\\素材\\撞击.wav\" alias bombmusic"), NULL, 0, NULL);
	mciSendString(_T("open \".\\素材\\girl_die.wav\" alias girl"), NULL, 0, NULL);
	mciSendString(_T("open \".\\素材\\hami.wav\" alias hami"), NULL, 0, NULL);
	mciSendString(_T("open \".\\素材\\hanbao.wav\" alias hanbao"), NULL, 0, NULL);
	mciSendString(_T("open \".\\素材\\garlic.wav\" alias garlic"), NULL, 0, NULL);
	mciSendString(_T("open \".\\素材\\lemon.wav\" alias lemon"), NULL, 0, NULL);
	mciSendString(_T("open \".\\素材\\furu.wav\" alias furu"), NULL, 0, NULL);
	mciSendString(_T("open \".\\素材\\finish.wav\" alias finish"), NULL, 0, NULL);
	mciSendString(_T("open \".\\素材\\失败.wav\" alias shibai"), NULL, 0, NULL);


	loadimage(&bk, _T(".\\素材\\bk.png"));
	loadimage(&src, _T(".\\素材\\walk.png")); 
	loadimage(&brick, _T(".\\素材\\walls.png"));
	loadimage(&fire, _T(".\\素材\\fire.png"));
	loadimage(&plane, _T(".\\素材\\plane.png"));
	loadimage(&bad1, _T(".\\素材\\stand1.png"));
	loadimage(&bad2, _T(".\\素材\\攻击1.png"));
	loadimage(&bad3, _T(".\\素材\\stand2.png"));
	loadimage(&bad4, _T(".\\素材\\攻击2.png"));
	loadimage(&peashooter, _T(".\\素材\\peashooter.png"));
	loadimage(&lift, _T(".\\素材\\lift.png"));
	loadimage(&bounce, _T(".\\素材\\bounce.png"));
	loadimage(&wc, _T(".\\素材\\wc.png"));
	loadimage(&stone, _T(".\\素材\\stone.png"));
	loadimage(&pea, _T(".\\素材\\pea.png"));
	loadimage(&flash, _T(".\\素材\\flash.png"));
	loadimage(&bullet_l, _T(".\\素材\\bullet.png"));
	loadimage(&bullet_r, _T(".\\素材\\bullet_副本.png"));
	loadimage(&burger, _T(".\\素材\\burger.png"));
	loadimage(&hami, _T(".\\素材\\hami.png"));
	loadimage(&furu, _T(".\\素材\\furu.png"));
	loadimage(&lemon, _T(".\\素材\\lemon.png"));
	loadimage(&garlic, _T(".\\素材\\garlic.png"));
	loadimage(&lifeup, _T(".\\素材\\life_up.png"));
	loadimage(&lifedown, _T(".\\素材\\life_down.png"));
	loadimage(&start, _T(".\\素材\\开始.png"));
	loadimage(&button, _T(".\\素材\\按钮.png"));
	loadimage(&guang, _T(".\\素材\\guang.png"));
	loadimage(&winning, _T(".\\素材\\winning.png"));
	loadimage(&loser, _T(".\\素材\\失败.png"));

	girl_x = 0;
	girl_y = 480;
	photo_x = 0;
	photo_y = 79;
	direction = 1;//起始向右
	walkgap = 10;
	jump_state = 0;
	jump_t = 0;
	bar_left=bar_right = 0;
	lift_y = 330;
	lift_state = 1;
	lift_start = 0;
	lift_t = 0;
	beng_state = 0;
	stone_x = 150;
	push_t = 0;
	panda1_x = 270;
	panda2_x = 840;
	plane_x = 480;
	plane_t = 0;
	solid_t1 = 0;
	solid_t2 = 0;
	solid_t3 = 0;
	solid_t4 = 0;
	solid_t5 = 0;
	turn1 = 1;
	turn2 = 2;
	fire1 = fire2 = 0;
	pos1 = 0;
	fire_t = 0;
	kill_t1 = kill_t2 = kill_t3 = 0;
	lifebar = 240;
	move_t = 0;
	pos1 = pos2 = pos3 = pos4=0;
	girl_s = 0;
	bad_s[0]= bad_s[1]= bad_s[2]=0;//存在状态，0为存在1不存在
	shottime[0]= shottime[1]= shottime[2]=0;//敌人中弹数
	collect=0;
	light1 = 0;
	light2=0;
	winn = 0;
	losing = 0;
	for (i = 0;i < 30;i++)
	{
		bullet_r_x[i] = bullet_r_y[i] = bullet_p1_x[i] = bullet_p2_x[i] = bullet_s_x[i] = pea_x[i] = 0;
	}
	BeginBatchDraw();

	//初始化地图
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, ".\\素材\\map.dat", "r+");
	if (err == 0)
	{
		for (i = 0;i < 20;i++)
		{
			for (j = 0;j < 30;j++)
				fscanf_s(fp, "%d", &b[i][j]);
		}
	}
	else
		printf("ERROR:FAILED TO OPEN MAP.DAT");
	fclose(fp);
}
void show()
{
	cleardevice();
	putimage(0, 0, &bk);
	transparentimagesp(NULL, 0,0, &lifedown);
	transparentimage(NULL, 0, 0, &lifeup, 0, 0, lifebar, 30);
	//豌豆
	if (bad_s[2] == 0)
	{
		for (i = 0;i < 30;i++)
			if (pea_x[i] != 0)
				transparentimagesp(NULL, pea_x[i], 30, &pea);
	}

	//熊猫的子弹
	if (bad_s[0] == 0)
	{
		for (i = 0;i < 30;i++)
			if (bullet_p1_x[i] != 0)
				transparentimagesp(NULL, bullet_p1_x[i], 330, &flash);
	}
	if (bad_s[1] == 0)
	{
		for (i = 0;i < 30;i++)
			if (bullet_p2_x[i] != 0)
				transparentimagesp(NULL, bullet_p2_x[i], 240, &flash);
	}


	//人的子弹
	if (girl_s == 0)
	{
		for (i = 0;i < 30;i++)
			if (bullet_r_x[i] != 0)
			{
				if (bullet_r_x[i]<girl_x)
					transparentimagesp(NULL, bullet_r_x[i], bullet_r_y[i], &bullet_l);
				else
					transparentimagesp(NULL, bullet_r_x[i], bullet_r_y[i], &bullet_r);
			}
		transparentimage(NULL, girl_x, girl_y, &src, photo_x, photo_y, 30, 60);
	}


	transparentimagesp(NULL, plane_x, 420, &plane);
	bricks();
	setlinecolor(RGB(192, 192, 192));
	line(15, 210, 15, lift_y);
	line(45, 210, 45, lift_y);
	transparentimagesp(NULL, 0, lift_y, &lift);
	transparentimagesp(NULL, 840, 270, &bounce);
	transparentimagesp(NULL, 180, 540, &fire);
	if (bad_s[2] == 0)
	{
		transparentimagesp(NULL, 660, 30, &peashooter);
	}
	transparentimagesp(NULL, stone_x, 150, &stone);
	transparentimagesp(NULL, 840, 0, &wc);

	//熊猫
	if (bad_s[0] == 0)
	{
		if(turn1==1&&fire1==0)
			transparentimagesp(NULL, 270, 330, &bad1);
		else if (turn1 == 1 && fire1 == 1)
			transparentimagesp(NULL, 270, 330, &bad2);
		else if (turn1 ==0 && fire1 == 0)
			transparentimagesp(NULL, 270, 330, &bad3);
		else if(turn1 == 0 && fire1 == 1)
			transparentimagesp(NULL, 270, 330, &bad4);
	}
	if (bad_s[1] == 0)
	{
		transparentimagesp(NULL, panda2_x, 210, &bad4);
	}
	//文字提示
	if (girl_s == 0)
	{
		//settextcolor(RED);
		//if(girl)
	}
	if (girl_s == 0)
	{
		settextcolor(RED);
		setbkmode(TRANSPARENT);
		settextstyle(30, 0, _T("宋体"));
		if (girl_y <= 480 && girl_y >= 420 && girl_x >= 0 && girl_x <= 450)
		{
			TCHAR s[] = _T("注意飞机和火焰");
			outtextxy(240, 0, s);
		}
		else if (bad_s[0] == 0 && girl_y <= 360 && girl_y >= 300 && girl_x >= 510 && girl_x < 810 && fire1 == 0)
		{
			TCHAR s[] = _T("熊猫处于防御状态");
			outtextxy(240, 0, s);
		}
		else if(girl_y == lift_y && girl_x <= 30&&lift_start==0)
		{
			TCHAR s[] = _T("空格键启动电梯");
			outtextxy(240, 0, s);
		}
		else if (bad_s[1] == 0 && girl_y == 210&& girl_x >60 )
		{
			TCHAR s[] = _T("电梯可以防弹哦！");
			outtextxy(240, 0, s);
		}
		else if (girl_y == 210 && girl_x >= 840&&beng_state==0)
		{
			TCHAR s[] = _T("在蹦床上跳的更高");
			outtextxy(240, 0, s);
		}
		else if (girl_y <= 0 && girl_x >= 690 && collect < 5)
		{
			TCHAR s[] = _T("食物没有集齐！");
			outtextxy(240, 0, s);
		}
	}
	FlushBatchDraw();
}
void updatewithoutinput()
{
	int down_x, down_y, left_x, left_y, right_x, right_y, up_x,up_y;
	static int up1=0;//跳跃高度计数
	static int up2=0;
	if (solid_t1 < 5)
		solid_t1++;
	if(solid_t2<5)
		solid_t2++;
	if (solid_t3 < 5)
		solid_t3++;
	if (kill_t1 < 120)
		kill_t1++;
	if (kill_t2 < 120)
		kill_t2++;
	if (kill_t3 < 30)
		kill_t3++;
	//存活
	if (shottime[0] == 6&&bad_s[0]==0)
	{
		bad_s[0] = 1;
		mciSendString(_T("close die"), NULL, 0, NULL); // 仅播放一次
		mciSendString(_T("open \".\\素材\\die.wav\" alias die"), NULL, 0, NULL);
		mciSendString(_T("play die"), NULL, 0, NULL); // 仅播放一次
	}
	if (shottime[1] == 6 && bad_s[1] == 0)
	{
		bad_s[1] = 1;
		mciSendString(_T("close die"), NULL, 0, NULL); // 仅播放一次
		mciSendString(_T("open \".\\素材\\die.wav\" alias die"), NULL, 0, NULL);
		mciSendString(_T("play die"), NULL, 0, NULL); // 仅播放一次
	}

	if (shottime[2] == 10&&bad_s[2]==0)
		bad_s[2] = 1;
	//掉入火焰
	if (girl_x >= 180 && girl_x <= 210&&girl_y>=500 && girl_s == 0)
	{
		girl_s = 1;
		lifebar = 0;
		mciSendString(_T("close girl"), NULL, 0, NULL); // 仅播放一次
		mciSendString(_T("open \".\\素材\\girl_die.wav\" alias girl"), NULL, 0, NULL);
		mciSendString(_T("play girl"), NULL, 0, NULL); // 仅播放一次
	}
	//食物消失
	if (girl_x < 600 && girl_x > 540 && girl_y < 480&&girl_y>450&&b[15][19]!= 0)//furu
	{
		mciSendString(_T("close furu"), NULL, 0, NULL); // 仅播放一次
		mciSendString(_T("open \".\\素材\\furu.wav\" alias furu"), NULL, 0, NULL);
		mciSendString(_T("play furu"), NULL, 0, NULL); // 仅播放一次
		b[15][19] = 0;
		collect++;
	}
	if (girl_x < 510 && girl_x > 450 && girl_y < 330&&girl_y>300&& b[10][16] != 0)//lemon
	{
		mciSendString(_T("close lemon"), NULL, 0, NULL); // 仅播放一次
		mciSendString(_T("open \".\\素材\\lemon.wav\" alias lemon"), NULL, 0, NULL);
		mciSendString(_T("play lemon"), NULL, 0, NULL); // 仅播放一次
		b[10][16] = 0;
		collect++;
	}
	if (girl_x < 120 && girl_x > 60 && girl_y < 330&&girl_y>300&& b[10][3] != 0)//dasuan
	{
		mciSendString(_T("close garlic"), NULL, 0, NULL); // 仅播放一次
		mciSendString(_T("open \".\\素材\\garlic.wav\" alias garlic"), NULL, 0, NULL);
		mciSendString(_T("play garlic"), NULL, 0, NULL); // 仅播放一次
		b[10][3] = 0;
		collect++;
	}	
	if (girl_x < 810 && girl_x > 750 && girl_y < 240&&girl_y>210&& b[7][26] != 0)//hanbao
	{
		mciSendString(_T("close hanbao"), NULL, 0, NULL); // 仅播放一次
		mciSendString(_T("open \".\\素材\\hanbao.wav\" alias hanbao"), NULL, 0, NULL);
		mciSendString(_T("play hanbao"), NULL, 0, NULL); // 仅播放一次
		b[7][26] = 0;
		collect++;
	}
	if (girl_x == 480&&girl_y==0&&b[0][17]!= 0)//hami
	{
		mciSendString(_T("close hami"), NULL, 0, NULL); // 仅播放一次
		mciSendString(_T("open \".\\素材\\hami.wav\" alias hami"), NULL, 0, NULL);
		mciSendString(_T("play hami"), NULL, 0, NULL); // 仅播放一次
		b[0][17] = 0;
		collect++;
	}

	//人的子弹运动
	for (i = 0;i < 30;i++)
		if (bullet_r_x[i] != 0)
		{
			if (bullet_r_x[i] >= girl_x)
				bullet_r_x[i]++;
			else
				bullet_r_x[i]--;
			if (bullet_r_x[i] >900)
				bullet_r_x[i]=0;
			if (bad_s[0]==0&&bullet_r_y[i] < 360&& bullet_r_y[i]>300&& bullet_r_x[i]==295 &&fire1==1)
			{
				shottime[0]++;
				bullet_r_x[i]=0;
			}
			if (bad_s[1] == 0&&bullet_r_y[i] >=210&& bullet_r_y[i] <= 240 && bullet_r_x[i] == 795 )
			{
				shottime[1]++;
				bullet_r_x[i] = 0;
			}
			if ( bad_s[2] == 0&&bullet_r_y[i] < 60 && bullet_r_y[i]>0 && bullet_r_x[i] == 615)
			{
				shottime[2]++;
				bullet_r_x[i] = 0;
			}
		}
	//敌人子弹运动
	for (i = 0;i < 30;i++)
		if (bullet_p1_x[i] != 0)
		{
			if (bullet_p1_x[i] >= 270)
				bullet_p1_x[i]++;
			else
				bullet_p1_x[i]--;
			if (bullet_p1_x[i] > 900)
				bullet_p1_x[i] = 0;	
			if (bullet_p1_x[i] == girl_x + 15 && girl_y > 300 && 360 > girl_y&& girl_s == 0)
			{
				if (lifebar > 0)
				{
					lifebar -= 40;
					bullet_p1_x[i] = 0;
				}
				else
				{
					girl_s = 1;
					mciSendString(_T("close girl"), NULL, 0, NULL); // 仅播放一次
					mciSendString(_T("open \".\\素材\\girl_die.wav\" alias girl"), NULL, 0, NULL);
					mciSendString(_T("play girl"), NULL, 0, NULL); // 仅播放一次
				}

			}
		}	
	for (i = 0;i < 30;i++)
	{
		if (bullet_p2_x[i] != 0)
			bullet_p2_x[i]--;
		if (bullet_p2_x[i] == girl_x + 15 && girl_y >= 210 && 240 >= girl_y && girl_s == 0)
		{
			if (lifebar > 0)
			{
				if (!(girl_y == lift_y && girl_x <= 30))
				{
					bullet_p2_x[i] = 0;
					lifebar -= 40;
				}
			}			
			else
			{
				girl_s = 1;
				mciSendString(_T("close girl"), NULL, 0, NULL); // 仅播放一次
				mciSendString(_T("open \".\\素材\\girl_die.wav\" alias girl"), NULL, 0, NULL);
				mciSendString(_T("play girl"), NULL, 0, NULL); // 仅播放一次
			}

		}
	}

	for (i = 0;i < 30;i++)
	{
		if (pea_x[i] != 0)
			pea_x[i]--;
		if (pea_x[i] == girl_x + 15  &&30 >= girl_y&& girl_s == 0)
		{
			if (lifebar > 0)
			{
				lifebar -= 40;
				pea_x[i] = 0;
			}
			else
			{
				girl_s = 1;
				mciSendString(_T("close girl"), NULL, 0, NULL); // 仅播放一次
				mciSendString(_T("open \".\\素材\\girl_die.wav\" alias girl"), NULL, 0, NULL);
				mciSendString(_T("play girl"), NULL, 0, NULL); // 仅播放一次
			}
		}
	}

	//坏人转向
	if (girl_y < 360 && girl_y >= 298 && girl_x <= 510 && girl_x>30)
	{
		if (girl_x >= panda1_x)
			turn1 = 1;
		else
			turn1 = 0;
		fire1 = 1;
	}
	else
		fire1 = 0;
	if (girl_y ==210 && girl_x > 30)
		fire2 = 1;
	else
		fire2 = 0;
	//坏人开枪
	if (kill_t1 == 120)
	{
		if (bad_s[0]==0&&fire1 == 1)
		{
			if (turn1 == 1)
				bullet_p1_x[pos2] = 270;
			else
				bullet_p1_x[pos2] = 240;
			pos2++;
			if (pos2 == 30)
				pos2 = 0;
		}
		kill_t1 = 0;
	}
	if (kill_t2 == 120)
	{
		if (bad_s[1] == 0 && fire2 == 1)
		{
			bullet_p2_x[pos3] = panda2_x;
			pos3++;
			if (pos3 == 30)
				pos3 = 0;
		}
		kill_t2 = 0;
	}
	if (kill_t3 == 30)
	{
		if (bad_s[2] == 0 && girl_y <= 60)
		{
			pea_x[pos4] = 660;
			pos4++;
			if (pos4 == 30)
				pos4 = 0;
		}
		kill_t3 = 0;
	}

	//确定左右是否有障碍物
	bar_right=bar_left = 0;
	if (girl_x % 30 == 0)
	{
		left_x = girl_x/ 30-1;
		if (left_x < 0)
			left_x = 0;
		left_y = girl_y/30;
		right_x = girl_x / 30+1;
		right_y = girl_y / 30;
		if ((girl_y % 30 == 0 &&( b[right_y][right_x] != 0 || b[right_y + 1][right_x] != 0)) || (girl_y % 30 != 0 && (b[right_y][right_x] !=0 || b[right_y + 1][right_x] != 0 || b[right_y + 2][right_x] != 0)))
			bar_right = 1;
		if ((girl_y % 30 == 0 && (b[left_y][left_x] != 0 || b[left_y + 1][left_x] != 0)) || (girl_y % 30 != 0 && (b[left_y][left_x] != 0 || b[left_y + 1][left_x] != 0 || b[left_y + 2][left_x] != 0)))
			bar_left = 1;
	}
	//飞机运动
	if (plane_t < 5)
		plane_t++;
	if (plane_t == 5)
	{
		if (plane_x > -60)
			plane_x--;
		else
		{
			plane_x = 480;
		}
		plane_t = 0;
	}
	if (girl_y < 450 && girl_y>420 && girl_x + 30 > plane_x&& girl_x < plane_x + 60&&girl_s==0)
	{
		mciSendString(_T("close bombmusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
		mciSendString(_T("open \".\\素材\\撞击.wav\" alias bombmusic"), NULL, 0, NULL);
		mciSendString(_T("play bombmusic"), NULL, 0, NULL); // 仅播放一次
		girl_s = 1;
		lifebar = 0;
		mciSendString(_T("close girl"), NULL, 0, NULL); // 仅播放一次
		mciSendString(_T("open \".\\素材\\girl_die.wav\" alias girl"), NULL, 0, NULL);
		mciSendString(_T("play girl"), NULL, 0, NULL); // 仅播放一次
	}
	//动作：跳跃、乘电梯、自由落体、 蹦床	
	if (beng_state == 1)
	{
		if (solid_t1 == 5)
		{
			up2++;
			//Sleep(5);
			solid_t1 = 0;
			up_x = girl_x / 30;
			up_y = girl_y / 30;
			if (up2 <= 30)
			{
				if ((girl_x % 30 != 0 && b[up_y][up_x] == 0 && b[up_y][up_x + 1] == 0) || (girl_x % 30 == 0 && b[up_y][up_x] == 0))
					girl_y -= 3;
			}
			else
			{
				beng_state = 0;
				up2 = 0;
			}
		}
	}
	else if (girl_y==lift_y&&girl_x<=30)
	{
		if(lift_start==1)
			if(lift_state==1)
			{
				if (girl_y > 210)
				{
					girl_y--;
				}
				else
				{
					lift_start = 0;	
					lift_state = -lift_state;
				}
			}
			else
			{
				if (girl_y < 330)
				{
					girl_y++;
				}
				else
				{
					lift_start = 0;	
					lift_state = -lift_state;
				}
			}
		lift_y = girl_y;
	}
	else if (jump_state == 1)
	{
		if (solid_t2 == 5)
		{
			up1++;
			photo_x = 0;
			solid_t2 = 0;
			up_x = girl_x / 30;
			up_y = girl_y / 30;
			if (up1 <= 30)
			{
				if ((girl_x % 30 != 0 && b[up_y][up_x] == 0 && b[up_y][up_x + 1] == 0) || (girl_x % 30 == 0 && b[up_y][up_x] == 0))
					girl_y -= 2;
			}
			else
			{
				jump_state = 0;
				up1 = 0;
			}	
		}
	}
	else
	{
		down_x = girl_x/30;
		down_y = (girl_y + 60) / 30;
		if( (girl_x%30!=0&&b[down_y][down_x] == 0&&b[down_y][down_x+1]==0)||(girl_x%30==0&&b[down_y][down_x]==0))
		{
			if (!(girl_y==90&&girl_x>stone_x-30&&girl_x<stone_x+30))
			{
				if (solid_t3 == 5)
				{
		 			girl_y+=2;
					solid_t3=0;
				}
			}
		}
	}
	if (girl_s == 1)
	{
		losing = 1;
		Sleep(20);
		mciSendString(_T("close shibai"), NULL, 0, NULL); // 仅播放一次
		mciSendString(_T("close welmusic"), NULL, 0, NULL);  // 循环播放
		mciSendString(_T("open \".\\素材\\失败.wav\" alias shibai"), NULL, 0, NULL);
		mciSendString(_T("play shibai"), NULL, 0, NULL); // 仅播放一次
	}
}
void updatewithinput()
{
	if (fire_t < 60)
		fire_t++;
	down_x = girl_x / 30;
	down_y = (girl_y + 60) / 30;
	if (move_t < 5)
		move_t++;
	if (solid_t4 < 20)
		solid_t4++;
	if (solid_t5 < 20)
		solid_t5++;
	if ((girl_x % 30 != 0 && (b[down_y][down_x] != 0 || b[down_y][down_x + 1] != 0)) || (girl_x % 30 == 0 && b[down_y][down_x] != 0)||(girl_x==stone_x&&girl_y+60==150))
	{
		if ((GetAsyncKeyState(VK_UP) & 0x8000)&&girl_s==0)
		{
			PlaySound(TEXT(".\\素材\\tiao.wav"), NULL, SND_FILENAME | SND_ASYNC);
			if (girl_y == 210 && girl_x >= 840)
				beng_state = 1;
			else
				jump_state= 1;				
		}
		//jump_t = 0;
	}
	if (move_t == 5)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			if (girl_x == stone_x + 30 && girl_y == 120)
			{
				if (solid_t4 == 20)
				{
					solid_t4 = 0;
					if (stone_x > 60)
					{
						girl_x -= 1;
						stone_x = girl_x-30;
					//Sleep(20);
					}
				}
			}	
			else if(bar_left!=1&&girl_x>0)
				if(!(girl_x == stone_x + 30 && girl_y > 90 && girl_y < 120))
					girl_x -= 1;
			photo_y = 0;
			direction = 0;
			k++;
			if (walkgap == k)
			{
				photo_x += 49;
				if (photo_x >147)
					photo_x = 0;
				k = 0;
			}
		//Sleep(5);
		}		
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			if (girl_x == stone_x - 30 && girl_y == 120)
			{
				if (solid_t5 == 20)
				{
					solid_t5 = 0;
				if (stone_x <= 310)
				{
					girl_x += 1;
					stone_x = girl_x+30;
					//Sleep(20);
				}
				}
			}
			else if (bar_right != 1 && girl_x < 870)
				if (!(girl_x == stone_x - 30 && girl_y > 90 && girl_y < 120)) 
					girl_x += 1;
			photo_y = 79;
			direction = 1;
			k++;
			if (walkgap == k)
			{
				photo_x += 49;
				if (photo_x > 147)
				photo_x = 0;
				k = 0;
			}
		}
		move_t = 0;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (girl_y == lift_y && girl_x <= 30)
			lift_start = 1;
	}
	if (fire_t == 60&&girl_s==0)
	{
		fire_t = 0;
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if(direction==1)
				bullet_r_x[pos1] = girl_x;
			else
				bullet_r_x[pos1] = girl_x - 30;
			PlaySound(TEXT(".\\素材\\shoot.wav"), NULL, SND_FILENAME | SND_ASYNC);
			bullet_r_y[pos1] = girl_y ;
			if (pos1< 29)
				pos1++;
			else
				pos1 = 0;
		}
	}
	if (collect == 5 && girl_x >= 840 && girl_y <= 0)
	{
		winn = 1;
		mciSendString(_T("close welmusic"), NULL, 0, NULL);  // 循环播放
		mciSendString(_T("close finish"), NULL, 0, NULL); // 仅播放一次
		mciSendString(_T("open \".\\素材\\finish.wav\" alias finish"), NULL, 0, NULL);
		mciSendString(_T("play finish"), NULL, 0, NULL); // 仅播放一次
	}
}
//开始界面
void welcome()
{
	cleardevice();
	while (wel == 1)
	{
		putimage(0, 0, &start);
		transparentimagesp(NULL,580, 400, &button);//300*90
		transparentimagesp(NULL, 580, 500, &button);
		settextcolor(WHITE);
		setbkmode(TRANSPARENT);
		settextstyle(60, 0, _T("汉仪尚巍手书W"));
		TCHAR s[] = _T("开 始");
		outtextxy(670, 420, s);
		TCHAR c[] = _T("退 出");
		outtextxy(670, 520, c);
		if (light1 == 1)
		{
			settextcolor(RGB(255, 255, 0));
			outtextxy(670, 420, s);
		}
		if (light2 == 1)
		{
			settextcolor(RGB(255, 255, 0));
			outtextxy(670, 520, c);
		}
		FlushBatchDraw();
		MOUSEMSG m;
		while (MouseHit())
		{
			m = GetMouseMsg();
			if (m.x >= 580 && m.y >= 430 && m.x <= 880 && m.y <= 490)
				light1 = 1;
			else
				light1 = 0;
			if (m.x >= 580 && m.y >= 530 && m.x <= 880 && m.y <= 590)
				light2 = 1;
			else
				light2 = 0;

			if (m.uMsg == WM_LBUTTONDOWN && m.x >= 580 && m.y >= 430 && m.x <= 880 && m.y <= 490)//
			{
				startup();
				PlaySound(TEXT(".\\素材\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				mciSendString(_T("open \".\\素材\\bgm.mp3\" alias welmusic"), NULL, 0, NULL);//打开背景音乐
				mciSendString(_T("play welmusic repeat"), NULL, 0, NULL);  // 循环播放
				mciSendString(_T("close wel"), NULL, 0, NULL); // 仅播放一次
				wel = 0;
			}
			else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 580 && m.y >= 530 && m.x <= 880 && m.y <= 590)
			{
				PlaySound(TEXT(".\\素材\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				Sleep(500);
				exit(0);
			}
		}
	}
}
//成功界面
void win()
{
	while (winn == 1)
	{
		transparentimagesp(NULL, 200, 50, &guang);//500
		transparentimagesp(NULL, 350, 270, &winning);//200*60
		transparentimagesp(NULL, 300, 400, &button);//300*90
		transparentimagesp(NULL, 300, 500, &button);
		settextcolor(WHITE);
		setbkmode(TRANSPARENT);
		settextstyle(60, 0, _T("汉仪尚巍手书W"));
		TCHAR s[] = _T("主菜单");
		outtextxy(390, 420, s);
		TCHAR c[] = _T("退 出");
		outtextxy(390, 520, c);
		if (light1 == 1)
		{
			settextcolor(RGB(255, 255, 0));
			outtextxy(390, 420, s);
		}
		if (light2 == 1)
		{
			settextcolor(RGB(255, 255, 0));
			outtextxy(390, 520, c);
		}
		FlushBatchDraw();
		MOUSEMSG m;
		while (MouseHit())
		{
			m = GetMouseMsg();
			if (m.x >= 300 && m.y >= 430 && m.x <= 600 && m.y <= 490)
				light1 = 1;
			else
				light1 = 0;
			if (m.x >= 300 && m.y >= 530 && m.x <= 600 && m.y <= 590)
				light2 = 1;
			else
				light2 = 0;

			if (m.uMsg == WM_LBUTTONDOWN && m.x >= 300 && m.y >= 430 && m.x <= 600 && m.y <= 490)//
			{
				PlaySound(TEXT(".\\素材\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				winn = 0;
				wel = 1;
				mciSendString(_T("open \".\\素材\\开始.mp3\" alias wel"), NULL, 0, NULL);//打开背景音乐
				mciSendString(_T("play wel repeat"), NULL, 0, NULL);  // 循环播放
			}
			else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 300 && m.y >= 530 && m.x <= 600 && m.y <= 590)
			{
				PlaySound(TEXT(".\\素材\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				Sleep(500);
				exit(0);
			}
		}
	}
}
//失败界面
void lose()
{
	mciSendString(_T("close welmusic repeat"), NULL, 0, NULL);  // 循环播放
	while (losing == 1)
	{
		transparentimagesp(NULL, 350, 200, &loser);//200*130
		transparentimagesp(NULL, 300, 400, &button);//300*90
		transparentimagesp(NULL, 300, 500, &button);
		settextcolor(WHITE);
		setbkmode(TRANSPARENT);
		settextstyle(60, 0, _T("汉仪尚巍手书W"));
		TCHAR s[] = _T("主菜单");
		outtextxy(390, 420, s);
		TCHAR c[] = _T("退 出");
		outtextxy(390, 520, c);
		if (light1 == 1)
		{
			settextcolor(RGB(255, 255, 0));
			outtextxy(390, 420, s);
		}
		if (light2 == 1)
		{
			settextcolor(RGB(255, 255, 0));
			outtextxy(390, 520, c);
		}
		FlushBatchDraw();
		MOUSEMSG m;
		while (MouseHit())
		{
			m = GetMouseMsg();
			if (m.x >= 300 && m.y >= 430 && m.x <= 600 && m.y <= 490)
				light1 = 1;
			else
				light1 = 0;
			if (m.x >= 300 && m.y >= 530 && m.x <= 600 && m.y <= 590)
				light2 = 1;
			else
				light2 = 0;

			if (m.uMsg == WM_LBUTTONDOWN && m.x >= 300 && m.y >= 430 && m.x <= 600 && m.y <= 490)//
			{
				PlaySound(TEXT(".\\素材\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				losing = 0;
				wel = 1;
				mciSendString(_T("open \".\\素材\\开始.mp3\" alias wel"), NULL, 0, NULL);//打开背景音乐
				mciSendString(_T("play wel repeat"), NULL, 0, NULL);  // 循环播放
			}
			else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 300 && m.y >= 530 && m.x <= 600 && m.y <= 590)
			{
				PlaySound(TEXT(".\\素材\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				Sleep(500);
				exit(0);
			}
		}
	}
}
// 主函数
int main()
{
	initgraph(900, 600); // 初始化图形窗口 
	startup();
	mciSendString(_T("open \".\\素材\\开始.mp3\" alias wel"), NULL, 0, NULL);//打开背景音乐
	mciSendString(_T("play wel repeat"), NULL, 0, NULL);  // 循环播放
	while (1)
	{
		welcome();
		show();
		updatewithinput();
		updatewithoutinput();
		win();
		lose();
	}
	// 按任意键退出
	_getch();
	closegraph();
	return 0;
}