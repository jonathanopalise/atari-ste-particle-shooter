<?php

$angleCount = 256;
$radiansIncrement = (M_PI * 2) / $angleCount;

if ($argc < 2) {
    echo("usage: generate_sin_cos [outputFile]");
    exit(1);
}

$lines = [
    '#include "../sin_cos.h"',
    '',
    'int32_t sin_table[] = {',
];

$currentRadians = 0;
for ($index = 0; $index < $angleCount; $index++) {
    $sinValue = intval(round(sin($currentRadians) * 65535));
    $line = '    ' . $sinValue;
    if ($index != ($angleCount - 1)) {
        $line .= ',';
    }
    $lines[] = $line;
    $currentRadians += $radiansIncrement;
}

$lines = array_merge(
    $lines,
    [
        '};',
        '',
        'int32_t cos_table[] = {',
    ]
);

$currentRadians = 0;
for ($index = 0; $index < $angleCount; $index++) {
    $cosValue = intval(round(cos($currentRadians) * 65535));
    $line = '    ' . $cosValue;
    if ($index != ($angleCount - 1)) {
        $line .= ',';
    }
    $lines[] = $line;
    $currentRadians += $radiansIncrement;
}

$lines = array_merge(
    $lines,
    [
        '};'
    ]
);

$contents = implode("\n", $lines);

$result = file_put_contents($argv[1], $contents);
if ($result === false) {
    echo("unable to write sin_cos file");
    exit(1);
}


