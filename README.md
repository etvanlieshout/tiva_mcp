# tiva_mcp
Basic scheduler & process management for TI Tiva-C ARM MCU.

## MCP: MAIN CONTROL PROGRAM

The Burroughs Corporation created their Master Control Program as an operating
system for their mainframe computers.

The name is cooler than "operating system" and lives on here as Main Control
Program or Main Control Procedures or whatever sounds coolest and is abbreviated
MCP.

## Features and Limitations

This extremely bare-bones scheduler and process management does no error
checking and only implements basic functionality. Processes cannot take initial
parameters.

**Features**
+ Simple multitasking
    + Scheduler with priority-based ready queue
    + Scheduler algo is round-robin within given priority
    + Main mcp process (quasi-kernel) runs at regular intervals between other
      processes
+ Basic process management: create, exit, and kill processes
+ Each process given its own stack/ memory region
+ Simple heap allocator
+ Simple library + drivers for:
    + GPIO
    + SPI
    + 1502 LCD screen (via daughter board)

Target: Designed to run on the TI Tiva tm4c123gh6pm mcu (ARM Cortex-M4).

Intended to be built within the TI Code Composer Studio IDE. NOTE:
"${PROJECT_ROOT}/incl/" must be added to include path within CCS project build
settings.

Portability:
Timer interrupt initialization is board-specific; haven't checked compatibility
with other Tiva mcus. Test code (blinking LEDs) is also specific to the
tm4c123gh6pm mcu. Some features (LCD screen demo) are specific to the ALICE
EDUBASE V-2 daughter board from trainer4edu.com.

TODO:
+ Switch rescheduling timer to SysTick
