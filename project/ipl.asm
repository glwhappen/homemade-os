; hello-os
; TAB=4 推荐TAB是4
CYLS	EQU		10				; 读到10个柱面 EQU equal 相当于 #define CYLS 10
		ORG		0x7c00			; 指明程序的装载地址

; 以下这段是标准FAT12格式专用的代码

		JMP		entry
		DB		0x90
		DB		"HELLOIPL"		; 启动区的名称可以是任意的字符串（8字节）
		DW		512				; 每个扇区（sector）的大小（必须512字节）
		DB		1				; 簇（cluster）的大小（必须为1个扇区）
		DW		1				; FAT的起始位置（一般从第一个扇区开始）
		DB		2				; FAT的个数（必须为2）
		DW		224				; 根目录的大小（一般设成224项）
		DW		2880			; 该磁盘的大小（必须是2880扇区）
		DB		0xf0			; 磁盘的种类（必须是0xf0）
		DW		9				; FAT的长度（必须是9扇区）
		DW		18				; 1个磁道（track）有几个扇区（必须是18）
		DW		2				; 磁盘数（必须是2）
		DD		0				; 不使用分区，必须是0
		DD		2880			; 重写一次磁盘大小
		DB		0,0,0x29		; 意义不明，固定
		DD		0xffffffff		; （可能是）卷标号码
		DB		"HELLO-OS   "	;  磁盘的名称（11字节）
		DB		"FAT12   "		; 磁盘格式名称（8字节）
		RESB	18				; 先空出18字节

; 程序主体
entry:
		MOV		AX,0			; 初始化寄存器
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX			; 段寄存器初始化的高位初始化为0

; 读磁盘

		MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0			; 柱面0
		MOV		DH,0			; 磁头0
		MOV		CL,2			; 扇区2

readloop:
		MOV		SI,0

retry:
		MOV		AH,0x02			; AH=0x02 : 读盘
		MOV		AL,1			; 1个扇区
		MOV		BX,0
		MOV		DL,0x00			; A驱动器
		INT		0x13			; 调用磁盘BIOS
		JNC		next			; 没出错的话就跳转到next
		ADD		SI,1			; 出错就SI++
		CMP		SI,5			; SI与5比较
		JAE		error			; SI >= 5是，跳转到error
		
		MOV		AH,0x00			; 系统复位
		MOV		DL,0x00			
		INT		0x13			; 重置驱动器
		JMP		retry
		
next:
		MOV		AX,ES			; 把内存地址往后移0x200   我感觉这里因该是0x020
		ADD		AX,0x0020		; 0x20是16进制下512/16的结果
		MOV		ES,AX			; 因为没有ADD ES,0x020指令，所以这里稍微绕个弯
		ADD		CL,1			; 往CL里加1
		CMP		CL,18			; 比较CL与18
		JBE		readloop		; 如果CL <= 18 就跳转至readloop
		MOV		CL,1
		ADD		DH,1
		CMP		DH,2
		JB		readloop		; 如果DH < 2, 则跳转到readloop
		MOV		DH,0
		ADD		CH,1
		CMP		CH,CYLS
		JB		readloop		; CH < CYLS, 则跳转到readloop

; 读完盘以后执行asmhead.sys！
		MOV		[0x0ff0],CH		; 把IPL读到哪里做个笔记，CYLS读到的地方存一下
		JMP		0xc200
		
fin:
		HLT						; 让CPU停止，等待指令
		JMP		fin				; 无限循环

error:
		MOV		SI,msg
		
putloop:
		MOV		AL,[SI]
		HLT
		ADD		SI,1			; 给SI加1
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 显示一个文字
		MOV		BX,15			; 指定字符颜色
		INT		0x10			; 调用显卡BIOS
		JMP		putloop

msg:
		DB		0x0a, 0x0a		; 换行2次
		DB		"load error"
		DB		0x0a			; 换行
		DB		0
		
		RESB	0x7dfe-$		; 填写0x00,直到 0x001fe 
		
		DB		0x55, 0xaa		; 第一个扇区的最后两个字节，只有是0x55AA的时候，计算机才会确认这个磁盘时启动盘