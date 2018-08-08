#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([OpenDSIM], [0.0.1], [diyer175@hotmail.com])
AC_CANONICAL_HOST
AM_INIT_AUTOMAKE([1.9])
LT_PREREQ([2.4.6])
LT_INIT
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([config.mk])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_SED
AC_PROG_LIBTOOL
AC_CHECK_TOOL(SED, sed, :)
AC_CHECK_TOOL(PYTHON, python, :)

if test "$SED" = ":"; then
  AC_MSG_ERROR([The $PACKAGE package needs the 'sed' to compile properly.])
fi
if test "$python" = ":"; then
  AC_MSG_ERROR([The $PACKAGE package needs the 'python' to compile properly.])
fi

# C Compiler standard
CFLAGS="-std=c99 -g"

# Checks for debug options
AC_ARG_ENABLE([debug],
  AC_HELP_STRING([--enable-debug],
                 [include debug output @<:@default=no@:>@]),
  [case "$enableval" in
   yes) enable_debug="yes" ;;
   no)  enable_debug="no"  ;;
   *)   enable_debug="no"  ;;
   esac],
  [enable_debug="no"])
if test "$enable_debug" = yes; then
  AC_DEFINE(DEBUG, 1, [Define if debug output should be supported.])
  QT_DEF="-DQT_DEBUG"
  if test "x$GCC" = xyes; then
    CFLAGS="`echo $CFLAGS | sed -e 's/\ -O2//g'`"
    CXXFLAGS="`echo $CXXFLAGS | sed -e 's/\ -O2//g'`"
  fi
else
  AC_DEFINE(NDEBUG, 1, [Define if debug code should be suppressed.])
  QT_DEF="-DQT_NO_DEBUG -DQT_NO_DEBUG_OUTPUT"
  if test "x$GCC" = xyes; then
    CFLAGS="`echo $CFLAGS | sed -e 's/\-g //g'`"
    CXXFLAGS="`echo $CXXFLAGS | sed -e 's/\-g //g'`"
  fi
fi

