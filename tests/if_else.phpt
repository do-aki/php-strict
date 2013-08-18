--TEST--
if_else statement
--INI--
strict.use = 1
--FILE--
<?php
if (1) {
  $a = 1;
} else {
  $b = 2;
}

echo $a + $b;
--EXPECTF--
Warning: Use of unassigned local variable $a. in %s on line 8

Warning: Use of unassigned local variable $b. in %s on line 8
