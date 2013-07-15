/* scanners.h -- defs for interface to scanners.c
   Copyright (C) 1986, 1995, 1996, 1999 Free Software Foundation, Inc.
   Written by Greg McGary <gkm@gnu.ai.mit.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#ifndef _scanners_h_
#define _scanners_h_

#include "xobstack.h"

extern int log_8_member_files;	/* log base 8 of the # of files.
				   e.g., log_8 (32768) == 5 */

struct token
{
  unsigned short tok_count;
  unsigned char tok_flags;
  unsigned char tok_hits_name[1];
};

#define TOKEN_HITS(TOKEN) ((TOKEN)->tok_hits_name)
#define TOKEN_NAME(TOKEN) ((TOKEN)->tok_hits_name + log_8_member_files)
#define OFFSETOF_TOKEN_NAME (offsetof (struct token, tok_hits_name) + log_8_member_files)

typedef struct token *(*get_token_func_t) __P((FILE *in_FILE, void const *args, int *flags));
typedef void *(*parse_args_func_t) __P((char **argv, int argc));
typedef void (*help_me_func_t) __P((void));

struct language
{
  char const *lg_name;
  parse_args_func_t lg_parse_args;
  get_token_func_t lg_get_token;
  help_me_func_t lg_help_me;
  int lg_argc;
  char *lg_argv[16];
};

struct lang_args
{
  struct language const *la_language;
  char const *la_pattern;	/* fnmatch(3) pattern */
  char *la_args_string;	/* human-readable scanner args */
  void const *la_args_digested;	/* pre-parsed scanner args */
  int la_index;
  struct lang_args *la_next;
};

extern void language_help_me __P((void));
extern void language_getopt __P((void));
extern void language_save_arg __P((char *arg));
extern struct language *get_language __P((char const *lang_name));
extern void parse_language_map __P((char const *file_name));
extern void set_default_language __P((char const *lang_name));

extern struct lang_args *lang_args_default;
extern struct lang_args *lang_args_list;

extern struct obstack tokens_obstack;
extern unsigned char *scanner_buffer;

#endif /* not _scanners_h_ */
