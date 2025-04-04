#include "__hopt_.h"

t_hopt_sort*
hopt_new_node(unsigned int index, unsigned int argc)
{
	t_hopt_sort*	ret;

	ret = malloc(sizeof(t_hopt_sort));
	if (ret)
	{
		ret->index = index;
		ret->argc = argc;
		ret->next = NULL;
	}
	return (ret);
}

void
hopt_add_front(t_hopt_sort** head, t_hopt_sort* node)
{
	if (!*head)
		*head = node;
	else
	{
		t_hopt_sort*	tmp = *head;
		*head = node;
		(*head)->next = tmp;
	}
}

void
hopt_add_back(t_hopt_sort** head, t_hopt_sort* node) //? Opti --> Sauvegarder l'adresse du dernier element et modifier son next, tout simple N fois plus rapide
{
	//if (!*head)
	//	*head = node;
	//else
	{
		t_hopt_sort*	tmp = *head;
		//while (tmp->next && tmp->index != node->index)
		//	tmp = tmp->next;
		if (tmp->index == node->index)
		{
			if (node->argc > tmp->argc)
				tmp->argc = node->argc;
			free(node);
		}
		else
			tmp->next = node;
	}
}

void
hopt_free_lstsort(t_hopt_sort* head)
{
	while (head)
	{
		t_hopt_sort* tmp = head;
		head = head->next;
		free(tmp);
	}
}
