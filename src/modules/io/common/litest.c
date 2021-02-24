/*
 * litest.c
 *
 *  Created on: 2016.11.10
 *      Author: Yun
 */
#include "liCommonInner.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

void lipriorityqueuetest(){
	int i,n=5;
	char** ss= malloc(sizeof*ss* n);
	char *s, *filename="hhiihijklmn";

	for(i=0;i<n;i++)
	{
		s=lisubstringN(filename, i, -1);
		lipriorityqueue_push(ss,&s,i+1,sizeof(*ss),licmpss);
	}

	for(i=n;i>0;i--){
		printf("%d:%s\n",i,ss[0]);
		lipriorityqueue_pop(ss, i,sizeof(*ss), licmpss);
	}
	lifreess(ss,n);
}
void litest2() {
	int i = 0;
	if (i--) {
		printf("i:%d", i);
	} else {
		printf("i2:%d", i);
	}
}

void liheapsorttest2() {
	int n=5;
	char* ab[5] = { "helloc", "ok", "what", "noo", "haha" };
	liheapsort(ab, n, sizeof(*ab), licmpss);
	liprintfss(ab, n);
}

void liheapsorttest() {
	int ab[] = { 36, 28, 15, 33, 22, 99, 8, 92 };
	liheapsort(ab,  sizeof(ab)/sizeof(*ab), sizeof(*ab), licmpint);
	liprintfint(ab,  sizeof(ab)/sizeof(*ab));
}

void liquicksorttest() {
	int ab[] = { 36, 28, 15, 33, 22, 99, 8, 92 };
	liquicksort(ab, sizeof(ab)/sizeof(*ab), sizeof(*ab), licmpint);
	liprintfint(ab, sizeof(ab)/sizeof(*ab));
}

void limergesorttest() {
	char* ab[5] = { "helloc", "ok", "what", "noo", "haha" };
	limergesort(ab, 5, sizeof(*ab), licmpss);

	liprintfss(ab, 5);
}

void lishuffletest2() {
	int ia[5] = { 3, 6, 24, 87, 99 };
	lishuffle(ia, 5, sizeof(int), 123);
	liprintfint(ia, 5);
	lideshuffle(ia, 5, sizeof(int), 123);
	liprintfint(ia, 5);
}

void lishuffletest() {
	char* sa[5] = { "helloc", "ok", "what", "noo", "haha" };
	lishuffle(sa, 5, sizeof(*sa), 123);
	liprintfss(sa, 5);
	lideshuffle(sa, 5, sizeof(*sa), 123);
	liprintfss(sa, 5);
}

void litest() {
//	lipriorityqueuetest();
//	liheapsorttest2();
//	liheapsorttest();
//	liquicksorttest();
	litestlistring();
	litestlifile();
	litestAlgo();
//	lipriorityqueuetest();
//	litestHuff();

}

void lidebugtest(){
	lipriorityqueuetest();
	liheapsorttest();
	limergesorttest();
	lishuffletest();
	lisearchFiles2Ntest(".");
	lisearchFileSufNtest();
}

void lidebugtest2(int a){
    lipriorityqueuetest();
    liheapsorttest();
    limergesorttest();
    lishuffletest();
//	lisearchFiles2Ntest(".");
//	lisearchFileSufNtest();

}