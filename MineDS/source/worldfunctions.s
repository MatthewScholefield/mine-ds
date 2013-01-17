	.align	2
	.global	brightnessSpread
	.code	16
	.thumb_func
	.type	brightnessSpread, %function

brightnessSpread:
	@@brightnessSpread(worldObject* world,int x,int y, int brightness)
	.cfi_startproc
	push	{r0, r1, r2, r3, r4, r5, r6, r7, lr}
	cmp r2, #129 @@ if (y>129) return; (129==WORLD_HEIGHT+1)
	bgt return
	cmp r2,#0 @@ if (y<0) return;
	blt return
	cmp r3,#15 @@ if (brightness>15) return;
	bgt return
	mov r6, r1
	lsl r6, #7
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
	add r2, r2, #1
	mov r5, r3
	cmp r7,#1
	beq _A1	
	push {r7}
	mov r7,#2
	bl brightnessAddAmount
	bl brightnessSpread
	pop {r7}
_A1:
	sub r2, r2, #2	
	cmp r7,#2
	beq _A2
	push {r7}
	mov r7,#1
	bl brightnessAddAmount
	bl brightnessSpread
	pop {r7}
_A2:
	add r2, r2, #1
	add r1, r1, #1
	bl brightnessAddAmount
	bl brightnessSpread
	sub r1, r1, #2
	bl brightnessAddAmount
	bl brightnessSpread
return:
	pop	{r0, r1, r2, r3, r4, r5, r6, r7, pc}
	.align	2
words:
	.word	132354 @@add to worldObject* to get pointer to brighntess
	.word	-1057808
	.word	-1057800
	.cfi_endproc
brightnessAddAmount:
	@@ r0= worldObject*
	@@ r1= x
	@@ r2= y
	@@ r5= baseBrightness
	@@ r6 is erased
	@@ r3= newbrightness
	push {r0,r7, lr}
	mov r7, r1
	lsl r6, r7, #7
	add r6, r6, r7
	add r6, r6, r2
	lsl r6, r6, #2
	add r6, r6, r0
	ldr r0, [r6]
	push {r1, r2, r4, r5}
	bl	_Z18isBlockWalkThroughi
	pop {r1, r2, r4, r5}
	mov r3, #1
	cmp r0, #1
	bge _B2
	mov r3, #3


_B2:	add r3, r3, r5
	pop {r0,r7, pc}
	
	
sizes:
	.size	brightnessSpread, .-brightnessSpread
