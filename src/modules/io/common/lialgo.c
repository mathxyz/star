#include "listring.h"
#include "lialgo_inner.h"

#include "listring.h"
//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include "lialgo.h"
#include "lifile.h"

int xjmd5() {
    return 1;
}


/*
 * n is priority queue length after pushing.
 * priority queue pop largest node, push node to priority queue
 *the first liswap  can not use memcpy instead, 'cause we are not responsible for free, but why?
 * p is parent node, j is child node.
 */
void lipriorityqueue_push2(void *s, void *node, int n, int esize,
                           int (*f)(void *, void *, ...), void *finput) {
    int j = n - 1;
    memcpy((char *) s + esize * j, node, esize);
    satisfyHeapAsLeafNode2(s, j, n, esize, f, finput);
}

/*
 * Enhance means this operation violates queue property.
 * pop start to last.
 * n is priority queue length. After popping, n decreases one.
 * pop s[start], move last to s[start], and satisfyHeap
 */
int lipriorityqueueEnhance_pop2(void *s, int start, int n, int esize,
                                int (*f)(void *, void *, ...), void *finput) {
    if (n <= 0 || start >= n || start < 0)
        return 0;
    if (n == 1)
        return 1;
    liswap((char *) s + esize * start, (char *) s + esize * (n - 1), esize);
    satisfyHeapAsLeafNode2(s, start, n - 1, esize, f, finput);
    satisfyHeapAsRootNode2(s, start, n - 1, esize, f, finput);
    return 1;
}

/*
 * pop first to last.
 * n is priority queue length. After popping, n decreases one.
 * pop s[0], move last to s[0], and satisfyHeap
 */
void lipriorityqueue_pop2(void *s, int n, int esize, int (*f)(void *, void *, ...), void *finput) {
    lipriorityqueueEnhance_pop2(s, 0, n, esize, f, finput);
}

/*
 * node start breaks heap property.
 * n is heap size
 * we just make child tree heap, where node start is child tree root.
 * So size n tree may not be heap.
 */
void satisfyHeapAsRootNode2(void *s, int start, int n, int esize,
                            int (*f)(void *, void *, ...), void *finput) {
    int root = start, lc, big;
    while ((lc = 2 * root + 1) < n) {
        big = root;
        if (f((char *) s + esize * big, (char *) s + esize * lc, finput) < 0)
            big = lc;
        if (lc + 1 < n && f((char *) s + esize * big, (char *) s + esize * (lc + 1), finput) < 0)
            big = lc + 1;
        if (big != root) {
            liswap((char *) s + esize * big, (char *) s + esize * root, esize);
            root = big;
        } else
            break;
    }
}

/*
 * node start breaks sub-tree heap property where node start is leaf node of this sub-tree.
 * root of sub-tree is root of tree
 * n is heap size of tree
 * we just make child tree heap, where node start is child tree leaf node.
 * So size n tree may not be heap.
 */
void satisfyHeapAsLeafNode2(void *s, int start, int n, int esize,
                            int (*f)(void *, void *, ...), void *finput) {
    if (start >= n || start < 0)
        return;
    int j = start, p;
    while ((p = (j - 1) / 2) != j) {
        if (f((char *) s + esize * p, (char *) s + esize * j, finput) < 0) {
            liswap((char *) s + esize * p, (char *) s + esize * j, esize);
            j = p;
        } else
            break;
    }
}

/*
 * Enhance means this operation violates queue property.
 * node start violates heap.
 * n is priority queue length. After edit, and satisfyHeap
 */
int lipriorityqueueEnhance_AfterEdit2(void *s, int start, int n, int esize,
                                      int (*f)(void *, void *, ...), void *finput) {
    if (n <= 0 || start >= n || start < 0)
        return 0;
    if (n == 1)
        return 1;
    satisfyHeapAsLeafNode2(s, start, n, esize, f, finput);
    satisfyHeapAsRootNode2(s, start, n, esize, f, finput);
    return 1;
}

/*
 * heapify from bottom to top
 * par is internal node of tree
 * par decrease one every time.
 * n is numbers
 * we grow heap from tree bottom to tree root.
 */
void heapify2(void *s, int n, int esize, int (*f)(void *, void *, ...), void *finput) {
    int par = n / 2 - 1;
    while (par >= 0) {
        satisfyHeapAsRootNode2(s, par, n, esize, f, finput);
        par--;
    }
}

/*
 * use max heap to realize sort from small to big.
 */
