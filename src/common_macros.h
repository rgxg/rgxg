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

#ifndef _COMMON_MACROS_H_INCLUDED
#define _COMMON_MACROS_H_INCLUDED

/* needed for isprint */
#include <ctype.h>

#define EASY_HELP_OPTION(command) \
case 'h': \
    command##_print_help(); \
    cont = 0; \
    break;

#define EASY_OPTION(char, option) \
case char: \
    options |= option; \
    break;

#define EASY_NOOUTERPARENS_OPTION EASY_OPTION('N', RGXG_NOOUTERPARENS)

#define EASY_DEFAULT(command) \
default: /* case '?' || ':' */ \
    if (c == ':' ) { \
        fprintf (stderr, "rgxg "#command": option -%c requires an argument.\n", optopt); \
    } else if (isprint (optopt)) { \
        fprintf (stderr, "rgxg "#command": unknown option `-%c'.\n", optopt); \
    } else { \
        fprintf (stderr, "rgxg "#command": unknown option character `\\x%x'.\n", optopt); \
    } \
    cont = 0; \
    command##_print_help(); \
    exit_status = 1;

#endif /* _COMMON_MACROS_H_INCLUDED */
