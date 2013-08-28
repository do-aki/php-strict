<?php
  if (($a=1) && ($b=2)) {
    echo $a, $b; // OK
  } else {
    echo $a, $b; // a:OK b:NG
  }
  echo $a, $b; // a:OK b:NG
