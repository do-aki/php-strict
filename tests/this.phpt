--TEST--
$this is already defined
--INI--
strict.use = 1
--FILE--
<?php

class A {
  
  function x() {
    var_dump($this);
  }
}
--EXPECT--

