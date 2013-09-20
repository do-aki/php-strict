--TEST--
if statement
--INI--
strict.use = 1
--FILE--
<?php
  if (1) {
    $a = 1;
  } else {
    $a = 2;
  }
  $b = $a;
--EXPECT--

