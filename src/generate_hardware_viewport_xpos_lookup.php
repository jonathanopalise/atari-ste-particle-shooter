<?php

if ($argc < 2) {
    echo("Usage: php generate_hardware_viewport_xpos_lookup.php [outputFile]\n");
    exit(1);
}

$outputFilename = $argv[1];

ob_start();
echo("#include \"inttypes.h\"\n");
echo("#include \"../hardware_viewport_xpos_lookup.h\"\n");
echo("uint32_t hardware_viewport_xpos_lookup[] = {\n");
for ($value = 0; $index < 960; $index++) {
    $value = $index >> 1 & 0xfffffff8;
    if ($index & 8) {
        $value++;
    }
    printf("    %d,\n",$value);
}
echo("};\n");
$output = ob_get_clean();

$result = file_put_contents($outputFilename, $output);
if ($result === false) {
    echo("Unable to write hardware viewport xpos lookup data");
    exit(1);
}

