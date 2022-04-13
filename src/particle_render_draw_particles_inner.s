    public _particle_render_draw_particles_inner

; these need to align with the Particle struct
PRECISION_WORLD_XPOS_OFS equ 0
PRECISION_WORLD_YPOS_OFS equ 4
TYPE_OFS equ 16
TIME_TO_LIVE_OFS equ 18
NEXT_PARTICLE_OFS equ 22

_particle_render_draw_particles_inner:
    move.l a7,a0
    movem.l d2-d7/a2-a6,-(sp)

    ; a0 = hardware_playfield_buffer
    ; a1 = scratch register
    ; a2 = current_particle_draw_pointer
    ; a3 = particle pointer tracker
    ; a4 = hardware playfield ypos lookup
    ; a5 = hardware viewport xpos lookup
    ; a6 = or table mask lookup

    ; d1 = scratch register for movep
    ; ...
    ; d7 = or table

    move.l 4(a0),a3  ; a3 = pointer to current particle
    move.w 10(a0),d3 ; d3 = hardware viewport left xpos
    move.l 12(a0),a2 ; a2 = current particle draw pointer
    move.l 20(a0),a4 ; a4 = ypos pointers
    move.l 16(a0),a0 ; a0 = hardware_playfield_buffer
    move.w _logical_viewport_left_xpos,d4 ; d4 = logical viewport left xpos
    lea _hardware_viewport_xpos_lookup,a5
    lea _or_table,a0
    lea _or_table_mask_lookup,a6

    sub.w d3,d4 ; precalc below calculations

    move.w #99,d7   ; MUST MATCH PARTICLE_COUNT in particle_common.h

.handle_particle:
    move.w TIME_TO_LIVE_OFS(a3),d6
    beq.s .next_particle

    ; get current_particle->precision_world_ypos >> 16 (because of address +2)
    ; if above top of screen, move to next particle
    move.w PRECISION_WORLD_YPOS_OFS(a3),d2
    ;blt.s .next_particle

    ; get current_particle->precision_world_xpos >> 16 (because of address +2)
    move.w (a3),d5

    ; adjust xpos to location within hardware viewport
    sub.w d4,d5

    ; get the pointer to the start of the buffer line where the particle
    ; needs to be drawn
    add.w d2,d2
    add.w d2,d2
    move.l (a4,d2.w),a1

    ; add the offset within the hardware viewport representing the adjusted xpos
    add.w d5,d5
    add.w (a5,d5.w),a1

    ; derive offset within or table
    move.w (a6,d5.w),d2

    ; plot the pixel
    movep.l 0(a1),d1
    and.l (a0,d2.w),d1
    add.b .exhaust_trail_colours(pc,d6.w),d2
    or.l (a0,d2.w),d1
    movep.l d1,0(a1)

    ; store pointer to write location to erase later
    move.l a1,(a2)+

.next_particle:

    lea 26(a3),a3
    dbra d7,.handle_particle

.alldone:

    ; calculate number of particles drawn
    move.l a2,d0
    movem.l (sp)+,d2-d7/a2-a6

    sub.l 12(a7),d0
    lsr.l #2,d0

    ; particles drawn returned in d0
    rts

.exhaust_trail_colours:
    rept 16
    dc.b 2*4
    endr
    rept 16
    dc.b 6*4
    endr
    rept 16
    dc.b 10*4
    endr
    rept 16
    dc.b 14*4
    endr