int liheapsort2(void *s, int n, int esize, int (*f)(void *, void *, ...), void *finput) {
    if (n < 0)
        return 0;
    if (n <= 1)
        return 1;
    heapify2(s, n, esize, f, finput);
    int i = n - 1;
    liswap(s, (char *) s + esize * i, esize);
    while (i > 1) {
        satisfyHeapAsRootNode2(s, 0, i, esize, f, finput);
        i--;
        liswap(s, (char *) s + esize * i, esize);
    }
    return 1;
}

int liquicksort2(void *s, int n, int esize, int (*f)(void *, void *, ...), void *finput) {
    void *pivot = s;
    int i, j;
    if (n <= 1)
        return 1;
    i = 0;
    j = n;
    while (1) {
        while (i < n - 1 && f((char *) s + esize * (++i), pivot, finput) <= 0);
        while (j > 0 && f((char *) s + esize * (--j), pivot, finput) >= 0);
        if (i < j)
            liswap((char *) s + esize * i, (char *) s + esize * j, esize);
        else
            break;
    }
    liswap(pivot, (char *) s + esize * j, esize);
    liquicksort2(s, j, esize, f, finput);
    liquicksort2((char *) s + esize * (j + 1), n - j - 1, esize, f, finput);
    return 1;
}

int limergesort2(void *s, int n, int esize, int (*f)(void *, void *, ...), void *finput) {
    int l, r, i, j, k;
    l = n / 2;
    r = n - n / 2;
    if (n > 1) {
        limergesort2(s, l, esize, f, finput);
        limergesort2((char *) s + esize * l, r, esize, f, finput);
    }
    void *sn = malloc(n * esize);
    if (!sn)
        return 0;
    i = 0;
    j = 0;
    k = 0;
    while (i < l && j < r) {
        // <= must have =, which ensure stability.
        if (f((char *) s + esize * i, (char *) s + esize * (l + j), finput) <= 0) {
            memcpy((char *) sn + esize * k, (char *) s + esize * i, esize);
            i++;
        } else {
            memcpy((char *) sn + esize * k, (char *) s + esize * (l + j), esize);
            j++;
        }
        k++;
    }
    if (i < l) {
        memcpy((char *) sn + esize * k, (char *) s + esize * i, esize * (l - i));
    } else {
        memcpy((char *) sn + esize * k, (char *) s + esize * (l + j), esize * (r - j));
    }
    memcpy(s, sn, esize * n);
    free(sn);
    return 1;
}


/*
 * old lipriorityqueue_push, can be deleted.
 */
void lipriorityqueue_pushzzzzz(void *s, void *node, int n, int esize,
                               int (*f)(void *, void *, ...), void *finput) {
    int j = n - 1, p;
    memcpy((char *) s + esize * j, node, esize);
    while ((p = (j - 1) / 2) != j) {
        if (f((char *) s + esize * p, (char *) s + esize * j) < 0) {
            liswap((char *) s + esize * p, (char *) s + esize * j, esize);
            j = p;
        } else
            break;
    }
}

/*
 * n is priority queue length after pushing.
 * priority queue pop largest node, push node to priority queue
 *the first liswap  can not use memcpy instead, 'cause we are not responsible for free, but why?
 * p is parent node, j is child node.
 */
void lipriorityqueue_push(void *s, void *node, int n, int esize,
                          int (*f)(void *, void *)) {
    int j = n - 1;
    memcpy((char *) s + esize * j, node, esize);
    satisfyHeapAsLeafNode(s, j, n, esize, f);
}

/*
 * pop first to last.
 * n is priority queue length. After popping, n decreases one.
 * pop s[0], move last to s[0], and satisfyHeap
 */
void lipriorityqueue_popZZZ(void *s, int n, int esize, int (*f)(void *, void *)) {
    if (n <= 1)
        return;
    liswap(s, (char *) s + esize * (n - 1), esize);
    satisfyHeapAsRootNode(s, 0, n - 1, esize, f);
}

/*
 * Enhance means this operation violates queue property.
 * pop start to last.
 * n is priority queue length. After popping, n decreases one.
 * pop s[start], move last to s[start], and satisfyHeap
 */
int lipriorityqueueEnhance_pop(void *s, int start, int n, int esize,
                               int (*f)(void *, void *)) {
    if (n <= 0 || start >= n || start < 0)
        return 0;
    if (n == 1)
        return 1;
    liswap((char *) s + esize * start, (char *) s + esize * (n - 1), esize);
    satisfyHeapAsLeafNode(s, start, n - 1, esize, f);
    satisfyHeapAsRootNode(s, start, n - 1, esize, f);
    return 1;
}

