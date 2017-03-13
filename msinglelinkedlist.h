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

#ifndef MSINGLELINKEDLIST_H
#define MSINGLELINKEDLIST_H

#include <stdint.h> /* uintmax_t */

#ifndef MSLL_VALUE_TYPE
#define MSLL_VALUE_TYPE void*
#endif

typedef MSLL_VALUE_TYPE msll_value_t;

typedef int (*msll_eqfn_t)(msll_value_t data, msll_value_t needle);
typedef int (*msll_cmpfn_t)(msll_value_t data, msll_value_t needle);
typedef int (*msll_traversefn_t)(msll_value_t value, void *ctx);

typedef struct msll_t {
    struct msll_item_t *first, *last;
    uintmax_t length;
} msll_t;

typedef struct msll_item_t {
    msll_value_t value;
    struct msll_item_t *next;
} msll_item_t;

msll_t *msll_new(void);
void msll_free(msll_t *msll);

/* traversal */
msll_item_t *msll_first(msll_t *msll);
msll_item_t *msll_last(msll_t *msll);
msll_item_t *msll_at(msll_t *msll, uintmax_t index);
msll_item_t *msll_next(msll_t *msll, msll_item_t *item);
msll_item_t *msll_prev(msll_t *msll, msll_item_t *item);

/* index-based operations */
msll_item_t *msll_insert_before(msll_t *msll, uintmax_t index, msll_value_t data);
msll_item_t *msll_insert_after(msll_t *msll, uintmax_t index, msll_value_t data);
int msll_remove(msll_t *msll, uintmax_t index, msll_value_t *value);

msll_item_t *msll_push(msll_t *msll, msll_value_t value);
int msll_pop(msll_t *msll, msll_value_t *value);

msll_item_t *msll_append(msll_t *msll, msll_value_t value);
int msll_shift(msll_t *msll, msll_value_t *value);

/* item-based operations */
msll_item_t *msll_insert_before_item(msll_t *msll, msll_item_t *item, msll_value_t data);
msll_item_t *msll_insert_after_item(msll_t *msll, msll_item_t *item, msll_value_t data);
int msll_remove_item(msll_t *msll, msll_item_t *item, msll_value_t *value);

uintmax_t msll_length(msll_t *msll);
void msll_reverse(msll_t *msll);

msll_item_t *msll_lfind(msll_t *msll, msll_value_t needle, msll_eqfn_t cmp);
msll_item_t *msll_rfind(msll_t *msll, msll_value_t needle, msll_eqfn_t cmp);

msll_t *msll_copy(msll_t *msll);
int msll_consume(msll_t *msll, msll_t *items);

int msll_traverse(msll_t *msll, msll_traversefn_t fn, void *ctx);

void msll_msort(msll_t *msll, msll_cmpfn_t fn);

#endif /* MSINGLELINKEDLIST_H */
