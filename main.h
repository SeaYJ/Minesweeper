#ifndef _MAIN_H
#define _MAIN_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <easyx.h>// ����ͼ�ο�
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


#define ROW 20
#define COL 20
#define MineNum 60
#define picsNum 17
#define picsWidth 24
#define picsHeight 24
#define encryptCode 20
// ɨ����Ϣ���û����������ȷ�����ף�
#define CM_SWEEPER_ID 0x0123
// �����Ϣ���û���������С�죩
#define CM_MARK_ID 0x124
// ѯ����Ϣ���û�������������
#define CM_ASK_ID 0x125

IMAGE imgs[picsNum];
unsigned int numOfclicked = 0;

int init(int map[ROW][COL]);// ��ʼ����ͼ������ʮ����
void show(int map[ROW][COL]);// ��ʾ��ͼ
void loadResources(int map[ROW][COL]);// ����ͼƬ��Դ
void draw(int map[ROW][COL]);// ���Ƶ�ͼ
void mouseClick(ExMessage *msg, int map[ROW][COL], int clickType);// ����¼�����
void expendBlankArea(int center_x, int center_y, int map[ROW][COL]);// չ����Ч�����߼�����ӦΪ 0 ������λ�ã�
int judgeGame(int map[ROW][COL], int clicked_x, int clicked_y);

#endif // !_MAIN_H
