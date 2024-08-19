/**
 * @file 		ll.h 
 *
 * @brief 		Header file for Multi Thread Safe Linked List 
 *
 * @copyright 	Copyright (C) 2024 Barrett Edwards. All rights reserved.
 *
 * @date 		Aug 2024
 * @author 		Barrett Edwards
 *
 * Macro / Enumeration Prefixes 
 * 
 */

#ifndef _LL_H
#define _LL_H

/* INCLUDES ==================================================================*/

/* pthread_mutex_t 
 */
#include <pthread.h>

#include <stddef.h>

/* MACROS ====================================================================*/

#define ll_first(head, item, name)		item = ll_first_(head, offsetof(typeof(*item), name))

#define ll_next(head, item, name) 		ll_next_(head, item, offsetof(typeof(*item), name))

#define ll_for_each(head, item, name)  	for(item = ll_first_(head, offsetof(typeof(*item), name)) ; item != NULL ; item = ll_next_(head, item, offsetof(typeof(*item), name)))

#define ll_add(head, item, name) 		ll_add_(head, (struct ll_node*)(((char*)item)+offsetof(typeof(*item), name)))

#define ll_del(head, item, name) 		ll_del_(head, (struct ll_node*)(((char*)item)+offsetof(typeof(*item), name)))

#define ll_pop(head, item, name) 		item = ll_pop_(head, offsetof(typeof(*item), name))

#define ll_pop_each(head, item, name)  	for(item = ll_pop_(head, offsetof(typeof(*item), name)) ; item != NULL ; item = ll_pop_(head, offsetof(typeof(*item), name)))

#define ll_print(head, type, name)		ll_print_(head, offsetof(type, name))

/* ENUMERATIONS ==============================================================*/

/* STRUCTS ===================================================================*/

struct ll_node
{
	struct ll_head *head;
	struct ll_node *next;
	struct ll_node *prev;
};

struct ll_head
{
	struct ll_node n; 
	pthread_mutex_t mtx;
	int num;
};

/* GLOBAL VARIABLES ==========================================================*/

/* PROTOTYPES ================================================================*/

int  	ll_add_(struct ll_head *h, struct ll_node *n);
int  	ll_del_(struct ll_head *h, struct ll_node *n);
int 	ll_empty(struct ll_head *h);
void * 	ll_first_(struct ll_head *h, int offset);
void 	ll_free(struct ll_head *h);
void 	ll_init(struct ll_head *h);
void * 	ll_next_(struct ll_head *h, void *item, int offset);
int  	ll_num(struct ll_head *h);
void * 	ll_pop_(struct ll_head *h, int offset);
void 	ll_print_(struct ll_head *h, int offset);

#endif //ifndef _LL_H

