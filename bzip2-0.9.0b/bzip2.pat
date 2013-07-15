--- bzip2.c.orig	Wed Sep 09 13:25:50 1998
+++ bzip2.c	Sat Oct 24 22:49:46 1998
@@ -181,6 +181,7 @@
 #   include <io.h>
 #   include <fcntl.h>
 #   include <sys\stat.h>
+#   include <sys\utime.h>
 
 #   define NORETURN       /**/
 #   define PATH_SEP       '\\'
@@ -781,7 +782,7 @@
 /*---------------------------------------------*/
 void copyDatePermissionsAndOwner ( Char *srcName, Char *dstName )
 {
-#if BZ_UNIX
+// #if BZ_UNIX
    IntNative      retVal;
    struct MY_STAT statBuf;
    struct utimbuf uTimBuf;
@@ -804,7 +805,7 @@
    */
    retVal = utime ( dstName, &uTimBuf );
    ERROR_IF_NOT_ZERO ( retVal );
-#endif
+// #endif
 }
 
 
