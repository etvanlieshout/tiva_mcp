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
	//while (queue_table[temp.next].pid >= 0) {
	//	if (queue_table[temp.next].priority > pri) {
	//		break;
	//	}
	//	temp = queue_table[temp.next];
	//}

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

void q_init(int qid)
{
	queue_table[QHEAD(qid)].pid = QHEAD(qid);
	queue_table[QHEAD(qid)].priority = -1;
	queue_table[QHEAD(qid)].next = QTAIL(qid);
	queue_table[QHEAD(qid)].prev = -1;

	queue_table[QTAIL(qid)].pid = QTAIL(qid);
	queue_table[QTAIL(qid)].priority = PRI_BOUND+1;
	queue_table[QTAIL(qid)].next = -1;
	queue_table[QTAIL(qid)].prev = QHEAD(qid);

	return;
}
