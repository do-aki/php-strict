--TEST--
foreach statement
--INI--
strict.use = 1
--FILE--
<?php

foreach(array(1) as $k => $v) {
  $x = $k = $v;
}

echo $x;
--EXPECTF--
Warning: Use of unassigned local variable $x. in %s on line 7
