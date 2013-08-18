--TEST--
Check for strict presence
--INI--
strict.use = 1
--FILE--
<?php 

function x($a) {
  return $x + $a;
}

x();
?>
--EXPECTF--
Warning: Use of unassigned local variable $x. in %s on line 4

