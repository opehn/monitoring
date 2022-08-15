#include <stdlib.h>

struct test {
	struct test *next;
};

struct test	*make_list(void)
{
	struct test *t;
	struct test *head;
	struct test *cur;
	struct test *new;
	int			i = 0;

	head = NULL;
	while (i < 10)	
	{
		new = (struct test *)malloc(sizeof(struct test));
		new->next = NULL;
		if (!head)
		{
			head = new;
			cur = new;
		}
		else
		{
			cur->next = new;
			cur = cur->next;
		}
		i++;
	}
	return (head);
}

void	free_all(struct test **t)
{
	struct test *temp;

	while (*t)
	{
		temp = (*t)->next;
		free(*t);
		(*t) = temp;
	}
}
int main(void)
{
	struct test *t;

	t = make_list();
	free_all(&t);

}
