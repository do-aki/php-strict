dnl $Id$
dnl config.m4 for extension strict

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(strict, for strict support,
dnl Make sure that the comment is aligned:
dnl [  --with-strict             Include strict support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(strict, whether to enable strict support,
Make sure that the comment is aligned:
[  --enable-strict           Enable strict support])

if test "$PHP_STRICT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-strict -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/strict.h"  # you most likely want to change this
  dnl if test -r $PHP_STRICT/$SEARCH_FOR; then # path given as parameter
  dnl   STRICT_DIR=$PHP_STRICT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for strict files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       STRICT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$STRICT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the strict distribution])
  dnl fi

  dnl # --with-strict -> add include path
  dnl PHP_ADD_INCLUDE($STRICT_DIR/include)

  dnl # --with-strict -> check for lib and symbol presence
  dnl LIBNAME=strict # you may want to change this
  dnl LIBSYMBOL=strict # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $STRICT_DIR/lib, STRICT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_STRICTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong strict lib version or lib not found])
  dnl ],[
  dnl   -L$STRICT_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(STRICT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(strict, strict.c strict_op.c source_graph.c source_file.c, $ext_shared)
fi
