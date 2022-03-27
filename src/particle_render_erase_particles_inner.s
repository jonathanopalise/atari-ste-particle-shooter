    public _particle_render_erase_particles_inner

_particle_render_erase_particles_inner:

    move.l a7,a0
    move.l a2,-(a7)

    move.w 6(a0),d0  ; particles drawn
    move.l 16(a0),a1 ; dest buffer
    sub.l 12(a0),a1 ; store difference between dest buffer and restore buffer
    move.l 8(a0),a0  ; particle draw offsets

    bra.s .end_loop
.loop:
    move.l (a0)+,a2  ; get offset and move to next offset address
    sub.l a1,a2
    movep.l 0(a2),d1 ; read source
    add.l a1,a2
    movep.l d1,0(a2) ; restore pixel

.end_loop
    dbra d0,.loop   ; decrement particles drawn

    move.l (a7)+,a2
    rts
