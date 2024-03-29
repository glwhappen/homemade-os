#include <stdio.h>

void io_hlt(void);		//中断
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void write_mem8(int addr, int data);	//给addr这个内存地址赋值 已经废弃的函数

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1); //绘制矩形
void init_screen(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfont16(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);

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

#define uchar unsigned char

struct BOOTINFO 
{
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};

void HariMain(void)
{
	init_palette();/* 设定调色板 */
	struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
	extern char hankaku[4096];
	char s[40];
	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
	
	sprintf(s, "scrnx = %d", binfo->scrnx);
	putfonts8_asc(binfo->vram, binfo->scrnx, 16, 64, COL8_FFFFFF, s);
	
	putfonts8_asc(binfo->vram, binfo->scrnx, 8, 8, COL8_FFFFFF, "Haribote OS.");
	putfonts8_asc(binfo->vram, binfo->scrnx, 8, 25, COL8_000000, "Haribote OS.");
	
	putfonts8_asc(binfo->vram, binfo->scrnx, 45, 45, COL8_000000, "Haribote OS.");
	putfonts8_asc(binfo->vram, binfo->scrnx, 44, 44, COL8_FFFFFF, "Haribote OS.");
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


void init_screen(char *vram, int x, int y)
{
	boxfill8(vram, x, COL8_008484,  0,     0,      x -  1, y - 29);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
	boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);

	boxfill8(vram, x, COL8_FFFFFF,  3,     y - 24, 59,     y - 24);
	boxfill8(vram, x, COL8_FFFFFF,  2,     y - 24,  2,     y -  4);
	boxfill8(vram, x, COL8_848484,  3,     y -  4, 59,     y -  4);
	boxfill8(vram, x, COL8_848484, 59,     y - 23, 59,     y -  5);
	boxfill8(vram, x, COL8_000000,  2,     y -  3, 59,     y -  3);
	boxfill8(vram, x, COL8_000000, 60,     y - 24, 60,     y -  3);

	boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
	boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
	boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
	boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);
	return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}
void putfont16(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	int pos = 0;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[pos++];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
		d = font[pos++];
		if ((d & 0x80) != 0) { p[8] = c; }
		if ((d & 0x40) != 0) { p[9] = c; }
		if ((d & 0x20) != 0) { p[10] = c; }
		if ((d & 0x10) != 0) { p[11] = c; }
		if ((d & 0x08) != 0) { p[12] = c; }
		if ((d & 0x04) != 0) { p[13] = c; }
		if ((d & 0x02) != 0) { p[14] = c; }
		if ((d & 0x01) != 0) { p[15] = c; }
	}
	return;
}

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
	extern char hankaku[4096];
	for(; *s != 0x00; s++)
	{
		putfont8(vram, xsize, x, y , c, hankaku + *s * 16);
		x += 8;
	}
	return ;
}


