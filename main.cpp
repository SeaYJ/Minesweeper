#include "main.h"

int main(int argc, char *argv[])
{
GAME_START:
	int map[ROW][COL] = { 0 };
	int result = 0;// 用于接受游戏结束条件判断函数的返回值

	srand((unsigned)time(NULL));

	initgraph(picsWidth * ROW, picsHeight * COL);// 创建游戏图形界面
	loadResources(map);// 导入图片资源
	
	init(map);// 初始化地图逻辑
	//show(map);

	draw(map);// 在游戏图形界面绘制地图

	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg, EM_MOUSE))// 捕捉到鼠标事件
		{
			switch (msg.message)
			{
			case WM_LBUTTONUP:// 鼠标左键弹起（扫除）
				PlaySound("./resources/audio/click.wav", NULL, SND_ASYNC | SND_FILENAME);
				mouseClick(&msg, map, CM_SWEEPER_ID);
				result = judgeGame(map, msg.y / picsHeight, msg.x / picsWidth);
				//printf("result=%d\tc_x=%d\tc_y=%d\n", result, msg.y / picsHeight, msg.x / picsWidth);
				break;
			case WM_RBUTTONUP:// 鼠标右键弹起（插小旗）
				PlaySound("./resources/audio/flag.wav", NULL, SND_ASYNC | SND_FILENAME);
				mouseClick(&msg, map, CM_MARK_ID);
				break;
			case WM_MBUTTONUP:// 鼠标中键弹起（未定；询问）
				PlaySound("./resources/audio/ask.wav", NULL, SND_ASYNC | SND_FILENAME);
				mouseClick(&msg, map, CM_ASK_ID);
				break;
			}
		}
		
		draw(map);// 刷新地图

		if (result != 0) break;
	}

	int msgBoxRet = MessageBox(NULL, __TEXT("提示"), __TEXT("是否重新开始？"), MB_YESNO | MB_DEFBUTTON1);
	if (msgBoxRet == IDYES)
	{
		goto GAME_START;
	}
	return 0;
}

int init(int map[ROW][COL])
{
	numOfclicked = 0;

	for (int i=0; i<MineNum; )
	{
		int r = rand() % ROW;// 生成雷的横坐标（0 ~ ROW）
		int c = rand() % COL;// 生成雷的纵坐标（0 ~ COL）

		if (map[r][c] == 0) // 此处未设置雷
		{
			map[r][c] = -1;// 设置雷的位置，雷用 -1 表示
			i++; // 只有雷设置成功了才进行下一次循环（每次循环必须设置一个雷）

			// 做好雷附近的数字提示（用来显示周围雷个数的数字）
			for (int j=r-1; j<=r+1; j++)// 变量雷一周九个的位置，依次+1（雷位除外）
			{
				for (int k=c-1; k<c+1; k++)
				{
					if ( map[j][k] != -1 && (j>=0 && j<ROW &&  k>=0 && k<COL /*防止堆溢出(数组访问越界)*/) )
					{
						map[j][k]++;
					}
				}
			}

		}
	}

	/*******************************************************************************\
	* 为什么要加密：																	*
	*	1. 前提须知：地图逻辑中会生成 0（无雷）、1~8（周围雷的数量）、-1（雷）三种量；		*
	*	2. 扫雷实际上类似于翻牌子游戏，先用一张图（后面将称为“未扫图”）覆盖所有类容；		*
	*	3. 然后用户点击未扫图，显示未扫图下面的内容（前面那三种量）。						*
	*																				*
	* 实现逻辑：																		*
	*	1. 生成地图逻辑后，通过给每个逻辑量（-1~8）加 一个数（20）来加密；				*
	*	2. 检查地图逻辑，只要逻辑量大于这个数（20）-1（注意雷是 -1），就采用“未扫图”；	*
	*	3. 用户点击“未扫图”，就减去这个数（20），就达到解密效果；						*
	*	4. 解密后就又得到了部分逻辑量（-1~8），然后重新绘制即可！						*
	\*******************************************************************************/

	// 加密
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			map[i][j] += encryptCode;
		}
	}

	return 0;
}

void show(int map[ROW][COL])
{

	for (int i=0; i<ROW; i++) 
	{
		for (int j=0; j<COL; j++)
		{
			printf("%d\t", map[i][j]);
		}
		putchar('\n');
	}
}

void loadResources(int map[ROW][COL])
{
	wchar_t imgsPath[_MAX_PATH] = { 0 };

	for (int i=0; i<9; i++)// 无雷图一张、1~8数字标识各一张，共九张
	{
		wsprintf((LPSTR)imgsPath, __TEXT("./resources/%d.gif"), i);
		loadimage(&imgs[i], (LPCTSTR)imgsPath, picsWidth, picsHeight);
	}

	loadimage(&imgs[9], (LPCTSTR)__TEXT("./resources/ask.gif"), picsWidth, picsHeight);
	loadimage(&imgs[10], (LPCTSTR)__TEXT("./resources/blank.gif"), picsWidth, picsHeight);
	loadimage(&imgs[11], (LPCTSTR)__TEXT("./resources/blood.gif"), picsWidth, picsHeight);
	loadimage(&imgs[12], (LPCTSTR)__TEXT("./resources/error.gif"), picsWidth, picsHeight);
	loadimage(&imgs[13], (LPCTSTR)__TEXT("./resources/flag.gif"), picsWidth, picsHeight);
	loadimage(&imgs[14], (LPCTSTR)__TEXT("./resources/mine.gif"), picsWidth, picsHeight);
	loadimage(&imgs[15], (LPCTSTR)__TEXT("./resources/Ooo.gif"));
	loadimage(&imgs[16], (LPCTSTR)__TEXT("./resources/smile.gif"));
}

