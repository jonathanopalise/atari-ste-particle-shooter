    public _player_render_draw_inner_16
    public _player_render_draw_inner_32
    public _player_render_erase_inner_16
    public _player_render_erase_inner_32

_player_render_draw_inner_unshifted:



    rts

_player_render_draw_inner_shifted:
    ; d1 needs to be 16 (16 lines into ycount)
    ; d6 needs to be #c0 (control hog word)
    ; a0 needs to be graphics source
    ; a1 needs to be graphics destination
    ; a2 needs to be ycount register
    ; a4 needs to be source address register
    ; a5 needs to be destination address register
    ; a6 needs to be control register

    move.b #$c0,d6                      ; blitter start instruction
    move.w #$201,($ffff8a3a).w ; hop/op: read from source, source & destination

    rept 3
    bsr.s drawsceneryplane
    addq.l #2,a1                        ; move to next bitplane
    endr
    bsr.s drawsceneryplane

    subq.l #6,a1                        ; move destination back to initial bitplane
    move.w #$0207,($ffff8a3a).w         ; hop/op: read from source, source | destination

    addq.l #2,a0                        ; move source to next bitplane
    bsr.s drawsceneryplane
    addq.l #2,a1                        ; move destination to next bitplane
    addq.l #2,a0                        ; move source to next bitplane
    bsr.s drawsceneryplane
    addq.l #2,a1                        ; move destination to next bitplane
    addq.l #2,a0                        ; move source to next bitplane
    bsr.s drawsceneryplane
    addq.l #2,a1                        ; move destination to next bitplane
    addq.l #2,a0                        ; move source to next bitplane
    bsr.s drawsceneryplane

alldone:

    rts

drawsceneryplane:
    move.l a0,(a4)             ; set source address
    move.l a1,(a5)             ; set destination
    move.w d1,(a2)             ; ycount (needs to be 16, for 16 lines)
    move.b d6,(a6)             ; control
    rts

leftendmasks:

    dc.w %1111111111111111
    dc.w %0111111111111111
    dc.w %0011111111111111
    dc.w %0001111111111111
    dc.w %0000111111111111
    dc.w %0000011111111111
    dc.w %0000001111111111
    dc.w %0000000111111111
    dc.w %0000000011111111
    dc.w %0000000001111111
    dc.w %0000000000111111
    dc.w %0000000000011111
    dc.w %0000000000001111
    dc.w %0000000000000111
    dc.w %0000000000000011
    dc.w %0000000000000001

rightendmasks:

    dc.w %1111111111111111
    dc.w %1000000000000000
    dc.w %1100000000000000
    dc.w %1110000000000000
    dc.w %1111000000000000
    dc.w %1111100000000000
    dc.w %1111110000000000
    dc.w %1111111000000000
    dc.w %1111111100000000
    dc.w %1111111110000000
    dc.w %1111111111000000
    dc.w %1111111111100000
    dc.w %1111111111110000
    dc.w %1111111111111000
    dc.w %1111111111111100
    dc.w %1111111111111110



_player_render_erase_inner_16:
    rts

_player_render_erase_inner_32:
    rts
