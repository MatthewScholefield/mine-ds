	.section .itcm,"ax",%progbits
	.align	2
	.global	brightnessSpread
	.code	16
	.thumb_func
	.type	brightnessSpread, %function
brightnessSpread:
	@@brightnessSpread(worldObject* world,int x,int y, int brightness)
	.cfi_startproc
	push	{r4, r5, r6, r7, lr}

	cmp r2, #129 @@ if (y>129) return; (129==WORLD_HEIGHT+1)
	bgt return
	cmp r2,#0 @@ if (y<0) return;
	blt return
	cmp r3,#15 @@ if (brightness>15) return;
	bgt return
	lsl r6, r1, #7
	add r6, r6, r1
	add r6, r2, r6 @@ r6 = x*129+y	
_S1:
	ldr r4, words
	add r5, r6, r4
	lsl r5, r5, #2 @ r5 = int* brightness[x][y];
	ldr r4, [r5, r0] @ r4 = world->brightness[x][y];
	cmp r4, r3
	ble return @@ if (world->brightness[x][y]>brightness) return;
	str r3, [r5, r0] @@ world->brightness[x][y]=brightness
	add r2, #1
	add r6, #1
	bl _S2
	b return
_S2: 
	mov r7, lr
	@ r4, r5 do not retain values anymore
	mov r5, r0
	mov r4, r3
	lsl r6, r6, #2
	ldr r0, [r6, r5] @ r0 = world->blocks[x-1][y]
	lsr r6, r6, #2
	bl	_Z18isBlockWalkThroughi
	add r3, r3, #4
	cmp r0, #0
	beq _S3
	sub r3, r3, #3
_S3:
	mov r0, r5
	bl brightnessSpread
	mov r3, r4
	mov pc, r7
return:
	pop	{r4, r5, r6, r7, pc}
	.align	2
words:
	.word	264450 @@add to worldObject* to get pointer to 
	.word	-1057808
	.word	-1057800
	.cfi_endproc
sizes:
	.size	brightnessSpread, .-brightnessSpread
