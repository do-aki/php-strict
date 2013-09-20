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
    }
    return $a + $b; // ng
--EXPECTF--
Warning: Use of unassigned local variable $a. in %s on line 10

Warning: Use of unassigned local variable $b. in %s on line 10
