    public _particle_render_erase_particles_inner

_particle_render_erase_particles_inner:

    move.l a7,a0
    movem.l d2/a2-a3,-(a7)

    moveq.l #0,d0
    move.w 6(a0),d0  ; particles drawn
    move.l 12(a0),a1 ; restore buffer
    move.l 16(a0),a2 ; dest buffer
    move.l 8(a0),a0  ; particle draw offsets

    sub.l a1,a2      ; difference between dest buffer and restore buffer

    bra.s .end_loop
.loop:
    move.l (a0)+,d1  ; get offset and move to next offset address
    move.l a1,a3     ; a3 = restore buffer base address
    add.l d1,a3      ; add offset to base address
    movep.l 0(a3),d2 ; read source
    add.l a2,a3
    movep.l d2,0(a3) ; restore pixel

.end_loop
    dbra d0,.loop   ; decrement particles drawn

    movem.l (a7)+,d2/a2-a3
    rts
