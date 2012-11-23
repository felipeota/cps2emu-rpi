	;@ sprite drawing

#include "draw_arm.h"

.global draw_tile_8_norm
.global draw_tile_8_xyflip
.global draw_tile_8_xflip
.global draw_tile_8_yflip
.global draw_tile_16_norm
.global draw_tile_16_xyflip
.global draw_tile_16_xflip
.global draw_tile_16_yflip
.global draw_tile_rows_norm
.global draw_tile_rows_xyflip
.global draw_tile_rows_xflip
.global draw_tile_rows_yflip
.global draw_tile_32_norm
.global draw_tile_32_xyflip
.global draw_tile_32_xflip
.global draw_tile_32_yflip

	.align 4

	;;@ Draw a line of a sprite.
.macro DRAW_SPR_LINE8
	ldr regTile,[regSrc, #4]
	@cmp regTile,#0
	@beq 1f

	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #2]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #4]
	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #6]

	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #8]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #10]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #12]
	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #14]

@1:
.endm

	;;@ Draw a line of a sprite xflipped.
.macro DRAW_SPR_LINE8_XFLIP
	ldr regTile,[regSrc,#4]
	@cmp regTile,#0
	@beq 1f

	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #2]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #4]
	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #6]

	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #8]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #10]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #12]
	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #14]

@1:
.endm

.macro DRAW_LINE8_XFLIP
	DRAW_SPR_LINE8_XFLIP
	add regSrc, regSrc, #8
	add regDst, regDst, #PITCH
.endm
.macro DRAW_LINE8_XYFLIP
	DRAW_SPR_LINE8_XFLIP
	add regSrc, regSrc, #8
	sub regDst, regDst, #PITCH
.endm
.macro DRAW_LINE8_YFLIP
	DRAW_SPR_LINE8
	add regSrc, regSrc, #8
	sub regDst, regDst, #PITCH
.endm
.macro DRAW_LINE8_NORM
	DRAW_SPR_LINE8
	add regSrc, regSrc, #8
	add regDst, regDst, #PITCH
.endm

	;;@ Draw a line of a sprite.
.macro DRAW_SPR_LINE16
	ldr regTile,[regSrc]
	@cmp regTile,#0
	@beq 1f

	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #2]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #4]
	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #6]

	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #8]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #10]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #12]
	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #14]

@1:
	ldr regTile,[regSrc,#4]
	@cmp regTile,#0
	@beq 2f

	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #16]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #18]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #20]
	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #22]

	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #24]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #26]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #28]
	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #30]
@2:
.endm

	;;@ Draw a line of a sprite xflipped.
.macro DRAW_SPR_LINE16_XFLIP
	ldr regTile,[regSrc,#4]
	@cmp regTile,#0
	@beq 1f

	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #2]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #4]
	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #6]

	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #8]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #10]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #12]
	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #14]

@1:
	ldr regTile,[regSrc]
	@cmp regTile,#0
	@beq 2f

	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #16]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #18]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #20]
	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #22]

	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #24]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #26]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #28]
	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #30]
@2:
.endm

.macro DRAW_LINE16_XFLIP
	DRAW_SPR_LINE16_XFLIP
	add regSrc, regSrc, #8
	add regDst, regDst, #PITCH
.endm
.macro DRAW_LINE16_XYFLIP
	DRAW_SPR_LINE16_XFLIP
	add regSrc, regSrc, #8
	sub regDst, regDst, #PITCH
.endm
.macro DRAW_LINE16_YFLIP
	DRAW_SPR_LINE16
	add regSrc, regSrc, #8
	sub regDst, regDst, #PITCH
.endm
.macro DRAW_LINE16_NORM
	DRAW_SPR_LINE16
	add regSrc, regSrc, #8
	add regDst, regDst, #PITCH
.endm

.macro DRAW_LINE16_XFLIP_ROWS
	DRAW_SPR_LINE16_XFLIP
	add regSrc, regSrc, #8
	add regDst, regDst, #PITCH
	ldr regTemp, [regRows, #4]
	add regDst, regDst, regTemp
	add regRows, regRows, #4
.endm
.macro DRAW_LINE16_XYFLIP_ROWS
	DRAW_SPR_LINE16_XFLIP
	add regSrc, regSrc, #8
	sub regDst, regDst, #PITCH
	ldr regTemp, [regRows, #4]
	add regDst, regDst, regTemp
	add regRows, regRows, #4
.endm
.macro DRAW_LINE16_YFLIP_ROWS
	DRAW_SPR_LINE16
	add regSrc, regSrc, #8
	sub regDst, regDst, #PITCH
	ldr regTemp, [regRows, #4]
	add regDst, regDst, regTemp
	add regRows, regRows, #4
.endm
.macro DRAW_LINE16_NORM_ROWS
	DRAW_SPR_LINE16
	add regSrc, regSrc, #8
	add regDst, regDst, #PITCH
	ldr regTemp, [regRows, #4]
	add regDst, regDst, regTemp
	add regRows, regRows, #4
.endm

	;;@ Draw a line of a sprite.
.macro DRAW_SPR_LINE32
	ldr regTile,[regSrc]
	@cmp regTile,#0
	@beq 1f

	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #2]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #4]
	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #6]

	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #8]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #10]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #12]
	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #14]

@1:
	ldr regTile,[regSrc,#4]
	@cmp regTile,#0
	@beq 2f

	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #16]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #18]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #20]
	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #22]

	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #24]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #26]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #28]
	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #30]
@2:
	ldr regTile,[regSrc,#8]
	@cmp regTile,#0
	@beq 3f

	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #32]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #34]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #36]
	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #38]

	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #40]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #42]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #44]
	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #46]

@3:
	ldr regTile,[regSrc,#12]
	@cmp regTile,#0
	@beq 4f

	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #48]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #50]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #52]
	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #54]

	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #56]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #58]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #60]
	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #62]
@4:
.endm

	;;@ Draw a line of a sprite xflipped.
.macro DRAW_SPR_LINE32_XFLIP
	ldr regTile,[regSrc,#12]
	@cmp regTile,#0
	@beq 1f

	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #2]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #4]
	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #6]

	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #8]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #10]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #12]
	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #14]

@1:
	ldr regTile,[regSrc,#8]
	@cmp regTile,#0
	@beq 2f

	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #16]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #18]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #20]
	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #22]

	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #24]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #26]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #28]
	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #30]
@2:
	ldr regTile,[regSrc,#4]
	@cmp regTile,#0
	@beq 3f

	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #32]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #34]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #36]
	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #38]

	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #40]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #42]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #44]
	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #46]

@3:
	ldr regTile,[regSrc]
	@cmp regTile,#0
	@beq 4f

	ands regPixel,regMask,regTile, lsr #28
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #48]
	ands regPixel,regMask,regTile, lsr #24
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #50]
	ands regPixel,regMask,regTile, lsr #20
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #52]
	ands regPixel,regMask,regTile, lsr #16
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #54]

	ands regPixel,regMask,regTile, lsr #12
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #56]
	ands regPixel,regMask,regTile, lsr #8
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #58]
	ands regPixel,regMask,regTile, lsr #4
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #60]
	ands regPixel,regMask,regTile
	ldrne regColor,[regPal, regPixel, lsl #2]
	strneh regColor,[regDst, #62]
@4:
.endm

.macro DRAW_LINE32_XFLIP
	DRAW_SPR_LINE32_XFLIP
	add regSrc, regSrc, #16
	add regDst, regDst, #PITCH
.endm
.macro DRAW_LINE32_XYFLIP
	DRAW_SPR_LINE32_XFLIP
	add regSrc, regSrc, #16
	sub regDst, regDst, #PITCH
.endm
.macro DRAW_LINE32_YFLIP
	DRAW_SPR_LINE32
	add regSrc, regSrc, #16
	sub regDst, regDst, #PITCH
.endm
.macro DRAW_LINE32_NORM
	DRAW_SPR_LINE32
	add regSrc, regSrc, #16
	add regDst, regDst, #PITCH
.endm

.macro DRAW_TILE8 name
	stmdb sp!, {r3-r9}
	mov regLoop, #8
	mov regMask, #0xF

.lineloop8_\name:
	DRAW_LINE8_\name
	subs regLoop, regLoop, #1
	bne .lineloop8_\name

	ldmia sp!, {r3-r9}
	mov pc,lr               ;@ return
.endm

.macro DRAW_TILE16 name
	stmdb sp!, {r3-r9}
	mov regLoop, #16
	mov regMask, #0xF

.lineloop16_\name:
	DRAW_LINE16_\name
	subs regLoop, regLoop, #1
	bne .lineloop16_\name

	ldmia sp!, {r3-r9}
	mov pc,lr               ;@ return
.endm

.macro DRAW_TILE32 name
	stmdb sp!, {r3-r9}
	mov regLoop, #32
	mov regMask, #0xF

.lineloop32_\name:
	DRAW_LINE32_\name
	subs regLoop, regLoop, #1
	bne .lineloop32_\name

	ldmia sp!, {r3-r9}
	mov pc,lr               ;@ return
.endm

	.align 4
draw_tile_8_xyflip:
	add regDst, regDst, #PITCH * 7
	DRAW_TILE8 XYFLIP

	.align 4
draw_tile_8_xflip:
	DRAW_TILE8 XFLIP

	.align 4
draw_tile_8_yflip:
	add regDst, regDst, #PITCH * 7
	DRAW_TILE8 YFLIP

	.align 4
draw_tile_8_norm:
	DRAW_TILE8 NORM

	.align 4
draw_tile_16_xyflip:
	add regDst, regDst, #PITCH * 15
	DRAW_TILE16 XYFLIP

	.align 4
draw_tile_16_xflip:
	DRAW_TILE16 XFLIP

	.align 4
draw_tile_16_yflip:
	add regDst, regDst, #PITCH * 15
	DRAW_TILE16 YFLIP

	.align 4
draw_tile_16_norm:
	DRAW_TILE16 NORM

	.align 4
draw_tile_rows_xyflip:
	add regDst, regDst, #PITCH * 15
	DRAW_TILE16 XYFLIP_ROWS

	.align 4
draw_tile_rows_xflip:
	DRAW_TILE16 XFLIP_ROWS

	.align 4
draw_tile_rows_yflip:
	add regDst, regDst, #PITCH * 15
	DRAW_TILE16 YFLIP_ROWS

	.align 4
draw_tile_rows_norm:
	DRAW_TILE16 NORM_ROWS

	.align 4
draw_tile_32_xyflip:
	add regDst, regDst, #PITCH * 31
	DRAW_TILE32 XYFLIP

	.align 4
draw_tile_32_xflip:
	DRAW_TILE32 XFLIP

	.align 4
draw_tile_32_yflip:
	add regDst, regDst, #PITCH * 31
	DRAW_TILE32 YFLIP

	.align 4
draw_tile_32_norm:
	DRAW_TILE32 NORM
