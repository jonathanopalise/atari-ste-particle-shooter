<?php

if ($argc < 3) {
    echo("usage: generate_palette [inputFile] [outputFile]");
    exit(1);
}

function generateSteNibble($value)
{
    $amigaNibble = ($value >> 4);
    return (($amigaNibble >> 1) | (($amigaNibble & 1) << 3));
}

$image = imagecreatefromgif($argv[1]);
if ($image === false) {
    echo("unable to open palette\n");
    exit(1);
}


$stePalette = array_fill(0, 16, 0);
$totalColours = imagecolorstotal($image);
for ($index = 0; $index < $totalColours; $index++) {
    $colours = imagecolorsforindex($image, $index);
    var_dump($colours);

    $red = $colours['red'];
    $green = $colours['green'];
    $blue = $colours['blue'];

    $steRed = generateSteNibble($red);
    $steGreen = generateSteNibble($green);
    $steBlue = generateSteNibble($blue);

    $stePalette[$index] = ($steRed << 8) | ($steGreen << 4) | ($steBlue);
}

$identifier = '_palette';

$lines = [
    '    public ' . $identifier,
    '',
    $identifier . ':'
];

foreach ($stePalette as $entry) {
    $lines[] = '    dc.w $' . dechex($entry);
}

$contents = implode("\n", $lines);

$result = file_put_contents($argv[2], $contents);
if ($result === false) {
    echo("unable to write palette file");
    exit(1);
}

