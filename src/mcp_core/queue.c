/* ===========================================================================*\
 * queue.c
 *
 * Defines queue manipulation functions for ready Q (and other, future queues)
 *
 * Contents:
 * dequeue()
 * q_insert()
 * q_init()
 *
 * NOTES:
 * Assumed to be called from within syscalls
 *============================================================================*/

#include <mcp.h>

/* -- dequeue ------------------------------------------------------------------
 * Removes first item in queue
 */
int	dequeue(q)
{
	int pid = queue_table[QFIRST(q)].pid;

	// decouple from list
	queue_table[QHEAD(q)].next = queue_table[pid].next;
	queue_table[queue_table[pid].next].prev = QHEAD(q);

	return pid;
}

/* -- q_insert -----------------------------------------------------------------
 * Inserts new process into queue. Position determined by priority. makes no
 * check on validity of params.
 *
 * ARGS:
 * q: integer identifying QHEAD
 *
 * NOTES:
 * Queue structure: Fixed HEAD & TAIL nodes, empty queue is HEAD + TAIL
 */
int	q_insert(q, pid, pri)
{
	// Get head of queue specified by q arg
	struct q_entry temp = queue_table[QHEAD(q)];

	// cycle through queue to find insertion position
	while (queue_table[temp.next].pid < QTAIL(q)) {
		if (queue_table[temp.next].priority > pri) {
			break;
		}
		temp = queue_table[temp.next];
	}

	// insert new process after temp
	queue_table[pid].pid = pid;
	queue_table[pid].priority = pri;
	queue_table[pid].next = temp.next;
	queue_table[pid].prev = temp.pid;
	queue_table[temp.next].prev = pid;
	queue_table[temp.pid].next  = pid;

	return pid;
}

/* -- q_remove -----------------------------------------------------------------
 */
void q_remove(int qid, int pid)
{
	int next = queue_table[pid].next;
	int prev = queue_table[pid].prev;

	queue_table[prev].next = queue_table[pid].next;
	queue_table[next].prev = queue_table[pid].prev;
}

/* -- q_init -------------------------------------------------------------------
 */
void q_init(int qid)
{
	// Initialize Head of queue
	queue_table[QHEAD(qid)].pid = QHEAD(qid);
	queue_table[QHEAD(qid)].priority = -1;
	queue_table[QHEAD(qid)].next = QTAIL(qid);
	queue_table[QHEAD(qid)].prev = -1;

	// Initialize Tail of queue
	queue_table[QTAIL(qid)].pid = QTAIL(qid);
	queue_table[QTAIL(qid)].priority = PRI_BOUND+1;
	queue_table[QTAIL(qid)].next = -1;
	queue_table[QTAIL(qid)].prev = QHEAD(qid);

	return;
}
