#include <graphics.h>		// EasyX_20190219(beta)
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include<Mmsystem.h>
#include<iostream>
// ���øÿ����ʹ�� AlphaBlend ����
#pragma comment( lib, "MSIMG32.LIB")
#pragma comment(lib,"Winmm.lib")

using namespace std;
char input;
int i, j;
int photo_x=0, photo_y=79;//����ͼƬ���Ͻ�
int girl_x, girl_y;
int b[20][30];//����λ��
int direction;//0����1����
int k,walkgap;//����֡����һ��
int jump_state;//1����Ծ�����У�0������Ծ������
int bar_left,bar_right;//�����ƶ��Ƿ����ϰ���0�ޣ�1��
int jump_t;//��Ծ���֡��
int lift_y;//��������
int lift_state;//����״̬��1�ϣ�-1��
int lift_start;//�����Ƿ�������1����
int lift_t;//������ȴʱ��
int beng_state;//�Ƿ��ڱĴ�����Ծ
int stone_x;//ʯͷ����
int push_t;//��ʯ�ӳ�
int panda1_x, panda2_x;//������è����
int plane_x;//�ɻ�����
int plane_t;//�ɻ����֡��
int move_t;//�ƶ����֡��
int solid_t1,solid_t2,solid_t3,solid_t4,solid_t5;//�̶����֡��
int down_x, down_y;//������������
int turn1, turn2;//���˷���0��1��
int fire1, fire2;//����
int bullet_r_x[30], bullet_r_y[30];//�˷������ӵ�����
int bullet_p1_x[30], bullet_p2_x[30];//��è���ӵ�����
int bullet_s_x[30];//���ֵ��ӵ�����
int pos1,pos2,pos3,pos4;//�ӵ��������е�λ��
int fire_t;//�ӵ���ȴ֡��
int kill_t1, kill_t2, kill_t3;//�����ӵ����֡��
int pea_x[30];//�㶹����
int lifebar;
int girl_s,bad_s[3];//����״̬��0Ϊ����1������
int shottime[3];//�����е���
int collect;
int wel = 1;
int light1, light2;
int winn,losing;

IMAGE src, bk, brick, fire, plane, bad1, bad2, bad3, bad4, peashooter, lift, bounce, stone, wc;
IMAGE pea, flash, bullet_l, bullet_r, burger, furu, garlic, hami, lemon,lifeup,lifedown,start;
IMAGE button,winning,guang,loser;

