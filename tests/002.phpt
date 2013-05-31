--TEST--
IMagickConvertCommand() and convert PNG to JPG
--SKIPIF--
<?php if (!function_exists("IMagickConvertCommand")) print "skip"; ?>
--FILE--
<?php
$src = dirname(__FILE__)."/test.png";
$dst = dirname(__FILE__)."/test.jpg";
@unlink($dst);

echo IMagickConvertCommand(array('-resize', '50%', $src, $dst)), PHP_EOL;
@unlink($dst);

echo "Done\n";
?>
--EXPECTF--
100,100,PNG
Done
