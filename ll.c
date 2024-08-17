/**
 * @file 		ll.c
 *
 * @brief 		Code file for Multi Thread Safe Linked List 
 *
 * @copyright 	Copyright (C) 2024 Barrett Edwards. All rights reserved.
 *
 * @date 		Aug 2024
 * @author 		Barrett Edwards
 * 
 */

/* INCLUDES ==================================================================*/

/* printf()
 */
#include <stdio.h>

#include "ll.h"

/* MACROS ====================================================================*/

#define LIB_EXPORT          __attribute__ ((visibility ("default")))

/* ENUMERATIONS ==============================================================*/

/* STRUCTS ===================================================================*/

/* GLOBAL VARIABLES ==========================================================*/

/* PROTOTYPES ================================================================*/

/* FUNCTIONS =================================================================*/

LIB_EXPORT int ll_add_(struct ll_head *h, struct ll_node *n)
{
	int rv = -1;
	
	if (h == NULL || n == NULL)
		return -1;

	pthread_mutex_lock(&h->mtx);
	{
		if (n->head != NULL)
			goto unlock;

		n->head = h;
		n->next = &h->n;
		n->prev = h->n.prev;

		h->n.prev->next = n;
		h->n.prev = n;

		h->num++;
		rv = h->num;
	}

unlock:

	pthread_mutex_unlock(&h->mtx);

	return rv;
}

LIB_EXPORT int ll_del_(struct ll_head *h, struct ll_node *n)
{
	int rv = -1;

	if (h == NULL || n == NULL)
		return -1;

	pthread_mutex_lock(&h->mtx);
	{
		if (n->head != h)
			goto unlock;

		n->prev->next = n->next;
		n->next->prev = n->prev;

		n->head = NULL;
		n->next = NULL;
		n->prev = NULL;

		h->num--;
		rv = h->num;
	}

unlock:

	pthread_mutex_unlock(&h->mtx);

	return rv;
}

LIB_EXPORT void ll_init(struct ll_head *h)
{
	pthread_mutex_init(&h->mtx, NULL); 	
	h->n.head = NULL;
	h->n.next = &h->n;
	h->n.prev = &h->n;
	h->num = 0;
}

LIB_EXPORT int ll_empty(struct ll_head *h)
{
	return h->num == 0;
}

LIB_EXPORT void *ll_first_(struct ll_head *h, int offset)
{
	void* ptr = NULL; 

	if (h == NULL)
		return NULL;

	pthread_mutex_lock(&h->mtx);
	{
		if (h->n.next != &h->n)
		{
			ptr = ((char*)h->n.next)-offset;
		}
	}
	pthread_mutex_unlock(&h->mtx);
	return ptr;	
}

LIB_EXPORT void ll_free(struct ll_head *h)
{
	pthread_mutex_destroy(&h->mtx);
}

LIB_EXPORT void *ll_next_(struct ll_head *h, void *item, int offset)
{
	void *ptr = NULL; 
	struct ll_node *n;

	if (h == NULL || item == NULL)
		return NULL;

	n = (struct ll_node*) (((char*)item)+offset);

	pthread_mutex_lock(&h->mtx);
	{
		if (n->head != h)
			goto unlock;

		if (n->next != &h->n)
		{
			ptr = ((char*)n->next)-offset;
		}
	}

unlock:

	pthread_mutex_unlock(&h->mtx);
	return ptr;	
}

LIB_EXPORT int ll_num(struct ll_head *h)
{
	return h->num;
}

LIB_EXPORT void *ll_pop_(struct ll_head *h, int offset)
{
	void* ptr = NULL; 
	struct ll_node *n;

	if (h == NULL)
		return NULL;

	pthread_mutex_lock(&h->mtx);
	{
		if (h->n.next != &h->n)
		{
			n = h->n.next;

			ptr = ((char*)n)-offset;

			n->prev->next = n->next;
			n->next->prev = n->prev;
	
			n->head = NULL;
			n->next = NULL;
			n->prev = NULL;
	
			h->num--;
		}
	}
	pthread_mutex_unlock(&h->mtx);
	return ptr;	
}

LIB_EXPORT void ll_print_(struct ll_head *h, int offset)
{
	struct ll_node *n;
	if (h == NULL)
		return;

	pthread_mutex_lock(&h->mtx);
	{
		printf("ll_head: head: %p node: %p next: %p prev: %p num: %d\n", h, &h->n, h->n.next, h->n.prev, h->num);

		n = h->n.next; 

		while ( n != &h->n)
		{
			printf("ll_node: head: %p node: %p next: %p prev: %p obj: %p\n", n->head, n, n->next, n->prev, (((char*)n)-offset));
			n = n->next;
		}
	}
	pthread_mutex_unlock(&h->mtx);
}