void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, int a, int b, int w, int h)//��͸����Ϣ
{//xy����ڱ�����λ�ã�ab��ͼ����ʼ��ʾλ�ã�wh��ʾ��ͼ�Ŀ�͸�
	HDC dstDC = GetImageHDC(dstimg);	
	HDC srcDC = GetImageHDC(srcimg);
	// �ṹ��ĵ�������Ա��ʾ�����͸���ȣ�0 ��ʾȫ͸����255 ��ʾ��͸����
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	// ʹ�� Windows GDI ����ʵ�ְ�͸��λͼ
	AlphaBlend(dstDC, x, y, w, h, srcDC, a, b, w, h, bf);
}
void transparentimagesp(IMAGE* dstimg, int x, int y, IMAGE* srcimg)//����͸����ͼ����
{//NULL,xy����ڱ�����λ��
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();

	// �ṹ��ĵ�������Ա��ʾ�����͸���ȣ�0 ��ʾȫ͸����255 ��ʾ��͸����
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	// ʹ�� Windows GDI ����ʵ�ְ�͸��λͼ
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

void bricks()
{//0�������1���ש�飬2������⣬3�����4�������ǹ��5����ɻ�,6�����,8�������,
	//9����Ĵ���10���wc
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
	mciSendString(_T("open \".\\�ز�\\die.wav\" alias die"), NULL, 0, NULL);
	mciSendString(_T("open \".\\�ز�\\ײ��.wav\" alias bombmusic"), NULL, 0, NULL);
	mciSendString(_T("open \".\\�ز�\\girl_die.wav\" alias girl"), NULL, 0, NULL);
	mciSendString(_T("open \".\\�ز�\\hami.wav\" alias hami"), NULL, 0, NULL);
	mciSendString(_T("open \".\\�ز�\\hanbao.wav\" alias hanbao"), NULL, 0, NULL);
	mciSendString(_T("open \".\\�ز�\\garlic.wav\" alias garlic"), NULL, 0, NULL);
	mciSendString(_T("open \".\\�ز�\\lemon.wav\" alias lemon"), NULL, 0, NULL);
	mciSendString(_T("open \".\\�ز�\\furu.wav\" alias furu"), NULL, 0, NULL);
	mciSendString(_T("open \".\\�ز�\\finish.wav\" alias finish"), NULL, 0, NULL);
	mciSendString(_T("open \".\\�ز�\\ʧ��.wav\" alias shibai"), NULL, 0, NULL);


	loadimage(&bk, _T(".\\�ز�\\bk.png"));
	loadimage(&src, _T(".\\�ز�\\walk.png")); 
	loadimage(&brick, _T(".\\�ز�\\walls.png"));
	loadimage(&fire, _T(".\\�ز�\\fire.png"));
	loadimage(&plane, _T(".\\�ز�\\plane.png"));
	loadimage(&bad1, _T(".\\�ز�\\stand1.png"));
	loadimage(&bad2, _T(".\\�ز�\\����1.png"));
	loadimage(&bad3, _T(".\\�ز�\\stand2.png"));
	loadimage(&bad4, _T(".\\�ز�\\����2.png"));
	loadimage(&peashooter, _T(".\\�ز�\\peashooter.png"));
	loadimage(&lift, _T(".\\�ز�\\lift.png"));
	loadimage(&bounce, _T(".\\�ز�\\bounce.png"));
	loadimage(&wc, _T(".\\�ز�\\wc.png"));
	loadimage(&stone, _T(".\\�ز�\\stone.png"));
	loadimage(&pea, _T(".\\�ز�\\pea.png"));
	loadimage(&flash, _T(".\\�ز�\\flash.png"));
	loadimage(&bullet_l, _T(".\\�ز�\\bullet.png"));
	loadimage(&bullet_r, _T(".\\�ز�\\bullet_����.png"));
	loadimage(&burger, _T(".\\�ز�\\burger.png"));
	loadimage(&hami, _T(".\\�ز�\\hami.png"));
	loadimage(&furu, _T(".\\�ز�\\furu.png"));
	loadimage(&lemon, _T(".\\�ز�\\lemon.png"));
	loadimage(&garlic, _T(".\\�ز�\\garlic.png"));
	loadimage(&lifeup, _T(".\\�ز�\\life_up.png"));
	loadimage(&lifedown, _T(".\\�ز�\\life_down.png"));
	loadimage(&start, _T(".\\�ز�\\��ʼ.png"));
	loadimage(&button, _T(".\\�ز�\\��ť.png"));
	loadimage(&guang, _T(".\\�ز�\\guang.png"));
	loadimage(&winning, _T(".\\�ز�\\winning.png"));
	loadimage(&loser, _T(".\\�ز�\\ʧ��.png"));

	girl_x = 0;
	girl_y = 480;
	photo_x = 0;
	photo_y = 79;
	direction = 1;//��ʼ����
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
	bad_s[0]= bad_s[1]= bad_s[2]=0;//����״̬��0Ϊ����1������
	shottime[0]= shottime[1]= shottime[2]=0;//�����е���
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

	//��ʼ����ͼ
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, ".\\�ز�\\map.dat", "r+");
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
	//�㶹
	if (bad_s[2] == 0)
	{
		for (i = 0;i < 30;i++)
			if (pea_x[i] != 0)
				transparentimagesp(NULL, pea_x[i], 30, &pea);
	}

	//��è���ӵ�
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


	//�˵��ӵ�
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

	//��è
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
	//������ʾ
	if (girl_s == 0)
	{
		//settextcolor(RED);
		//if(girl)
	}
	if (girl_s == 0)
	{
		settextcolor(RED);
		setbkmode(TRANSPARENT);
		settextstyle(30, 0, _T("����"));
		if (girl_y <= 480 && girl_y >= 420 && girl_x >= 0 && girl_x <= 450)
		{
			TCHAR s[] = _T("ע��ɻ��ͻ���");
			outtextxy(240, 0, s);
		}
		else if (bad_s[0] == 0 && girl_y <= 360 && girl_y >= 300 && girl_x >= 510 && girl_x < 810 && fire1 == 0)
		{
			TCHAR s[] = _T("��è���ڷ���״̬");
			outtextxy(240, 0, s);
		}
		else if(girl_y == lift_y && girl_x <= 30&&lift_start==0)
		{
			TCHAR s[] = _T("�ո����������");
			outtextxy(240, 0, s);
		}
		else if (bad_s[1] == 0 && girl_y == 210&& girl_x >60 )
		{
			TCHAR s[] = _T("���ݿ��Է���Ŷ��");
			outtextxy(240, 0, s);
		}
		else if (girl_y == 210 && girl_x >= 840&&beng_state==0)
		{
			TCHAR s[] = _T("�ڱĴ������ĸ���");
			outtextxy(240, 0, s);
		}
		else if (girl_y <= 0 && girl_x >= 690 && collect < 5)
		{
			TCHAR s[] = _T("ʳ��û�м��룡");
			outtextxy(240, 0, s);
		}
	}
	FlushBatchDraw();
}
void updatewithoutinput()
{
	int down_x, down_y, left_x, left_y, right_x, right_y, up_x,up_y;
	static int up1=0;//��Ծ�߶ȼ���
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
	//���
	if (shottime[0] == 6&&bad_s[0]==0)
	{
		bad_s[0] = 1;
		mciSendString(_T("close die"), NULL, 0, NULL); // ������һ��
		mciSendString(_T("open \".\\�ز�\\die.wav\" alias die"), NULL, 0, NULL);
		mciSendString(_T("play die"), NULL, 0, NULL); // ������һ��
	}
	if (shottime[1] == 6 && bad_s[1] == 0)
	{
		bad_s[1] = 1;
		mciSendString(_T("close die"), NULL, 0, NULL); // ������һ��
		mciSendString(_T("open \".\\�ز�\\die.wav\" alias die"), NULL, 0, NULL);
		mciSendString(_T("play die"), NULL, 0, NULL); // ������һ��
	}

	if (shottime[2] == 10&&bad_s[2]==0)
		bad_s[2] = 1;
	//�������
	if (girl_x >= 180 && girl_x <= 210&&girl_y>=500 && girl_s == 0)
	{
		girl_s = 1;
		lifebar = 0;
		mciSendString(_T("close girl"), NULL, 0, NULL); // ������һ��
		mciSendString(_T("open \".\\�ز�\\girl_die.wav\" alias girl"), NULL, 0, NULL);
		mciSendString(_T("play girl"), NULL, 0, NULL); // ������һ��
	}
	//ʳ����ʧ
	if (girl_x < 600 && girl_x > 540 && girl_y < 480&&girl_y>450&&b[15][19]!= 0)//furu
	{
		mciSendString(_T("close furu"), NULL, 0, NULL); // ������һ��
		mciSendString(_T("open \".\\�ز�\\furu.wav\" alias furu"), NULL, 0, NULL);
		mciSendString(_T("play furu"), NULL, 0, NULL); // ������һ��
		b[15][19] = 0;
		collect++;
	}
	if (girl_x < 510 && girl_x > 450 && girl_y < 330&&girl_y>300&& b[10][16] != 0)//lemon
	{
		mciSendString(_T("close lemon"), NULL, 0, NULL); // ������һ��
		mciSendString(_T("open \".\\�ز�\\lemon.wav\" alias lemon"), NULL, 0, NULL);
		mciSendString(_T("play lemon"), NULL, 0, NULL); // ������һ��
		b[10][16] = 0;
		collect++;
	}
	if (girl_x < 120 && girl_x > 60 && girl_y < 330&&girl_y>300&& b[10][3] != 0)//dasuan
	{
		mciSendString(_T("close garlic"), NULL, 0, NULL); // ������һ��
		mciSendString(_T("open \".\\�ز�\\garlic.wav\" alias garlic"), NULL, 0, NULL);
		mciSendString(_T("play garlic"), NULL, 0, NULL); // ������һ��
		b[10][3] = 0;
		collect++;
	}	
	if (girl_x < 810 && girl_x > 750 && girl_y < 240&&girl_y>210&& b[7][26] != 0)//hanbao
	{
		mciSendString(_T("close hanbao"), NULL, 0, NULL); // ������һ��
		mciSendString(_T("open \".\\�ز�\\hanbao.wav\" alias hanbao"), NULL, 0, NULL);
		mciSendString(_T("play hanbao"), NULL, 0, NULL); // ������һ��
		b[7][26] = 0;
		collect++;
	}
	if (girl_x == 480&&girl_y==0&&b[0][17]!= 0)//hami
	{
		mciSendString(_T("close hami"), NULL, 0, NULL); // ������һ��
		mciSendString(_T("open \".\\�ز�\\hami.wav\" alias hami"), NULL, 0, NULL);
		mciSendString(_T("play hami"), NULL, 0, NULL); // ������һ��
		b[0][17] = 0;
		collect++;
	}

	//�˵��ӵ��˶�
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
	//�����ӵ��˶�
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
					mciSendString(_T("close girl"), NULL, 0, NULL); // ������һ��
					mciSendString(_T("open \".\\�ز�\\girl_die.wav\" alias girl"), NULL, 0, NULL);
					mciSendString(_T("play girl"), NULL, 0, NULL); // ������һ��
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
				mciSendString(_T("close girl"), NULL, 0, NULL); // ������һ��
				mciSendString(_T("open \".\\�ز�\\girl_die.wav\" alias girl"), NULL, 0, NULL);
				mciSendString(_T("play girl"), NULL, 0, NULL); // ������һ��
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
				mciSendString(_T("close girl"), NULL, 0, NULL); // ������һ��
				mciSendString(_T("open \".\\�ز�\\girl_die.wav\" alias girl"), NULL, 0, NULL);
				mciSendString(_T("play girl"), NULL, 0, NULL); // ������һ��
			}
		}
	}

	//����ת��
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
	//���˿�ǹ
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

	//ȷ�������Ƿ����ϰ���
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
	//�ɻ��˶�
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
		mciSendString(_T("close bombmusic"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
		mciSendString(_T("open \".\\�ز�\\ײ��.wav\" alias bombmusic"), NULL, 0, NULL);
		mciSendString(_T("play bombmusic"), NULL, 0, NULL); // ������һ��
		girl_s = 1;
		lifebar = 0;
		mciSendString(_T("close girl"), NULL, 0, NULL); // ������һ��
		mciSendString(_T("open \".\\�ز�\\girl_die.wav\" alias girl"), NULL, 0, NULL);
		mciSendString(_T("play girl"), NULL, 0, NULL); // ������һ��
	}
	//��������Ծ���˵��ݡ��������塢 �Ĵ�	
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
		mciSendString(_T("close shibai"), NULL, 0, NULL); // ������һ��
		mciSendString(_T("close welmusic"), NULL, 0, NULL);  // ѭ������
		mciSendString(_T("open \".\\�ز�\\ʧ��.wav\" alias shibai"), NULL, 0, NULL);
		mciSendString(_T("play shibai"), NULL, 0, NULL); // ������һ��
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
			PlaySound(TEXT(".\\�ز�\\tiao.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
			PlaySound(TEXT(".\\�ز�\\shoot.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
		mciSendString(_T("close welmusic"), NULL, 0, NULL);  // ѭ������
		mciSendString(_T("close finish"), NULL, 0, NULL); // ������һ��
		mciSendString(_T("open \".\\�ز�\\finish.wav\" alias finish"), NULL, 0, NULL);
		mciSendString(_T("play finish"), NULL, 0, NULL); // ������һ��
	}
}
//��ʼ����
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
		settextstyle(60, 0, _T("������Ρ����W"));
		TCHAR s[] = _T("�� ʼ");
		outtextxy(670, 420, s);
		TCHAR c[] = _T("�� ��");
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
				PlaySound(TEXT(".\\�ز�\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				mciSendString(_T("open \".\\�ز�\\bgm.mp3\" alias welmusic"), NULL, 0, NULL);//�򿪱�������
				mciSendString(_T("play welmusic repeat"), NULL, 0, NULL);  // ѭ������
				mciSendString(_T("close wel"), NULL, 0, NULL); // ������һ��
				wel = 0;
			}
			else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 580 && m.y >= 530 && m.x <= 880 && m.y <= 590)
			{
				PlaySound(TEXT(".\\�ز�\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				Sleep(500);
				exit(0);
			}
		}
	}
}
//�ɹ�����
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
		settextstyle(60, 0, _T("������Ρ����W"));
		TCHAR s[] = _T("���˵�");
		outtextxy(390, 420, s);
		TCHAR c[] = _T("�� ��");
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
				PlaySound(TEXT(".\\�ز�\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				winn = 0;
				wel = 1;
				mciSendString(_T("open \".\\�ز�\\��ʼ.mp3\" alias wel"), NULL, 0, NULL);//�򿪱�������
				mciSendString(_T("play wel repeat"), NULL, 0, NULL);  // ѭ������
			}
			else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 300 && m.y >= 530 && m.x <= 600 && m.y <= 590)
			{
				PlaySound(TEXT(".\\�ز�\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				Sleep(500);
				exit(0);
			}
		}
	}
}
//ʧ�ܽ���
void lose()
{
	mciSendString(_T("close welmusic repeat"), NULL, 0, NULL);  // ѭ������
	while (losing == 1)
	{
		transparentimagesp(NULL, 350, 200, &loser);//200*130
		transparentimagesp(NULL, 300, 400, &button);//300*90
		transparentimagesp(NULL, 300, 500, &button);
		settextcolor(WHITE);
		setbkmode(TRANSPARENT);
		settextstyle(60, 0, _T("������Ρ����W"));
		TCHAR s[] = _T("���˵�");
		outtextxy(390, 420, s);
		TCHAR c[] = _T("�� ��");
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
				PlaySound(TEXT(".\\�ز�\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				losing = 0;
				wel = 1;
				mciSendString(_T("open \".\\�ز�\\��ʼ.mp3\" alias wel"), NULL, 0, NULL);//�򿪱�������
				mciSendString(_T("play wel repeat"), NULL, 0, NULL);  // ѭ������
			}
			else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 300 && m.y >= 530 && m.x <= 600 && m.y <= 590)
			{
				PlaySound(TEXT(".\\�ز�\\click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				Sleep(500);
				exit(0);
			}
		}
	}
}
// ������
int main()
{
	initgraph(900, 600); // ��ʼ��ͼ�δ��� 
	startup();
	mciSendString(_T("open \".\\�ز�\\��ʼ.mp3\" alias wel"), NULL, 0, NULL);//�򿪱�������
	mciSendString(_T("play wel repeat"), NULL, 0, NULL);  // ѭ������
	while (1)
	{
		welcome();
		show();
		updatewithinput();
		updatewithoutinput();
		win();
		lose();
	}
	// ��������˳�
	_getch();
	closegraph();
	return 0;
}