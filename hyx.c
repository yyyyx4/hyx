/*
 *
 * Copyright (c) 2016 Lorenz Panny
 *
 * This is hyx version 0.1.1 (23 May 2016).
 * Check for newer versions at https://home.in.tum.de/~panny.
 * Please report bugs to lorenz.panny@tum.de.
 *
 * This program is released under the MIT license; see license.txt.
 *
 */

#include "common.h"
#include "blob.h"
#include "view.h"
#include "input.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>


struct blob blob;
struct view view;
struct input input;

bool quit;


void die(char const *s)
{
    fprintf(stderr, "%s\n", s);
    view_text(&view);
    exit(1);
}

void pdie(char const *s)
{
    perror(s);
    view_text(&view);
    exit(1);
}

static void sighdlr(int num)
{
    switch (num) {
    case SIGWINCH:
        view.winch = true;
        break;
    case SIGINT:
        quit = true;
        break;
    default:
        die("unrecognized signal");
    }
}

__attribute__((noreturn)) void version()
{
    printf("This is hyx version 0.1.1 (23 May 2016).\n");
    exit(0);
}

__attribute__((noreturn)) void help(int st)
{
    bool tty = isatty(fileno(stdout));

    printf("\n");
    printf("    %shyx: a minimalistic hex editor%s\n",
            tty ? "\x1b[32m" : "", tty ? "\x1b[0m" : "");
    printf("    ------------------------------\n\n");

    printf("    %sinvocation:%s hyx [$filename]\n\n",
            tty ? "\x1b[33m" : "", tty ? "\x1b[0m" : "");

    printf("    %skeys:%s\n\n",
            tty ? "\x1b[33m" : "", tty ? "\x1b[0m" : "");
    printf("q               quit\n");
    printf("\n");
    printf("h, j, k, l      move cursor\n");
    printf("(hex digits)    edit bytes in hex mode\n");
    printf("(printable)     edit bytes in ascii mode\n");
    printf("i               switch between replace and insert modes\n");
    printf("tab             switch between hex and ascii input\n");
    printf("\n");
    printf("u               undo\n");
    printf("ctrl+r          redo\n");
    printf("\n");
    printf("v               start a selection\n");
    printf("escape          abort a selection\n");
    printf("x               delete current byte or selection\n");
    printf("s               substitute current byte or selection\n");
    printf("y               copy current byte or selection to clipboard\n");
    printf("p               paste\n");
    printf("P               paste and move cursor\n");
    printf("\n");
    printf("[               decrement number of columns\n");
    printf("]               increment number of columns\n");
    printf("\n");
    printf("g               jump to start of screen or file\n");
    printf("G               jump to end of screen or file\n");
    printf("\n");
    printf(":               enter command\n");
    printf("/x (hex string) search for hexadecimal bytes\n");
    printf("/s (characters) search for ascii string\n");
    printf("\n");

    printf("    %scommands:%s\n\n",
            tty ? "\x1b[33m" : "", tty ? "\x1b[0m" : "");
    printf("$offset         jump to offset (supports hex/dec/oct)\n");
    printf("q               quit\n");
    printf("w [$filename]   save\n");
    printf("wq [$filename]  save and quit\n");
    printf("color y/n       toggle colors\n");

    printf("\n");

    exit(st);
}

int main(int argc, char **argv)
{
    struct sigaction sigact;

    char *filename = NULL;

    for (size_t i = 1; i < (size_t) argc; ++i) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
            help(0);
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
            version();
        else if (!filename)
            filename = argv[i];
        else
            help(1);
    }

    blob_init(&blob);
    view_init(&view, &blob, &input);
    blob_load(&blob, filename);
    view.color = true;
    input_init(&input, &view);

    /* set up signal handler */
    memset(&sigact, 0, sizeof(sigact));
    sigact.sa_handler = sighdlr;
    sigaction(SIGWINCH, &sigact, NULL);
    sigaction(SIGINT, &sigact, NULL);

    view_winch(&view);
    view_visual(&view);

    do {
        if (view.winch) {
            view_winch(&view);
            view.winch = false;
        }
        view_update(&view);
        input_get(&input, &quit);
    } while (!quit);

    printf("\x1b[H"); /* move cursor to the top */
    printf("\x1b[2J"); /* clrscr() */
    view_text(&view);

    input_free(&input);
    view_free(&view);
    blob_free(&blob);
}

