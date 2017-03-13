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

#ifndef MSINGLELINKEDLIST_C
#define MSINGLELINKEDLIST_C

#include <errno.h> /* errno */
#include <stdlib.h> /* calloc, free */
#include <string.h> /* memcpy, memset, strcmp */

#include "mlinkedlist.h"

msll_t *msll_new(void) {
    return calloc(sizeof(msll_t), 1);
}

msll_item_t *_msll_item_new(msll_value_t value) {
    msll_item_t *item = calloc(sizeof(msll_item_t), 1);
    item->value = value;
    return item;
}

void msll_clear(msll_t *msll) {
    msll_item_t *next = msll->first;
    while(next) {
        msll_item_t *item = next;
        next = item->next;
        free(item);
    }
    memset(msll, 0, sizeof(msll_t));
}

void msll_free(msll_t *msll) {
    if(msll) {
        msll_clear(msll);
        free(msll);
    }
}

uintmax_t msll_length(msll_t *msll) {
    return msll->length;
}

msll_value_t msll_value(msll_item_t *item) {
    return item->value;
}

msll_item_t *mll_first(msll_t *msll) {
    return msll->first;
}

msll_item_t *msll_last(msll_t *msll) {
    return msll->last;
}

msll_item_t *msll_at(msll_t *msll, uintmax_t index) {
    msll_item_t *item = msll->first;
    while(item && index--) { item = item->next; }
    return item;
}

msll_item_t *msll_next(msll_t *msll, msll_item_t *item) {
    return item != NULL ? item->next : msll->first;
}

msll_item_t *msll_prev(msll_t *msll, msll_item_t *item) {
    msll_item_t *prev;
    if(item == NULL) { return msll->last; }
    if(item == msll->first) { return NULL; }
    for(prev = msll->first; prev && prev->next != item; prev = prev->next);
    return prev;
}

msll_item_t *msll_insert_before(msll_t *msll, uintmax_t index, msll_value_t value) {
    if(index == 0) {
        return msll_push(msll, value);
    } else {
        msll_item_t *prev = msll_at(msll, index - 1);
        if(prev == NULL) { errno = EINVAL; return NULL; }
        return msll_insert_after_item(msll, prev, value);
    }
}

msll_item_t *msll_insert_before_item(msll_t *msll, msll_item_t *next, msll_value_t value) {
    msll_item_t *item;

    if(next == NULL) { errno = EINVAL; return NULL; }
    if(msll->length == UINTMAX_MAX) { errno = EOVERFLOW; return NULL; }
    if((item = _msll_item_new(value)) == NULL) { return NULL; }

    if(next == msll->first) {
        msll->first = item;
    } else {
        msll_prev(msll, next)->next = item;
    }
    item->next = next;

    msll->length++;
    return item;
}

msll_item_t *msll_insert_after(msll_t *msll, uintmax_t index, msll_value_t value) {
    msll_item_t *prev = msll_at(msll, index);
    if(prev == NULL) { errno = EINVAL; return NULL; }
    else { return msll_insert_after_item(msll, prev, value); }
}

msll_item_t *msll_insert_after_item(msll_t *msll, msll_item_t *prev, msll_value_t value) {
    msll_item_t *item;

    if(prev == NULL) { errno = EINVAL; return NULL; }
    if(msll->length == UINTMAX_MAX) { errno = EOVERFLOW; return NULL; }
    if((item = _msll_item_new(value)) == NULL) { return NULL; }

    if(prev == msll->last) {
        msll->last = item;
    }

    item->next = prev->next;
    prev->next = item;
    msll->length++;

    return item;
}

msll_item_t *msll_push(msll_t *msll, msll_value_t value) {
    msll_item_t *item = _msll_item_new(value);
    if(item == NULL) { return NULL; }
    if(msll->length == 0) {
        msll->last = item;
    }
    item->next = msll->first;
    msll->first = item;
    msll->length++;
    return item;
}

int msll_pop(msll_t *msll, msll_value_t *value) {
    return msll_remove_item(msll, msll->first, value);
}

msll_value_t msll_pop_value(msll_t *msll) {
    msll_value_t value;
    return msll_remove_item(msll, msll->first, &value) == 0
        ? value : (msll_value_t) 0;
}

msll_item_t *msll_append(msll_t *msll, msll_value_t value) {
    msll_item_t *item = _msll_item_new(value);
    if(item == NULL) { return NULL; }
    if(msll->first == NULL) {
        msll->first = item;
    } else {
        msll->last->next = item;
    }
    msll->last = item;
    msll->length++;
    return item;
}

