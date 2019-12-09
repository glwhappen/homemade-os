void io_hlt(void);		//中断
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void write_mem8(int addr, int data);	//给addr这个内存地址赋值 已经废弃的函数

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1); //绘制矩形

#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

void HariMain(void)
{
	int i;
	char *p;
	
	init_palette(); /* 设定调色板 */
	p = (char *) 0xa0000; /* 指定地址 */
	int r = 0;
	int l = 0;
	for(i = 0; i < 450; i++)
	{
		l += 15;
		boxfill8(p, 320, i % 18,  l,  r, l + 12, r + 12);
		if((i + 1) % 9 == 0)
		{
			r += 15;
			l = 0;
		}
			
	}
	
	//boxfill8(p, 320, COL8_FF0000,  20,  20, 120, 120);
	//boxfill8(p, 320, COL8_00FF00,  70,  50, 170, 150);
	//boxfill8(p, 320, COL8_0000FF, 120,  80, 220, 180);

	//for(i = 0; i <= 0xffff; i++)
	//{
		//p[i] = i & 0x0f;
		//p = (char *)i;
		//*p = i & 0x0f;
		//write_mem8(i, i & 0x0f); /* MOV BYTE[i],15*/
		//i = i & 0x0f;
	//}
	for(;;)
	{
		io_hlt();
	}

}

void init_palette(void)
{
	const unsigned char ADDCOLOR = 16; 
	static unsigned char table_rgb[(16 + ADDCOLOR) * 3] = {
		0x00, 0x00, 0x00,	/*  0：黑色   */
		0xff, 0x00, 0x00,	/*  1：亮红色 */
		0x00, 0xff, 0x00,	/*  2：亮绿色 */
		0xff, 0xff, 0x00,	/*  3：亮黄色 */
		0x00, 0x00, 0xff,	/*  4：亮蓝色 */
		0xff, 0x00, 0xff,	/*  5：亮紫色 */
		0x00, 0xff, 0xff,	/*  6：亮蓝色 */
		0xff, 0xff, 0xff,	/*  7：白色   */
		0xc6, 0xc6, 0xc6,	/*  8：浅灰色 */
		0x84, 0x00, 0x00,	/*  9：深红色 */
		0x00, 0x84, 0x00,	/* 10：深绿色 */
		0x84, 0x84, 0x00,	/* 11：深黄色 */
		0x00, 0x00, 0x84,	/* 12：深蓝色 */
		0x84, 0x00, 0x84,	/* 13：深紫色 */
		0x00, 0x84, 0x84,	/* 14：深蓝色 */
		0x84, 0x84, 0x84,	/* 15：深灰色 */
		// 此处可以继续增加新的配色
		0x5c, 0x97, 0x95,	/*  青色   */
		0xE7, 0x40, 0x40,	/*  红色 */
		0x3d, 0x6e, 0xd3	/*  蓝色 */
		
		
	};
	set_palette(0, 15 + ADDCOLOR, table_rgb);
	return;

	/* static char 指令只能用于数据，但相当于DB指令 */
}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	/* 记录中断许可标志的值*/
	io_cli(); 					/* 将许可标志置为0，禁止中断 */
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);	/* 恢复许可标志的值 */
	return;
}
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}

