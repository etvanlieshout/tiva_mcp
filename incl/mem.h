/* mem.h
 * Header file for memory-related functions &c.
 */

#ifndef	_MEM_H
#define	_MEM_H

void	*alloc(uint16_t size);
void	*heap_reset();
void	alloc_copy(void *p);

#endif
