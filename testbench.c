/**
 * @file 		testbench.c
 *
 * @brief 		Code file for testbench of Multi Thread Safe Linked List 
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

/* malloc()
 */
#include <stdlib.h>

/* uint8_t 
 */
#include <stdint.h>

/* memset()
 */
#include <string.h>

#include "ll.h"

/* MACROS ====================================================================*/

#define AUVB_DEFAULT_WIDTH 	16	//!< Default width for printing uint8_t buffers 
#define AUVB_MIN_WIDTH 		4	//!< Minium width for printing buffers

/* ENUMERATIONS ==============================================================*/

/* STRUCTS ===================================================================*/

struct test 
{
	char *name;
	int (*fn)();
};

/* 
 * This is an example object that would be added to the linked list
 */
struct object
{
	int i;
	struct ll_node list;	//!< The name of the ll_node is 
};

/* PROTOTYPES ================================================================*/

int test_for_each();

/* GLOBAL VARIABLES ==========================================================*/

struct test tests[] = 
{
		{"for_each()", 			test_for_each},
		{0,0}
};

/* FUNCTIONS =================================================================*/

/*
 * Print a unsigned char buffer 
 */
void autl_prnt_buf(void *buf, unsigned long len, unsigned long width, int print_header)
{
	unsigned long i, j, k, rows;
	uint8_t *ptr;

	/* STEP 1: Verify Inputs */
	if ( buf == NULL) 
		return;

	if ( len == 0 )
		return;
	
	if ( width == 0 )
		width = AUVB_DEFAULT_WIDTH;
	
	if ( width < AUVB_MIN_WIDTH )
		width = AUVB_MIN_WIDTH;

	ptr = (uint8_t*) buf;

	/* Compute the number of rows to print */
	rows = len / width;
	if ( (len % width) > 0)
		rows++;

	/* Print index '0x0000: ' */
	if (print_header) {
		printf("            ");
		for ( i = 0 ; i < width ; i++ )
			printf("%02lu ", i);
		printf("\n");
	}

	k = 0;
	for ( i = 0 ; i < rows ; i++ ) {
		printf("0x%08lx: ", i * width);
		for ( j = 0 ; j < width ; j++ ) {
			if (k >= len)
				break;

			printf("%02x ", ptr[i*width + j]);

			k++;
		}
		printf("\n");
	}

	return;
}

int test_for_each()
{
	struct object array[10];
	struct ll_head h; 
	struct object *a =  NULL;

	// Initialize variables
	memset(array, 0, 10 * sizeof(struct object));
	ll_init(&h);

	// Set field in each object 
	for ( int i = 0 ; i < 10 ; i++ )
		array[i].i = i;

	// Print out the address of each object in the array 
	printf("Object Array:\n");
	for ( int i = 0 ; i < 10 ; i++ )
		printf("[%i] obj: %p ll_node: %p\n", i, &array[i], &array[i].list);

	// Add each object to the list 
	for ( int i = 0 ; i < 10 ; i++ )
		ll_add(&h, &array[i], list);

	ll_print(&h, struct object, list);

	printf("Test ll_first() -------------------\n");	
	ll_first(&h, a, list);
	printf("obj: %p val: %d\n", a, a->i);

	printf("Test ll_next() --------------------\n");	
	ll_first(&h, a, list);
	while (a != NULL)
	{
		printf("obj: %p val: %d\n", a, a->i);
		a = ll_next(&h, a, list);
	}

	printf("Test ll_for_each() ----------------\n");	
	ll_for_each(&h, a, list)
		printf("obj: %p val: %d\n", a, a->i);
	
	printf("Test ll_del() middle entry---------\n");
	ll_del(&h, &array[4], list);
	ll_for_each(&h, a, list)
		printf("obj: %p val: %d\n", a, a->i);
	
	printf("num in list after delete: %d\n", ll_num(&h));

	printf("Test ll_del() last entry ----------\n");
	ll_del(&h, &array[9], list);
	ll_for_each(&h, a, list)
		printf("obj: %p val: %d\n", a, a->i);
	
	printf("num: %d\n", ll_num(&h));

	printf("Test ll_del() first entry----------\n");
	ll_del(&h, &array[0], list);
	ll_for_each(&h, a, list)
		printf("obj: %p val: %d\n", a, a->i);

	printf("num: %d\n", ll_num(&h));

	printf("Test pop() ------------------------\n");
	printf("num in list before pop(): %d\n", ll_num(&h));
	ll_pop(&h, a, list);
	while (a != NULL)
	{
		printf("pop(): obj: %p val: %d\n", a, a->i);
		ll_pop(&h, a, list);
	}
	printf("num in list after pop(): %d\n", ll_num(&h));

	printf("Test for_pop_each -----------------\n");

	// Add each object back into the list 
	for ( int i = 0 ; i < 10 ; i++ )
		ll_add(&h, &array[i], list);

	printf("num in array before ll_pop_each(): %d\n", ll_num(&h));

	ll_pop_each(&h, a, list)
		printf("obj: %p val: %d\n", a, a->i);
	
	printf("num in array after ll_pop_each(): %d\n", ll_num(&h));

	ll_free(&h);

	return 0;
}

int main(int argc, char **argv)
{
	int rv, i, max;
	struct test *t;

	// Initialize variables
	rv = 1; 

	// Find length of tests array 
	for ( max = 0, t = tests ; t->fn != NULL ; t++)
		max++;

	// Get selection or print name of selection
	if (argc > 1)
		i = atoi(argv[1]);
	else 
	{
		for ( i = 0, t = tests ; t->fn != NULL ; t++)
			printf("%d: %s\n", i++, t->name);
		goto end;
	}

	// Validate user selection 
	if (i < 0 || i > max)
		goto end;

	// Execute Test 
	printf("TEST %d: %s\n", i, tests[i].name);

	rv = tests[i].fn();

end:

	return rv;
}

