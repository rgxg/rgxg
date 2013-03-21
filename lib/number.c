/* rgxg - ReGular eXpression Generator
 *
 * Copyright (c) 2010-2013 Hannes von Haugwitz
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

/* needed for rgxg_number_* */
#include "rgxg/number.h"

/* needed for LLONG_MAX */
#include <limits.h>

/* needed for NULL */
#include <stdlib.h>

/* pre-compile limits */
long long limits[] = {
    0L, LLONG_MAX, LLONG_MAX/2, LLONG_MAX/3, LLONG_MAX/4, LLONG_MAX/5,
    LLONG_MAX/6, LLONG_MAX/7, LLONG_MAX/8, LLONG_MAX/9, LLONG_MAX/10,
    LLONG_MAX/11, LLONG_MAX/12, LLONG_MAX/13, LLONG_MAX/14, LLONG_MAX/15,
    LLONG_MAX/16, LLONG_MAX/17, LLONG_MAX/18, LLONG_MAX/19, LLONG_MAX/20,
    LLONG_MAX/21, LLONG_MAX/22, LLONG_MAX/23, LLONG_MAX/24, LLONG_MAX/25,
    LLONG_MAX/26, LLONG_MAX/27, LLONG_MAX/28, LLONG_MAX/29, LLONG_MAX/30,
    LLONG_MAX/31, LLONG_MAX/32
};

#define EASY_CHAR(char) \
    if (regex) { regex[n] = char; } \
    n++;

static int rgxg_number_of_digits_long_long(long long number, int base) {
    long long power = base;
    long long limit = limits[base];
    int n = 1;

    while(number >= power) {
        n++;
        if (power > limit) break;
        power *= base;
    }

    return n;
}

static long long rgxg_power(int base, int exp) {
    long long r = 1;
    while (exp--) { r *=base; }
    return r;
}

static char rgxg_base32_char(long long number, int upper) {
    /* 'A'-10 = '7', 'a'-10='W' */
    return number+(number < 10 ? '0' : (upper ? '7' : 'W'));
}

static int rgxg_base32_range(long long first, long long
        last, char* regex, rgxg_options_t options) {
    /* size 4 is enough as long base <=36 */
    long long stk[4];
    int top, brackets;
    int n = 0;

    if (first < 0 || last < first || last > 32) {
        return -1;
    } else {
        top = -1;
        brackets=first-last || (last > 9 && !(RGXG_NOUPPERCASE&options || RGXG_NOLOWERCASE&options));
        if (brackets) { EASY_CHAR('['); }
        stk[++top] = first;
        stk[++top] = last;
        do {
            last = stk[top--];
            first = stk[top--];
            switch (last-first) {
                case 0:
                    if (first <= 9 || !(RGXG_NOUPPERCASE&options)) {
                        EASY_CHAR(rgxg_base32_char(first, 1));
                    }
                    if (first > 9 && !(RGXG_NOLOWERCASE&options)) {
                        EASY_CHAR(rgxg_base32_char(first, 0));
                    }
                    break;
                case 1:
                    if (first <= 9 || !(RGXG_NOUPPERCASE&options)) {
                        EASY_CHAR(rgxg_base32_char(first, 1));
                    }
                    if (first > 9 && !(RGXG_NOLOWERCASE&options)) {
                        EASY_CHAR(rgxg_base32_char(first, 0));
                    }

                    if (last <= 9 || !(RGXG_NOUPPERCASE&options)) {
                        EASY_CHAR(rgxg_base32_char(last, 1));
                    }
                    if (last > 9 && !(RGXG_NOLOWERCASE&options)) {
                        EASY_CHAR(rgxg_base32_char(last, 0));
                    }
                    break;
                default:
                    if (first <= 9 && last >= 10) {
                        stk[++top] = 10;
                        stk[++top] = last;
                        stk[++top] = first;
                        stk[++top] = 9;
                    } else {
                        if (first <= 9 || !(RGXG_NOUPPERCASE&options)) {
                            EASY_CHAR(rgxg_base32_char(first, 1));
                            EASY_CHAR('-');
                            EASY_CHAR(rgxg_base32_char(last, 1));
                        }
                        if (first > 9 && !(RGXG_NOLOWERCASE&options)) {
                            EASY_CHAR(rgxg_base32_char(first, 0));
                            EASY_CHAR('-');
                            EASY_CHAR(rgxg_base32_char(last, 0));
                        }
                    }
                    break;
            }
        } while (top > 0);
        if (brackets) { EASY_CHAR(']'); }
        return n;
    }
}

