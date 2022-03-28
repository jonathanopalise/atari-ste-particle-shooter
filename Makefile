CC = /home/jonathan/brown/bin/m68k-atarisuperbrowner-elf-gcc-11.2.0
CFLAGS = -D__ATARI__ -D__M68000__ -DELF_CONFIG_STACK=1024 -fleading-underscore -fomit-frame-pointer -O3 -m68000 -Wl,--traditional-format -Wall
VASM = vasmm68k_mot
VASM_OPTS = -no-opt
VLINK = vlink
PHP = php

OBJECT_FILES = src/shooter.o src/movep.o src/particle_render_erase_particles_inner.o src/particle_render_draw_particles_inner.o src/game_loop.o src/hardware_playfield.o src/generated/hardware_playfield_restore_buffer.o src/generated/hardware_playfield_ypos_lookup.o src/generated/hardware_viewport_xpos_lookup.o src/generated/or_table_mask_lookup.o src/hardware_viewport.o src/logical_viewport.o src/player_behaviour.o src/random.o src/particle_system.o src/particle_render.o src/vbl_handler.o src/generated/or_table.o src/generated/palette.o src/initialise.o

bin/shooter.prg: $(OBJECT_FILES)
	$(CC) -o src/shooter.elf libcxx/brownboot.o libcxx/browncrti.o libcxx/browncrtn.o libcxx/browncrt++.o libcxx/zerolibc.o libcxx/zerocrtfini.o $(OBJECT_FILES) -O3 -Wl,--emit-relocs -Wl,-e_start -Ttext=0 -nostartfiles -m68000 -Ofast -fomit-frame-pointer -D__ATARI__ -D__M68000__ -DELF_CONFIG_STACK=1024 -fstrict-aliasing -fcaller-saves -flto -ffunction-sections -fdata-sections -fleading-underscore
	./brown.out -i src/shooter.elf -o bin/shooter.prg
	chmod +x bin/shooter.prg

src/shooter.o: src/shooter.c $(OBJECT_FILES)
	$(CC) $(CFLAGS) -c src/shooter.c -o src/shooter.o

src/movep.o: src/movep.s src/movep.h
	$(VASM) $(VASM_OPTS) src/movep.s -Felf -o src/movep.o

src/particle_render_erase_particles_inner.o: src/particle_render_erase_particles_inner.s
	$(VASM) $(VASM_OPTS) src/particle_render_erase_particles_inner.s -Felf -o src/particle_render_erase_particles_inner.o

src/particle_render_draw_particles_inner.o: src/particle_render_draw_particles_inner.s
	$(VASM) $(VASM_OPTS) src/particle_render_draw_particles_inner.s -Felf -o src/particle_render_draw_particles_inner.o

src/game_loop.o: src/game_loop.c src/game_loop.h src/hardware_playfield.h src/hardware_viewport.h src/initialise.h src/particle_system.h src/vbl_handler.h
	$(CC) $(CFLAGS) -c src/game_loop.c -o src/game_loop.o

src/hardware_playfield.o: src/hardware_playfield.c src/hardware_playfield.h src/particle_common.h src/logical_playfield.h src/palette.h
	$(CC) $(CFLAGS) -c src/hardware_playfield.c -o src/hardware_playfield.o

src/hardware_viewport.o: src/hardware_viewport.c src/hardware_viewport.h src/logical_viewport.h src/hardware_playfield.h
	$(CC) $(CFLAGS) -c src/hardware_viewport.c -o src/hardware_viewport.o

src/logical_viewport.o: src/logical_viewport.c src/logical_viewport.h
	$(CC) $(CFLAGS) -c src/logical_viewport.c -o src/logical_viewport.o

src/player_behaviour.o: src/player_behaviour.c src/player_behaviour.h src/particle_system.h src/initialise.h
	$(CC) $(CFLAGS) -c src/player_behaviour.c -o src/player_behaviour.o

