--TEST--
IMagickConvertCommand() and empty/missing file
--SKIPIF--
<?php if (!function_exists("IMagickConvertCommand")) print "skip"; ?>
--FILE--
<?php
$src = dirname(__FILE__)."/src.png";
$dst = dirname(__FILE__)."/dst.jpg";
@unlink($dst);

echo IMagickConvertCommand(array('-resize', '50%', $src, $dst)), PHP_EOL;
@unlink($dst);

echo "Done\n";
?>
--EXPECTF--
Warning: IMagickConvertCommand(): no images defined `%sdst.jpg' @ %s/%d in %s001.php on line %d

Done