#define EASY_IF_ERROR_ELSE(condition, error) \
    if (condition) { \
        n = error; \
    } else

int rgxg_number(long long number, int base, char* regex, rgxg_options_t
        options) {
    int n;
    long long m;

    EASY_IF_ERROR_ELSE(base < 2 || base > 32, RGXG_ERROR_BASE)
    EASY_IF_ERROR_ELSE(number < 0, RGXG_ERROR_NEGARG) {
        n = 0;
        m = number;
        do {
            n += (m%base <= 9 || RGXG_NOUPPERCASE&options || RGXG_NOLOWERCASE&options) ? 1 : 4;
            m /= base;
        } while (m);

        if (regex) {
            int pos = n;
            do {
                m = number%base;
                if (m <= 9 || RGXG_NOUPPERCASE&options || RGXG_NOLOWERCASE&options) {
                    regex[--pos] = rgxg_base32_char(m, RGXG_NOLOWERCASE&options);
                } else {
                    regex[--pos] = ']';
                    regex[--pos] = rgxg_base32_char(m, 0);
                    regex[--pos] = rgxg_base32_char(m, 1);
                    regex[--pos] = '[';
                }
                number /= base;
            } while (number);
        }
    }
    return n;
}

#define EASY_NUMBER_ASSIGNMENT(number) \
    n += rgxg_number(number, base, (regex ? regex+n : NULL), options);

#define EASY_BASE32_RANGE_ASSIGNMENT(first, last) \
    n += rgxg_base32_range(first, last, (regex ? regex+n : NULL), options);

int rgxg_number_range(long long first, long long last, int base, char* regex,
        rgxg_options_t options) {
    long long prefix, prefix_range_first, prefix_range_last, current_last;
    int n, min, max, number_of_leading_zeros, parenthesis, max_num_of_digits;

    EASY_IF_ERROR_ELSE(base < 2 || base > 32, RGXG_ERROR_BASE)
    EASY_IF_ERROR_ELSE(first < 0 || last < 0, RGXG_ERROR_NEGARG)
    EASY_IF_ERROR_ELSE(first > last , RGXG_ERROR_RANGE) {
        n = 0;
        parenthesis = 0;
        max_num_of_digits = rgxg_number_of_digits_long_long(last, base);
        current_last = last;

        do {
            min = max = 0;
            prefix = current_last/base;
            prefix_range_first = first/base != current_last/base ? 0 : first%base;
            prefix_range_last = current_last%base;

            while (prefix_range_last == base-1 && (prefix*rgxg_power(base,max+1)-1 > first) ) {
                ++max;
                current_last /= base;
                prefix = current_last/base;
                prefix_range_first =
                    prefix*rgxg_power(base,max+1)+!prefix*rgxg_power(base,max) > first
                    ? (((RGXG_LEADINGZERO&options) && first == 0) ? 0 : !prefix)
                    : first/rgxg_power(base,max)%base+(prefix*rgxg_power(base,max+1)+(first/rgxg_power(base,max)%base)*rgxg_power(base,max) != first);
                prefix_range_last = current_last%base;
            }
            current_last = prefix*rgxg_power(base,max+1)+ prefix_range_first*rgxg_power(base,max);
            min = max;
            if (max && prefix_range_first == 1 && prefix_range_last == base-1 && !(RGXG_LEADINGZERO&options)) {
                while (current_last/base && current_last/base >= first) {
                    min--;
                    current_last /= base;
                }
            }
            current_last--;

            if (!parenthesis && !(RGXG_NOOUTERPARENS&options) && first <= current_last ) {
                if (!(max == 1 && first ==0 && ((current_last == base -1
                    && min == 1) || (current_last == 0 && min == 0)))
                    ) { /* no parenthesis around 1?[0-9] and [1-9]?[0-9] */
                    EASY_CHAR('(');
                    parenthesis = 1;
                }
            }

            if (RGXG_LEADINGZERO&options) {
                number_of_leading_zeros = max_num_of_digits-(prefix ? rgxg_number_of_digits_long_long(prefix, base): 0)-max-1;
                if (number_of_leading_zeros < 5) { /* 0000000000 -> 0{10} but 0000 -> 0000 */
                    while (number_of_leading_zeros-- > 0) {
                        EASY_CHAR('0');
                    }
                } else {
                    EASY_CHAR('0');
                    EASY_CHAR('{');
                    n += rgxg_number(number_of_leading_zeros, 10, (regex ? regex+n : NULL), 0);
                    EASY_CHAR('}');
                }
            }

            if (prefix) {
                EASY_NUMBER_ASSIGNMENT(prefix)
            }
            if (prefix_range_first == 0 && prefix_range_last == base-1) { /* 55[0-9][0-9] => 55[0-9]{2} */
                max++; min++;
            } else {
                EASY_BASE32_RANGE_ASSIGNMENT(prefix_range_first, prefix_range_last)
            }
            if (max) { /* handle multiple [0-9] */
                if (max == 1 && first ==0 && ((current_last == base -1
                    && min == 1) || (current_last == 0 && min == 0))
                    ) { /* 1[0-9]|[0-9] => 1?[0-9] and [1-9][0-9]?|0 -> [1-9]?[0-9] */
                    EASY_CHAR('?');
                    EASY_BASE32_RANGE_ASSIGNMENT(0, base-1);
                    current_last = -1;
                } else {
                    EASY_BASE32_RANGE_ASSIGNMENT(0, base-1);
                    if (max == 1 && min == 0) { /* 5[0-9]{0,1} => 5[0-9]? */
                        EASY_CHAR('?');
                    } else if (max > 1) {
                        EASY_CHAR('{');
                        if (min != max) {
                            n += rgxg_number(min, 10, (regex ? regex+n : NULL), 0);
                            EASY_CHAR(',');
                        }
                        n += rgxg_number(max, 10, (regex ? regex+n : NULL), 0);
                        EASY_CHAR('}');
                    }
                }
            }
            if (first <= current_last) {
                EASY_CHAR('|');
            }
        } while (first <= current_last);
        if (parenthesis) {
            EASY_CHAR(')');
        }
    }
    return n;
}

