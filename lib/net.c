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

#include "rgxg/net.h"

/* needed for rgxg_options_t */
#include "rgxg/types.h"

/* needed for rgxg_number_range */
#include "rgxg/number.h"

/* needed for EASY_CHAR */
#include "common_macros.h"

/* needed for NULL, strtol */
#include <stdlib.h>

/* needed for uint8_t, uint16_t */
#include <inttypes.h>

int rgxg_net_cidr_ipv4 (const ipv4_t *address, int prefix, char *regex,
        rgxg_options_t options) {
    if (prefix < 0 || prefix > 32) {
        return RGXG_ERROR_PREFIX;
    } else {
        int i, n, x, bits;
        n = 0;
        x = 1;
        ipv4_t first, last;
        uint8_t mask;
        for (i = 0; i < 4; ++i) {
            bits = i*8;
            mask = ~( (prefix ? (1 <<
            (prefix <= bits ? 8 : (prefix > (bits+8) ? 0 : (bits+8)-prefix))
                            ) : 0) - 1);
            first.octet[i] = (address->octet[i])&mask;
            last.octet[i] = (address->octet[i])|~mask;
        }
        for (i = 0; i < 4; ++i) {
            while (i < 2 && first.octet[i] == first.octet[i+1]
                    && last.octet[i] == last.octet[i+1]
                    && (first.octet[i] != last.octet[i] || first.octet[i] > 9) /* 192\.192\. => (192\.){2} but 0\.0\. => 0\.0\. */
                  ) {
                i++;
                x++;
            }
            if (x > 1) {
                EASY_CHAR('(');
            }
            n += rgxg_number_range(first.octet[i], last.octet[i],
                    10, 0, (regex ? regex+n : NULL), 0);
            if (i < 3) {
                EASY_CHAR('\\');
                EASY_CHAR('.');
            }
            if (x > 1) {
                EASY_CHAR(')');
                EASY_CHAR('{');
                n += rgxg_number(x, 10, (regex ? regex+n : NULL), 0);
                EASY_CHAR('}');
                x = 1;
            }
        }
        return n;
    }
}

int rgxg_net_cidr_ipv6 (const ipv6_t *address, int prefix, char *regex,
        rgxg_options_t options) {
    if (prefix < 0 || prefix > 128) {
        return RGXG_ERROR_PREFIX;
    } else {
        int i, n, max, bits, zeros, open_parentheses;
        n = 0;
        max = 1;
        ipv6_t first, last;
        uint16_t mask;
        for (i = 0; i < 8; ++i) {
            bits = i*16;
            mask = ~( (prefix ? (1 <<
            (prefix <= bits ? 16 : (prefix > (bits+16) ? 0 : (bits+16)-prefix))
                            ) : 0) - 1);
            first.hextet[i] = (address->hextet[i])&mask;
            last.hextet[i] = (address->hextet[i])|~mask;
        }

        open_parentheses = 0;
        for (int j = 0; j < 8; ++j) {
            i = j;
            if (!(RGXG_NOZEROCOMPRESSION&options) && first.hextet[j] == 0) {
                if (j > 0 || !(RGXG_NOOUTERPARENS&options)) {
                    open_parentheses++;
                    EASY_CHAR('(');
                }
                if (j == 0) { EASY_CHAR(':'); } /* :: at the beginning */
                EASY_CHAR(':');
                i++; /* omit current zero hextet */
                zeros = 1;
            } else {
                zeros = 0;
            }
            for (int k = !zeros ; k < 2; ++k) {
                for (; i < ((k && !zeros) ? j+1 : 8) ; ++i) {
                    if (zeros || (RGXG_NOZEROCOMPRESSION&options)) {
                        while (i < 6 && first.hextet[i] == first.hextet[i+1]
                                && last.hextet[i] == last.hextet[i+1]) {
                            if (RGXG_NOZEROCOMPRESSION&options) { j++; }
                            i++;
                            max++;
                        }
                    }
                    if (zeros && first.hextet[i] != 0) {
                        zeros = 0;
                    }
                    if (zeros || max > 1) {
                        EASY_CHAR('(');
                    }
                    n+= rgxg_number_range(first.hextet[i], last.hextet[i],
                            16, 4, (regex ? regex+n : NULL),
                            (options&(RGXG_NOUPPERCASE|RGXG_NOLOWERCASE))|RGXG_VARLEADINGZERO);
                    if (i < 7) {
                        EASY_CHAR(':');
                    }
                    if (zeros || max > 1) {
                        EASY_CHAR(')');
                        if (zeros && max == 1) {
                            EASY_CHAR('?');
                        } else {
                            EASY_CHAR('{');
                            if (zeros) {
                                EASY_CHAR('0');
                                EASY_CHAR(',');
                            }
                            n += rgxg_number(max, 10, (regex ? regex+n : NULL), 0);
                            EASY_CHAR('}');
                            max = 1;
                        }
                    }
                }
                if (!k) {
                    EASY_CHAR('|');
                    i = j;
                    zeros = 0;
                }
            }
        }
        for (i = 0; i < open_parentheses; ++i) {
            EASY_CHAR(')');
        }

        return n;
    }
}

