;===============================================================================
;	initialization assembly code
;===============================================================================

	.text
	.global psp_init
	.global psp_set
	.global msp_get
	.global mcp_start
	.global svc_code_get
	.global msp_svc_test

; -- psp_init ------------------------------------------------------------------
; Sets the MCU's control register to have the processor use the PSP; sets the
; initial PSP value to values passed in r0. Should only be called during
; initialization, directly before service call that switches to mcp_run flow
psp_init:
	MSR	PSP, r0
	MOV	r0, #0x2
	MSR	CONTROL, r0
	MOV	pc, lr

; -- psp_set -------------------------------------------------------------------
; Sets systems PSP to point to the addr in r0
psp_set:
	MSR	PSP, r0
	MOV	pc, lr

; -- msp_get -------------------------------------------------------------------
; Returns addr of MSP. Intended to be used during init and maybe in certain
; housekeeping functions.
msp_get:
	MRS	r0, MSP
	MOV	pc, lr

; -- msp_set -------------------------------------------------------------------
; Sets MSP addr to value in r0. Beware.
msp_set:
	MSR	MSP, r0
	MOV	pc, lr

; -- mcp_start ---------------------------------------------------------
; TODO: Update to take an arg that can be used by the svc handler to determine
; which syscalls to invoke.
; Syscall used to start main mcp process / control flow
mcp_start:
	SVC #0xFF ; svc call -1

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
