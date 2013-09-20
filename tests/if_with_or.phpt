--TEST--
if statement with or
--INI--
strict.use = 1
--FILE--
<?php
  if (0 || ($a=0)) {
    echo $a;
  } else {
    echo $a;
  }
  echo $a;
--EXPECTF--
Warning: Use of unassigned local variable $a. in %s on line 3

Warning: Use of unassigned local variable $a. in %s on line 5

Warning: Use of unassigned local variable $a. in %s on line 7
