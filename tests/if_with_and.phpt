--TEST--
if statement with and
--INI--
strict.use = 1
--FILE--
<?php
  if (($a=1) && ($b=2)) {
    echo $a, $b; // OK
  } else {
    echo $a, $b; // a:OK b:NG
  }
  echo $a, $b; // a:OK b:NG
--EXPECTF--
Warning: Use of unassigned local variable $b. in %s on line 5

Warning: Use of unassigned local variable $b. in %s on line 7
