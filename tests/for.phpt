--TEST--
for statement
--INI--
strict.use = 1
--FILE--
<?php

for($i=0;$i<0;++$i) {
  $a = 1;
}

echo $a;
--EXPECTF--
Warning: Use of unassigned local variable $a. in %s on line 7
