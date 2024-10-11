;===============================================================================
;	initialization assembly code
;===============================================================================

	.text
	.global psp_set
	.global msp_get
	.global svc_call

; -- psp_set -------------------------------------------------------------------
; Sets systems PSP to point to the addr in r0, then sets the MCU's control
; register to have the processor use the PSP
psp_set:
	MSR	PSP, r0
	MOV	r0, #0x2
	MSR	CONTROL, r0
	MOV	pc, lr


; -- msp_get -------------------------------------------------------------------
; Returns addr of MSP. Not currently used. Intended to be used during init and
; maybe in certain housekeeping functions. Probably not needed.
msp_get:
	MRS	r0, MSP
	MOV	pc, lr

; -- MAIN TRAP HANDLER ---------------------------------------------------------
; TODO: Update to take an arg that can be used by the svc handler to determine
; which syscalls to invoke.
svc_call:
	SVC #0

