    public _sprite_render_inner_draw
    public _sprite_render_inner_erase

    macro drawplane
    move.l a0,(a2)      ; set source address
    move.l a1,(a3)      ; set destination
    move.w d0,(a4)     ; ycount (needs to be 16, for 16 lines)
    move.b d1,(a5)    ; control
    endm

_sprite_render_inner_draw:
    move.l a7,a0
    movem.l a2-a6/d2-d3,-(sp)

    ; a0(4) = source
    ; a0(8) = destination
    ; a0(12+2) = skew (ignore for now, assume 0)

    move.w 12+2(a0),d0                           ; skew
    move.l 8(a0),a1                              ; dest address 8a32
    move.l 4(a0),a0                              ; source address 8a24

    move.b d0,$ffff8a3d.w
    tst.w d0
    beq .lines_of_16_pixels

.lines_of_32_pixels:

    ; first generate shifted mask data for 16 lines

    moveq.l #2,d2

    lea $ffff8a20.w,a3
    move.w d2,(a3)+                ; srcxinc 8a20
    clr.w (a3)+                    ; srcyinc 8a22
    move.l a0,(a3)+                ; source address 8a24
    moveq.l #-1,d1
    move.w d1,(a3)+                ; endmask1 8a28
    move.w d1,(a3)+                ; endmask2 8a2a
    move.w d1,(a3)+                ; endmask3 8a2c
    move.w d2,(a3)+                ; destxinc 8a2e
    move.w d2,(a3)+                ; destyinc 8a30
    lea .shifted_buffer,a2         ; get dest buffer address
    move.l a2,(a3)+                ; dest 8a32
    move.w d2,(a3)+                ; xcount 8a36
    move.w #16,(a3)+               ; ycount 8a38
    move.w #$020c,(a3)+            ; hop/op 8a3a
    move.b d0,d1                   ; copy skew to d1
    or.b #$80,d1                   ; apply fsxr
    move.b d1,1(a3)                ; skew/fxsr register
    move.w #$c0,d1
    move.b d1,(a3)                 ; control

    ; a2 still contains shifted buffer address
    ; now draw the mask

    move.w #8,$ffff8a2e.w          ; destxinc
    move.w #-6,$ffff8a30.w         ; destyinc
    move.w #2,$ffff8a36.w          ; xcount
    move.w #0,$ffff8a3a.w          ; hop/op
    move.b #0,$ffff8a3d.w          ; skew/fxsr register

    move a0,usp
    lea $ffff8a28.w,a0             ; endmask1
    lea $ffff8a2c.w,a3             ; endmask3
    lea $ffff8a38.w,a4             ; ycount
    lea $ffff8a32.w,a5             ; dest address
    lea $ffff8a3c.w,a6             ; control
    moveq.l #4,d2

    rept 16
    move.w (a2)+,(a0)              ; endmask1
    move.w (a2)+,(a3)              ; endmask3
    move.w d2,(a4)                 ; ycount
    move.l a1,(a5)                 ; dest address
    move.b d1,(a6)                 ; control
    lea 480(a1),a1                 ; next line 
    endr

    move usp,a0

    lea 66(a0),a0 ; move onto colour data
    lea -480*16(a1),a1 ; reset dest to original position for colour pass

    ; time for the colour pass

    lea $ffff8a20.w,a3
    clr.w (a3)+                              ; source x increment 8a20
    move.w #8,(a3)+                             ; source y increment 8a22
    ; source 8a24 set per pass
    move.b d0,$ffff8a3d.w
    add.w d0,d0
    move.w .leftendmasks(pc,d0),$ffff8a28.w      ; endmask1 8a28
    move.w #$ffff,$ffff8a2a.w                    ; endmask2 8a2a
    move.w .rightendmasks(pc,d0),$ffff8a2c.w     ; endmask3 8a2c
    move.w #8,$ffff8a2e.w                        ; dest x increment 8a2e 
    move.w #480-8,$ffff8a30.w                    ; dest y increment 8a30
    move.w #2,$ffff8a36.w                        ; xcount 8a36
    ; ycount 8a38 set per pass
    move.w #$0207,$ffff8a3a.w           ; hop/op: read from source, source | destination
    ; control 8a3c set per pass
    or.b #$40,$ffff8a3d.w                        ; nfsr

    bra .after_size

