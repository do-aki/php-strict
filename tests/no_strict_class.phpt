--TEST--
no strict class
--INI--
strict.use = 1
strict.execute = 0
--FILE--
<?php
/**
 * @no strict
 */
class A {
  public function f() {
    echo $a;
  }
}

--EXPECTF--
