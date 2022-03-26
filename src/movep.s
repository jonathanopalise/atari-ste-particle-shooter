    public _movep_plot_pixel
    public _movep_restore_pixel

_movep_plot_pixel:
    move.l 4(a7),a0  ; get address
    movep.l 0(a0),d0  ; get pixel
    and.l 8(a7),d0   ; apply AND
    or.l 12(a7),d0   ; apply OR
    movep.l d0,0(a0)  ; write back
    rts

_movep_restore_pixel:
    move.l 4(a7),a0  ; restore buffer address
    move.l 8(a7),a1  ; destination address
    movep.l 0(a0),d0  ; read source
    movep.l d0,0(a1)  ; restore destination
    rts