int rgxg_number_greaterequal(long long number, int base, char* regex, rgxg_options_t options) {
    int n, count, is_power_of_base;
    long long boundary;
    rgxg_options_t range_options;

    EASY_IF_ERROR_ELSE(base < 2 || base > 32, RGXG_ERROR_BASE)
    EASY_IF_ERROR_ELSE(number < 0, RGXG_ERROR_NEGARG)
    EASY_IF_ERROR_ELSE(number > rgxg_power(base, rgxg_number_of_digits_long_long(limits[base], base)), RGXG_ERROR_ARG2BIG) {
        n = 0;
        count = rgxg_number_of_digits_long_long(number ,base);

        boundary = rgxg_power(base,count-1);
        is_power_of_base = (number != boundary); /* ([1-9][0-9]{3,}|[1-9][0-9]{2}) => [1-9][0-9]{2,} */

        if (!(RGXG_NOOUTERPARENS&options) && number != 1 && is_power_of_base) {
            EASY_CHAR('(');
        }

        EASY_BASE32_RANGE_ASSIGNMENT(1, base-1)
        EASY_BASE32_RANGE_ASSIGNMENT(0, base-1)
        count -= (!is_power_of_base);
        if (count == 0) { /* [1-9][0-9]{0,} => [1-9][0-9]* */
            EASY_CHAR('*');
        } else if (count == 1) { /* [1-9][0-9]{1,} => [1-9][0-9]+ */
            EASY_CHAR('+');
        } else {
            EASY_CHAR('{');
            n += rgxg_number(count, 10, (regex ? regex+n : NULL), 0);
            EASY_CHAR(',');
            EASY_CHAR('}');
        }
        if (is_power_of_base) {
            boundary *= base;
            EASY_CHAR('|');
            if (RGXG_LEADINGZERO&options) {
                EASY_CHAR('0');
                range_options = options&(~RGXG_NOOUTERPARENS);
            } else {
                range_options = RGXG_NOOUTERPARENS|options;
            }
            n += rgxg_number_range(number, boundary-1, base, (regex ? regex+n : NULL), range_options);
            if (!(RGXG_NOOUTERPARENS&options)) {
                EASY_CHAR(')');
            }
        }
    }

    return n;
}
