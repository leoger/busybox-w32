/*
 * Mini head implementation for busybox
 *
 *
 * Copyright (C) 1999 by Lineo, inc.
 * Written by John Beppu <beppu@line.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include "internal.h"
#include <errno.h>
#include <stdio.h>

const char head_usage[] =
"Usage: head [OPTION]... [FILE]...\n"
"Print first 10 lines of each FILE to standard output.\n"
"With more than one FILE, precede each with a header giving the file name.\n"
"With no FILE, or when FILE is -, read standard input.\n\n"
"  -h	display this help and exit\n";

int
head(int len, FILE *src)
{
    int	    i;
    char    buffer[1024];

    for (i = 0; i < len; i++) {
	fgets(buffer, 1024, src);
	if (feof(src)) { break; }
	fputs(buffer, stdout);
    }
    return 0;
}

/* BusyBoxed head(1) */
int
head_main(int argc, char **argv)
{
    int	    i = 1;
    char    opt;
    int	    len = 10;

    /* 1st option is potentially special */
    if ((argv[1][0] == '-') && isDecimal(argv[1][1])) {
	int tmplen = atoi(&argv[1][1]);
	if (tmplen) { len = tmplen; }
	i = 2;
    }

    /* parse argv[] */
    for ( ; i < argc; i++) {
	if (argv[i][0] == '-') {
	    opt = argv[i][1];
	    switch (opt) {
		case 'h':
		    usage(head_usage);
		default:
		    fprintf(stderr, "head: invalid option -- %c\n", opt);
		    usage(head_usage);
	    }
	} else {
	    break;
	}
    }

    /* get rest of argv[] or stdin if nothing's left */
    if (i >= argc) {
	head(len, stdin);

    } else {
	int need_headers = ((argc - i) > 1);
	for ( ; i < argc; i++) {
	    FILE *src;
	    src = fopen(argv[i], "r");
	    if (!src) {
		fprintf(stderr,"head: %s: %s\n", argv[i], strerror(errno));
	    } else {
		/* emulating GNU behaviour */
		if (need_headers) { 
		    fprintf(stdout, "==> %s <==\n", argv[i]);
		}
		head(len, src);
		if (i < argc - 1) {
		    fprintf(stdout, "\n");
		}
	    }
	}
    }
    exit(0);
}

/* $Id: head.c,v 1.1 1999/12/10 07:42:50 beppu Exp $ */
