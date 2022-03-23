build_options = -mips3 -EB -G0
gcc_options = -O2 -I./ -c -mno-shared -mfix4300 -fno-PIC -mno-abicalls -mno-shared

all: build_rom

build_rom: boot game assets
	../Utils/MakeCart/makecart Build/boot Build/game Build/assets ../../testCart.rom

assets: FORCE
	../Utils/PackAssets/packassets RawAssets Build/assets

FORCE: ;

boot: Source/boot.s
	mips-linux-gnu-as $(build_options) Source/Boot.s -o Obj/boot.o
	mips-linux-gnu-ld $(build_options) -Ttext 0xA4000040 Obj/boot.o -o Obj/boot
	mips-linux-gnu-objcopy -j .text* -j .data* -j .rodata* -S Obj/boot -O binary Build/boot

game: link_game
	mips-linux-gnu-objcopy -j .text* -j .data* -j .rodata* -S Obj/game -O binary Build/game

link_game: main.c.o main.s.o vi.c.o draw_text.c.o get_asset.c.o
	mips-linux-gnu-ld $(build_options) -Ttext 0x80000400 Obj/main.c.o Obj/main.s.o Obj/vi.c.o Obj/draw_text.c.o Obj/get_asset.c.o -o Obj/game

vi.c.o: Source/vi.c Source/vi.h
	mips-linux-gnu-gcc $(build_options) $(gcc_options) Source/vi.c -o Obj/vi.c.o

get_asset.c.o: Source/get_asset.c Source/get_asset.h
	mips-linux-gnu-gcc $(build_options) $(gcc_options) Source/get_asset.c -o Obj/get_asset.c.o

draw_text.c.o: Source/draw_text.c Source/draw_text.h
	mips-linux-gnu-gcc $(build_options) $(gcc_options) Source/draw_text.c -o Obj/draw_text.c.o

main.c.o: Source/main.c 
	mips-linux-gnu-gcc $(build_options) $(gcc_options) Source/main.c -o Obj/main.c.o

main.s.o: Source/main.s
	mips-linux-gnu-as -march=4300 $(build_options) Source/main.s -o Obj/main.s.o

clean:
	rm Obj/*
	rm Build/*
	rm ../../testCart.rom