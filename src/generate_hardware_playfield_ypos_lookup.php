<?php

if ($argc < 2) {
    echo("Usage: php generate_hardware_playfield_ypos_lookup.php [outputFile]\n");
    exit(1);
}

$outputFilename = $argv[1];

ob_start();
echo("#include \"inttypes.h\"\n");
echo("#include \"../hardware_playfield_ypos_lookup.h\"\n");
echo("uint32_t hardware_playfield_ypos_lookup[] = {\n");
for ($index = 0; $index < 200; $index++) {
    printf("    %d,\n",$index*480);
}
echo("};\n");
$output = ob_get_clean();

$result = file_put_contents($outputFilename, $output);
if ($result === false) {
    echo("Unable to write hardware playfield ypos lookup data");
    exit(1);
}

