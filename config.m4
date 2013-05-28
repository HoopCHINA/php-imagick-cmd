dnl
dnl $Id: config.m4 2013-05-24 11:16:23Z Wang Wenlin $
dnl

PHP_ARG_WITH(imagick-cmd, whether to enable the imagick-cmd extension,
[  --with-imagick-cmd[=DIR]  Enables the imagick-cmd extension. DIR is the prefix to Imagemagick installation directory.], no)

if test $PHP_IMAGICK_CMD != "no"; then

  AC_MSG_CHECKING(ImageMagick MagickWand API configuration program)
    
  for i in $PHP_IMAGICK_CMD /usr/local /usr;
  do
    test -r $i/bin/MagickWand-config && IMAGICK_AFTER_BWC_BREAK=true && WAND_BINARY=$i/bin/MagickWand-config && break
  done

  if test -z "$WAND_BINARY"; then
    for i in $PHP_IMAGICK_CMD /usr/local /usr;
    do
      test -r $i/bin/Wand-config && WAND_BINARY=$i/bin/Wand-config && break
    done
  fi
    
  if test -z "$WAND_BINARY"; then
    AC_MSG_ERROR(not found. Please provide a path to MagickWand-config or Wand-config program.)
  fi
    
  AC_MSG_RESULT(found in $WAND_BINARY)

  IMAGEMAGICK_VERSION_ORIG=`$WAND_BINARY --version`
  IMAGEMAGICK_VERSION_MASK=`echo ${IMAGEMAGICK_VERSION_ORIG} | awk 'BEGIN { FS = "."; } { printf "%d", ($1 * 1000 + $2) * 1000 + $3;}'`

  AC_MSG_CHECKING(if ImageMagick version is at least 6.5.0)
  if test "$IMAGEMAGICK_VERSION_MASK" -ge 6005000; then
    AC_MSG_RESULT(found version $IMAGEMAGICK_VERSION_ORIG)
  else
    AC_MSG_ERROR(no. You need at least Imagemagick version 6.5.0 to use IMagickCommand.)
  fi

  WAND_DIR=`$WAND_BINARY --prefix` 
    
  if test -z "$IMAGICK_AFTER_BWC_BREAK"; then
    AC_MSG_CHECKING(for magick-wand.h header file)
    if test -r $WAND_DIR/include/wand/magick-wand.h; then
      AC_MSG_RESULT(found in $WAND_DIR/include/wand/magick-wand.h)
    else
      AC_MSG_ERROR(Cannot locate header file magick-wand.h)
    fi  
  else  
    AC_MSG_CHECKING(for MagickWand.h header file)

    if test -r $WAND_DIR/include/ImageMagick-6/wand/MagickWand.h; then
      AC_MSG_RESULT(found in $WAND_DIR/include/ImageMagick-6/wand/MagickWand.h)
    elif test -r $WAND_DIR/include/ImageMagick/wand/MagickWand.h; then
      AC_MSG_RESULT(found in $WAND_DIR/include/ImageMagick/wand/MagickWand.h)
    else
      AC_MSG_ERROR(Cannot locate header file MagickWand.h)
    fi
    AC_DEFINE(IMAGICK_USE_NEW_HEADER,1,[ ])
    PHP_IMAGICK_CMD_USE_NEW_HEADER=1
  fi

  AC_MSG_CHECKING([PHP version is at least 5.1.3])

  if test -z "${PHP_VERSION_ID}"; then
    if test -z "${PHP_CONFIG}"; then
      AC_MSG_ERROR([php-config not found])
    fi
    if test -z "${AWK}"; then
      AC_MSG_ERROR([awk not found])
    fi
    PHP_IMAGICK_CMD_FOUND_VERSION=`${PHP_CONFIG} --version`
    PHP_IMAGICK_CMD_FOUND_VERNUM=`echo "${PHP_IMAGICK_CMD_FOUND_VERSION}" | $AWK 'BEGIN { FS = "."; } { printf "%d", ([$]1 * 100 + [$]2) * 100 + [$]3;}'`
  else
    PHP_IMAGICK_CMD_FOUND_VERSION="${PHP_VERSION}"
    PHP_IMAGICK_CMD_FOUND_VERNUM="${PHP_VERSION_ID}"
  fi

  if test "$PHP_IMAGICK_CMD_FOUND_VERNUM" -ge "50103"; then
    AC_MSG_RESULT(yes. found $PHP_IMAGICK_CMD_FOUND_VERSION)
  else 
    AC_MSG_ERROR(no. found $PHP_IMAGICK_CMD_FOUND_VERSION)
  fi

  AC_DEFINE(HAVE_IMAGICK_CMD, 1, [Whether you want imagick-cmd support])

  IMAGICK_LIBS=`$WAND_BINARY --libs`
  IMAGICK_INCS=`$WAND_BINARY --cflags`

  PHP_EVAL_LIBLINE($IMAGICK_LIBS, IMAGICK_CMD_SHARED_LIBADD)
  PHP_EVAL_INCLINE($IMAGICK_INCS)

  PHP_SUBST(IMAGICK_CMD_SHARED_LIBADD)
  
  PHP_NEW_EXTENSION(imagick_cmd, php_imagick_cmd.c, $ext_shared)

dnl this is needed to build the extension with phpize and -Wall

  if test "$PHP_DEBUG" = "yes"; then
    CFLAGS="$CFLAGS -Wall"
  fi

fi
