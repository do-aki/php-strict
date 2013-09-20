--TEST--
try statement
--INI--
strict.use = 1
--FILE--
<?php
  try {
    $a = 1;
  } catch (Exception $e) {
    $b = 2;
  }

echo $a, $b;
--EXPECTF--

Warning: Use of unassigned local variable $b. in %s on line 8