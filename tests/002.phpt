--TEST--
Check for strict presence
--INI--
strict.use = 1
--FILE--
<?php
  $a=$b;
?>
--EXPECTF--
Warning: Use of unassigned local variable $b. in %s on line 2

