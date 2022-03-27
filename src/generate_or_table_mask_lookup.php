<?php

if ($argc < 2) {
    echo("Usage: php generate_or_table_mask_lookup.php [outputFile]\n");
    exit(1);
}

$outputFilename = $argv[1];

ob_start();
echo("#include \"inttypes.h\"\n");
echo("#include \"../or_table_mask_lookup.h\"\n");
echo("uint16_t or_table_mask_lookup[] = {\n");
for ($value = 0; $index < 960; $index++) {
    $value = ($index & 7) << 4;
    printf("    %d,\n",$value);
}
echo("};\n");
$output = ob_get_clean();

$result = file_put_contents($outputFilename, $output);
if ($result === false) {
    echo("Unable to write or table mask lookup data");
    exit(1);
}

