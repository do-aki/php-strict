--TEST--
Check for strict presence
--INI--
strict.use = 1
--FILE--
<?php 

class Hoge {
  
  function x() {
    if (1) {
      $a = 1;
      if (1) {
        $b = 1;
      } else {
        echo $a;
      }
    }
    return $a + $b;
  }
}

--EXPECTF--
Warning: Use of unassigned local variable $a. in %s on line 14

Warning: Use of unassigned local variable $b. in %s on line 14
