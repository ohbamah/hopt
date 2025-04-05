/*
 * MIT License
 * 
 * Copyright (c) 2025 OhBama (Yann Manchon)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

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
hopt_add_back(t_hopt_sort** head, t_hopt_sort* node)
{
	if (!*head)
		*head = node;
	else
	{
		t_hopt_sort*	tmp = *head;
		while (tmp->next && tmp->index != node->index)
			tmp = tmp->next;
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
