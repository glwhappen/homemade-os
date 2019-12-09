# 第四天

## 运行方法

1. 双击cmd，输入make run

## 当前说明

开始往屏幕上显示内容

简单的显示颜色，在naskfunc中写了对应的几个绘制函数，然后再bootpack.c中实战了一下，绘制出了好看的colorful矩形

# 必备知识点

## 汇编常用指令

```assembly
;define byte 往文件里写一个字节的指令
DB 0xeb,0x4e
;reserve byte 空多少个字节
RESB 10
;define word 在计算机中word是16位
DW 2
;define double-word 32位
DD 2
;可以这样输出文字
DB "HELLOIPL"
; origin
ORG ; 开始执行的时候，把这些机器语言装在到内存的哪个地址
JUM ; 相当与C语言的goto 英文jump
entry: ; 标签的声明，用于指定JMP指令的跳转目的地
MOV AX,0 ; 相当于 AX=0 这样的一个赋值语句 move移动
ADD SI,1 ; 相当于 SI = SI + 1
CMP ; 是比较指令，例如a和3进行比较
CMP a,3
JE ; 是跳转指令，如果CMP比较相等则跳转，如果比较不相等则不跳转
; 因此
CMP AL,0
JE fin
; 相当于
if(AL == 0){goto fin;}

INT ; 软件中断指令 interrupt 中途打断
BIOS ; basic input output system 基本输入输出系统
ROM ; read only memory 只读存储器，不能写入，切换电源后内容不会消失

BIOS的功能非常多 INT就是用来调用这些功能的函数，INT后面跟上数组，代表调用不同的函数
INT 0x10 ; 即16号函数，他的功能是控制显卡

HLT  ; halt 让CPU停止动作的指令，不是彻底的停止，而是让CPU进入待机状态，只要外部发生变化，比如按下键盘，或是移动鼠标，CPU就是醒过来继续执行程序。
JC ; jump if carry 如果进位标志（carry flag）是1的话，就跳转。
JNC ; jump if not carry 如果进位标志是0的话就跳转
JBE ; jump if below or equal 小于等于跳转
JB  ; jump if below
EQU ; equal 相当于 #define CYLS 10
```

## 寄存器

### 16位寄存器

| 名称 | 二进制表示 | 英文解释          | 中文解释       |
| ---- | ---------- | ----------------- | -------------- |
| AX   | 0x05       | accumulator       | 累加寄存器     |
| CX   | 0x81C1     | counter           | 计数寄存器     |
| DX   | 0x81C2     | data              | 数据寄存器     |
| BX   | 0x81C3     | base              | 基址寄存器     |
| SP   | 0x81C4     | stack pointer     | 栈指针寄存器   |
| BP   | 0x81C5     | base pointer      | 基址指针寄存器 |
| SI   | 0x81C6     | source index      | 原变址寄存器   |
| DI   | 0x81C7     | destination index | 目的变址寄存器 |