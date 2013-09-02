--TEST--
Check for strict presence
--INI--
strict.use = 1
--FILE--
<?php 

function x(&$a) {
  return $x + $a;
}

$x = 1;
x($x); // ZEND_SEND_REF
y($x); // ZEND_SEND_VAL
?>
--EXPECTF--
Warning: Use of unassigned local variable $x. in %s on line 4

