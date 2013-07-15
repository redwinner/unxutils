/* C code produced by gperf version 2.7 */
/* Command-line: gperf -D -c -p -t -T -g -j1 -o -K rwd -N is_reserved indent.gperf  */

#define TOTAL_KEYWORDS 31
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 8
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 42
/* maximum key range = 39, duplicates = 0 */


#ifdef __GNUC__
__inline
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static unsigned char asso_values[] =
    {
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43,  6,  9,
      10,  0, 16,  5,  4, 24, 43,  0, 20,  4,
      20,  0, 43, 43,  6,  0,  0, 10, 10,  2,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43, 43, 43, 43, 43,
      43, 43, 43, 43, 43, 43
    };
  return len + asso_values[(unsigned char)str[len - 1]] + asso_values[(unsigned char)str[0]];
}

#ifdef __GNUC__
__inline
#endif
struct templ *
is_reserved (str, len)
     register const char *str;
     register unsigned int len;
{
  static struct templ wordlist[] =
    {
      {"else", rw_sp_else,},
      {"short", rw_decl,},
      {"struct", rw_struct_like,},
      {"while", rw_sp_paren,},
      {"enum", rw_enum,},
      {"goto", rw_break,},
      {"switch", rw_switch,},
      {"break", rw_break,},
      {"do", rw_sp_nparen,},
      {"case", rw_case,},
      {"const", rw_decl,},
      {"static", rw_decl,},
      {"double", rw_decl,},
      {"default", rw_case,},
      {"volatile", rw_decl,},
      {"char", rw_decl,},
      {"register", rw_decl,},
      {"float", rw_decl,},
      {"sizeof", rw_sizeof,},
      {"typedef", rw_decl,},
      {"void", rw_decl,},
      {"for", rw_sp_paren,},
      {"extern", rw_decl,},
      {"int", rw_decl,},
      {"unsigned", rw_decl,},
      {"long", rw_decl,},
      {"global", rw_decl,},
      {"return", rw_return,},
      {"union", rw_struct_like,},
      {"va_dcl", rw_decl,},
      {"if", rw_sp_paren,}
    };

  static short lookup[] =
    {
      -1, -1, -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
      10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
      24, 25, -1, 26, 27, -1, -1, 28, 29, -1, -1, -1, -1, -1,
      30
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = wordlist[index].rwd;

              if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
                return &wordlist[index];
            }
          else if (index < -TOTAL_KEYWORDS)
            {
              register int offset = - 1 - TOTAL_KEYWORDS - index;
              register struct templ *wordptr = &wordlist[TOTAL_KEYWORDS + lookup[offset]];
              register struct templ *wordendptr = wordptr + -lookup[offset + 1];

              while (wordptr < wordendptr)
                {
                  register const char *s = wordptr->rwd;

                  if (*str == *s && !strncmp (str + 1, s + 1, len - 1))
                    return wordptr;
                  wordptr++;
                }
            }
        }
    }
  return 0;
}
