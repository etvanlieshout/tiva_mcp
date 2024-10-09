;===============================================================================
; initialization assembly code
;===============================================================================

	.text
	.global psp_set
	.global msp_get
	.global svc_call
	;.global start_mcp_proc

psp_set:
	MSR	PSP, r0
	MOV	r0, #0x2
	MSR	CONTROL, r0
	MOV	pc, lr

;start_mcp_proc:
	;MOV	 r0, #0xFFFD	; overwrite EXC_RETURN in stack frame to drop
	;MOVT	 r0, #0xFFFF	; into Thread mode with PSP on return from svc
	;STR	 r0, [sp, #-8]
	;MOV	lr, #0xFFFD
	;MOVT	lr, #0xFFFF
	;bx	lr

msp_get:
	MRS	r0, MSP
	MOV	pc, lr

svc_call:
	SVC #0
