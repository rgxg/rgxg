/* rgxg - ReGular eXpression Generator
 *
 * Copyright (c) 2013 Hannes von Haugwitz
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

#include "escape.h"
#include "common_macros.h"

/* needed for exit_status */
#include "rgxg.h"

/* needed for rgxg_utils_escape */
#include "rgxg/utils.h"

/* needed for fprintf */
#include <stdio.h>

/* needed for strlen */
#include <string.h>

/* needed for getopt, CONFORMING TO POSIX.1-2001 */
#include <unistd.h>

char* string;

void escape_print_help () {
    fprintf(stdout,
        "Usage: rgxg escape [options] STRING\n\n"
        "    -h         display this help message\n"
    );
}

void escape_set_defaults () {
    /* no defaults */
}

int escape_argv_parse (int argc, char **argv) {
    int cont = 1;
    int c;
    while (cont && (c = getopt (argc, argv, "h")) != -1)
        switch (c) {
            EASY_HELP_OPTION(escape)
            EASY_DEFAULT(escape)
        }
    if (cont) {
        optind++; /* skip command string */
        if (argc > optind) {
            string = argv[optind];
        } else {
            fprintf (stderr, "rgxg escape: missing argument: string to be escaped is missing.\n");
            cont = 0;
            exit_status = 1;
        }
    }
    return cont;
}

int escape_generate_regex (char * regex) {
    return rgxg_utils_escape_string(string, strlen(string), regex);
}
