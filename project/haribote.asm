; haribote-os
; TAB=4

; BOOT_INFO关系
CYLS	EQU		0x0ff0			; 由引导扇区设置
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2			; 有关颜色数的信息。 有多少位颜色？
SCRNX	EQU		0x0ff4			; X分辨率
SCRNY	EQU		0x0ff6			; Y分辨率
VRAM	EQU		0x0ff8			; 图形缓冲区的起始地址

		ORG		0xc200		; 这个程序被装载到内存的什么地方？
		MOV		AL,0x13		; VGA显卡，320*200*8位彩色
		MOV		AH,0x00
		INT		0x10
		
		MOV		BYTE [VMODE],8	; 记下画面模式
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000

; 让BIOS告诉您键盘的LED状态

		MOV		AH,0x02
		INT		0x16 			; keyboard BIOS
		MOV		[LEDS],AL
		
fin:
		HLT
		JMP		fin