# Platform specific, find libraries, setup compiler flags
case $host_os in
  *linux* | *cygwin* | *mingw* | *msys* | *bsd* )

    dnl Set Clang
    if test "$CXX -dM -E - < /dev/null | grep __clang__" ; then
      CFLAGS="$CFLAGS -pipe"
      CXXFLAGS="$CXXFLAGS -pipe -fno-exceptions -Wno-deprecated-register"
      LDDFLAGS="$LDDFLAGS"
      use_CLANG="yes"
    fi

    dnl Set GCC.
    if test "x$use_CLANG" != "xyes" -a "x$GCC" = "xyes" ; then
      CFLAGS="$CFLAGS -pipe"
      CXXFLAGS="$CXXFLAGS -pipe -fno-exceptions -fno-check-new"
      if test x$WIN32 = xyes; then
        CXXFLAGS="$CXXFLAGS"
      fi
    fi

    if test "$enable_debug" = yes; then
      CFLAGS="$CFLAGS -W -Wall -Wmissing-prototypes"
      CXXFLAGS="$CXXFLAGS -W -Wall"
    fi

    dnl Enable C++11 support
    dnl CXXFLAGS="$CXXFLAGS -std=c++0x"

    dnl handle provided QTDIR environment variable
    QTDIR_paths=""
    if test "x$QT5DIR" != "x"; then
      echo "QT5DIR provided: $QT5DIR"
      QTDIR=$QT5DIR
    elif test "x$QTDIR" != "x"; then
      echo "QTDIR provided: $QTDIR"
    fi
    QTDIR_paths="$QTDIR/lib $QTDIR/lib64"

    dnl Check include path to Qt.
    QT_INCLUDES=""
    QT_VER=5
    AC_MSG_CHECKING([for Qt headers])
    paths="$QTDIR/include $QTDIR/include/qt5 \
      /usr/local/qt5/include /usr/include/qt5 \
      /usr/include /usr/lib/qt/include /usr/X11R6/include/X11/qt5 \
      /usr/X11R6/include/qt5 /usr/X11R6/include /sw/include/qt5 \
      /usr/local/include/qt5"
    for path in $paths; do
      if test -f "$path/QtWidgets/qapplication.h"; then
        QT_INCLUDES="$path -I$path/QtWidgets -I$path/QtGui -I$path/QtCore -I$path/QtConcurrent -I$path/QtSvg -I$path/QtXml -I$path/QtScript"
        QT_VER=5
        break
      fi
    done
    if test "x$QT_INCLUDES" != "x"; then
      AC_MSG_RESULT([found in $QT_INCLUDES])
      QT_INCLUDES="-I$QT_INCLUDES"
    else
      AC_MSG_ERROR([not found])
    fi
    AC_SUBST(QT_INCLUDES)

    dnl Check for multi-threaded option.
    AC_ARG_ENABLE([mt],
      AC_HELP_STRING([--disable-mt],
                     [link to non-threaded Qt (deprecated)]),
      enable_mt="$enableval",
      [if test $QT_VER = 5; then
         enable_mt="yes"
       else
         enable_mt="no"
       fi])
    if test "$enable_mt" = yes; then
      QT_LDF=""
      QT_LIB="-lQt5Widgets -lQt5Core -lQt5Gui -lQt5Xml -lQt5Concurrent -lQt5Svg -lQt5Script"
      QT_INC="$QT_DEF -DQT_THREAD_SUPPORT -D_REENTRANT"
      [case $host_os in
        *freebsd*) QT_LIB="$QT_LIB -pthread" ;;
        *mingw* | *msys*) QT_INC="$QT_INC -mthreads"; QT_LDF="$QT_LDF -mthreads" ;;
      esac]
      QT_MTS="multi-threaded"
    else
      QT_LDF=""
      QT_LIB="-lQt5Widgets -lQt5Core -lQt5Gui -lQt5Xml -lQt5Concurrent -lQt5Svg -lQt5Script"
      QT_INC="$QT_DEF"
      QT_MTS="non-threaded"
    fi
    case $host_os in
      *mingw* | *msys*)
      QT_LIB="-lQt5Widgets -lQt5Core -lQt5Gui -lQt5Xml -lQt5Concurrent -lQt5Svg -lQt5Script"
      QT_INC="$QT_INC -DQT_DLL -DUNICODE"
      QT_LDF="$QT_LDF -mwindows"
      ;;
    esac
    unset enable_mt

    dnl Check library path to Qt.
    QT_LDFLAGS=""
    QT_LIBS=""
    AC_MSG_RESULT([checking for Qt... $QT_VER ($QT_MTS)])
    AC_MSG_CHECKING([for Qt library])
    paths="$QTDIR_paths /usr/local/qt5/lib /usr/lib/qt5 /usr/lib/qt5/Qtconf /usr/lib \
      /usr/X11R6/lib/X11/qt5 /usr/X11R6/lib/X11/qt5 /usr/X11R6/lib/qt5 \
      /usr/X11R6/lib /sw/lib /usr/lib64/qt5 /usr/X11R6/lib/qt5 /usr/local/lib/qt5"
    AC_LANG(C++)
    for path in $paths; do
      save_LIBS="$LIBS"
      save_LDFLAGS="$LDFLAGS"
      save_CXXFLAGS="$CXXFLAGS"
      LIBS="$LIBS $X11_LIBS $QT_LIB"
      LDFLAGS="$LDFLAGS $X11_LDFLAGS -L$path $QT_LDF"
      CXXFLAGS="$CXXFLAGS $X11_INCLUDES $QT_INCLUDES $QT_INC"
      AC_LINK_IFELSE(
          [AC_LANG_SOURCE([#include <QApplication>
                           int main (int argc, char ** argv) {
                           QApplication a (argc, argv); a.exec (); return 0; }])],
                           [
                                 QT_LDFLAGS="$path";
                                 QT_INCLUDES="$QT_INCLUDES $QT_INC";
                                 break;
                           ]
      )
      LIBS="$save_LIBS"
      LDFLAGS="$save_LDFLAGS"
      CXXFLAGS="$save_CXXFLAGS"
    done

    dnl Capture dir found for Qt
    QT_DIR="$path"

    LIBS="$save_LIBS"
    LDFLAGS="$save_LDFLAGS"
    CXXFLAGS="$save_CXXFLAGS"
    if test "x$QT_LDFLAGS" != "x"; then
      AC_MSG_RESULT([found in $QT_LDFLAGS])
      QT_LDFLAGS="-L$QT_LDFLAGS $QT_LDF"
      QT_LIBS="$QT_LIB"
    else
      AC_MSG_ERROR([not found])
    fi
    ;;

esac

case $host_os in
  *cygwin* | *mingw* | *msys* )
  
    LDFLAGS="$LDFLAGS -Wl,-subsystem,console"
  ;;
esac

AC_SUBST(QT_LDFLAGS)
AC_SUBST(QT_LIBS)
AC_SUBST(QT_INCLUDES)

# Checks for MOC/UIC/RCC support.
AC_PATH_PROGS(MOC, moc, :, [$QT_DIR/bin:$PATH])
AC_PATH_PROGS(UIC, uic, :, [$QT_DIR/bin:$PATH])
AC_PATH_PROGS(RCC, rcc, :, [$QT_DIR/bin:$PATH])

if test "$MOC" = ":"; then
  AC_MSG_ERROR([The $PACKAGE package needs the 'Qt Meta Object Compiler' to compile properly.])
fi

if test "$UIC" = ":"; then
  AC_MSG_ERROR([The $PACKAGE package needs the 'Qt User Interface Compiler' to compile properly.])
fi

if test "$RCC" = ":"; then
  AC_MSG_ERROR([The $PACKAGE package needs the 'Qt Resource Collection Compiler' to compile properly.])
fi

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h stddef.h stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove memset pow])

