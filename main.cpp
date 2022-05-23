#include "main.h"

int main(int argc, char *argv[])
{
GAME_START:
	int map[ROW][COL] = { 0 };
	int result = 0;// ���ڽ�����Ϸ���������жϺ����ķ���ֵ

	srand((unsigned)time(NULL));

	initgraph(picsWidth * ROW, picsHeight * COL);// ������Ϸͼ�ν���
	loadResources(map);// ����ͼƬ��Դ
	
	init(map);// ��ʼ����ͼ�߼�
	//show(map);

	draw(map);// ����Ϸͼ�ν�����Ƶ�ͼ

	ExMessage msg;
	while (true)
	{
		if (peekmessage(&msg, EM_MOUSE))// ��׽������¼�
		{
			switch (msg.message)
			{
			case WM_LBUTTONUP:// ����������ɨ����
				PlaySound("./resources/audio/click.wav", NULL, SND_ASYNC | SND_FILENAME);
				mouseClick(&msg, map, CM_SWEEPER_ID);
				result = judgeGame(map, msg.y / picsHeight, msg.x / picsWidth);
				//printf("result=%d\tc_x=%d\tc_y=%d\n", result, msg.y / picsHeight, msg.x / picsWidth);
				break;
			case WM_RBUTTONUP:// ����Ҽ����𣨲�С�죩
				PlaySound("./resources/audio/flag.wav", NULL, SND_ASYNC | SND_FILENAME);
				mouseClick(&msg, map, CM_MARK_ID);
				break;
			case WM_MBUTTONUP:// ����м�����δ����ѯ�ʣ�
				PlaySound("./resources/audio/ask.wav", NULL, SND_ASYNC | SND_FILENAME);
				mouseClick(&msg, map, CM_ASK_ID);
				break;
			}
		}
		
		draw(map);// ˢ�µ�ͼ

		if (result != 0) break;
	}

	int msgBoxRet = MessageBox(NULL, __TEXT("��ʾ"), __TEXT("�Ƿ����¿�ʼ��"), MB_YESNO | MB_DEFBUTTON1);
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
		int r = rand() % ROW;// �����׵ĺ����꣨0 ~ ROW��
		int c = rand() % COL;// �����׵������꣨0 ~ COL��

		if (map[r][c] == 0) // �˴�δ������
		{
			map[r][c] = -1;// �����׵�λ�ã����� -1 ��ʾ
			i++; // ֻ�������óɹ��˲Ž�����һ��ѭ����ÿ��ѭ����������һ���ף�

			// �����׸�����������ʾ��������ʾ��Χ�׸��������֣�
			for (int j=r-1; j<=r+1; j++)// ������һ�ܾŸ���λ�ã�����+1����λ���⣩
			{
				for (int k=c-1; k<c+1; k++)
				{
					if ( map[j][k] != -1 && (j>=0 && j<ROW &&  k>=0 && k<COL /*��ֹ�����(�������Խ��)*/) )
					{
						map[j][k]++;
					}
				}
			}

		}
	}

	/*******************************************************************************\
	* ΪʲôҪ���ܣ�																	*
	*	1. ǰ����֪����ͼ�߼��л����� 0�����ף���1~8����Χ�׵���������-1���ף���������		*
	*	2. ɨ��ʵ���������ڷ�������Ϸ������һ��ͼ�����潫��Ϊ��δɨͼ���������������ݣ�		*
	*	3. Ȼ���û����δɨͼ����ʾδɨͼ��������ݣ�ǰ��������������						*
	*																				*
	* ʵ���߼���																		*
	*	1. ���ɵ�ͼ�߼���ͨ����ÿ���߼�����-1~8���� һ������20�������ܣ�				*
	*	2. ����ͼ�߼���ֻҪ�߼��������������20��-1��ע������ -1�����Ͳ��á�δɨͼ����	*
	*	3. �û������δɨͼ�����ͼ�ȥ�������20�����ʹﵽ����Ч����						*
	*	4. ���ܺ���ֵõ��˲����߼�����-1~8����Ȼ�����»��Ƽ��ɣ�						*
	\*******************************************************************************/

	// ����
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

	for (int i=0; i<9; i++)// ����ͼһ�š�1~8���ֱ�ʶ��һ�ţ�������
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
			if (0 <= map[i][j] && map[i][j] <= 8)// �����ֺ�����λ
			{
				// ���ϵ�ͼ�з��׵�ͼ��������������ͼƬ����ͼ���ļ����Ѿ���Ӧ�ã�
				putimage(j * picsWidth, i * picsHeight, &imgs[map[i][j]]);
			}
			else if (map[i][j] == -1)
			{
				putimage(j * picsWidth, i * picsHeight, &imgs[14]);// ����ͼ
			}
			else if (map[i][j] == -2)
			{
				putimage(j * picsWidth, i * picsHeight, &imgs[11]);// ���Ŵ����ͼ����Ϸ������
			}
			else if (map[i][j] == -3)
			{
				putimage(j * picsWidth, i * picsHeight, &imgs[12]);// ��������ͼ����Ϸ������
			}
			else if ( (encryptCode - 1) <= map[i][j] && map[i][j] <= (encryptCode + 8) )
			{
				putimage(j * picsWidth, i * picsHeight, &imgs[10]);// ��δɨͼ
			}
			else if ( (2 * encryptCode - 1) <= map[i][j] && map[i][j] <= (2 * encryptCode + 8) )
			{
				putimage(j * picsWidth, i * picsHeight, &imgs[13]);// ��С����
			}
			else if ((3 * encryptCode - 1) <= map[i][j] && map[i][j] <= (3 * encryptCode + 8))
			{
				putimage(j * picsWidth, i * picsHeight, &imgs[9]);// ��С����
			}
		}
	}
}

