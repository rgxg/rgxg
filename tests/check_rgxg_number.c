/* rgxg - ReGular eXpression Generator
 *
 * Copyright (c) 2019 Hannes von Haugwitz
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you
 *     must not claim that you wrote the original software. If you use
 *     this software in a product, an acknowledgment in the product
 *     documentation would be appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and
 *     must not be misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *     distribution.
 */

#include <check.h>

/* needed for malloc, free */
#include <stdlib.h>

#include "rgxg/number.h"

typedef struct {
    long long first;
    long long last;
    int base;
    size_t min_length;
    const char *expected_regex;
    rgxg_options_t options;
} range_test_t;

static range_test_t range_tests[] = {
    { 0, 31 , 2, 0, "(1[01]{0,4}|0)", 0 },
    { 0, 31 , 2, 0, "(1[01]{0,4}|0)", RGXG_NOUPPERCASE },
    { 0, 31 , 2, 0, "(1[01]{0,4}|0)", RGXG_NOLOWERCASE },
    { 0, 31 , 2, 0, "1[01]{0,4}|0", RGXG_NOOUTERPARENS },
    { 0, 31 , 2, 0, "1[01]{0,4}|0", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 0, "1[01]{0,4}|0", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 0, "[01]{5}", RGXG_LEADINGZERO },
    { 0, 31 , 2, 0, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 0, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 0, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 2, 0, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 0, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 0, "[01]{1,5}", RGXG_VARLEADINGZERO },
    { 0, 31 , 2, 0, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 0, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 0, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 2, 0, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 0, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 2, 1, "(1[01]{0,4}|0)", 0 },
    { 0, 31 , 2, 1, "(1[01]{0,4}|0)", RGXG_NOUPPERCASE },
    { 0, 31 , 2, 1, "(1[01]{0,4}|0)", RGXG_NOLOWERCASE },
    { 0, 31 , 2, 1, "1[01]{0,4}|0", RGXG_NOOUTERPARENS },
    { 0, 31 , 2, 1, "1[01]{0,4}|0", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 1, "1[01]{0,4}|0", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 1, "[01]{5}", RGXG_LEADINGZERO },
    { 0, 31 , 2, 1, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 1, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 1, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 2, 1, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 1, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 1, "[01]{1,5}", RGXG_VARLEADINGZERO },
    { 0, 31 , 2, 1, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 1, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 1, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 2, 1, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 1, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 2, 5, "(1[01]{0,4}|0)", 0 },
    { 0, 31 , 2, 5, "(1[01]{0,4}|0)", RGXG_NOUPPERCASE },
    { 0, 31 , 2, 5, "(1[01]{0,4}|0)", RGXG_NOLOWERCASE },
    { 0, 31 , 2, 5, "1[01]{0,4}|0", RGXG_NOOUTERPARENS },
    { 0, 31 , 2, 5, "1[01]{0,4}|0", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 5, "1[01]{0,4}|0", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 5, "[01]{5}", RGXG_LEADINGZERO },
    { 0, 31 , 2, 5, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 5, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 5, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 2, 5, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 5, "[01]{5}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 5, "[01]{1,5}", RGXG_VARLEADINGZERO },
    { 0, 31 , 2, 5, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 5, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 5, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 2, 5, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 5, "[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 2, 10, "(1[01]{0,4}|0)", 0 },
    { 0, 31 , 2, 10, "(1[01]{0,4}|0)", RGXG_NOUPPERCASE },
    { 0, 31 , 2, 10, "(1[01]{0,4}|0)", RGXG_NOLOWERCASE },
    { 0, 31 , 2, 10, "1[01]{0,4}|0", RGXG_NOOUTERPARENS },
    { 0, 31 , 2, 10, "1[01]{0,4}|0", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 10, "1[01]{0,4}|0", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 10, "0{5}[01]{5}", RGXG_LEADINGZERO },
    { 0, 31 , 2, 10, "0{5}[01]{5}", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 10, "0{5}[01]{5}", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 10, "0{5}[01]{5}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 2, 10, "0{5}[01]{5}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 10, "0{5}[01]{5}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 10, "0{0,5}[01]{1,5}", RGXG_VARLEADINGZERO },
    { 0, 31 , 2, 10, "0{0,5}[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 10, "0{0,5}[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 2, 10, "0{0,5}[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 2, 10, "0{0,5}[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 2, 10, "0{0,5}[01]{1,5}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 3, 0, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", 0 },
    { 0, 31 , 3, 0, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", RGXG_NOUPPERCASE },
    { 0, 31 , 3, 0, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", RGXG_NOLOWERCASE },
    { 0, 31 , 3, 0, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 0, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 0, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 0, "(101[01]|100[0-2]|0[0-2]{3})", RGXG_LEADINGZERO },
    { 0, 31 , 3, 0, "(101[01]|100[0-2]|0[0-2]{3})", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 0, "(101[01]|100[0-2]|0[0-2]{3})", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 0, "101[01]|100[0-2]|0[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 0, "101[01]|100[0-2]|0[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 0, "101[01]|100[0-2]|0[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 0, "(101[01]|100[0-2]|0?[0-2]{1,3})", RGXG_VARLEADINGZERO },
    { 0, 31 , 3, 0, "(101[01]|100[0-2]|0?[0-2]{1,3})", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 0, "(101[01]|100[0-2]|0?[0-2]{1,3})", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 0, "101[01]|100[0-2]|0?[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 0, "101[01]|100[0-2]|0?[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 0, "101[01]|100[0-2]|0?[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 3, 1, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", 0 },
    { 0, 31 , 3, 1, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", RGXG_NOUPPERCASE },
    { 0, 31 , 3, 1, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", RGXG_NOLOWERCASE },
    { 0, 31 , 3, 1, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 1, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 1, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 1, "(101[01]|100[0-2]|0[0-2]{3})", RGXG_LEADINGZERO },
    { 0, 31 , 3, 1, "(101[01]|100[0-2]|0[0-2]{3})", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 1, "(101[01]|100[0-2]|0[0-2]{3})", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 1, "101[01]|100[0-2]|0[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 1, "101[01]|100[0-2]|0[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 1, "101[01]|100[0-2]|0[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 1, "(101[01]|100[0-2]|0?[0-2]{1,3})", RGXG_VARLEADINGZERO },
    { 0, 31 , 3, 1, "(101[01]|100[0-2]|0?[0-2]{1,3})", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 1, "(101[01]|100[0-2]|0?[0-2]{1,3})", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 1, "101[01]|100[0-2]|0?[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 1, "101[01]|100[0-2]|0?[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 1, "101[01]|100[0-2]|0?[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 3, 2, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", 0 },
    { 0, 31 , 3, 2, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", RGXG_NOUPPERCASE },
    { 0, 31 , 3, 2, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", RGXG_NOLOWERCASE },
    { 0, 31 , 3, 2, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 2, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 2, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 2, "(101[01]|100[0-2]|0[0-2]{3})", RGXG_LEADINGZERO },
    { 0, 31 , 3, 2, "(101[01]|100[0-2]|0[0-2]{3})", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 2, "(101[01]|100[0-2]|0[0-2]{3})", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 2, "101[01]|100[0-2]|0[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 2, "101[01]|100[0-2]|0[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 2, "101[01]|100[0-2]|0[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 2, "(101[01]|100[0-2]|0?[0-2]{1,3})", RGXG_VARLEADINGZERO },
    { 0, 31 , 3, 2, "(101[01]|100[0-2]|0?[0-2]{1,3})", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 2, "(101[01]|100[0-2]|0?[0-2]{1,3})", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 2, "101[01]|100[0-2]|0?[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 2, "101[01]|100[0-2]|0?[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 2, "101[01]|100[0-2]|0?[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 3, 5, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", 0 },
    { 0, 31 , 3, 5, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", RGXG_NOUPPERCASE },
    { 0, 31 , 3, 5, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", RGXG_NOLOWERCASE },
    { 0, 31 , 3, 5, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 5, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 5, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 5, "(0101[01]|0100[0-2]|00[0-2]{3})", RGXG_LEADINGZERO },
    { 0, 31 , 3, 5, "(0101[01]|0100[0-2]|00[0-2]{3})", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 5, "(0101[01]|0100[0-2]|00[0-2]{3})", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 5, "0101[01]|0100[0-2]|00[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 5, "0101[01]|0100[0-2]|00[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 5, "0101[01]|0100[0-2]|00[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 5, "(0?101[01]|0?100[0-2]|0?0?[0-2]{1,3})", RGXG_VARLEADINGZERO },
    { 0, 31 , 3, 5, "(0?101[01]|0?100[0-2]|0?0?[0-2]{1,3})", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 5, "(0?101[01]|0?100[0-2]|0?0?[0-2]{1,3})", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 5, "0?101[01]|0?100[0-2]|0?0?[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 5, "0?101[01]|0?100[0-2]|0?0?[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 5, "0?101[01]|0?100[0-2]|0?0?[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 3, 10, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", 0 },
    { 0, 31 , 3, 10, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", RGXG_NOUPPERCASE },
    { 0, 31 , 3, 10, "(101[01]|100[0-2]|[12][0-2]{0,2}|0)", RGXG_NOLOWERCASE },
    { 0, 31 , 3, 10, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 10, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 10, "101[01]|100[0-2]|[12][0-2]{0,2}|0", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 10, "(0{6}101[01]|0{6}100[0-2]|0{7}[0-2]{3})", RGXG_LEADINGZERO },
    { 0, 31 , 3, 10, "(0{6}101[01]|0{6}100[0-2]|0{7}[0-2]{3})", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 10, "(0{6}101[01]|0{6}100[0-2]|0{7}[0-2]{3})", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 10, "0{6}101[01]|0{6}100[0-2]|0{7}[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 10, "0{6}101[01]|0{6}100[0-2]|0{7}[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 10, "0{6}101[01]|0{6}100[0-2]|0{7}[0-2]{3}", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 10, "(0{0,6}101[01]|0{0,6}100[0-2]|0{0,7}[0-2]{1,3})", RGXG_VARLEADINGZERO },
    { 0, 31 , 3, 10, "(0{0,6}101[01]|0{0,6}100[0-2]|0{0,7}[0-2]{1,3})", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 10, "(0{0,6}101[01]|0{0,6}100[0-2]|0{0,7}[0-2]{1,3})", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 3, 10, "0{0,6}101[01]|0{0,6}100[0-2]|0{0,7}[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 3, 10, "0{0,6}101[01]|0{0,6}100[0-2]|0{0,7}[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 3, 10, "0{0,6}101[01]|0{0,6}100[0-2]|0{0,7}[0-2]{1,3}", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 10, 0, "(3[01]|[12]?[0-9])", 0 },
    { 0, 31 , 10, 0, "(3[01]|[12]?[0-9])", RGXG_NOUPPERCASE },
    { 0, 31 , 10, 0, "(3[01]|[12]?[0-9])", RGXG_NOLOWERCASE },
    { 0, 31 , 10, 0, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 0, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 0, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 0, "(3[01]|[0-2][0-9])", RGXG_LEADINGZERO },
    { 0, 31 , 10, 0, "(3[01]|[0-2][0-9])", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 0, "(3[01]|[0-2][0-9])", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 0, "3[01]|[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 0, "3[01]|[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 0, "3[01]|[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 0, "(3[01]|[0-2]?[0-9])", RGXG_VARLEADINGZERO },
    { 0, 31 , 10, 0, "(3[01]|[0-2]?[0-9])", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 0, "(3[01]|[0-2]?[0-9])", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 0, "3[01]|[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 0, "3[01]|[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 0, "3[01]|[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 10, 1, "(3[01]|[12]?[0-9])", 0 },
    { 0, 31 , 10, 1, "(3[01]|[12]?[0-9])", RGXG_NOUPPERCASE },
    { 0, 31 , 10, 1, "(3[01]|[12]?[0-9])", RGXG_NOLOWERCASE },
    { 0, 31 , 10, 1, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 1, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 1, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 1, "(3[01]|[0-2][0-9])", RGXG_LEADINGZERO },
    { 0, 31 , 10, 1, "(3[01]|[0-2][0-9])", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 1, "(3[01]|[0-2][0-9])", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 1, "3[01]|[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 1, "3[01]|[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 1, "3[01]|[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 1, "(3[01]|[0-2]?[0-9])", RGXG_VARLEADINGZERO },
    { 0, 31 , 10, 1, "(3[01]|[0-2]?[0-9])", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 1, "(3[01]|[0-2]?[0-9])", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 1, "3[01]|[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 1, "3[01]|[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 1, "3[01]|[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 10, 2, "(3[01]|[12]?[0-9])", 0 },
    { 0, 31 , 10, 2, "(3[01]|[12]?[0-9])", RGXG_NOUPPERCASE },
    { 0, 31 , 10, 2, "(3[01]|[12]?[0-9])", RGXG_NOLOWERCASE },
    { 0, 31 , 10, 2, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 2, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 2, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 2, "(3[01]|[0-2][0-9])", RGXG_LEADINGZERO },
    { 0, 31 , 10, 2, "(3[01]|[0-2][0-9])", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 2, "(3[01]|[0-2][0-9])", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 2, "3[01]|[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 2, "3[01]|[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 2, "3[01]|[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 2, "(3[01]|[0-2]?[0-9])", RGXG_VARLEADINGZERO },
    { 0, 31 , 10, 2, "(3[01]|[0-2]?[0-9])", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 2, "(3[01]|[0-2]?[0-9])", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 2, "3[01]|[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 2, "3[01]|[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 2, "3[01]|[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 10, 5, "(3[01]|[12]?[0-9])", 0 },
    { 0, 31 , 10, 5, "(3[01]|[12]?[0-9])", RGXG_NOUPPERCASE },
    { 0, 31 , 10, 5, "(3[01]|[12]?[0-9])", RGXG_NOLOWERCASE },
    { 0, 31 , 10, 5, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 5, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 5, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 5, "(0003[01]|000[0-2][0-9])", RGXG_LEADINGZERO },
    { 0, 31 , 10, 5, "(0003[01]|000[0-2][0-9])", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 5, "(0003[01]|000[0-2][0-9])", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 5, "0003[01]|000[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 5, "0003[01]|000[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 5, "0003[01]|000[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 5, "(0?0?0?3[01]|0?0?0?[0-2]?[0-9])", RGXG_VARLEADINGZERO },
    { 0, 31 , 10, 5, "(0?0?0?3[01]|0?0?0?[0-2]?[0-9])", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 5, "(0?0?0?3[01]|0?0?0?[0-2]?[0-9])", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 5, "0?0?0?3[01]|0?0?0?[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 5, "0?0?0?3[01]|0?0?0?[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 5, "0?0?0?3[01]|0?0?0?[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 10, 10, "(3[01]|[12]?[0-9])", 0 },
    { 0, 31 , 10, 10, "(3[01]|[12]?[0-9])", RGXG_NOUPPERCASE },
    { 0, 31 , 10, 10, "(3[01]|[12]?[0-9])", RGXG_NOLOWERCASE },
    { 0, 31 , 10, 10, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 10, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 10, "3[01]|[12]?[0-9]", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 10, "(0{8}3[01]|0{8}[0-2][0-9])", RGXG_LEADINGZERO },
    { 0, 31 , 10, 10, "(0{8}3[01]|0{8}[0-2][0-9])", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 10, "(0{8}3[01]|0{8}[0-2][0-9])", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 10, "0{8}3[01]|0{8}[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 10, "0{8}3[01]|0{8}[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 10, "0{8}3[01]|0{8}[0-2][0-9]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 10, "(0{0,8}3[01]|0{0,8}[0-2]?[0-9])", RGXG_VARLEADINGZERO },
    { 0, 31 , 10, 10, "(0{0,8}3[01]|0{0,8}[0-2]?[0-9])", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 10, "(0{0,8}3[01]|0{0,8}[0-2]?[0-9])", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 10, 10, "0{0,8}3[01]|0{0,8}[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 10, 10, "0{0,8}3[01]|0{0,8}[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 10, 10, "0{0,8}3[01]|0{0,8}[0-2]?[0-9]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 16, 0, "1?[0-9A-Fa-f]", 0 },
    { 0, 31 , 16, 0, "1?[0-9a-f]", RGXG_NOUPPERCASE },
    { 0, 31 , 16, 0, "1?[0-9A-F]", RGXG_NOLOWERCASE },
    { 0, 31 , 16, 0, "1?[0-9A-Fa-f]", RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 0, "1?[0-9a-f]", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 0, "1?[0-9A-F]", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 0, "[01][0-9A-Fa-f]", RGXG_LEADINGZERO },
    { 0, 31 , 16, 0, "[01][0-9a-f]", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 0, "[01][0-9A-F]", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 0, "[01][0-9A-Fa-f]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 0, "[01][0-9a-f]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 0, "[01][0-9A-F]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 0, "[01]?[0-9A-Fa-f]", RGXG_VARLEADINGZERO },
    { 0, 31 , 16, 0, "[01]?[0-9a-f]", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 0, "[01]?[0-9A-F]", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 0, "[01]?[0-9A-Fa-f]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 0, "[01]?[0-9a-f]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 0, "[01]?[0-9A-F]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 16, 1, "1?[0-9A-Fa-f]", 0 },
    { 0, 31 , 16, 1, "1?[0-9a-f]", RGXG_NOUPPERCASE },
    { 0, 31 , 16, 1, "1?[0-9A-F]", RGXG_NOLOWERCASE },
    { 0, 31 , 16, 1, "1?[0-9A-Fa-f]", RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 1, "1?[0-9a-f]", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 1, "1?[0-9A-F]", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 1, "[01][0-9A-Fa-f]", RGXG_LEADINGZERO },
    { 0, 31 , 16, 1, "[01][0-9a-f]", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 1, "[01][0-9A-F]", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 1, "[01][0-9A-Fa-f]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 1, "[01][0-9a-f]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 1, "[01][0-9A-F]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 1, "[01]?[0-9A-Fa-f]", RGXG_VARLEADINGZERO },
    { 0, 31 , 16, 1, "[01]?[0-9a-f]", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 1, "[01]?[0-9A-F]", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 1, "[01]?[0-9A-Fa-f]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 1, "[01]?[0-9a-f]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 1, "[01]?[0-9A-F]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 16, 2, "1?[0-9A-Fa-f]", 0 },
    { 0, 31 , 16, 2, "1?[0-9a-f]", RGXG_NOUPPERCASE },
    { 0, 31 , 16, 2, "1?[0-9A-F]", RGXG_NOLOWERCASE },
    { 0, 31 , 16, 2, "1?[0-9A-Fa-f]", RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 2, "1?[0-9a-f]", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 2, "1?[0-9A-F]", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 2, "[01][0-9A-Fa-f]", RGXG_LEADINGZERO },
    { 0, 31 , 16, 2, "[01][0-9a-f]", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 2, "[01][0-9A-F]", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 2, "[01][0-9A-Fa-f]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 2, "[01][0-9a-f]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 2, "[01][0-9A-F]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 2, "[01]?[0-9A-Fa-f]", RGXG_VARLEADINGZERO },
    { 0, 31 , 16, 2, "[01]?[0-9a-f]", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 2, "[01]?[0-9A-F]", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 2, "[01]?[0-9A-Fa-f]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 2, "[01]?[0-9a-f]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 2, "[01]?[0-9A-F]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 16, 5, "1?[0-9A-Fa-f]", 0 },
    { 0, 31 , 16, 5, "1?[0-9a-f]", RGXG_NOUPPERCASE },
    { 0, 31 , 16, 5, "1?[0-9A-F]", RGXG_NOLOWERCASE },
    { 0, 31 , 16, 5, "1?[0-9A-Fa-f]", RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 5, "1?[0-9a-f]", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 5, "1?[0-9A-F]", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 5, "000[01][0-9A-Fa-f]", RGXG_LEADINGZERO },
    { 0, 31 , 16, 5, "000[01][0-9a-f]", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 5, "000[01][0-9A-F]", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 5, "000[01][0-9A-Fa-f]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 5, "000[01][0-9a-f]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 5, "000[01][0-9A-F]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 5, "0?0?0?[01]?[0-9A-Fa-f]", RGXG_VARLEADINGZERO },
    { 0, 31 , 16, 5, "0?0?0?[01]?[0-9a-f]", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 5, "0?0?0?[01]?[0-9A-F]", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 5, "0?0?0?[01]?[0-9A-Fa-f]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 5, "0?0?0?[01]?[0-9a-f]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 5, "0?0?0?[01]?[0-9A-F]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 16, 10, "1?[0-9A-Fa-f]", 0 },
    { 0, 31 , 16, 10, "1?[0-9a-f]", RGXG_NOUPPERCASE },
    { 0, 31 , 16, 10, "1?[0-9A-F]", RGXG_NOLOWERCASE },
    { 0, 31 , 16, 10, "1?[0-9A-Fa-f]", RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 10, "1?[0-9a-f]", RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 10, "1?[0-9A-F]", RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 10, "0{8}[01][0-9A-Fa-f]", RGXG_LEADINGZERO },
    { 0, 31 , 16, 10, "0{8}[01][0-9a-f]", RGXG_LEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 10, "0{8}[01][0-9A-F]", RGXG_LEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 10, "0{8}[01][0-9A-Fa-f]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 10, "0{8}[01][0-9a-f]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 10, "0{8}[01][0-9A-F]", RGXG_LEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 10, "0{0,8}[01]?[0-9A-Fa-f]", RGXG_VARLEADINGZERO },
    { 0, 31 , 16, 10, "0{0,8}[01]?[0-9a-f]", RGXG_VARLEADINGZERO|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 10, "0{0,8}[01]?[0-9A-F]", RGXG_VARLEADINGZERO|RGXG_NOLOWERCASE },
    { 0, 31 , 16, 10, "0{0,8}[01]?[0-9A-Fa-f]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS },
    { 0, 31 , 16, 10, "0{0,8}[01]?[0-9a-f]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOUPPERCASE },
    { 0, 31 , 16, 10, "0{0,8}[01]?[0-9A-F]", RGXG_VARLEADINGZERO|RGXG_NOOUTERPARENS|RGXG_NOLOWERCASE },

    { 0, 31 , 4, 0, "(1[0-3]{2}|[1-3]?[0-3])", 0 },
    { 0, 31 , 4, 0, "(1[0-3]{2}|[1-3]?[0-3])", RGXG_NOLOWERCASE },
    { 0, 31 , 4, 0, "[01][0-3]{2}", RGXG_LEADINGZERO },
    { 0, 31 , 4, 0, "[01][0-3]{2}", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 5, 0, "(11[01]|10[0-4]|[1-4]?[0-4])", 0 },
    { 0, 31 , 5, 0, "(11[01]|10[0-4]|[1-4]?[0-4])", RGXG_NOLOWERCASE },
    { 0, 31 , 5, 0, "(11[01]|10[0-4]|0[0-4]{2})", RGXG_LEADINGZERO },
    { 0, 31 , 5, 0, "(11[01]|10[0-4]|0[0-4]{2})", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 6, 0, "(5[01]|[1-4]?[0-5])", 0 },
    { 0, 31 , 6, 0, "(5[01]|[1-4]?[0-5])", RGXG_NOLOWERCASE },
    { 0, 31 , 6, 0, "(5[01]|[0-4][0-5])", RGXG_LEADINGZERO },
    { 0, 31 , 6, 0, "(5[01]|[0-4][0-5])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 7, 0, "(4[0-3]|[1-3]?[0-6])", 0 },
    { 0, 31 , 7, 0, "(4[0-3]|[1-3]?[0-6])", RGXG_NOLOWERCASE },
    { 0, 31 , 7, 0, "(4[0-3]|[0-3][0-6])", RGXG_LEADINGZERO },
    { 0, 31 , 7, 0, "(4[0-3]|[0-3][0-6])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 8, 0, "[1-3]?[0-7]", 0 },
    { 0, 31 , 8, 0, "[1-3]?[0-7]", RGXG_NOLOWERCASE },
    { 0, 31 , 8, 0, "[0-3][0-7]", RGXG_LEADINGZERO },
    { 0, 31 , 8, 0, "[0-3][0-7]", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 9, 0, "(3[0-4]|[12]?[0-8])", 0 },
    { 0, 31 , 9, 0, "(3[0-4]|[12]?[0-8])", RGXG_NOLOWERCASE },
    { 0, 31 , 9, 0, "(3[0-4]|[0-2][0-8])", RGXG_LEADINGZERO },
    { 0, 31 , 9, 0, "(3[0-4]|[0-2][0-8])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 11, 0, "(2[0-9]|1?[0-9Aa])", 0 },
    { 0, 31 , 11, 0, "(2[0-9]|1?[0-9A])", RGXG_NOLOWERCASE },
    { 0, 31 , 11, 0, "(2[0-9]|[01][0-9Aa])", RGXG_LEADINGZERO },
    { 0, 31 , 11, 0, "(2[0-9]|[01][0-9A])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 12, 0, "(2[0-7]|1?[0-9AaBb])", 0 },
    { 0, 31 , 12, 0, "(2[0-7]|1?[0-9AB])", RGXG_NOLOWERCASE },
    { 0, 31 , 12, 0, "(2[0-7]|[01][0-9AaBb])", RGXG_LEADINGZERO },
    { 0, 31 , 12, 0, "(2[0-7]|[01][0-9AB])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 13, 0, "(2[0-5]|1?[0-9A-Ca-c])", 0 },
    { 0, 31 , 13, 0, "(2[0-5]|1?[0-9A-C])", RGXG_NOLOWERCASE },
    { 0, 31 , 13, 0, "(2[0-5]|[01][0-9A-Ca-c])", RGXG_LEADINGZERO },
    { 0, 31 , 13, 0, "(2[0-5]|[01][0-9A-C])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 14, 0, "(2[0-3]|1?[0-9A-Da-d])", 0 },
    { 0, 31 , 14, 0, "(2[0-3]|1?[0-9A-D])", RGXG_NOLOWERCASE },
    { 0, 31 , 14, 0, "(2[0-3]|[01][0-9A-Da-d])", RGXG_LEADINGZERO },
    { 0, 31 , 14, 0, "(2[0-3]|[01][0-9A-D])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 15, 0, "(2[01]|1?[0-9A-Ea-e])", 0 },
    { 0, 31 , 15, 0, "(2[01]|1?[0-9A-E])", RGXG_NOLOWERCASE },
    { 0, 31 , 15, 0, "(2[01]|[01][0-9A-Ea-e])", RGXG_LEADINGZERO },
    { 0, 31 , 15, 0, "(2[01]|[01][0-9A-E])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 17, 0, "(1[0-9A-Ea-e]|[0-9A-Ga-g])", 0 },
    { 0, 31 , 17, 0, "(1[0-9A-E]|[0-9A-G])", RGXG_NOLOWERCASE },
    { 0, 31 , 17, 0, "(1[0-9A-Ea-e]|0[0-9A-Ga-g])", RGXG_LEADINGZERO },
    { 0, 31 , 17, 0, "(1[0-9A-E]|0[0-9A-G])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 18, 0, "(1[0-9A-Da-d]|[0-9A-Ha-h])", 0 },
    { 0, 31 , 18, 0, "(1[0-9A-D]|[0-9A-H])", RGXG_NOLOWERCASE },
    { 0, 31 , 18, 0, "(1[0-9A-Da-d]|0[0-9A-Ha-h])", RGXG_LEADINGZERO },
    { 0, 31 , 18, 0, "(1[0-9A-D]|0[0-9A-H])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 19, 0, "(1[0-9A-Ca-c]|[0-9A-Ia-i])", 0 },
    { 0, 31 , 19, 0, "(1[0-9A-C]|[0-9A-I])", RGXG_NOLOWERCASE },
    { 0, 31 , 19, 0, "(1[0-9A-Ca-c]|0[0-9A-Ia-i])", RGXG_LEADINGZERO },
    { 0, 31 , 19, 0, "(1[0-9A-C]|0[0-9A-I])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 20, 0, "(1[0-9AaBb]|[0-9A-Ja-j])", 0 },
    { 0, 31 , 20, 0, "(1[0-9AB]|[0-9A-J])", RGXG_NOLOWERCASE },
    { 0, 31 , 20, 0, "(1[0-9AaBb]|0[0-9A-Ja-j])", RGXG_LEADINGZERO },
    { 0, 31 , 20, 0, "(1[0-9AB]|0[0-9A-J])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 21, 0, "(1[0-9Aa]|[0-9A-Ka-k])", 0 },
    { 0, 31 , 21, 0, "(1[0-9A]|[0-9A-K])", RGXG_NOLOWERCASE },
    { 0, 31 , 21, 0, "(1[0-9Aa]|0[0-9A-Ka-k])", RGXG_LEADINGZERO },
    { 0, 31 , 21, 0, "(1[0-9A]|0[0-9A-K])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 22, 0, "(1[0-9]|[0-9A-La-l])", 0 },
    { 0, 31 , 22, 0, "(1[0-9]|[0-9A-L])", RGXG_NOLOWERCASE },
    { 0, 31 , 22, 0, "(1[0-9]|0[0-9A-La-l])", RGXG_LEADINGZERO },
    { 0, 31 , 22, 0, "(1[0-9]|0[0-9A-L])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 23, 0, "(1[0-8]|[0-9A-Ma-m])", 0 },
    { 0, 31 , 23, 0, "(1[0-8]|[0-9A-M])", RGXG_NOLOWERCASE },
    { 0, 31 , 23, 0, "(1[0-8]|0[0-9A-Ma-m])", RGXG_LEADINGZERO },
    { 0, 31 , 23, 0, "(1[0-8]|0[0-9A-M])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 24, 0, "(1[0-7]|[0-9A-Na-n])", 0 },
    { 0, 31 , 24, 0, "(1[0-7]|[0-9A-N])", RGXG_NOLOWERCASE },
    { 0, 31 , 24, 0, "(1[0-7]|0[0-9A-Na-n])", RGXG_LEADINGZERO },
    { 0, 31 , 24, 0, "(1[0-7]|0[0-9A-N])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 25, 0, "(1[0-6]|[0-9A-Oa-o])", 0 },
    { 0, 31 , 25, 0, "(1[0-6]|[0-9A-O])", RGXG_NOLOWERCASE },
    { 0, 31 , 25, 0, "(1[0-6]|0[0-9A-Oa-o])", RGXG_LEADINGZERO },
    { 0, 31 , 25, 0, "(1[0-6]|0[0-9A-O])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 26, 0, "(1[0-5]|[0-9A-Pa-p])", 0 },
    { 0, 31 , 26, 0, "(1[0-5]|[0-9A-P])", RGXG_NOLOWERCASE },
    { 0, 31 , 26, 0, "(1[0-5]|0[0-9A-Pa-p])", RGXG_LEADINGZERO },
    { 0, 31 , 26, 0, "(1[0-5]|0[0-9A-P])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 27, 0, "(1[0-4]|[0-9A-Qa-q])", 0 },
    { 0, 31 , 27, 0, "(1[0-4]|[0-9A-Q])", RGXG_NOLOWERCASE },
    { 0, 31 , 27, 0, "(1[0-4]|0[0-9A-Qa-q])", RGXG_LEADINGZERO },
    { 0, 31 , 27, 0, "(1[0-4]|0[0-9A-Q])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 28, 0, "(1[0-3]|[0-9A-Ra-r])", 0 },
    { 0, 31 , 28, 0, "(1[0-3]|[0-9A-R])", RGXG_NOLOWERCASE },
    { 0, 31 , 28, 0, "(1[0-3]|0[0-9A-Ra-r])", RGXG_LEADINGZERO },
    { 0, 31 , 28, 0, "(1[0-3]|0[0-9A-R])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 29, 0, "(1[0-2]|[0-9A-Sa-s])", 0 },
    { 0, 31 , 29, 0, "(1[0-2]|[0-9A-S])", RGXG_NOLOWERCASE },
    { 0, 31 , 29, 0, "(1[0-2]|0[0-9A-Sa-s])", RGXG_LEADINGZERO },
    { 0, 31 , 29, 0, "(1[0-2]|0[0-9A-S])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 30, 0, "(1[01]|[0-9A-Ta-t])", 0 },
    { 0, 31 , 30, 0, "(1[01]|[0-9A-T])", RGXG_NOLOWERCASE },
    { 0, 31 , 30, 0, "(1[01]|0[0-9A-Ta-t])", RGXG_LEADINGZERO },
    { 0, 31 , 30, 0, "(1[01]|0[0-9A-T])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 31, 0, "(10|[0-9A-Ua-u])", 0 },
    { 0, 31 , 31, 0, "(10|[0-9A-U])", RGXG_NOLOWERCASE },
    { 0, 31 , 31, 0, "(10|0[0-9A-Ua-u])", RGXG_LEADINGZERO },
    { 0, 31 , 31, 0, "(10|0[0-9A-U])", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },

    { 0, 31 , 32, 0, "[0-9A-Va-v]", 0 },
    { 0, 31 , 32, 0, "[0-9A-V]", RGXG_NOLOWERCASE },
    { 0, 31 , 32, 0, "[0-9A-Va-v]", RGXG_LEADINGZERO },
    { 0, 31 , 32, 0, "[0-9A-V]", RGXG_NOLOWERCASE|RGXG_LEADINGZERO },
};

static int num_range_tests = sizeof range_tests / sizeof(range_test_t);

START_TEST (test_range_regex) {
    char *regex = NULL;
    int n = rgxg_number_range(range_tests[_i].first, range_tests[_i].last, range_tests[_i].base, range_tests[_i].min_length, regex, range_tests[_i].options);
    if (n > 0) {
        regex = malloc(sizeof(char) * (n+1)); /* freed below */
        rgxg_number_range(range_tests[_i].first, range_tests[_i].last, range_tests[_i].base, range_tests[_i].min_length, regex, range_tests[_i].options);
        regex[n] = '\0';
        ck_assert(regex != NULL);
        ck_assert_msg(strcmp(range_tests[_i].expected_regex, regex) == 0, "rgxg_number_range: %lli %lli (base: %d, min_length: %d, options: %d): regex returned '%s' != '%s'", range_tests[_i].first, range_tests[_i].last, range_tests[_i].base, range_tests[_i].min_length, range_tests[_i].options, regex, range_tests[_i].expected_regex);
        free(regex);
    } else {
        ck_abort_msg("rgxg_number_range: unexpected return value %d", n);
    }
}
END_TEST

Suite *make_rgxg_number_suite(void) {

    Suite *s = suite_create ("rgxg/number");

    TCase *tc_rgxg_number_range = tcase_create ("rgxg_number_range");

    tcase_add_loop_test (tc_rgxg_number_range, test_range_regex, 0, num_range_tests);

    suite_add_tcase (s, tc_rgxg_number_range);

    return s;
}
