--TEST--
switch statement
--INI--
strict.use = 1
--FILE--
<?php
switch($argc) {
  case 1:
    $a = 1;
    break;
  case 2:
    $b = 2;
  default:
    $c = 3;
}
$x = $a + $b + $c;
--EXPECTF--
Warning: Use of unassigned local variable $a. in %s on line 11

Warning: Use of unassigned local variable $b. in %s on line 11

Warning: Use of unassigned local variable $c. in %s on line 11

