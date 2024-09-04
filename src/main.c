#include <stdio.h>
#include <mcp.h>
/**
 * main.c - Tiva-C scheduler
 */
int main(void)
{
    /* includes spurious code with impossible call to resched_timer_isr
     * to prevent his being optimized out.
     */
    resched_timer_isr();
	return 0;
}
