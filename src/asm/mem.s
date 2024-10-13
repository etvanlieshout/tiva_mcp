;===============================================================================
;	Set up memory space for user processes and stack mgmt
;===============================================================================

	.data	; mem begins @ 0x20000000; 32 KB available (Total SRAM)

mcpmem:	.space	0x1FFF	; Label to mark top of system allocation and bump
			; next label to top of user mem, since stack grows down

usrmem:	.word	0x0	; Label to mark the start of user process memory/
			; stack space; this allocated space is 8 KB for now

	.text
	.global	init_usrmem

usrmem_p:	.word	usrmem

init_usrmem:
	ldr  r0, usrmem_p
	mov  pc, lr

