#include "collect.h"
#include "agent_queue.h"

void    free_queue(aqueue *q)
{
    aqueue_node *temp;
    aqueue_node *head;
    head = q->head;
	int i = 0;
    while (head)
    {
        temp = head->next;
        free(head->data->payload);
        free(head->data);
        free(head);
        head = temp;
		i++;
    }
	free(q);
}

int main(void)
{
	pthread_t		collect_tid;
	pthread_t		send_tid;
	aparam			*p;
	aqueue			*q;
	struct sigaction act;

	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	sigaction(SIGPIPE, &act, NULL);	
	p = malloc(sizeof (aparam));
	p->q = init_aqueue();
	pthread_create(&collect_tid, NULL, collect_routine, (void *)p);
	pthread_create(&send_tid, NULL, send_routine, (void *)p);

	pthread_join(collect_tid, NULL);
	pthread_join(send_tid, NULL);

	pthread_mutex_destroy(&p->q->aqueue_lock);
	free_queue(p->q);
	free(p);
	return (0);
}
