<?php

require_once 'library.php';

if ($argc < 3) {
    echo("Usage: php generate_sprite_data.php [inputFile] [outputFile]\n");
    exit(1);
}

$spriteDefinitions = [
    // player
    [
        'x' => 0,
        'y' => 200,
    ],
    // mine
    [
        'x' => 16,
        'y' => 200,
    ],
    // explosion frame 1
    [
        'x' => 0,
        'y' => 216,
    ],
    // explosion frame 2
    [
        'x' => 16,
        'y' => 216,
    ],
    // explosion frame 3
    [
        'x' => 32,
        'y' => 216,
    ],
    // explosion frame 4
    [
        'x' => 48,
        'y' => 216,
    ],
    // explosion frame 5
    [
        'x' => 64,
        'y' => 216,
    ],
    // explosion frame 6
    [
        'x' => 80,
        'y' => 216,
    ],
    // explosion frame 7
    [
        'x' => 96,
        'y' => 216,
    ],
    // explosion frame 8
    [
        'x' => 112,
        'y' => 216,
    ],
    // explosion frame 9
    [
        'x' => 128,
        'y' => 216,
    ],
    // explosion frame 10
    [
        'x' => 144,
        'y' => 216,
    ],
    // explosion frame 11
    [
        'x' => 160,
        'y' => 216,
    ],
    // explosion frame 12
    [
        'x' => 176,
        'y' => 216,
    ],
    // explosion frame 13
    [
        'x' => 192,
        'y' => 216,
    ],
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
echo("uint8_t sprite_data[][194] = {\n");

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

    $chunkedBytes = array_chunk($bytes, 10);

    $maskBytes = [0xff, 0xff];
    $colourBytes = [];
    foreach ($chunkedBytes as $chunk) {
        $maskBytes = array_merge(
            $maskBytes,
            [
                $chunk[0],
                $chunk[1],
                0xff,
                0xff,
            ]
        );

        $colourBytes = array_merge(
            $colourBytes,
            [
                $chunk[2],
                $chunk[3],
                $chunk[4],
                $chunk[5],
                $chunk[6],
                $chunk[7],
                $chunk[8],
                $chunk[9],
            ]
        );

        $bytes = array_merge(
            $maskBytes,
            $colourBytes
        );
    }

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