#define EASY_RETURN(return_value, ptr) \
    if (endptr != NULL) { *endptr = (char *) ptr; } \
    return return_value;

int rgxg_net_cidr_string (const char *cidr, char** endptr, char *regex,
        rgxg_options_t options) {
    int n, i;
    char *ptr;
    long value;
    long prefix;

    i = 0;

    strtol(cidr, &ptr, 16); /* pre-check to determine address type */
    if (*ptr == ':') { /* IPV6 address */
        ipv6_t address;
        int start_of_zero_section = -1;
        do {
            if (cidr[1] == 'x') {
                EASY_RETURN(RGXG_ERROR_SYNTAX, cidr+1);
            }
            value = strtol(cidr, &ptr, 16);
            if (value > 65535 || value < 0) {
                EASY_RETURN(value < 0 ? RGXG_ERROR_NEGARG : RGXG_ERROR_ARG2BIG, cidr)
            } else if (value == 0 && cidr == ptr) {
                if (start_of_zero_section < 0 && *ptr == ':') { /* no double zero sections */
                    start_of_zero_section = i;
                    if (i == 0 || i == 7) {
                        ptr++; /* skip 2nd ':' at the beginning/end */
                        if (*ptr != (i == 0 ? ':' : '/')) { /* force 2nd : at the beginning/end */
                            EASY_RETURN(RGXG_ERROR_SYNTAX, ptr)
                        }
                    }
                } else if (*ptr != '/') {
                    EASY_RETURN(RGXG_ERROR_SYNTAX, ptr)
                }
            } else if (*(ptr+1) != '/' && (start_of_zero_section < 0 || i < 6 )) {
                address.hextet[i++] = value;
            } else {
                EASY_RETURN(RGXG_ERROR_SYNTAX, ptr)
            }

            cidr = ptr+1;
        } while (*ptr == ':' && i < 8);
        if (start_of_zero_section >= 0) {
            /* move hextets behind the zero section at the end and fill the gap with zeros */
            int length = 8 - i;
            for (i = 7; i >= start_of_zero_section; --i) {
                address.hextet[i] = ((i-length) < start_of_zero_section) ? 0 : address.hextet[i-length];
            }
        }
        if (*ptr == '/' && (start_of_zero_section >= 0 || i == 8)) {
            prefix = strtol(cidr, &ptr, 10);
            if (prefix > 128 || prefix < 0) {
                EASY_RETURN(RGXG_ERROR_PREFIX, cidr)
            } else if (prefix == 0 && cidr == ptr) {
                EASY_RETURN(RGXG_ERROR_SYNTAX, cidr)
            }
        } else {
            EASY_RETURN(RGXG_ERROR_SYNTAX, ptr)
        }
        n = rgxg_net_cidr_ipv6 (&address, prefix, regex, options);
    } else { /* IPV4 address */
        ipv4_t address;
        do {
            value = strtol(cidr, &ptr, 10);
            if (value > 255 || value < 0) {
                  EASY_RETURN(value < 0 ? RGXG_ERROR_NEGARG : RGXG_ERROR_ARG2BIG, cidr)
              } else if (value == 0 && cidr == ptr) {
                  EASY_RETURN(RGXG_ERROR_SYNTAX, ptr)
              } else {
                address.octet[i++] = value;
            }
            cidr = ptr+1;
        } while (*ptr == '.' && i < 4);
        if (*ptr == '/' && i == 4) {
            prefix = strtol(cidr, &ptr, 10);
            if (prefix > 32 || prefix < 0) {
                EASY_RETURN(RGXG_ERROR_PREFIX, cidr)
            } else if (prefix == 0 && cidr == ptr) {
                EASY_RETURN(RGXG_ERROR_SYNTAX, cidr)
            }
        } else {
            EASY_RETURN(RGXG_ERROR_SYNTAX, ptr)
        }
        n = rgxg_net_cidr_ipv4 (&address, prefix, regex, 0);
    }

    EASY_RETURN(n, ptr)
}
