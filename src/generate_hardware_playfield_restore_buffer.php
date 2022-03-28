<?php

require_once 'library.php';

if ($argc < 3) {
    echo("Usage: php generate_hardware_playfield_restore_buffer.php [inputFile] [outputFile]\n");
    exit(1);
}

$inputFilename = $argv[1];
$outputFilename = $argv[2];

if (!file_exists($inputFilename)) {
    echo("Input filename does not exist\n");
    exit(1);
}

$indexedBitmap = IndexedBitmap::loadGif($inputFilename);
$indexedBitmap = $indexedBitmap->extractRegionToIndexedBitmap(0,0,960,200,0,0);
$unmaskedSprite = SpriteConvertor::createUnmaskedSprite($indexedBitmap);
$planarData = $unmaskedSprite->exportToPlanarData();
$bytes = $planarData->getBytes();

ob_start();
echo("#include \"../hardware_playfield_restore_buffer.h\"\n");
echo("uint8_t hardware_playfield_restore_buffer[] = {\n");
foreach ($bytes as $byte) {
    printf("    %d,\n",$byte);
}
echo("};\n");
$output = ob_get_clean();

$result = file_put_contents($outputFilename, $output);
if ($result === false) {
    echo("Unable to write hardware playfield restore buffer data");
    exit(1);
}

