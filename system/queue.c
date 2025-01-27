/* queue.c - enqueue, dequeue, isempty, nonempty, et al. */

#include <xinu.h>
#include <stdlib.h>

/**
 * Prints out contents of a queue
 * @param q	pointer to a queue
 */
void	printqueue(struct queue *q)
{
	//TODO - print all contents from head to tail

	//TODO - format should be [(pid,key), (pid,key), ...]
	kprintf("[");
	struct qentry *checkQ = q->head;
	while(checkQ->nextMember != NULL) {
		kprintf("(pid=%d), ", checkQ->pid);
		checkQ = checkQ->nextMember;
	}
	kprintf("(pid=%d)]\n", checkQ->pid);
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	isempty(struct queue *q)
{
	//TODO
	if(q->size == 0 || q == NULL) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

/**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	nonempty(struct queue *q)
{
	//TODO - don't just check q's size because q could be NULL
	if(q == NULL || q->size == 0) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}


/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	isfull(struct queue *q)
{
	//TODO - check if there are at least NPROC processes in the queue
	if(q->size == NPROC) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


/**
 * Insert a process at the tail of a queue
 * @param pid	ID process to insert
 * @param q	Pointer to the queue to use
 *
 * @return pid on success, SYSERR otherwise
 */
pid32 enqueue(pid32 pid, struct queue *q)
{
        //TODO - check if queue is full and if pid is illegal, and return SYSERR if either is true
		if(isfull(q) || isbadpid(pid)) {
			return SYSERR;
		}
        //TODO - allocate space on heap for a new QEntry
		struct qentry *newEntry = (struct qentry*)malloc(sizeof(struct qentry));
        //TODO - initialize the new QEntry
		if(!isempty(q)) {
			newEntry->nextMember = NULL;
			newEntry->prevMember = q->tail;
			newEntry->pid = pid;
        	//TODO - insert into tail of queue
			q->tail->nextMember = newEntry;
			q->tail = newEntry;
		}
		else {
			//if queue is empty
			newEntry->nextMember = NULL;
			newEntry->prevMember = NULL;
			newEntry->pid = pid;
        	//TODO - insert into tail of queue
			q->tail = newEntry;
			q->head = newEntry;
		}
        //TODO - return the pid on success
		q->size++;
		return pid;
}


/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return pid of the process removed, or EMPTY if queue is empty
 */
pid32 dequeue(struct queue *q)
{
        //TODO - return EMPTY if queue is empty
		if(isempty(q)) {
			return EMPTY;
		}
        //TODO - get the head entry of the queue
		struct qentry *headEntry = q->head;
        //TODO - unlink the head entry from the rest
		q->head = headEntry->nextMember;
        //TODO - free up the space on the heap
		pid32 headPid = headEntry->pid;
		free(headEntry, sizeof(struct qentry));
		q->size--;
        //TODO - return the pid on success
		return headPid;
}


/**
 * Finds a queue entry given pid
 * @param pid	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct qentry *getbypid(pid32 pid, struct queue *q)
{
	//TODO - return NULL if queue is empty or if an illegal pid is given
	if(isempty(q) || isbadpid(pid)) {
		return NULL;
	}
	//TODO - find the qentry with the given pid
	struct qentry *checkQ = q->head;
	while(checkQ->nextMember != NULL) {
		if(checkQ->pid == pid) {
			return checkQ;
		}
		else {
			checkQ = checkQ->nextMember;
		}
	}
	//TODO - return a pointer to it
	if(q->tail->pid == pid) {
		return q->tail;
	}
	else {
		return NULL;
	}
}

/**
 * Remove a process from the front of a queue (pid assumed valid with no check)
 * @param q	pointer to a queue
 * @return pid on success, EMPTY if queue is empty
 */
pid32	getfirst(struct queue *q)
{
	//TODO - return EMPTY if queue is empty
	if(isempty(q)) {
		return EMPTY;
	}
	//TODO - remove process from head of queue and return its pid
	return dequeue(q);
}

/**
 * Remove a process from the end of a queue (pid assumed valid with no check)
 * @param q	Pointer to the queue
 * @return pid on success, EMPTY otherwise
 */
pid32	getlast(struct queue *q)
{
	//TODO - return EMPTY if queue is empty
	if(isempty(q)) {
		return EMPTY;
	}
	//TODO - remove process from tail of queue and return its pid
	return remove(q->tail->pid, q);
}



/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
pid32	remove(pid32 pid, struct queue *q)
{
	//TODO - return EMPTY if queue is empty
	if(isempty(q)) {
		return EMPTY;
	}
	//TODO - return SYSERR if pid is illegal
	if(isbadpid(pid)) {
		return SYSERR;
	}
	//TODO - remove process identified by pid parameter from the queue and return its pid
	struct qentry *foundEntry = getbypid(pid, q);
	//TODO - if pid does not exist in the queue, return SYSERR
	if(foundEntry == NULL) {
		return SYSERR;
	}
	else {
		pid32 foundEntryPid = foundEntry->pid;
		if(foundEntry->prevMember == NULL) {
			foundEntryPid = dequeue(q);
		}
		else if(foundEntry->nextMember == NULL) {
			foundEntry->prevMember->nextMember = NULL;
			foundEntryPid = foundEntry->pid;
		}
		else {
			foundEntry->prevMember->nextMember = foundEntry->nextMember;
			foundEntry->nextMember->prevMember = foundEntry->prevMember;
			foundEntryPid = foundEntry->pid;
		}
		free(foundEntry, sizeof(struct qentry));
		q->size--;
		return foundEntryPid;
	}
}
