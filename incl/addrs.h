/* addr.h
 * Headers containing #define constants for various core addresses
 * */

#ifndef _ADDR_H
#define _ADDR_H

#define COREBASE	0xE000E000	/* Core Peripherals Control Registers Base */
#define STCTRL	0x010      	/* SysTick Control Reg */
#define STRELOAD	0x014	/* SysTick Reload Value; 24-bit max value */
#define	STCURRENT	0x018	/* SysTick current value; reg is write-clear */

#define	SYSPRI3	0xD20	/* System IRQ Priorities Reg 3 */
#endif
