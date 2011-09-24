
@{{BLOCK(gfx2)

@=======================================================================
@
@	gfx2, 16x16@8, 
@	Transparent color : FF,00,FF
@	+ palette 256 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 512 + 256 = 768
@
@	Time-stamp: 2011-09-24, 18:18:06
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.8
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global gfx2Tiles		@ 256 unsigned chars
gfx2Tiles:
	.word 0x03010303,0x0301070D,0x0D090803,0x010F110A,0x05090103,0x0F110000,0x0D0F0103,0x06000200
	.word 0x00050103,0x05101616,0x00030803,0x030B1510,0x00030803,0x030E130B,0x03090903,0x0D030E13
	.word 0x030D0103,0x03030101,0x0D110F03,0x03090901,0x0500060D,0x0309090F,0x00000005,0x03010F05
	.word 0x16100005,0x03010500,0x150B0005,0x03070510,0x130E0005,0x0305100B,0x0E04030D,0x03031513
	.word 0x110F0903,0x010D050D,0x00050703,0x05050005,0x00000A03,0x000C0000,0x13000303,0x120C140E
	.word 0x000D0303,0x0C140D0E,0x00030303,0x0D04090D,0x01090103,0x03090903,0x01010303,0x03010303
	.word 0x050D0D05,0x0307050D,0x00050500,0x030A0005,0x00001800,0x03050017,0x140C1218,0x0510130E
	.word 0x04140C0D,0x05100B03,0x0E0F0903,0x03030803,0x03090803,0x03090903,0x03030103,0x03030101

	.section .rodata
	.align	2
	.global gfx2Pal		@ 512 unsigned chars
gfx2Pal:
	.hword 0x0009,0x09FA,0x1A96,0x0574,0x2359,0x0ADD,0x4BFF,0x135E
	.hword 0x269C,0x0E5C,0x17FE,0x1EF7,0x239A,0x0A7C,0x1F38,0x1B7E
	.hword 0x1AD7,0x1FFF,0x23BB,0x1F18,0x2379,0x1AF7,0x1AB6,0x1F39
	.hword 0x27DB,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(gfx2)
