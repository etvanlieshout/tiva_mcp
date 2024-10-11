# tiva_mcp
Basic scheduler & process management for TI Tiva-C ARM MCU.

## MCP: MAIN CONTROL PROGRAM

The Burroughs Corporation created their Master Control Program as an operating
system for the mainframe computers.

The name is cooler than "operating system" lives on here as Main Control Program
or Main Control Procedures or whatever sounds coolest and is abbrev. MCP.

## Features and Limitations

This extremely bare-bones scheduler and process management does no error
checking and only implements basic functionality. Processes must run forever,
and there is no process kill / abort and cleanup. Processes cannot take initial
parameters.

Context switch does not preserve r4-r11; this important fix is forthcoming ASAP.

Need to add a process end & clean-up function and put its address into the link
register values in each process's initial stack (ie during process create).

Target: Designed to run on the TI Tiva tm4c123gh6pm mcu (ARM Cortex-M4).

Portability:
Timer interrupt initialization is board-specific; haven't checked compatibility
with other Tiva mcus. Test code (blinking LEDs) is also specific to the
tm4c123gh6pm mcu.



<br>

### Keeping your version control clean: NOTE TO SELF
NOTE TO SELF: the ./tiva_mcp directory is the .git directory for this project,
but I have been using a local working directory (usually the copy in the CCS
workspace) when editing and adding files.

The local working directory will occasionally be synced with the .git directory
to be backed up. The idea is to maintain the .git repo as a safe, stable backup
point, while the working directory can be a bit more chaotic. This prevents the
repo from getting messey with rapid, chaotic changes in times of frustration.

This set-up also makes sure that code that gets into the repository has been
vetted, tested, or otherwise confirmed to not be a total mess.
