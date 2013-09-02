--TEST--
no strict function 
--INI--
strict.use = 1
strict.execute = 0
--FILE--
<?php
/**
 * @no strict
 */
function f() {
  echo $a;
}


--EXPECTF--
