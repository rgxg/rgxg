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

#ifndef _RGXG_NET_H_INCLUDED
#define _RGXG_NET_H_INCLUDED

/* needed for rgxg_options_t */
#include "types.h"

/* needed for uint8_t, uint16_t */
#include <inttypes.h>

typedef struct ipv4_s {
    uint8_t octet[4];
} ipv4_t;

typedef struct ipv6_s {
    uint16_t hextet[8];
} ipv6_t;

extern int rgxg_net_cidr_ipv4 (const ipv4_t *address, int prefix, char *regex,
        rgxg_options_t options);

extern int rgxg_net_cidr_ipv6 (const ipv6_t *address, int prefix, char *regex,
        rgxg_options_t options);

extern int rgxg_net_cidr_string (const char *cidr, char **endptr, char *regex,
        rgxg_options_t options);

#endif /* _RGXG_NET_H_INCLUDED */
