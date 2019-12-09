TOOLPATH = ../tolset/z_tools/
MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
EDIMG    = $(TOOLPATH)edimg.exe
COPY     = copy
DEL      = del

# 默认动作

default :
	$(MAKE) img

# 生成规则

ipl.bin : ipl.asm Makefile
	$(NASK) ipl.asm ipl.bin ipl.lst

haribote.sys : haribote.asm Makefile
	$(NASK) haribote.asm haribote.sys haribote.lst

haribote.img : ipl.bin haribote.sys Makefile
	$(EDIMG)   imgin:../tolset/z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:haribote.sys to:@: \
		imgout:haribote.img

# 命令

img :
	$(MAKE) haribote.img

run :
	$(MAKE) img
	$(COPY) haribote.img ..\tolset\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../tolset/z_tools/qemu
	$(MAKE) clean

install :
	$(MAKE) img
	$(IMGTOL) w a: haribote.img

clean :
	-$(DEL) ipl.bin
	-$(DEL) ipl.lst
	-$(DEL) haribote.sys
	-$(DEL) haribote.lst
	-$(DEL) haribote.img
	
src_only :
	$(MAKE) clean
	-$(DEL) haribote.img
