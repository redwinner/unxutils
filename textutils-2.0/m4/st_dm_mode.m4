#serial 2

# Define HAVE_ST_DM_MODE if struct stat has an st_dm_mode member.

AC_DEFUN(AC_STRUCT_ST_DM_MODE,
 [AC_CACHE_CHECK([for st_dm_mode in struct stat], ac_cv_struct_st_dm_mode,
   [AC_TRY_COMPILE([#include <sys/types.h>
#include <sys/stat.h>], [struct stat s; s.st_dm_mode;],
     ac_cv_struct_st_dm_mode=yes,
     ac_cv_struct_st_dm_mode=no)])

  if test $ac_cv_struct_st_dm_mode = yes; then
    AC_DEFINE_UNQUOTED(HAVE_ST_DM_MODE, 1,
		       [Define if struct stat has an st_dm_mode member. ])
  fi
 ]
)
