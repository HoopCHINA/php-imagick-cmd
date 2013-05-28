/* Copyright 2013 Hupu Co., Ltd.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_var.h"
#include "ext/standard/info.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "php_imagick_cmd.h"

#if MagickLibVersion < 0x657
typedef MagickBooleanType
  (*MagickCommand)(ImageInfo *,int,char **,char **,ExceptionInfo *);
#endif

/* {{{ imagick_cmd_functions[]
 *
 * Every user visible function must have an entry in imagick_cmd_functions[].
 */
zend_function_entry imagick_cmd_functions[] = {
    PHP_FE(IMagickMogrifyCommand,   NULL)
    PHP_FE(IMagickConvertCommand,   NULL)
    PHP_FE(IMagickCompositeCommand, NULL)
    PHP_FE(IMagickIdentifyCommand,  NULL)
    PHP_FE(IMagickCompareCommand,   NULL)
    PHP_FE(IMagickConjureCommand,   NULL)
#if MagickLibVersion >= 0x650
    PHP_FE(IMagickStreamCommand,    NULL)
#endif
    PHP_FE(IMagickImportCommand,    NULL)
    PHP_FE(IMagickDisplayCommand,   NULL)
    PHP_FE(IMagickAnimateCommand,   NULL)
    PHP_FE(IMagickMontageCommand,   NULL)
    PHP_FE(IMagickCommandVersion,   NULL)
    { NULL, NULL, NULL }  /* Must be the last line in imagick_cmd_functions[] */
};
/* }}} */

/* {{{ imagick_cmd_module_entry
 */
zend_module_entry imagick_cmd_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "imagick_cmd",
    imagick_cmd_functions,
    PHP_MINIT(imagick_cmd),
    PHP_MSHUTDOWN(imagick_cmd),
    NULL,
    NULL,
    PHP_MINFO(imagick_cmd),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_IMAGICK_CMD_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_IMAGICK_CMD
ZEND_GET_MODULE(imagick_cmd)
#endif

static void
_dispatch_magick_cmd(INTERNAL_FUNCTION_PARAMETERS, MagickCommand cmd, char *command)
{
    zval *arr, **entry;
    HashPosition pos;

    int argc, i, k = -1;
    char *argv[32], **argvp;
    zval tval[32], *tvalp;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &arr) == FAILURE) {
        return;
    }

    /* Build command args
     */
    argc = zend_hash_num_elements(Z_ARRVAL_P(arr)) + 1;

    if (argc > 32) {
        argvp = (char **) ecalloc(argc, sizeof(char *));
        tvalp = (zval *) ecalloc(argc, sizeof(zval));
    } else {
        argvp = argv;
        tvalp = tval;
    }

    argvp[0] = command;

    for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(arr), &pos), i = 1;
        zend_hash_get_current_data_ex(Z_ARRVAL_P(arr), (void **)&entry, &pos) == SUCCESS;
        zend_hash_move_forward_ex(Z_ARRVAL_P(arr), &pos), ++i
    ) {
        if (Z_TYPE_PP(entry) == IS_ARRAY || Z_TYPE_PP(entry) == IS_OBJECT) {
            continue;
        }

        if (Z_TYPE_PP(entry) == IS_STRING) {
            argvp[i] = Z_STRVAL_PP(entry);
        } else {
            tvalp[++k] = **entry;
            zval_copy_ctor(&tvalp[k]);
            convert_to_string(&tvalp[k]);
            argvp[i] = Z_STRVAL(tvalp[k]);
        }
    }

    /* Imagick processes
     */
    {
        MagickBooleanType result;
        ImageInfo *ii;
        ExceptionInfo *ei;
        char *metadata = NULL;

        ii = AcquireImageInfo();
        ei = AcquireExceptionInfo();

        result = (cmd)(ii, argc, argvp, &metadata, ei);

        if (ei->reason) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s", ei->reason);
        }

        if (metadata) {
            RETVAL_STRING(metadata, 1);
        } else {
            RETVAL_BOOL(result);
        }

        if (metadata) {
            DestroyString(metadata);
        }
        DestroyImageInfo(ii);
        DestroyExceptionInfo(ei);
    }

    /* Release resources
     */
    while (k != -1) {
        zval_dtor(&tvalp[k--]);
    }

    if (argvp != argv) {
        efree(argvp);
        efree(tvalp);
    }
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(imagick_cmd)
{
    MagickCoreGenesis(NULL, MagickFalse);
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(imagick_cmd)
{
    MagickCoreTerminus();
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(imagick_cmd)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "IMagickCommand support", "enabled");
    php_info_print_table_row(2, "extension version", PHP_IMAGICK_CMD_VERSION);
    php_info_print_table_end();
}
/* }}} */

