--TEST--
$argv and $argc is already defined
--INI--
strict.use = 1
--FILE--
<?php
  $a = $argv;
  $a = $argc;
--EXPECT--

