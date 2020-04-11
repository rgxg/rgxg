/* rgxg - ReGular eXpression Generator
 *
 * Copyright (c) 2010-2013,2020 Hannes von Haugwitz
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

#include "config.h"

/* needed for rgxg_options_t */
#include "rgxg/types.h"

/* needed for rgxg commands */
#include "alternation.h"
#include "escape.h"
#include "range.h"
#include "cidr.h"

/* needed for malloc, free */
#include <stdlib.h>

/* needed for fprintf */
#include <stdio.h>

/* needed for strcmp */
#include <string.h>

int exit_status;

rgxg_options_t options;

typedef struct module_s {
    char *command;
    char *description;
    void (*set_defaults) ();
    int (*argv_parse) (int , char **);
    int (*generate_regex) (char *);
    void (*print_help) ();
} module_t;

#define EASY_MODULE(command, description) \
    { #command, description, \
    command##_set_defaults, command##_argv_parse, \
    command##_generate_regex, command##_print_help }

module_t modules[] = {
    EASY_MODULE(alternation,    "Create regex that matches any of the given patterns"),
    EASY_MODULE(cidr,           "Create regex that matches all addresses of the given CIDR block"),
    EASY_MODULE(escape,         "Escape the given string for use in a regex"),
    EASY_MODULE(range,          "Create regex that matches integers in a given range")
};

static void print_version(void) {
    fprintf(stdout, PACKAGE_NAME " " PACKAGE_VERSION "\n\n");
}

static void print_help() {
    int n = sizeof(modules)/sizeof(module_t);
    int i;
    fprintf(stdout,
                "Usage: rgxg COMMAND [ARGS]\n\n"
                "The available rgxg commands are:\n"
                );
    for (i = 0 ; i < n ; ++i) {
        fprintf(stdout, "  %-15s %s\n", modules[i].command, modules[i].description);
    }
    fprintf(stdout,"\nType 'rgxg help COMMAND' for help information on a specific command.\n");
    fprintf(stdout,"\nType 'rgxg version' to see the version of rgxg.\n");
}

static module_t *get_module(char *command) {
    int n = sizeof(modules)/sizeof(module_t);
    int i;
    for (i = 0 ; i < n ; ++i) {
        if (!strcmp(command, modules[i].command)) {
            return &modules[i];
        }
    }
    return NULL;
}

int main(int argc, char* *argv) {
    exit_status = 0;

    module_t *module;

    if (argc < 2) {
        print_help();
        exit_status = 1;
    } else if (!(strcmp("help", argv[1]))) { /* 'help' command */
        if (argc >= 3) {
            if ((module = get_module(argv[2])) != NULL) {
                module->print_help();
                exit_status = 0;
            } else {
                fprintf(stderr, "rgxg help: '%s' is not a rgxg command.\n", argv[2]);
                exit_status = 1;
            }
        } else {
            print_help();
            exit_status = 0;
        }
    } else if (!(strcmp("version", argv[1]))) { /* 'version' command */
            print_version();
            exit_status = 0;
    } else {
        int length;
        char *command = argv[1];
        char *regex = NULL;
        if ((module = get_module(command)) != NULL) {
            module->set_defaults();
            if (module->argv_parse(--argc, ++argv)) { /* skip command string */
                length = module->generate_regex(NULL);
                if (length >= 0) {
                    regex = malloc(sizeof(char) * (length+1));
                    module->generate_regex(regex);
                    fprintf(stdout, "%s\n", regex);
                    exit_status = 0;
                }
            }
        } else {
                fprintf(stderr, "rgxg: '%s' is not a rgxg command.\n", command);
                exit_status = 1;
        }
        free(regex);
    }

    return exit_status;
}
