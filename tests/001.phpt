--TEST--
Check for strict presence
--SKIPIF--
<?php if (!extension_loaded("strict")) print "skip"; ?>
--FILE--
<?php 
echo "strict extension is available";
?>
--EXPECT--
strict extension is available
