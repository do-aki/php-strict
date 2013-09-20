--TEST--
nested if statement
--INI--
strict.use = 1
--FILE--
<?php
  if (1) {
    $a = 1;
    if (1) {
      $b = 1;
    } else {
      echo $a; // ok
    }
  } else {
	$c = 1;
    if (1) {
      $c = 1;
    } else {
      $c = 1;
    }
	$c = 1;
  }
  return $a + $b + $c; // ng
--EXPECTF--
Warning: Use of unassigned local variable $a. in %s on line 18

Warning: Use of unassigned local variable $b. in %s on line 18

Warning: Use of unassigned local variable $c. in %s on line 18