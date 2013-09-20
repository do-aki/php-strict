--TEST--
switch statement
--INI--
strict.use = 1
--FILE--
<?php

switch($a=1) {
  case $a:
    $b = 2;
  case $c: // NG
    echo $b; // NG
  default:
    $c = 3;
}

echo $a, $b, $c; // a:OK b:NG c:NG
--EXPECTF--
Warning: Use of unassigned local variable $c. in %s on line 6

Warning: Use of unassigned local variable $b. in %s on line 7

Warning: Use of unassigned local variable $b. in %s on line 12

Warning: Use of unassigned local variable $c. in %s on line 12
