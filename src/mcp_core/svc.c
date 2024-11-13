/* ===========================================================================*\
 * svc.c
 *
 * Contains System Call / Service Call handlers and code
 *
 * Contents:
 * svc_handler()
 *============================================================================*/

#include <mcp.h>

/* -- SVC Handler ---------------------------------------------------------
 * Handler for SVC. Extracts immediate value and takes action accordingly.
 */
void svc_handler()
{
	uint8_t svc_code = svc_code_get();

	switch (svc_code) {
		case 0xFF:
			start_mcp_proc();
			break;
		case 0xE1:
			break;
		default:
			break;
	}
}

void systick_isr()
{
	_set_pendsv();
	return;
}

void _set_pendsv(){
	*(volatile uint32_t *) (0xE000ED04) |= 0x10000000;
}