int msll_remove_item(msll_t *msll, msll_item_t *item, msll_value_t *value) {
    if(item == NULL) {
        errno = EINVAL;
        return -1;
    }

    if(item == msll->first) {
        msll->first = item->next;
        if(item == msll->last) { msll->last = NULL; }
    } else {
        msll_item_t *prev = msll_prev(msll, item);
        prev->next = item->next;
        if(item == msll->last) { msll->last = prev; }
    }

    if(value) { *value = item->value; }
    free(item);
    msll->length--;
    return 0;
}

int msll_remove(msll_t *msll, uintmax_t index, msll_value_t *value) {
    return msll_remove_item(msll, msll_at(msll, index), value);
}

int msll_shift(msll_t *msll, msll_value_t *value) {
    return msll_remove_item(msll, msll->last, value);
}

msll_value_t msll_shift_value(msll_t *msll) {
    msll_value_t value;
    return msll_remove_item(msll, msll->last, &value) == 0
        ? value : (msll_value_t) 0;
}

void msll_reverse(msll_t *msll) {
    msll_item_t *prev = NULL, *item = msll->first;

    msll->first = msll->last;
    msll->last = item;

    while(item) {
        msll_item_t *next = item->next;
        item->next = prev;
        prev = item;
        item = next;
    }
}

msll_t *msll_copy(msll_t *msll) {
    msll_t *copy = msll_new();
    msll_item_t *item;
    if(copy == NULL) { return NULL; }
    for(item = msll->first; item; item = item->next) {
        if(msll_append(copy, item->value) == NULL) {
            msll_free(copy); 
            return NULL;
        }
    }
    return copy;
}

msll_item_t *msll_lfind(msll_t *msll, msll_value_t needle, msll_eqfn_t cmp) {
    msll_item_t *item;
    for(item = msll->first; item; item = item->next) {
        if(cmp(item->value, needle) == 0) {
            return item;
        }
    }
    return NULL;
}

msll_item_t *msll_rfind(msll_t *msll, msll_value_t needle, msll_eqfn_t cmp) {
    msll_item_t *item;
    for(item = msll->last; item; item = msll_prev(msll, item)) {
        if(cmp(item->value, needle) == 0) {
            return item;
        }
    }
    return NULL;
}

int msll_consume(msll_t *msll, msll_t *items) {
    if(msll->first == NULL) {
        memcpy(msll, items, sizeof(msll_t));
    } else if(items->first != NULL) {
        if(UINTMAX_MAX  - msll->length < items->length) {
            errno = EOVERFLOW;
            return -1;
        }

        msll->last->next = items->first;
        msll->last = items->last;
        msll->length += items->length;
    }

    free(items);
    return 0;
}

int msll_traverse(msll_t *msll, msll_traversefn_t fn, void *ctx) {
    msll_item_t *item;
    for(item = msll->first; item; item = item->next) {
        int ret = fn(item->value, ctx);
        if(ret != 0) { return ret; }
    }
    return 0;
}

msll_item_t *_msll_msort_merge(msll_item_t *left, msll_item_t *right, msll_cmpfn_t fn) {
    msll_item_t *sorted = NULL, **s = &sorted;
    while(left && right) {
        if(fn(left->value, right->value) <= 0) {
            *s = left;
            left = left->next;
        } else {
            *s = right;
            right = right->next;
        }
        s = &((*s)->next);
    }
    *s = left ? left : right;
    return sorted;
}

msll_item_t *_msll_msort(msll_item_t *items, uintmax_t n, msll_cmpfn_t fn) {
    msll_item_t *divider = items, *right = NULL;
    uintmax_t left_len = n / 2, right_len = n - left_len;

    if(items == NULL || items->next == NULL) { return items; }
    while(--left_len) {
        divider = items->next;
    }

    right = divider->next;
    divider->next = NULL;

    items = _msll_msort(items, n - right_len, fn);
    right = _msll_msort(right, right_len, fn);

    return _msll_msort_merge(items, right, fn);
}

void msll_msort(msll_t *msll, msll_cmpfn_t fn) {
    msll_item_t *last;
    msll->first = _msll_msort(msll->first, msll->length, fn);
    for(last = msll->first; last && last->next; last = last->next);
    msll->last = last;
}

#endif /* MSINGLELINKEDLIST_C */
