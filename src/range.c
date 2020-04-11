/* rgxg - ReGular eXpression Generator
 *
 * Copyright (c) 2013,2020 Hannes von Haugwitz
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

#include "range.h"

/* needed for exit_status */
#include "rgxg.h"

/* needed for rgxg_number* */
#include "rgxg/number.h"

#include "common_macros.h"

/* needed for strtol, strtoll */
#include <stdlib.h>

/* used with strtol, strtoll */
#include <errno.h>

/* needed for LLONG_MAX */
#include <limits.h>

/* needed for fprintf */
#include <stdio.h>

/* needed for getopt, CONFORMING TO POSIX.1-2001 */
#include <unistd.h>

/* needed for isprint, isdigit */
#include <ctype.h>

int input_base;

long base, min_length;

long long first, last;

rgxg_options_t options;

static int is_string_of_digits(const char *string) {
    int i = 0;
    while (string[i]) {
        if (!(isdigit(string[i++]))) {
            return 0;
        }
    }
    return 1;
}

void range_print_help () {
    fprintf(stdout,
        "Usage: rgxg range [options] FIRST [LAST]\n\n"
        "    -b BASE    match the numbers with base BASE (default is base 10)\n"
        "    -N         omit the outer parentheses, if any\n"
        "    -l         only match lower case letters\n"
        "    -U         only match upper case letters\n"
        "    -h         display this help message\n"
        "    -z         only match numbers with leading zeros\n"
        "    -Z         match numbers with a variable number of leading zeros\n"
        "    -m LENGTH  with -z or -Z, the minimum LENGTH of matched numbers\n"
        "\n"
    );
}

void range_set_defaults () {
    base = 10;
    input_base = 10;
    options = 0;
    min_length = 0;
}

int range_argv_parse (int argc, char **argv) {
    int cont = 1;
    int c;
    char *ptr;
    while (cont && (c = getopt (argc, argv, ":b:hm:zNUlZ")) != -1)
        switch (c) {
            EASY_HELP_OPTION(range)
            EASY_NOOUTERPARENS_OPTION
            case 'b':
                base = strtol(optarg, NULL, 10);
                if ((errno == ERANGE && (base == LONG_MIN || base == LONG_MAX))
                   || (base < 2 || base > 32)) {
                    fprintf (stderr, "rgxg range: invalid base: %s (The base must be between 2 and 32 inclusive).\n", optarg);
                    cont = 0;
                    exit_status = 1;
                }
                break;
            EASY_MUTEX_OPTION(range, 'z', 'Z', RGXG_LEADINGZERO, RGXG_VARLEADINGZERO)
            EASY_MUTEX_OPTION(range, 'Z', 'z', RGXG_VARLEADINGZERO, RGXG_LEADINGZERO)
            EASY_MUTEX_OPTION(range, 'l', 'U', RGXG_NOUPPERCASE, RGXG_NOLOWERCASE)
            EASY_MUTEX_OPTION(range, 'U', 'l', RGXG_NOLOWERCASE, RGXG_NOUPPERCASE)
            case 'm':
                min_length = strtol(optarg, &ptr, 10);
                if (min_length < 0 || min_length > INT_MAX || *ptr != '\0') {
                    fprintf (stderr, "rgxg range: invalid minimum length: %s (The minimum length must be a positive number lesser than or equal to %d).\n", optarg, INT_MAX);
                    cont = 0;
                    exit_status = 1;
                }
                break;
            EASY_DEFAULT(range)
        }
    if (cont) {
        if (argc > optind) {
            if (is_string_of_digits(argv[optind])) {
                first = strtoll(argv[optind], NULL, input_base);
                if (first == LLONG_MAX && errno == ERANGE) {
                    fprintf (stderr, "rgxg range: first number '%s' must be lesser than or equal to %lld.\n", argv[optind], LLONG_MAX);
                    cont = 0;
                    exit_status = 1;
                } else {
                    if (argc > ++optind) {
                        if (is_string_of_digits(argv[optind])) {
                            last = strtoll(argv[optind], NULL, input_base);
                            if (last == LLONG_MAX && errno == ERANGE) {
                                fprintf (stderr, "rgxg range: last number '%s' must be lesser than or equal to %lld.\n", argv[optind], LLONG_MAX);
                                cont = 0;
                                exit_status = 1;
                            }
                        } else {
                            fprintf (stderr, "rgxg range: last number '%s' contains non-digit characters.\n", argv[optind]);
                            cont = 0;
                            exit_status = 1;
                        }
                    } else { /* infinite range */
                        last = -1;
                    }
                }
            } else {
                fprintf (stderr, "rgxg range: first number '%s' contains non-digit characters.\n", argv[optind]);
                cont = 0;
                exit_status = 1;
            }


        } else {
            fprintf (stderr, "rgxg range: missing argument: first number of range is missing.\n");
            cont = 0;
            exit_status = 1;
        }
    }
    return cont;
}

int range_generate_regex (char * regex) {
    int n;
    if (last < 0) {
        n = rgxg_number_greaterequal(first, base, min_length, regex, options);
    } else {
        n = rgxg_number_range(first, last, base, min_length, regex, options);
    }
    switch (n) {
        /* RGXG_ERROR_BASE, RGXG_ERROR_NEGARG handled in range_argv_parse */
        case RGXG_ERROR_RANGE:
            fprintf (stderr, "rgxg range: range from %lld to %lld is invalid.\n", first, last);
            exit_status = 1;
            break;
        case RGXG_ERROR_ARG2BIG:
            fprintf (stderr, "rgxg range: the number '%lld' is too large.\n", first);
            exit_status = 1;
            break;
    }
    return n;
}
