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

#include "rgxg/utils.h"

/* needed for EASY_CHAR */
#include "common_macros.h"

/* needed for strlen */
#include <string.h>

/* needed for NULL */
#include <stdlib.h>

static int rgxg_is_escape_char(char c) {
    switch(c) {
        case '\\':
        case '^':
        case '$':
        case '*':
        case '+':
        case '?':
        case '.':
        case '(':
        case ')':
        case '|':
        case '{':
        case '}':
        case '[':
        case ']':
            return 1;
        default:
            return 0;
    }
}

int rgxg_utils_escape_string(const char* src, size_t length, char *regex) {
    size_t i;
    int n = 0;

    for (i = 0; i < length && src[i] != '\0'; ++i) {
        if (rgxg_is_escape_char(src[i])) {
            EASY_CHAR('\\')
        }
        EASY_CHAR(src[i])
    }
    if (i < length) {
        EASY_CHAR('\0')
    }

    return n;
}

int rgxg_utils_alternation(const char** list, size_t size, char *regex,
        rgxg_options_t options) {
    int n = 0;
    size_t i;

    if (size) {
        if (!(options&RGXG_NOOUTERPARENS) && size > 1) {
            EASY_CHAR('(')
        }
        for (i = 0; i < size; ++i) {
            if (i != 0) {
                EASY_CHAR('|')
            }
            n += rgxg_utils_escape_string(list[i], strlen(list[i]), (regex ? regex+n : NULL));
        }
        if (!(options&RGXG_NOOUTERPARENS) && size > 1) {
            EASY_CHAR(')')
        }
    }

    if (!(RGXG_NONULLBYTE&options) && regex) { regex[n] = '\0'; } \

    return n;
}
