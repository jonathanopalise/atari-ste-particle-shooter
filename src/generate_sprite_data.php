<?php

require_once 'library.php';

if ($argc < 3) {
    echo("Usage: php generate_sprite_data.php [inputFile] [outputFile]\n");
    exit(1);
}

$spriteDefinitions = [
    [
        'x' => 0,
        'y' => 200,
    ],
    [
        'x' => 16,
        'y' => 200,
    ]
];

$inputFilename = $argv[1];
$outputFilename = $argv[2];

if (!file_exists($inputFilename)) {
    echo("Input filename does not exist\n");
    exit(1);
}

$spriteSheetBitmap = IndexedBitmap::loadGif($inputFilename);
ob_start();
echo("#include \"../sprite_data.h\"\n");
echo("uint8_t sprite_data[][160] = {\n");

foreach ($spriteDefinitions as $key => $spriteDefinition) {
    $indexedBitmap = $spriteSheetBitmap->extractRegionToIndexedBitmap(
        $spriteDefinition['x'],
        $spriteDefinition['y'],
        16,
        16,
        0,
        0
    );
    $unmaskedSprite = SpriteConvertor::createMaskedSprite($indexedBitmap);
    $planarData = $unmaskedSprite->exportToPlanarData();
    $bytes = $planarData->getBytes();

    echo("    {".implode(',',$bytes)."}");
    if ($key != array_key_last($spriteDefinitions)) {
        echo(",");
    }
    echo("\n");
}

echo("};\n");
$output = ob_get_clean();

$result = file_put_contents($outputFilename, $output);
if ($result === false) {
    echo("Unable to write sprite data");
    exit(1);
}

