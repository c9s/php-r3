dnl vim:sw=2:ts=2:sts=2:
PHP_ARG_ENABLE(r3, whether to enable r3 support,
Make sure that the comment is aligned:
[ --enable-r3 Enable r3 support])

PHP_ARG_WITH(pcre-dir, libpcre install dir,
[  --with-pcre-dir=DIR PCRE: libpcre install prefix])

if test "$PHP_R3_DEBUG" != "no"; then
    CFLAGS="$CFLAGS -Wall -g -ggdb -O0 -DPHP_R3_DEBUG=1"
    AC_DEFINE(PHP_R3_DEBUG, 1, [Enable r3 debug support])
fi

if test $PHP_R3 != "no"; then
  r3_sources="
  r3/3rdparty/zmalloc.c
  r3/src/str.c
  r3/src/token.c
  r3/src/slug.c
  r3/src/edge.c
  r3/src/node.c
  r3/src/json.c
  r3/src/list.c
  r3/src/match_entry.c"
  PHP_NEW_EXTENSION(r3, $r3_sources php_r3.c, $ext_shared)

  dnl PHP_R3_CFLAGS="-DHAVE_CONFIG_H -I@ext_srcdir@/pcrelib -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1"

  PHP_ADD_INCLUDE("$srcdir/r3/include")

  dnl include zmalloc
  PHP_ADD_INCLUDE("$srcdir/r3/3rdparty")

  dnl Add libpcre include directory
  PHP_ADD_INCLUDE("$phpincludedir/ext/pcre/pcrelib")

  AC_CHECK_DECL(
    [HAVE_BUNDLED_PCRE],
    [AC_CHECK_HEADERS(
      [ext/pcre/php_pcre.h],
      [
        PHP_ADD_EXTENSION_DEP([r3], [pcre])
        AC_DEFINE([ZEPHIR_USE_PHP_PCRE], [1], [Whether PHP pcre extension is present at compile time])
      ],
      ,
      [[#include "main/php.h"]]
    )],[
      dnl Detect pcre
      if test "$PHP_PCRE_DIR" != "yes" ; then
        AC_MSG_CHECKING([for PCRE headers location])
        for i in $PHP_PCRE_DIR $PHP_PCRE_DIR/include $PHP_PCRE_DIR/include/pcre $PHP_PCRE_DIR/local/include; do
          test -f $i/pcre.h && PCRE_INCDIR=$i
        done
      else
        AC_MSG_CHECKING([for PCRE headers location])
        for i in /usr/include /usr/local/include /usr/local/include/pcre /opt/local/include; do
          test -f $i/pcre.h && PCRE_INCDIR=$i
        done
      fi

      if test -z "$PCRE_INCDIR"; then
        AC_MSG_ERROR([Could not find pcre.h in $PHP_PCRE_DIR])
      fi
      AC_MSG_RESULT([PCRE Include: $PCRE_INCDIR])
      
      
      AC_MSG_CHECKING([for PCRE library location])
      if test "$PHP_PCRE_DIR" != "yes" ; then
        for j in $PHP_PCRE_DIR $PHP_PCRE_DIR/$PHP_LIBDIR; do
          test -f $j/libpcre.a || test -f $j/libpcre.$SHLIB_SUFFIX_NAME && PCRE_LIBDIR=$j
        done
      else
        for j in /usr/lib /usr/local/lib /opt/local/lib; do
          test -f $j/libpcre.a || test -f $j/libpcre.$SHLIB_SUFFIX_NAME && PCRE_LIBDIR=$j
        done
      fi

      if test -z "$PCRE_LIBDIR" ; then
        AC_MSG_ERROR([Could not find libpcre.(a|$SHLIB_SUFFIX_NAME) in $PHP_PCRE_DIR])
      fi
      AC_MSG_RESULT([$PCRE_LIBDIR])

      AC_DEFINE(HAVE_PCRE, 1, [ ])
      PHP_ADD_INCLUDE($PCRE_INCDIR)
      PHP_ADD_LIBRARY_WITH_PATH(pcre, $PCRE_LIBDIR, R3_SHARED_LIBADD)



    ], [[#include "php_config.h"]]
  )


  PHP_SUBST(R3_SHARED_LIBADD)
fi