/*
 * pop first to last.
 * n is priority queue length. After popping, n decreases one.
 * pop s[0], move last to s[0], and satisfyHeap
 */
void lipriorityqueue_pop(void *s, int n, int esize, int (*f)(void *, void *)) {
    lipriorityqueueEnhance_pop(s, 0, n, esize, f);
}

/*
 * node start breaks heap property.
 * n is heap size
 * we just make child tree heap, where node start is child tree root.
 * So size n tree may not be heap.
 */
void satisfyHeapAsRootNode(void *s, int start, int n, int esize,
                           int (*f)(void *, void *)) {
    int root = start, lc, big;
    while ((lc = 2 * root + 1) < n) {
        big = root;
        if (f((char *) s + esize * big, (char *) s + esize * lc) < 0)
            big = lc;
        if (lc + 1 < n && f((char *) s + esize * big, (char *) s + esize * (lc + 1)) < 0)
            big = lc + 1;
        if (big != root) {
            liswap((char *) s + esize * big, (char *) s + esize * root, esize);
            root = big;
        } else
            break;
    }
}

/*
 * node start breaks sub-tree heap property where node start is leaf node of this sub-tree.
 * root of sub-tree is root of tree
 * n is heap size of tree
 * we just make child tree heap, where node start is child tree leaf node.
 * So size n tree may not be heap.
 */
void satisfyHeapAsLeafNode(void *s, int start, int n, int esize,
                           int (*f)(void *, void *)) {
    if (start >= n || start < 0)
        return;
    int j = start, p;
    while ((p = (j - 1) / 2) != j) {
        if (f((char *) s + esize * p, (char *) s + esize * j) < 0) {
            liswap((char *) s + esize * p, (char *) s + esize * j, esize);
            j = p;
        } else
            break;
    }
}

/*
 * Enhance means this operation violates queue property.
 * node start violates heap.
 * n is priority queue length. After edit, and satisfyHeap
 */
int lipriorityqueueEnhance_AfterEdit(void *s, int start, int n, int esize,
                                     int (*f)(void *, void *)) {
    if (n <= 0 || start >= n || start < 0)
        return 0;
    if (n == 1)
        return 1;
    satisfyHeapAsLeafNode(s, start, n, esize, f);
    satisfyHeapAsRootNode(s, start, n, esize, f);
    return 1;
}

/*
 * heapify from bottom to top
 * par is internal node of tree
 * par decrease one every time.
 * n is numbers
 * we grow heap from tree bottom to tree root.
 */
void heapify(void *s, int n, int esize, int (*f)(void *, void *)) {
    int par = n / 2 - 1;
    while (par >= 0) {
        satisfyHeapAsRootNode(s, par, n, esize, f);
        par--;
    }
}

/*
 * use max heap to realize sort from small to big.
 */
int liheapsort(void *s, int n, int esize, int (*f)(void *, void *)) {
    if (n < 0)
        return 0;
    if (n <= 1)
        return 1;
    heapify(s, n, esize, f);
    int i = n - 1;
    liswap(s, (char *) s + esize * i, esize);
    while (i > 1) {
        satisfyHeapAsRootNode(s, 0, i, esize, f);
        i--;
        liswap(s, (char *) s + esize * i, esize);
    }
    return 1;
}

int liquicksort(void *s, int n, int esize, int (*f)(void *, void *)) {
    void *pivot = s;
    int i, j;
    if (n <= 1)
        return 1;
    i = 0;
    j = n;
    while (1) {
        while (i < n - 1 && f((char *) s + esize * (++i), pivot) <= 0);
        while (j > 0 && f((char *) s + esize * (--j), pivot) >= 0);
        if (i < j)
            liswap((char *) s + esize * i, (char *) s + esize * j, esize);
        else
            break;
    }
    liswap(pivot, (char *) s + esize * j, esize);
    liquicksort(s, j, esize, f);
    liquicksort((char *) s + esize * (j + 1), n - j - 1, esize, f);
    return 1;
}