void draw(int map[ROW][COL])
{
	for (int i=0; i<ROW; i++)
	{
		for (int j=0; j<COL; j++)
		{
			if (0 <= map[i][j] && map[i][j] <= 8)// 贴数字和无雷位
			{
				// 贴上地图中非雷的图（按照数字码与图片名贴图，文件名已经对应好）
				putimage(j * picsWidth, i * picsHeight, &imgs[map[i][j]]);
			}
			else if (map[i][j] == -1)
			{
				putimage(j * picsWidth, i * picsHeight, &imgs[14]);// 贴雷图
			}
			else if (map[i][j] == -2)
			{
				putimage(j * picsWidth, i * picsHeight, &imgs[11]);// 贴排错的雷图（游戏结束）
			}
			else if (map[i][j] == -3)
			{
				putimage(j * picsWidth, i * picsHeight, &imgs[12]);// 贴插错旗的图（游戏结束）
			}
			else if ( (encryptCode - 1) <= map[i][j] && map[i][j] <= (encryptCode + 8) )
			{
				putimage(j * picsWidth, i * picsHeight, &imgs[10]);// 贴未扫图
			}
			else if ( (2 * encryptCode - 1) <= map[i][j] && map[i][j] <= (2 * encryptCode + 8) )
			{
				putimage(j * picsWidth, i * picsHeight, &imgs[13]);// 贴小旗子
			}
			else if ((3 * encryptCode - 1) <= map[i][j] && map[i][j] <= (3 * encryptCode + 8))
			{
				putimage(j * picsWidth, i * picsHeight, &imgs[9]);// 贴小旗子
			}
		}
	}
}

void mouseClick(ExMessage* msg, int map[ROW][COL], int clickType)
{
	// 获取鼠标点击图片的坐标（为替换该图片提供位置信息）
	int x = (msg->y) / picsHeight;
	int y = (msg->x) / picsWidth;

	if (clickType == CM_SWEEPER_ID)
	{
		if ( (encryptCode - 1) <= map[x][y] && map[x][y] <= (2 * encryptCode + 8) )
		{
			// 用户确定无雷（扫除；如果有小旗子会先取消小旗子）
			map[x][y] -= encryptCode;
			expendBlankArea(x, y, map);
		}
	}
	else if (clickType == CM_MARK_ID)
	{
		if ((encryptCode - 1) <= map[x][y] && map[x][y] <= (encryptCode + 8))
		{
			// 插旗子（二次加密逻辑地图）
			map[x][y] += encryptCode;
		}
		else if ( (2 * encryptCode - 1) <= map[x][y] && map[x][y] <= (2 * encryptCode + 8))
		{
			// 取消小旗子
			map[x][y] -= encryptCode;
		}
	}
	else if (clickType == CM_ASK_ID)
	{
		if ( (encryptCode - 1) <= map[x][y] && map[x][y] <= (encryptCode + 8) )
		{
			// 询问
			map[x][y] += 2 * encryptCode;
		}
		else if ((3 * encryptCode - 1) <= map[x][y] && map[x][y] <= (3 * encryptCode + 8))
		{
			// 取消询问
			map[x][y] -= 2 * encryptCode;
		}
	}
	
	//system("cls");
	//show(map);
}

void expendBlankArea(int center_x, int center_y, int map[ROW][COL])
{
	if (map[center_x][center_y] == 0)// 展开中心必须是空白图（非1~8，非棋子，非雷）
	{
		for (int i = center_x - 1; i <= center_x + 1; i++)
		{
			for (int j = center_y - 1; j <= center_y + 1; j++)
			{
				if ( (0 <= i && i < ROW && 0 <= j && j <= COL)// 防止数组访问越界（堆溢出）
					&& (encryptCode - 1 < map[i][j] && map[i][j] <= encryptCode + 8))
				{
					map[i][j] -= encryptCode;// 展开（解密）
					expendBlankArea(i, j, map);// 递归调用（最牛逼的逻辑操作，慢慢理解吧）
				}
			}
		}
	}
	
}

// 游戏结束判断条件
// 0 未结束； 1 游戏赢； -1 游戏输
int judgeGame(int map[ROW][COL], int clicked_x, int clicked_y)
{
	// 点到雷的位置 游戏输
	if (map[clicked_x][clicked_y] == encryptCode - 1 || map[clicked_x][clicked_y] == -1)
	{
		// 游戏已经结束，需要直接显示整个地图
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				if ((encryptCode - 1) <= map[i][j] && map[i][j] <= (encryptCode + 8))// 为未扫除位置解密，显示其真实内容
				{
					map[i][j] -= encryptCode;
				}
				else if ((2 * encryptCode - 1) <= map[i][j] && map[i][j] <= (2 * encryptCode + 8))// 为插小旗位解密，显示其真实内容
				{
					map[i][j] -= 2 * encryptCode;

					if (map[i][j] == -1)// 如果该插小旗位置解密后是雷位
					{
						map[i][j] = -3;
					}
				}
				else if ((3 * encryptCode - 1) <= map[i][j] && map[i][j] <= (3 * encryptCode + 8))// 为询问位置解密，显示其真实内容
				{
					map[i][j] -= 3 * encryptCode;
				}
			}
		}

		map[clicked_x][clicked_y] = -2;// 排错的雷

		return -1;
	}

	// 找出所有雷 游戏赢
	if (numOfclicked == ROW * COL - MineNum)
	{
		return 1;
	}

	return 0;
}
