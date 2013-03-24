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

#ifndef _ESCAPE_H_INCLUDED
#define _ESCAPE_H_INCLUDED

extern void escape_set_defaults ();

extern int escape_argv_parse (int, char **);

extern int escape_generate_regex (char *);

extern void escape_print_help ();

#endif /* _ESCAPE_H_INCLUDED */
