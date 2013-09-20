--TEST--
if statement
--INI--
strict.use = 1
--FILE--
<?php
  if (1) {
    $a = 1;
  }
  echo $a;
--EXPECTF--
Warning: Use of unassigned local variable $a. in %s on line 5