src/random.o: src/random.c src/random.h src/random.h src/random.h
	$(CC) $(CFLAGS) -c src/random.c -o src/random.o

src/particle_system.o: src/particle_system.c src/particle_system.h src/particle_common.h src/hardware_playfield.h
	$(CC) $(CFLAGS) -c src/particle_system.c -o src/particle_system.o

src/particle_render.o: src/particle_render.c src/hardware.h src/hardware_playfield.h src/hardware_viewport.h src/logical_viewport.h src/movep.h src/or_table.h src/particle_common.h src/particle_system.h src/viewport.h src/movep.h src/particle_render_erase_particles_inner.h src/particle_render_draw_particles_inner.h
	$(CC) $(CFLAGS) -c src/particle_render.c -o src/particle_render.o

src/vbl_handler.o: src/vbl_handler.c src/vbl_handler.h src/hardware_playfield.h src/hardware_viewport.h
	$(CC) $(CFLAGS) -c src/vbl_handler.c -o src/vbl_handler.o

src/generated/or_table.o: src/generated/or_table.c src/or_table.h
	$(CC) $(CFLAGS) -c src/generated/or_table.c -o src/generated/or_table.o

src/generated/hardware_playfield_restore_buffer.c: assets/clouds.gif src/generate_hardware_playfield_restore_buffer.php
	$(PHP) src/generate_hardware_playfield_restore_buffer.php assets/clouds.gif src/generated/hardware_playfield_restore_buffer.c

src/generated/hardware_playfield_restore_buffer.o: src/generated/hardware_playfield_restore_buffer.c src/hardware_playfield_restore_buffer.h
	$(CC) $(CFLAGS) -c src/generated/hardware_playfield_restore_buffer.c -o src/generated/hardware_playfield_restore_buffer.o

src/generated/hardware_playfield_ypos_lookup.c: src/generate_hardware_playfield_ypos_lookup.php
	$(PHP) src/generate_hardware_playfield_ypos_lookup.php src/generated/hardware_playfield_ypos_lookup.c

src/generated/hardware_playfield_ypos_lookup.o: src/generated/hardware_playfield_ypos_lookup.c src/hardware_playfield_ypos_lookup.h
	$(CC) $(CFLAGS) -c src/generated/hardware_playfield_ypos_lookup.c -o src/generated/hardware_playfield_ypos_lookup.o

src/generated/hardware_viewport_xpos_lookup.c: src/generate_hardware_viewport_xpos_lookup.php
	$(PHP) src/generate_hardware_viewport_xpos_lookup.php src/generated/hardware_viewport_xpos_lookup.c

src/generated/hardware_viewport_xpos_lookup.o: src/generated/hardware_viewport_xpos_lookup.c src/hardware_viewport_xpos_lookup.h
	$(CC) $(CFLAGS) -c src/generated/hardware_viewport_xpos_lookup.c -o src/generated/hardware_viewport_xpos_lookup.o

src/generated/or_table_mask_lookup.c: src/generate_or_table_mask_lookup.php
	$(PHP) src/generate_or_table_mask_lookup.php src/generated/or_table_mask_lookup.c

src/generated/or_table_mask_lookup.o: src/generated/or_table_mask_lookup.c src/or_table_mask_lookup.h
	$(CC) $(CFLAGS) -c src/generated/or_table_mask_lookup.c -o src/generated/or_table_mask_lookup.o

src/generated/or_table.c: src/generate_or_table.php
	$(PHP) src/generate_or_table.php > src/generated/or_table.c

src/generated/palette.o: src/generated/palette.s
	$(VASM) $(VASM_OPTS) src/generated/palette.s -Felf -o src/generated/palette.o

src/generated/palette.s: $(ASSETS_GIF) src/generate_palette.php
	$(PHP) src/generate_palette.php assets/clouds.gif src/generated/palette.s

src/initialise.o: src/initialise.s
	$(VASM) $(VASM_OPTS) src/initialise.s -Felf -o src/initialise.o


