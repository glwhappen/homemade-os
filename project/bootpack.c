void io_hlt(void);
void HariMain(void)
{

fin:
	/* 我想把HLT放在这里，但我不能在C语言中使用HLT！ */
	io_hlt();
	goto fin;

}
