#include "tap.c/tap.c"

#define MSLL_VALUE_TYPE int
#include "mlinkedlist.c"

int cmpfn(int a, int b) {
    return a < b ? -1 : a > b ? 1 : 0;
}

int main(void) {
    msll_t *msll = msll_new();
    msll_append(msll, 3);
    msll_append(msll, 2);
    msll_append(msll, 1);
    msll_msort(msll, cmpfn);

    tap_plan(4);
    tap_is_int(msll_length(msll), 3, NULL);
    tap_is_int(msll_at(msll, 0)->value, 1, NULL);
    tap_is_int(msll_at(msll, 1)->value, 2, NULL);
    tap_is_int(msll_at(msll, 2)->value, 3, NULL);

    msll_free(msll);
    return tap_finish();
}
