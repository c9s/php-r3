dnl vim:sw=2:ts=2:sts=2:
THIRDPARTY_BUILD_DIR="$srcdir/thirdparty/build"
PHP_ADD_INCLUDE("$srcdir/thirdparty/r3/3rdparty")
PHP_ADD_INCLUDE("$srcdir/thirdparty/r3/include")

PHP_ARG_ENABLE(r3, whether to enable r3 support,
Make sure that the comment is aligned:
[ --enable-r3 Enable r3 support])

PHP_ARG_WITH(pcre-dir, libpcre install dir,
[  --with-pcre-dir=DIR PCRE: libpcre install prefix])


if test "$PHP_R3_DEBUG" != "no"; then
    CFLAGS="$CFLAGS -Wall -g -ggdb -O0 -DPHP_R3_DEBUG=1"
    AC_DEFINE(PHP_R3_DEBUG, 1, [Enable r3 debug support])
fi

PHP_NEW_EXTENSION(r3,
  thirdparty/r3/3rdparty/zmalloc.c \
  thirdparty/r3/src/node.c \
  thirdparty/r3/src/edge.c \
  thirdparty/r3/src/str.c \
  thirdparty/r3/src/token.c \
  thirdparty/r3/src/match_entry.c \
  thirdparty/r3/src/slug.c \
  php_r3.c, $ext_shared)

if test $PHP_R3 != "no"; then

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
  AC_MSG_RESULT([$PCRE_INCDIR])

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
  PHP_ADD_LIBRARY_WITH_PATH(pcre, $PCRE_LIBDIR, R3_SHARED_LIBADD)
  PHP_ADD_INCLUDE($PCRE_INCDIR)

  PHP_SUBST(R3_SHARED_LIBADD)

  dnl PHP_CHECK_LIBRARY(r3, r3_version,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH(r3, $UV_DIR/lib, R3_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_R3LIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong r3 library version or library not found])
  dnl ],[
  dnl   -L$UV_DIR/lib -lm
  dnl ])
  dnl PHP_SUBST(R3_SHARED_LIBADD)
  dnl PHP_ADD_MAKEFILE_FRAGMENT([Makefile.thirdparty])
fi

