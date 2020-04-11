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

#include "alternation.h"
#include "common_macros.h"

/* needed for exit_status, options */
#include "rgxg.h"

/* needed for rgxg_utils_alternation */
#include "rgxg/utils.h"

/* needed for fprintf */
#include <stdio.h>

/* needed for getopt, CONFORMING TO POSIX.1-2001 */
#include <unistd.h>

char** list;
int length;

void alternation_print_help () {
    fprintf(stdout,
        "Usage: rgxg alternation [options] [PATTERN...]\n\n"
        "    -N         omit the outer parentheses, if any\n"
        "    -h         display this help message\n"
    );
}

void alternation_set_defaults () {
    list = NULL;
    length = 0;
    options = 0;
}

int alternation_argv_parse (int argc, char **argv) {
    int cont = 1;
    int c;
    while (cont && (c = getopt (argc, argv, "hN")) != -1)
        switch (c) {
            EASY_HELP_OPTION(alternation)
            EASY_NOOUTERPARENS_OPTION
            EASY_DEFAULT(alternation)
        }
    if (cont) {
        if (argc > optind) {
            length = argc-optind;
            list = &argv[optind];
        }
    }
    return cont;
}

int alternation_generate_regex (char * regex) {
    return rgxg_utils_alternation((const char**) list , length, regex, options);
}
