;===============================================================================
;	initialization assembly code
;===============================================================================

	.text
	.global svc_code_get
	.global msp_svc_test

; -- svc_code_get --------------------------------------------------------------
; Black magic uses value pushed to lr location on exception entry to pull the
; immediate value from the svc instruction call.
; NOTE: Assumes Thumb mode is on. ARM docs for SVC handlers has example code
; that detects Thumb mode or not and adjusts offset accordingly.
; USAGE: should be called immediately within svc handler.
svc_code_get:
	LDR	r0, [sp, #4]	; get handler lr value

	MOV	r1, #0xFFFB
	MOVT	r1, #0xFFFF
	BIC	r0, r0, r1	; clear out all bits except sp (bit 2)

	CMP	r0, #0x4	; use handler lr to check which sp to use
	ITTE	NE
	MRSNE	r0, MSP
	ADDNE	r0, #8		; since MSP used by handler, need to adjust
	MRSEQ	r0, PSP

	LDR	r1, [r0, #0x18] ; load lr address from exception stack
				; lr address is the addr of the svc instruction

	LDRB	r0, [r1, #-2]	; extracts 8-bit immediate from svc opcode.
				; -2 byte offset for Thumb mode, -4 otherwise
	MOV	pc, lr

; -- msp_svc_test --------------------------------------------------------------
; Tests that the svc_code_get routine correctly extracts SVC imm value.
msp_svc_test:
	SVC #0x33
