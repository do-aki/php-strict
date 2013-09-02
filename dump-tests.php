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

$dumpfile = $target;
$unlink = false;
if (preg_match('/\.phpt\z/', $target)) {
    $dumpfile = extract_php_file($target);
    $unlink = true;
}

$php =  '~/.phpenv/versions/5.3.27/bin/php';
$php .= ' -dextension=/home/do_aki/work/php-ext-strict/modules/strict.so';
$php .= ' -dstrict.dump=on -dstrict.verbose=on';
system("{$php} {$dumpfile}");

if ($unlink) {
    //echo "unlink {$dumpfile}\n";
    unlink($dumpfile);
}

function extract_php_file($phpt) {
    $in = fopen($phpt, 'r');
    $text = array();
    $section = 'TEST';
    $text[$section] = '';
    while($line = fgets($in)) {
        if (preg_match('/\A--([_A-Z]+)--/', $line, $m)) {
            $section = $m[1];
            $text[$section] = '';
            continue;
        }

        $text[$section] .= $line;
    }
    fclose($in);

    if (!isset($section['FILE'])) {
        die("no --FILE-- section\n");
    }

    $tmp = tempnam(sys_get_temp_dir(), 'php');
    $out = fopen($tmp, 'w');
    fwrite($out, $text['FILE']);
    fclose($out);
    
    return $tmp;
};
