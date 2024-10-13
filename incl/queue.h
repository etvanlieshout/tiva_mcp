/* queue.h
 * definitions for processes queues (ready_q, other queues to be added later)
 */

#ifndef	_QUEUE_H
#define	_QUEUE_H

#define TABLESZ	(NUMPROC + 2) //size of q data struct
#define READYQ	NUMPROC       //marks head node of ready queue; effectively a
			      //qid

struct	q_entry	{
	int	pid; //redundant, but makes life easier
	int	priority; // lower number => higher priority
	int	next; // prev & next are the index in the table of the next
	int	prev; // item in the queue
};

struct q_entry	queue_table[TABLESZ];

// queue macros
#define	QHEAD(q)	(q)
#define QTAIL(q)	(q+1)
#define QFIRST(q)	(queue_table[QHEAD(q)].next)

// queue manipulation funciton prototypes (definied in queue.c)
int	dequeue(int qid);
int	q_insert(int qid, int pid, int pri);
void	q_init(int qid);
void q_remove(int qid, int pid);

#endif
