    public _sprite_render_inner_draw
    public _sprite_render_inner_erase

_sprite_render_inner_draw:
    ; a0 needs to be graphics source
    ; a1 needs to be graphics destination

    ; source x increment 10
    ; dest x increment 8

    move.l a7,a0
    movem.l d2-d7/a2-a6,-(sp)

    ; a0(4) = source
    ; a0(8) = destination
    ; a0(12+2) = skew (ignore for now, assume 0)

    lea $ffff8a20.w,a3
    move.w #10,(a3)+                     ; source x increment 8a20
    move.w #10,(a3)+                     ; source y increment 8a22
    addq.l #4,a3                         ; skip source address 8a24 - we'll set it later
    move.w #$ffff,(a3)+                  ; endmask1 8a28
    move.w #$ffff,(a3)+                  ; endmask2 8a2a
    move.w #$ffff,(a3)+                  ; endmask3 8a2c
    move.w #8,(a3)+                      ; dest x increment 8a2e 
    ;move.w #480,(a3)+                    ; dest y increment 8a30

    move.w #$201,$ffff8a3a.w             ; hop/op: read from source, source & destination

    move.w 12+2(a0),d2                   ; skew = 0
    move.l 8(a0),a1                      ; dest address 8a32
    move.l 4(a0),a0                      ; source address 8a24

    tst.w d2
    beq.s .lines_of_16_pixels

.lines_of_32_pixels:

    ; 32 pixels size handling
    move.w #480-8,$ffff8a30.w             ; dest y increment 8a30
    move.w #2,$ffff8a36.w                ; xcount 8a36
    move.b d2,$ffff8a3d.w
    add.w d2,d2
    move.w leftendmasks(pc,d2),$ffff8a28.w
    move.w rightendmasks(pc,d2),$ffff8a2c.w
    bra.s .after_size

.lines_of_16_pixels
    ; 16 pixel size handling
    move.w #480,$ffff8a30.w             ; dest y increment 8a30
    move.w #1,$ffff8a36.w                ; xcount 8a36

.after_size

    rept 3
    bsr.s drawplane
    addq.l #2,a1                        ; move to next bitplane
    endr
    bsr.s drawplane

    subq.l #6,a1                        ; move destination back to initial bitplane
    move.w #$0207,$ffff8a3a.w           ; hop/op: read from source, source | destination

    addq.l #2,a0                        ; move source to next bitplane
    bsr.s drawplane
    addq.l #2,a1                        ; move destination to next bitplane
    addq.l #2,a0                        ; move source to next bitplane
    bsr.s drawplane
    addq.l #2,a1                        ; move destination to next bitplane
    addq.l #2,a0                        ; move source to next bitplane
    bsr.s drawplane
    addq.l #2,a1                        ; move destination to next bitplane
    addq.l #2,a0                        ; move source to next bitplane
    bsr.s drawplane

alldone:
    movem.l (sp)+,d2-d7/a2-a6
    rts

drawplane:
    move.l a0,$ffff8a24.w      ; set source address
    move.l a1,$ffff8a32.w      ; set destination
    move.w #16,$ffff8a38.w     ; ycount (needs to be 16, for 16 lines)
    move.b #$c0,$ffff8a3c.w    ; control
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

_sprite_render_inner_erase:
    ; struct SpriteDrawRecord {
    ;     uint8_t *draw_pointer;
    ;     uint16_t draw_width;
    ; };

    move.l a7,a0
    movem.l d2-d7/a2-a6,-(sp)
    ; don't forget push regs

    move.w 4+2(a0),d0 ; sprites drawn
    move.l 16(a0),a1  ; dest buffer
    sub.l 12(a0),a1   ; difference between dest buffer and restore buffer
    move.l 8(a0),a0   ; sprite draw records

    bra .end_loop
.loop:
    move.l (a0)+,a2  ; get dest pointer
    move.l a2,a3     ; copy dest pointer to a3
    sub.l  a1,a3     ; calc source pointer
    cmp.w #0,(a0)+   ; ignore draw width for now
    beq .clear_16_pixels

.clear_32_pixels
    rept 16
    move.l (a3)+,(a2)+
    move.l (a3)+,(a2)+
    move.l (a3)+,(a2)+
    move.l (a3)+,(a2)+
    lea 480-16(a3),a3
    lea 480-16(a2),a2
    endr
    bra .end_loop

.clear_16_pixels
    rept 16
    move.l (a3)+,(a2)+
    move.l (a3)+,(a2)+
    lea 480-8(a3),a3
    lea 480-8(a2),a2
    endr

.end_loop
    dbra d0,.loop   ; decrement sprites drawn
    movem.l (sp)+,d2-d7/a2-a6

    rts