/* {{{ proto mixed IMagickMogrifyCommand(array args)
 */
PHP_FUNCTION(IMagickMogrifyCommand)
{
    _dispatch_magick_cmd(INTERNAL_FUNCTION_PARAM_PASSTHRU, MogrifyImageCommand, "mogrify");
}
/* }}} */

/* {{{ proto mixed IMagickConvertCommand(array args)
 */
PHP_FUNCTION(IMagickConvertCommand)
{
    _dispatch_magick_cmd(INTERNAL_FUNCTION_PARAM_PASSTHRU, ConvertImageCommand, "convert");
}
/* }}} */

/* {{{ proto mixed IMagickCompositeCommand(array args)
 */
PHP_FUNCTION(IMagickCompositeCommand)
{
    _dispatch_magick_cmd(INTERNAL_FUNCTION_PARAM_PASSTHRU, CompositeImageCommand, "composite");
}
/* }}} */

/* {{{ proto mixed IMagickIdentifyCommand(array args)
 */
PHP_FUNCTION(IMagickIdentifyCommand)
{
    _dispatch_magick_cmd(INTERNAL_FUNCTION_PARAM_PASSTHRU, IdentifyImageCommand, "identify");
}
/* }}} */

/* {{{ proto mixed IMagickCompareCommand(array args)
 */
PHP_FUNCTION(IMagickCompareCommand)
{
    _dispatch_magick_cmd(INTERNAL_FUNCTION_PARAM_PASSTHRU, CompareImageCommand, "compare");
}
/* }}} */

/* {{{ proto mixed IMagickConjureCommand(array args)
 */
PHP_FUNCTION(IMagickConjureCommand)
{
    _dispatch_magick_cmd(INTERNAL_FUNCTION_PARAM_PASSTHRU, ConjureImageCommand, "conjure");
}
/* }}} */

#if MagickLibVersion >= 0x650
/* {{{ proto mixed IMagickStreamCommand(array args)
 */
PHP_FUNCTION(IMagickStreamCommand)
{
    _dispatch_magick_cmd(INTERNAL_FUNCTION_PARAM_PASSTHRU, StreamImageCommand, "stream");
}
/* }}} */
#endif

/* {{{ proto mixed IMagickImportCommand(array args)
 */
PHP_FUNCTION(IMagickImportCommand)
{
    _dispatch_magick_cmd(INTERNAL_FUNCTION_PARAM_PASSTHRU, ImportImageCommand, "import");
}
/* }}} */

/* {{{ proto mixed IMagickDisplayCommand(array args)
 */
PHP_FUNCTION(IMagickDisplayCommand)
{
    _dispatch_magick_cmd(INTERNAL_FUNCTION_PARAM_PASSTHRU, DisplayImageCommand, "display");
}
/* }}} */

/* {{{ proto mixed IMagickAnimateCommand(array args)
 */
PHP_FUNCTION(IMagickAnimateCommand)
{
    _dispatch_magick_cmd(INTERNAL_FUNCTION_PARAM_PASSTHRU, AnimateImageCommand, "animate");
}
/* }}} */

/* {{{ proto mixed IMagickMontageCommand(array args)
 */
PHP_FUNCTION(IMagickMontageCommand)
{
    _dispatch_magick_cmd(INTERNAL_FUNCTION_PARAM_PASSTHRU, MontageImageCommand, "montage");
}
/* }}} */

/* {{{ proto string IMagickCommandVersion()
 */
PHP_FUNCTION(IMagickCommandVersion)
{
    RETURN_STRING(PHP_IMAGICK_CMD_VERSION, 1);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