.leftendmasks:

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

.rightendmasks:

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


.shifted_buffer:
    ds.w 32 ; 1 empty word plus two mask words

.lines_of_16_pixels
    ; 16 pixel size handling
    ; this should have a dedicated drawing routine to itself
    ; we can use .clear_16_pixels below as a basis
    move.w #$ffff,$ffff8a28.w                  ; endmask1 8a28
    move.w #480,$ffff8a30.w             ; dest y increment 8a30
    move.w #1,$ffff8a36.w                ; xcount 8a36

.after_size

    lea $ffff8a24.w,a2
    lea $ffff8a32.w,a3
    lea $ffff8a38.w,a4
    lea $ffff8a3c.w,a5
    move.w #16,d0

    drawplane
    addq.l #2,a1                        ; move destination to next bitplane
    addq.l #2,a0                        ; move source to next bitplane
    drawplane
    addq.l #2,a1                        ; move destination to next bitplane
    addq.l #2,a0                        ; move source to next bitplane
    drawplane
    addq.l #2,a1                        ; move destination to next bitplane
    addq.l #2,a0                        ; move source to next bitplane
    drawplane

.alldone
    movem.l (sp)+,a2-a6/d2-d3
    rts

_sprite_render_inner_erase:
    ; struct SpriteDrawRecord {
    ;     uint8_t *draw_pointer;
    ;     uint16_t draw_width;
    ; };

    move.l a7,a0
    movem.l d2-d7/a2-a4,-(sp)

    move.w 4+2(a0),d0 ; sprites drawn
    move.l 16(a0),a1  ; dest buffer
    sub.l 12(a0),a1   ; difference between dest buffer and restore buffer
    move.l 8(a0),a0   ; sprite draw records

    moveq.l #-1,d2
    moveq.l #2,d3

    lea $ffff8a20.w,a4
    move.w d3,(a4)+      ; srcxinc 8a20
    addq.l #6,a4
    move.w d2,(a4)+   ; endmask1 8a28
    move.w d2,(a4)+   ; endmask2 8a2a
    move.w d2,(a4)+   ; endmask3 8a2c
    move.w d3,(a4)+      ; dstxinc 8a2e
    lea 10(a4),a4
    move.w #$0203,(a4)+  ; hop/op 8a3a

    move.w #480-14,d2
    move.w #480-6,d3
    moveq.l #16,d4
    move.w #$c000,d5
    moveq.l #4,d6
    moveq.l #8,d7

    bra.s .end_loop
.loop:
    move.l (a0)+,a2  ; get dest pointer
    move.l a2,a3     ; copy dest pointer to a3
    sub.l  a1,a3     ; calc source pointer
    tst.w (a0)+   ; ignore draw width for now
    beq.s .clear_16_pixels

.clear_32_pixels
    lea $ffff8a22.w,a4
    move.w d2,(a4)+  ; srcyinc 8a22      INCLUDE
    move.l a3,(a4)+      ; src address 8a24   INCLUDE
    addq.l #8,a4
    move.w d2,(a4)+  ; dstyinc 8a30      INCLUDE
    move.l a2,(a4)+      ; dst address 8a32   INCLUDE
    move.w d7,(a4)+      ; xcount 8a36        INCLUDE
    move.w d4,(a4)+     ; ycount 8a38        INCLUDE
    addq.l #2,a4
    move.w d5,(a4)+   ; blitter control 8a3c
    bra .end_loop

.clear_16_pixels
    lea $ffff8a22.w,a4
    move.w d3,(a4)+  ; srcyinc 8a22      INCLUDE
    move.l a3,(a4)+      ; src address 8a24   INCLUDE
    addq.l #8,a4
    move.w d3,(a4)+  ; dstyinc 8a30      INCLUDE
    move.l a2,(a4)+      ; dst address 8a32   INCLUDE
    move.w d6,(a4)+      ; xcount 8a36        INCLUDE
    move.w d4,(a4)+     ; ycount 8a38        INCLUDE
    addq.l #2,a4
    move.w d5,(a4)+   ; blitter control 8a3c

.end_loop
    dbra d0,.loop   ; decrement sprites drawn
    movem.l (sp)+,d2-d7/a2-a4

    rts