void mouseClick(ExMessage* msg, int map[ROW][COL], int clickType)
{
	// ��ȡ�����ͼƬ�����꣨Ϊ�滻��ͼƬ�ṩλ����Ϣ��
	int x = (msg->y) / picsHeight;
	int y = (msg->x) / picsWidth;

	if (clickType == CM_SWEEPER_ID)
	{
		if ( (encryptCode - 1) <= map[x][y] && map[x][y] <= (2 * encryptCode + 8) )
		{
			// �û�ȷ�����ף�ɨ���������С���ӻ���ȡ��С���ӣ�
			map[x][y] -= encryptCode;
			expendBlankArea(x, y, map);
		}
	}
	else if (clickType == CM_MARK_ID)
	{
		if ((encryptCode - 1) <= map[x][y] && map[x][y] <= (encryptCode + 8))
		{
			// �����ӣ����μ����߼���ͼ��
			map[x][y] += encryptCode;
		}
		else if ( (2 * encryptCode - 1) <= map[x][y] && map[x][y] <= (2 * encryptCode + 8))
		{
			// ȡ��С����
			map[x][y] -= encryptCode;
		}
	}
	else if (clickType == CM_ASK_ID)
	{
		if ( (encryptCode - 1) <= map[x][y] && map[x][y] <= (encryptCode + 8) )
		{
			// ѯ��
			map[x][y] += 2 * encryptCode;
		}
		else if ((3 * encryptCode - 1) <= map[x][y] && map[x][y] <= (3 * encryptCode + 8))
		{
			// ȡ��ѯ��
			map[x][y] -= 2 * encryptCode;
		}
	}
	
	//system("cls");
	//show(map);
}

void expendBlankArea(int center_x, int center_y, int map[ROW][COL])
{
	if (map[center_x][center_y] == 0)// չ�����ı����ǿհ�ͼ����1~8�������ӣ����ף�
	{
		for (int i = center_x - 1; i <= center_x + 1; i++)
		{
			for (int j = center_y - 1; j <= center_y + 1; j++)
			{
				if ( (0 <= i && i < ROW && 0 <= j && j <= COL)// ��ֹ�������Խ�磨�������
					&& (encryptCode - 1 < map[i][j] && map[i][j] <= encryptCode + 8))
				{
					map[i][j] -= encryptCode;// չ�������ܣ�
					expendBlankArea(i, j, map);// �ݹ���ã���ţ�Ƶ��߼��������������ɣ�
				}
			}
		}
	}
	
}

// ��Ϸ�����ж�����
// 0 δ������ 1 ��ϷӮ�� -1 ��Ϸ��
int judgeGame(int map[ROW][COL], int clicked_x, int clicked_y)
{
	// �㵽�׵�λ�� ��Ϸ��
	if (map[clicked_x][clicked_y] == encryptCode - 1 || map[clicked_x][clicked_y] == -1)
	{
		// ��Ϸ�Ѿ���������Ҫֱ����ʾ������ͼ
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				if ((encryptCode - 1) <= map[i][j] && map[i][j] <= (encryptCode + 8))// Ϊδɨ��λ�ý��ܣ���ʾ����ʵ����
				{
					map[i][j] -= encryptCode;
				}
				else if ((2 * encryptCode - 1) <= map[i][j] && map[i][j] <= (2 * encryptCode + 8))// Ϊ��С��λ���ܣ���ʾ����ʵ����
				{
					map[i][j] -= 2 * encryptCode;

					if (map[i][j] == -1)// ����ò�С��λ�ý��ܺ�����λ
					{
						map[i][j] = -3;
					}
				}
				else if ((3 * encryptCode - 1) <= map[i][j] && map[i][j] <= (3 * encryptCode + 8))// Ϊѯ��λ�ý��ܣ���ʾ����ʵ����
				{
					map[i][j] -= 3 * encryptCode;
				}
			}
		}

		map[clicked_x][clicked_y] = -2;// �Ŵ����

		return -1;
	}

	// �ҳ������� ��ϷӮ
	if (numOfclicked == ROW * COL - MineNum)
	{
		return 1;
	}

	return 0;
}
