/*
 * lialgo.h
 *
 *  Created on: 2016.11.8
 *      Author: Yun
 */

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

void litestAlgo();


//void lipriorityqueue_push2(void *s, void* node, int n,int esize, int (*f)(void*, void*, ...), void* finput);
//void lipriorityqueue_pop2(void *s, int n,int esize, int (*f)(void*, void*, ...), void* finput);
//int liheapsort2(void*s, int n, int esize, int (*f)(void*, void*, ...), void* finput);
//int liquicksort2(void*s, int n, int esize, int (*f)(void*, void*, ...), void* finput);
//int limergesort2(void* s, int n, int esize, int (*f)(void*, void*, ...), void* finput);

void lipriorityqueue_push(void *s, void *node, int n, int esize, int (*f)(void *, void *));

void lipriorityqueue_pop(void *s, int n, int esize, int (*f)(void *, void *));

int liheapsort(void *s, int n, int esize, int (*f)(void *, void *));

int liquicksort(void *s, int n, int esize, int (*f)(void *, void *));

int limergesort(void *s, int n, int esize, int (*f)(void *, void *));

int lilogarithm2(unsigned int);

void li10to2(int x);

int licharto2(char c, char *bits);

void li10tox(int base, char *s);

int li10toxN(int base, int sn, int **valp, int *usep);

void lixto10(int base, char *s);

void li2to10(char *s);

void lixto10(int base, char *s);

void li16to10(char *s);

int xjdecrypt(FILE *fps, FILE *fpt);

int xjencrypt(FILE *fps, FILE *fpt);

int liencrypt(char *cps, char *cpt);

int lidecrypt(char *cps, char *cpt);

int lisafeFile(char *input, char *out2, char *key);

#ifdef __cplusplus
}
#endif
