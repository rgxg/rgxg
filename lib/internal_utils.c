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

#include "internal_utils.h"

/* needed for RGXG_ERROR_NEGARG */
#include "rgxg/types.h"

int rgxg_plain_number_base10 (long long number, char *regex) {
    int n = 0;
    long long m = number;

    if (number < 0) {
        n = RGXG_ERROR_NEGARG;
    } else {
        do {
            ++n;
            m /= 10;
        } while (m);
        if (regex) {
            int pos = n;
            do {
                regex[--pos] = '0' + (number%10);
                number /= 10;
            } while (number);
        }
    }
    return n;
}
