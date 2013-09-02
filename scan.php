#!/usr/bin/env php
<?php

ini_set('output_buffering', false);
ini_set('implicit_flush', true);
ob_implicit_flush(true);

array_shift($argv);
$target = array_shift($argv);
if (!$target || !file_exists($target)) {
    die("no file\n");
}

if (is_file($target)) {
  scan($target);
} elseif (is_dir($target)) {
  scan_dir($target);
}

function scan_dir($dir) {
  $d = opendir($dir);
  while(($f = readdir($d)) !== false) {
    if ($f === '.' || $f === '..') {
      continue;
    }

    $p = "{$dir}/{$f}";
    if (is_dir($p)) {
      scan_dir($p);
    } elseif (preg_match('/\.php\z/', $f)) {
      print "scan: $p\n";
      scan($p);
    }
  }
  closedir($d);

}

function scan($file) {
  $php = '~/.phpenv/versions/5.3.27/bin/php';
  $php .= ' -dextension=/home/do_aki/work/php-ext-strict/modules/strict.so';
  $php .= ' -dstrict.use=on -dstrict.dump=off -dstrict.verbose=off -dstrict.execute=off';
  system("{$php} {$file}");
}


