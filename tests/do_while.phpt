
--TEST--
do-while statement
--INI--
strict.use = 1
strict.execute = 0
--FILE--
<?php
  do {
    $a = 1;
  } while (0);
  echo $a;
--EXPECTF--
