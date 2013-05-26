dnl
dnl $Id: config.m4 2013-05-24 11:16:23Z Wang Wenlin $
dnl

PHP_ARG_ENABLE(imagick-cmd, whether to enable imagick-cmd support,
[  --enable-imagick-cmd       Enable imagick-cmd support])

if test "$PHP_IMAGICK_CMD" != "no"; then

    AC_DEFINE(HAVE_IMAGICK_CMD, 1, [Whether you want imagick-cmd support])
    PHP_NEW_EXTENSION(imagick-cmd, php_imagick_cmd.c, $ext_shared)

dnl this is needed to build the extension with phpize and -Wall

  if test "$PHP_DEBUG" = "yes"; then
    CFLAGS="$CFLAGS -Wall"
  fi

fi
