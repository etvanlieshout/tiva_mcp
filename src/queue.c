/* queue.c
 * defines queue manipulation functions for ready q and others
 * dequeue(), insert()
 */

#include <mcp.h>

int	dequeue(q)
{
	int pid = queue_table[QFIRST(q)].pid;

	// decouple from list
	queue_table[QHEAD(q)].next = queue_table[pid].next;
	queue_table[queue_table[pid].next].prev = QHEAD(q);

	return pid;
}

// makes no check on validity of params
int	q_insert(q, pid, pri)
{
	//traverse queue for insertion position
	struct q_entry temp = queue_table[QHEAD(q)];
	while (queue_table[temp.next].pid > 0) {
		if (queue_table[temp.next].priority > pri) {
			break;
		}
		temp = queue_table[temp.next];
	}
	// insert new process after temp
	queue_table[pid].next = temp.next;
	queue_table[pid].prev = temp.pid;
	queue_table[temp.next].prev = pid;
	temp.next = pid;

	return pid;
}
