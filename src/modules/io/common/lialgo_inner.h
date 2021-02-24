/*
 * lialgo_inner.h
 *
 *  Created on: 2016.11.10
 *      Author: Yun
 */

#ifndef COMMON_LIALGO_INNER_H_
#define COMMON_LIALGO_INNER_H_

void satisfyHeapAsLeafNode2(void *s, int start, int n, int esize,
		int (*f)(void*, void*, ...), void* finput);
void satisfyHeapAsRootNode2(void*s, int start, int n, int esize,
		int (*f)(void*, void*, ...), void* finput);
void heapify2(void*s, int n, int esize, int (*f)(void*, void*, ...), void* finput);

void satisfyHeapAsLeafNode(void *s, int start, int n, int esize,
		int (*f)(void*, void*));
void satisfyHeapAsRootNode(void*s, int start, int n, int esize,
		int (*f)(void*, void*));
void heapify(void*s, int n, int esize, int (*f)(void*, void*));

#endif /* COMMON_LIALGO_INNER_H_ */