int limergesort(void *s, int n, int esize, int (*f)(void *, void *)) {
    int l, r, i, j, k;
    l = n / 2;
    r = n - n / 2;
    if (n > 1) {
        limergesort(s, l, esize, f);
        limergesort((char *) s + esize * l, r, esize, f);
    }
    void *sn = malloc(n * esize);
    if (!sn)
        return 0;
    i = 0;
    j = 0;
    k = 0;
    while (i < l && j < r) {
        // <= must have =, which ensure stability.
        if (f((char *) s + esize * i, (char *) s + esize * (l + j)) <= 0) {
            memcpy((char *) sn + esize * k, (char *) s + esize * i, esize);
            i++;
        } else {
            memcpy((char *) sn + esize * k, (char *) s + esize * (l + j), esize);
            j++;
        }
        k++;
    }
    if (i < l) {
        memcpy((char *) sn + esize * k, (char *) s + esize * i, esize * (l - i));
    } else {
        memcpy((char *) sn + esize * k, (char *) s + esize * (l + j), esize * (r - j));
    }
    memcpy(s, sn, esize * n);
    free(sn);
    return 1;
}

void li10to16(int x) {
    livlog("%x\n", x);
}

void li10to8(int x) {
    livlog("%o\n", x);
}

void li10to2(int x) {
    int i = 31;
    while (i >= 0) {
        if (x & 1 << i) {
            lilog("1");
        } else {
            lilog("0");
        }
        i--;
    }
    lilog("\n");
}

// should give char bits[8] first.
int licharto2(char c, char *bits) {
    int i;
    if (bits == 0)
        return 0;
    i = 8;
    while (--i >= 0) {
        if (c & 1)
            bits[i] = '1';
        else
            bits[i] = '0';
        c >>= 1;
    }
    return 1;
}

//void li10Tobase(int base, char* s) {
//
//	int i = listrlen(s);
//	if (i <= 0 || base <= 0)
//		return;
//	int sn = 0;
//	int re = liparseInt(s, &sn);
//	if (re == 0)
//		return;
//	int r = sn % base;
//	int p;
//	int* val;
//	int n=0, use=0;
//	do{
//		r = sn % base;
//		if(liensurevvN(&val, &n, &use, 8, sizeof(int), lifree2)==0){
//						return;
//		}
//		val[use++]=r;
//	}
//	while ((sn = sn / base));
//	while(use--){
//		livlog("%d ", val[use]);
//	}
//	lilog("\n");
//	lifree2((void**)&val,0);
//}
void litestAlgo() {

}

void li10tox(int base, char *s) {
    int *val;
    int use = 0, re;
    int sn = 0;
    if (liparseInt(s, &sn) == 0)
        return;

    if (li10toxN(base, sn, &val, &use) == 0) {
        return;
    }
    while (use--) {
        livlog("%d ", val[use]);
    }
    lilog("\n");
    lifree2((void **) &val, 0);
}

int li10toxN(int base, int sn, int **valp, int *usep) {

    if (base <= 0)
        return 0;

    int r = sn % base;
    int p;
    int n = 0;
    do {
        r = sn % base;
        if (liensurevvN((void **) valp, &n, usep, 8, sizeof(int), lifree2) == 0) {
            return 0;
        }
        (*valp)[(*usep)++] = r;
    } while ((sn = sn / base));
    return 1;
}

void lixto10(int base, char *s) {
    int i = listrlen(s);
    if (i <= 0)
        return;
    int val = 0;
    while (*s) {
        // get current character then increment
        int le = *s++;
        // transform s character to the 4bit equivalent number, using the ascii table indexes
        if (le >= '0' && le <= '9')
            le = le - '0';
        else if (le >= 'a' && le <= 'f')
            le = le - 'a' + 10;
        else if (le >= 'A' && le <= 'F')
            le = le - 'A' + 10;
        else {
            livlog("Error parsing base-%d number\n", base);
            return;
        }
        if (le >= base) {
            livlog("Error parsing base-%d number\n", base);
            return;
        }
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = val * base + le;
    }
    livlog("%d\n", val);
}

void li16to10(char *s) {
    int i = listrlen(s);
    if (i <= 0 || i > 8)
        return;
    int val = 0;
    while (*s) {
        // get current character then increment
        int le = *s++;
        // transform s character to the 4bit equivalent number, using the ascii table indexes
        if (le >= '0' && le <= '9')
            le = le - '0';
        else if (le >= 'a' && le <= 'f')
            le = le - 'a' + 10;
        else if (le >= 'A' && le <= 'F')
            le = le - 'A' + 10;
        else {
            lilog("Error parsing base-16 number\n");
            return;
        }
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | le;
    }
    livlog("%d\n", val);
}

