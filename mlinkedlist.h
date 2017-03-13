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

#ifndef MLINKEDLIST_H
#define MLINKEDLIST_H

typedef size_t (*mll_cmpfn_t)(void *data, void *needle);

typedef struct mll_t {
    void *data;
    struct mll_t *next;
} mll_t;

mll_t *mll_tail(mll_t *mll);
mll_t *mll_next(mll_t *mll);
mll_t *mll_at(mll_t *mll, size_t index);

int mll_insert(mll_t **mll, size_t index, void *data);
int mll_remove(mll_t **mll, mll_t *item);

size_t mll_size(mll_t *mll);

mll_t *mll_lfind(mll_t *mll, void *needle, mll_cmpfn_t cmp);
mll_t *mll_lfind_str(mll_t *mll, char *needle);
mll_t *mll_lfind_ptr(mll_t *mll, void *needle);

#endif /* MLINKEDLIST_H */
