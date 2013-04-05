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

#include "cidr.h"

/* needed for exit_status */
#include "rgxg.h"

/* needed for rgxg_net_cidr_string */
#include "rgxg/net.h"

#include "common_macros.h"

/* needed for fprintf */
#include <stdio.h>

/* needed for getopt, CONFORMING TO POSIX.1-2001 */
#include <unistd.h>

/* needed for isprint, isdigit */
#include <ctype.h>

char* cidr;

rgxg_options_t options;

void cidr_print_help () {
    fprintf(stdout,
        "Usage: rgxg cidr [options] CIDR\n\n"
        "    -h         display this help message\n"
        "    -u         do not match IPv6 addresses with zero compression\n"
        "    -s         do not match IPv6 addresses with mixed notation\n"
        "    -l         only match IPv6 addresses with lower case letters\n"
        "    -U         only match IPv6 addresses with upper case letters\n"
        "    -N         omit the outer parentheses, if any\n"
        "\n"
    );
}

void cidr_set_defaults () {
    cidr = NULL;
    options = 0;
}

int cidr_argv_parse (int argc, char **argv) {
    int cont = 1;
    int c;
    while (cont && (c = getopt (argc, argv, "hlsuUN")) != -1)
        switch (c) {
            EASY_HELP_OPTION(cidr)
            EASY_OPTION('u', RGXG_NOZEROCOMPRESSION)
            EASY_OPTION('s', RGXG_NOMIXEDNOTATION)
            EASY_NOOUTERPARENS_OPTION
            EASY_MUTEX_OPTION(cidr, 'l', 'U', RGXG_NOUPPERCASE, RGXG_NOLOWERCASE)
            EASY_MUTEX_OPTION(cidr, 'U', 'l', RGXG_NOLOWERCASE, RGXG_NOUPPERCASE)
            EASY_DEFAULT(cidr)
        }
    if (cont) {
        if (argc > optind) {
            cidr = argv[optind];
        } else {
            EASY_ERROR(cidr, "%s", "missing CIDR block");
            cont = 0;
        }
    }
    return cont;
}

int cidr_generate_regex (char * regex) {
    int n, position;
    char *ptr;
    n = rgxg_net_cidr_string(cidr, &ptr, regex, options);
    if (n >= 0 && *ptr != '\0') {
        n = RGXG_ERROR_SYNTAX;
    }
    position = (ptr-cidr)+1;
    switch (n) {
        case RGXG_ERROR_ARG2BIG:
            EASY_ERROR(cidr, syntax error: octet/hextet too big: %s, cidr)
            fprintf(stderr, "%*c\n", 47+position, '^');
            break;
        case RGXG_ERROR_NEGARG:
            EASY_ERROR(cidr, syntax error: negative octet/hextet: %s, cidr)
            fprintf(stderr, "%*c\n", 49+position, '^');
            break;
        case RGXG_ERROR_SYNTAX:
            EASY_ERROR(cidr, syntax error in CIDR block: %s, cidr)
            fprintf(stderr, "%*c\n", 39+position, '^');
            break;
        case RGXG_ERROR_PREFIX:
            EASY_ERROR(cidr, invalid network identifier in CIDR block: %s, cidr)
            fprintf(stderr, "%*c\n", 53+position, '^');
            break;
    }
    return n;
}