void li2to10(char *s) {
    int i = listrlen(s);
    if (i <= 0 || i > 32)
        return;
    int j = 0;
    int le;
    int val = 0;
    while (i--) {
        le = s[j] - '0';
        if (le <= 1 && le >= 0) {
            val += le << i;
        } else {
            lilog("Error parsing 0~1 number\n");
            return;
        }
        j++;
    }
    livlog("%d\n", val);
}

int lilogarithm2(unsigned int x) {
    int n = 0;
    if (x & 0xffff0000) {
        n += 16;
        x >>= 16;
    }
    if (x & 0xff00) {
        n += 8;
        x >>= 8;
    }
    if (x & 0xf0) {
        x >>= 4;
        n += 4;
    }
    if (x & 0xc) {
        x >>= 2;
        n += 2;
    }
    if (x & 0x2) {
        n += 1;
    }
    return n;
}

int xjencrypt(FILE *fps, FILE *fpt) {
    int ch;
    int j;
    if (fps == NULL) {
        return 0;
    }

    while (1) {
        ch = fgetc(fps);
        if (ch == EOF) {
            break;
        } else {
            j = ch - 1234;
            fputc(j, fpt);
        }
    }
    return 1;
}

int xjdecrypt(FILE *fps, FILE *fpt) {
    int ch;
    int j;
    if (fps == NULL) {
        return 0;
    }

    while (1) {
        j = fgetc(fps);
        if (j == EOF) {
            break;
        } else {
            ch = j + 1234;
            fputc(ch, fpt);
        }
    }
    return 1;
}

int liencrypt(char *cps, char *cpt) {
    if (!cps || !cpt)
        return 0;
    FILE *fps = fopen(cps, "r");
    if (!fps)
        return 0;
    FILE *fpt = fopen(cpt, "w");
    if (!fpt) {
        fclose(fps);
        return 0;
    }
    int re = xjencrypt(fps, fpt);
    fclose(fps);
    fclose(fpt);
    return re;
}

int lidecrypt(char *cps, char *cpt) {
    if (!cps || !cpt)
        return 0;
    FILE *fps = fopen(cps, "r");
    if (!fps)
        return 0;
    FILE *fpt = fopen(cpt, "w");
    if (!fpt) {
        fclose(fps);
        return 0;
    }
    int re = xjdecrypt(fps, fpt);
    fclose(fps);
    fclose(fpt);
    return re;
}

//int lisxor(char* s, char*key,int ksize){
//	int i=0,j=0;
//	while(s+j){
//		s[j]^key[i%ksize];
//	}
//}
int lixor(FILE *fps, FILE *fpt, char *key) {
    char ch;
    size_t j;
    int sl = listrlen(key);
    if (fps == NULL || sl <= 0) {
        return 0;
    }
    int fl = xjstreamsize(fps);
    int i = 0, per = 0;
    int l = fl / 10;
    if (l == 0)
        l = 1;
    while (i < fl) {
        if (i % l == 0) {
            if (per <= 100)
                livlog("processed %%%d\n", per);
            per += 10;
        }
        fread(&ch, 1, 1, fps);
//		ch = fgetc(fps);

        j = ch ^ *(key + i % sl);
        i++;
//		fputc(j, fpt);
        fwrite(&j, 1, 1, fpt);

    }
    return 1;
}

int lisafeFile(char *input, char *out2, char *key) {
    int re = 1;
    FILE *fpt = 0;
    FILE *fps = 0;
    char *out = 0;
    fps = fopen(input, "rb");
    if (!fps) {
        re = 0;
        goto label;
    }
    out = liscat2N(input, ".0000.tmp");
    if (!out) {
        re = 0;
        goto label;
    }
    if (re == 1 && (fpt = fopen(out, "wb")) != 0) {

        if (lixor(fps, fpt, key) == 0) {
            re = 0;
            goto label;
        }
    }

    lifclose(&fps);
    lifclose(&fpt);
    remove(input);

    if (liisNull(out2)) {
        if ((rename(out, input)) != 0)
            re = 0;
    } else {
        if ((rename(out, out2)) != 0)
            re = 0;
    }

    label:
    if (fpt) {
        fclose(fpt);
    }
    if (fps) {
        fclose(fps);
    }

    if (re)
        lilog("Safe Success!\n");
    else {
        lilog("Safe Fail!\n");
        remove(out);
    }
    free(out);
    return re;
}
