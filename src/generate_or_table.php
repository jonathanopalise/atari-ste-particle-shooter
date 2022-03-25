<?php

echo("#include \"../or_table.h\"\n");
echo("uint32_t or_table[] = {\n");
$andValue=0x80808080;
for ($bitPosition=0; $bitPosition<8; $bitPosition++) {
    for ($colour=0; $colour<16; $colour++) {
        if ($colour>0) {
            $tableValue=0;
            if ($colour&8) {
                $tableValue|=0x00000080;
            }
            if ($colour&4) {
                $tableValue|=0x00008000;
            }
            if ($colour&2) {
                $tableValue|=0x00800000;
            }
            if ($colour&1) {
                $tableValue|=0x80000000;
            }
            $tableValue>>=$bitPosition;
        } else {
            $tableValue=~$andValue;
        }
        printf("    0x%08x,\n",($tableValue&0xffffffff));
    }
    $andValue>>=1;
}
echo("};\n");

