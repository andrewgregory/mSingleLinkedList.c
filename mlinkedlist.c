/*
 * Copyright 2014 Andrew Gregory <andrew.gregory.8@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Project URL: http://github.com/andrewgregory/mLinkedList.c
 */

#ifndef MLINKEDLIST_C
#define MLINKEDLIST_C

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "mlinkedlist.h"

mll_t *mll_tail(mll_t *mll) {
    if(mll == NULL) { return NULL; }
    while(mll->next) { mll = mll->next; }
}

mll_t *mll_next(mll_t *mll) {
    if(mll) { return mll->next; }
    else { return NULL; }
}

mll_t *mll_at(mll_t *mll, size_t index) {
    if(mll == NULL) { return NULL; }
    while(mll->next && index--) { mll = mll->next; }
    return index == 0 ? mll : NULL;
}

mll_t *mll_push(mll_t **mll, void *data) {
    mll_t *item = malloc(sizeof(mll_t));
    if(item == NULL) { return NULL; }
    item->next = *mll;
    item->data = data;
    *mll = item;
    return item;
}

void *mll_pop(mll_t **mll) {
    if(*mll == NULL) {
        return NULL;
    } else {
        mll_t *head = *mll;
        void *data = head->data;
        *mll = head->next;
        free(head);
        return data;
    }
}

void mll_free(mll_t *mll) {
    mll_t *i;
    while((i = mll)) {
        mll = mll->next;
        free(i);
    }
}

mll_t *mll_insert_after(mll_t *mll, void *data) {
    mll_t *item = malloc(sizeof(mll_t));
    if(item == NULL) { return NULL; }
    item->data = data;
    item->next = mll->next;
    mll->next = item;
    return item;
}

int mll_remove_item(mll_t **mll, mll_t *item) {
    if(*mll == item) {
        *mll = item->next;
    } else {
        mll_t *i;
        for(i = *mll; i->next != NULL && i->next != item; i = i->next);
        if(i == NULL || i->next == NULL) { errno = EINVAL; return -1; }
        i->next = item->next;
    }
    free(item);
    return 0;
}

size_t mll_size(mll_t *mll) {
    size_t size;
    for(size = 0; mll; size++, mll = mll->next);
    return size;
}

void mll_reverse(mll_t **mll) {
    mll_t *prev = NULL, *head = *mll;
    while(head) {
        mll_t *next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    *mll = prev;
}

mll_t *mll_lfind(mll_t *mll, void *needle, mll_cmpfn_t cmp) {
    while(mll) {
        if(cmp(mll->data, needle) == 0) { return mll; }
        else { mll = mll->next; }
    }
    return NULL;
}

mll_t *mll_lfind_str(mll_t *mll, char *needle) {
    while(mll) {
        if(strcmp(mll->data, needle) == 0) { return mll; }
        else { mll = mll->next; }
    }
    return NULL;
}

mll_t *mll_lfind_ptr(mll_t *mll, void *needle) {
    while(mll) {
        if(mll->data == needle) { return mll; }
        else { mll = mll->next; }
    }
    return NULL;
}

#endif /* MLINKEDLIST_C */
