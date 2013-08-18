--TEST--
while statement
--INI--
strict.use = 1
--FILE--
<?php

while(0) {
  $a = 1;
}

echo $a;
--EXPECTF--
Warning: Use of unassigned local variable $a. in %s on line 7
