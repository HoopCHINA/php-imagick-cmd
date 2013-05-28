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

#ifndef PHP_IMAGICK_CMD_H__
#define PHP_IMAGICK_CMD_H__

extern zend_module_entry imagick_cmd_module_entry;
#define phpext_imagick_cmd_ptr &imagick_cmd_module_entry

#ifdef PHP_WIN32
#define PHP_IMAGICK_CMD_API __declspec(dllexport)
#else
#define PHP_IMAGICK_CMD_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/**
  @method:
  mixed IMagickConvertCommand(array args);

  @example:
  IMagickConvertCommand(array(
    'source.png',
    '-resize', '90%',
    'target.gif'
  ));
 */

PHP_MINIT_FUNCTION(imagick_cmd);
PHP_MSHUTDOWN_FUNCTION(imagick_cmd);
PHP_MINFO_FUNCTION(imagick_cmd);

PHP_FUNCTION(IMagickMogrifyCommand);
PHP_FUNCTION(IMagickConvertCommand);
PHP_FUNCTION(IMagickCompositeCommand);
PHP_FUNCTION(IMagickIdentifyCommand);
PHP_FUNCTION(IMagickCompareCommand);
PHP_FUNCTION(IMagickConjureCommand);
PHP_FUNCTION(IMagickStreamCommand);
PHP_FUNCTION(IMagickImportCommand);
PHP_FUNCTION(IMagickDisplayCommand);
PHP_FUNCTION(IMagickAnimateCommand);
PHP_FUNCTION(IMagickMontageCommand);
PHP_FUNCTION(IMagickCommandVersion);

#define PHP_IMAGICK_CMD_VERSION "0.1.0"

#endif  /* PHP_IMAGICK_CMD_H__ */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
