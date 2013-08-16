ImagickCommand PHP extension
============================

Top Tips
--------

ImageMagick default build with __OpenMP__ support, this will make bugs with PHP environment. You need turn off this support when building ImageMagick with:

```bash
# Build ImageMagick
./configure --disable-openmp
```

Documents
---------

>>> ImagickCommand Example

```php
IMagickConvertCommand(array(
  '-resize', '90%',
  'source.png',
  'target.gif'
));
```

>>> ImagickCommand Functions

```php
// IMagickCommands
mixed IMagickMogrifyCommand(array args);
mixed IMagickConvertCommand(array args);
mixed IMagickCompositeCommand(array args);
mixed IMagickIdentifyCommand(array args);
mixed IMagickCompareCommand(array args);
mixed IMagickConjureCommand(array args);
mixed IMagickStreamCommand(array args);
mixed IMagickImportCommand(array args);
mixed IMagickDisplayCommand(array args);
mixed IMagickAnimateCommand(array args);
mixed IMagickMontageCommand(array args);

// Current module version
string IMagickCommandVersion();
```

License
-------

>>> ImagickCommand is Copyright 2013 Hupu Co., Ltd.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
    
       http://www.apache.org/licenses/LICENSE-2.0
    
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

Credit
------

* Thanks to [Hupu.com](http://www.hupu.com) for sponsor resources to make
  this extension happen.
