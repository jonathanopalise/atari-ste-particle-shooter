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

    ; d0 = number of particles drawn
    ; d1 = scratch register for movep
    ; ...
    ; d7 = or table

    move.l 4(a0),a3  ; a3 = pointer to current particle
    move.w 10(a0),d3 ; d3 = hardware viewport left xpos
    move.l 12(a0),a2 ; a2 = current particle draw pointer
    move.l 16(a0),a0 ; a0 = hardware_playfield_buffer
    move.w _logical_viewport_left_xpos,d4 ; d4 = logical viewport left xpos
    lea _hardware_playfield_ypos_lookup,a4
    lea _hardware_viewport_xpos_lookup,a5
    lea _or_table,a6
    move.l a6,d7     ; d7 = or_table
    lea _or_table_mask_lookup,a6

    moveq.l #0,d0    ; number of particles drawn
    moveq.l #0,d6    ; clear top half to ensure correct functioning below

    cmp.l #0,a3
    beq.s .alldone

.handle_particle:
    ; logical_viewport_particle_ypos (d2) =
    ; current_particle->precision_world_xpos >> 16 (because of +2)
    move.w PRECISION_WORLD_YPOS_OFS(a3),d2
    tst.w d2
    blt.s .next_particle ; if above top of screen, move to next particle

    ; logical_viewport_particle_xpos (d5) =
    ; current_particle->precision_world_xpos >> 16 (because of +2)
    move.w (a3),d5

    ; at this point:
    ; d3 = hardware_viewport_left_xpos
    ; d5 = logical_viewport_particle_xpos
    ; d4 = logical_viewport_left_xpos
    ;
    ; d5 needs to be hardware_viewport_particle_xpos
    ; hardware_viewport_left_xpos + (logical_viewport_particle_xpos - logical_viewport_left_xpos);

    sub.w d4,d5 ; logical_viewport_particle_xpos -= logical_viewport_left_xpos
    add.w d3,d5 ; logical_viewport_particle_xpos += hardware_viewport_left_xpos

    ; d5 should now contain hardware_viewport_particle_xpos
    ; d4 is NOT available for reuse

    ; now we need logical_viewport_particle_ypos (d2)
    ;         and hardware_viewport_particle_xpos (d5)
    ;         and hardware_playfield_ypos_lookup (a4) (uint32_t array)
    ;         and hardware_viewport_xpos_lookup (a5) (uint16_t array)
    ; 
    ; hardware_playfield_ypos_lookup (a4)[logical_viewport_particle_ypos (d2)] +
    ; hardware_viewport_xpos_lookup (a5)[hardware_viewport_particle_xpos (d5)]

    ; generate offset into hardware_playfield_ypos_lookup
    add.w d2,d2
    add.w d2,d2
    move.l (a4,d2.w),d2 ; get value in hardware_playfield_ypos_lookup

    ; we should preserve the d5 value here to use for the 
    ; hardware_viewport_mask_lookup!
    add.w d5,d5
    move.w (a5,d5.w),d6 ; get value in hardware_viewport_xpos_lookup
    add.l d6,d2

    ; d2 now contains hardware_playfield_particle_offset
    ; WE CANNOT REUSE A3!
    ; calculate hardware_playfield_particle_ptr

    move.l a0,a1
    add.l d2,a1

    ; a1 now contains hardware_playfield_particle_ptr
    ; hardware_viewport_particle_xpos << 1 is preserved in d5

    ; derive or_table_mask_offsets
    move.w (a6,d5.w),d2 ; or_table_mask_lookup[hardware_viewport_particle_xpos]

    move.w TIME_TO_LIVE_OFS(a3),d5 ; time to live: 0 - 63
    lsr.w #4,d5 ; reduce to 0-3
    move.b .exhaust_trail_colours(pc,d5.w),d5
    add.w d5,d5
    add.w d5,d5
    add.w d2,d5

    ; we need the or_table in an address register!
    move.l a3,usp
    move.l d7,a3

    ; this is the movep plot pixel code
    movep.l 0(a1),d1  ; get pixel
    and.l (a3,d2.w),d1   ; apply AND
    or.l (a3,d5.w),d1   ; apply OR in WHITE
    movep.l d1,0(a1)  ; write back

    ; restore address
    move.l usp,a3

    ; now do the current particle draw pointer
    move.l a1,(a2)+

    addq.l #1,d0     ; inc number of particles drawn

.next_particle:

    move.l NEXT_PARTICLE_OFS(a3),a3   ; get pointer to particle
    cmp.l #0,a3
    bne.s .handle_particle 

.alldone:
    movem.l (sp)+,d2-d7/a2-a6

    ; particles drawn returned in d0
    rts

.exhaust_trail_colours:
    dc.b 4, 12, 14, 15
