dnl vim:sw=2:ts=2:sts=2:

THIRDPARTY_BUILD_DIR="$srcdir/thirdparty/build"
PHP_ADD_INCLUDE("$srcdir/thirdparty/r3/include")

PHP_ARG_ENABLE(r3, whether to enable r3 support,
Make sure that the comment is aligned:
[ --enable-r3 Enable r3 support])

PHP_ARG_WITH(pcre-dir, libpcre install dir,
[  --with-pcre-dir=DIR PCRE: libpcre install prefix])

if test "$PHP_PCRE_DIR" != "yes" ; then
  AC_MSG_CHECKING([for PCRE headers location])
  for i in $PHP_PCRE_DIR $PHP_PCRE_DIR/include $PHP_PCRE_DIR/include/pcre $PHP_PCRE_DIR/local/include; do
    test -f $i/pcre.h && PCRE_INCDIR=$i
  done
  if test -z "$PCRE_INCDIR"; then
    AC_MSG_ERROR([Could not find pcre.h in $PHP_PCRE_DIR])
  fi
  AC_MSG_RESULT([$PCRE_INCDIR])

  AC_MSG_CHECKING([for PCRE library location])
  for j in $PHP_PCRE_DIR $PHP_PCRE_DIR/$PHP_LIBDIR; do
    test -f $j/libpcre.a || test -f $j/libpcre.$SHLIB_SUFFIX_NAME && PCRE_LIBDIR=$j
  done

  if test -z "$PCRE_LIBDIR" ; then
    AC_MSG_ERROR([Could not find libpcre.(a|$SHLIB_SUFFIX_NAME) in $PHP_PCRE_DIR])
  fi
  AC_MSG_RESULT([$PCRE_LIBDIR])

  PHP_ADD_LIBRARY_WITH_PATH(pcre, $PCRE_LIBDIR)
  AC_DEFINE(HAVE_PCRE, 1, [ ])
  PHP_ADD_INCLUDE($PCRE_INCDIR)
fi

PHP_NEW_EXTENSION(r3, php_r3.c, $ext_shared)

PHP_ADD_MAKEFILE_FRAGMENT([Makefile.thirdparty])

PHP_ADD_INCLUDE(r3)
PHP_ADD_LIBRARY(r3)
