#ifndef _MAIN_H
#define _MAIN_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <easyx.h>// 引用图形库
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


#define ROW 20
#define COL 20
#define MineNum 60
#define picsNum 17
#define picsWidth 24
#define picsHeight 24
#define encryptCode 20
// 扫除消息（用户操作：点击确定无雷）
#define CM_SWEEPER_ID 0x0123
// 标记消息（用户操作：插小旗）
#define CM_MARK_ID 0x124
// 询问消息（用户操作：待定）
#define CM_ASK_ID 0x125

IMAGE imgs[picsNum];
unsigned int numOfclicked = 0;

int init(int map[ROW][COL]);// 初始化地图，布置十个雷
void show(int map[ROW][COL]);// 显示地图
void loadResources(int map[ROW][COL]);// 加载图片资源
void draw(int map[ROW][COL]);// 绘制地图
void mouseClick(ExMessage *msg, int map[ROW][COL], int clickType);// 鼠标事件监听
void expendBlankArea(int center_x, int center_y, int map[ROW][COL]);// 展开无效区域（逻辑变量应为 0 的连续位置）
int judgeGame(int map[ROW][COL], int clicked_x, int clicked_y);

#endif // !_MAIN_H