# Checks for host arch
cpu=$host_cpu
case "$cpu" in
  i*86)
    AC_DEFINE([ARCH_X86],[1], [Define host arch.])
  ;;
  x86_64)
    AC_DEFINE([ARCH_X86_64],[1], [Define host arch.])
  ;;
  alpha)
    AC_DEFINE([ARCH_ALPHA],[1], [Define host arch.])
  ;;
  armv4b)
    AC_DEFINE([ARCH_ARM],[1], [Define host arch.])
  ;;
  armv4l)
    AC_DEFINE([ARCH_ARM],[1], [Define host arch.])
  ;;
  cris)
    AC_DEFINE([ARCH_CRIS],[1], [Define host arch.])
  ;;
  hppa)
    AC_DEFINE([ARCH_HPPA],[1], [Define host arch.])
  ;;
  ia64*)
    AC_DEFINE([ARCH_IA64],[1], [Define host arch.])
  ;;
  m68k)
    AC_DEFINE([ARCH_M68K],[1], [Define host arch.])
  ;;
  mips)
    AC_DEFINE([ARCH_MIPS],[1], [Define host arch.])
  ;;
  mips64)
    AC_DEFINE([ARCH_MIPS64],[1], [Define host arch.])
  ;;
  ppc)
    AC_DEFINE([ARCH_PPC],[1], [Define host arch.])
  ;;
  ppc64)
    AC_DEFINE([ARCH_PPC64],[1], [Define host arch.])
  ;;
  s390)
    AC_DEFINE([ARCH_S390],[1], [Define host arch.])
  ;;
  sparc)
    AC_DEFINE([ARCH_SPARC],[1], [Define host arch.])
  ;;
  sparc64)
    AC_DEFINE([ARCH_SPARC64],[1], [Define host arch.])
  ;;
  *)
    AC_MSG_ERROR([Unsupported host CPU = $cpu])
  ;;
esac

# Checks for the endian of host CPU
bigendian="no"
if test "$cpu" = "armv4b" \
     -o "$cpu" = "hppa" \
     -o "$cpu" = "m68k" \
     -o "$cpu" = "mips" \
     -o "$cpu" = "mips64" \
     -o "$cpu" = "ppc" \
     -o "$cpu" = "ppc64" \
     -o "$cpu" = "s390" \
     -o "$cpu" = "sparc" \
     -o "$cpu" = "sparc64"; then
    bigendian="yes"
    AC_DEFINE([ENDIAN_BIG],[1], [Define host big endian.])
fi

# Checks  host long bits
hostlongbits="32"
if test "$cpu" = "x86_64" \
     -o "$cpu" = "alpha" \
     -o "$cpu" = "ia64" \
     -o "$cpu" = "sparc64" \
     -o "$cpu" = "ppc64"; then
    hostlongbits="64"
    AC_DEFINE([ARCH_BITS],[64], [Define host long 64 bit.])
else
    AC_DEFINE([ARCH_BITS],[32], [Define host long 32 bit.])
fi

# Checks for host OS
case $host_os in
  *linux* )
    AC_DEFINE([OS_LINUX],[1], [Define host os Linux.])
  ;;
  
  *cygwin* | *mingw* | *msys* )
    AC_DEFINE([OS_WIN32],[1], [Define host os Win32.])
  ;;
  
  *bsd* )
    AC_DEFINE([OS_BSD],[1], [Define host os BSD.])
  ;;
  
  else )
    AC_MSG_ERROR([Unsupported host OS: $host_os])
  ;;
esac

AC_CONFIG_FILES([Makefile
                 opendsim-core/Makefile
                 opendsim-core/models/Makefile
                 opendsim-frontend/Makefile
                 opendsim-frontend/devices/Makefile
                 testcase/Makefile])
                 
# Check for Git short SHA to tag version
# The release package should also keep it on the config.h
GIT="unknown"
if test -d ".git"; then
  m4_define([GIT_REVISION], m4_esyscmd([git log --pretty=format:'%h' -n 1u]))
  AC_DEFINE([GIT], ["GIT_REVISION"], [Git short revision hash.])
  GIT=GIT_REVISION
  echo "Found Git clone... $GIT"
fi

AC_OUTPUT

# Print results.
AC_MSG_RESULT([])
AC_MSG_RESULT([ $PACKAGE version $VERSION ($GIT) configured successfully.])
AC_MSG_RESULT([
Configure Information:
  C Compiler      : $CC
    CFLAGS        : $CFLAGS
    DEFS          : $DEFS
  C++ Compiler    : $CXX
    CXXFLAGS      : $CXXFLAGS

  Linker          : $LD
    LDFLAGS       : $LDFLAGS
    LIBS          : $LIBS

  Prefix          : $prefix
  QTDIR           : $QT_DIR
  QT_LIBS         : $QT_LIBS
  QT_LDFLAGS      : $QT_LDFLAGS
  QT_INCLUDES     : $QT_INCLUDES
])
AC_MSG_RESULT([])
