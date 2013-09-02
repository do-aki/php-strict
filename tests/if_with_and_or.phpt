--TEST--
if statement with and or
--INI--
strict.use = 1
--FILE--
<?php
  if (($a=2) && ($b=1) || ($c=0)) {
    echo $a, $b, $c; // a: OK b:NG c:NG
  } else {
    echo $a, $b, $c; // a:OK b:NG c:NG
  }
  echo $a, $b, $c; // a:OK b:NG c:NG
--EXPECTF--
Warning: Use of unassigned local variable $b. in %s on line 3

Warning: Use of unassigned local variable $c. in %s on line 3

Warning: Use of unassigned local variable $b. in %s on line 5

Warning: Use of unassigned local variable $c. in %s on line 5

Warning: Use of unassigned local variable $b. in %s on line 7

Warning: Use of unassigned local variable $c. in %s on line 7
