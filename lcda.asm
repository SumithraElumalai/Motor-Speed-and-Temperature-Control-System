;-----------------------------------------------------------------------------
; COMPANY      : Confederation College
; FILE         : LCDA.ASM
; FILE VERSION : 1.0
; PROGRAMMER   : Selumala
;-----------------------------------------------------------------------------
; REVISION HISTORY
;-----------------------------------------------------------------------------
;
; 1.0, 2024-12-10, Selumala
;   - Initial release
;
;-----------------------------------------------------------------------------
; MODULE DESCRIPTION
;-----------------------------------------------------------------------------
;
; Low-level internal support funtions for the LCD module driver.
;
;-----------------------------------------------------------------------------
; GLOBALS
;-----------------------------------------------------------------------------

                .global LCD_WriteNibble
                .global LCD_ReadNibble

;-----------------------------------------------------------------------------
; CONSTANTS
;-----------------------------------------------------------------------------

GPIO_PORTA_BASE .equ    0x40004000
GPIO_PORTE_BASE .equ    0x40024000

GPIO_O_DATA     .equ    0x000
GPIO_O_DIR      .equ    0x400

; Port A (Data)
LCD_DB7_DB3     .equ    0x80
LCD_DB6_DB2     .equ    0x40
LCD_DB5_DB1     .equ    0x20
LCD_DB4_DB0     .equ    0x10
LCD_BUS         .equ    (LCD_DB7_DB3 | LCD_DB6_DB2 | LCD_DB5_DB1 | LCD_DB4_DB0)

; Port E (Control)
LCD_RS          .equ    0x08
LCD_RW          .equ    0x04
LCD_E           .equ    0x02
LCD_PWR         .equ    0x01
LCD_CONTROL     .equ    (LCD_RS | LCD_RW | LCD_E | LCD_PWR)

;-----------------------------------------------------------------------------
; MACRO    : mov32
; PURPOSE  : Moves a 32-bit immediate value into the specified register
;-----------------------------------------------------------------------------

mov32           .macro  reg, imm
                movw    reg, #( imm & 0x0000ffff )
                movt    reg, #( imm >> 16 )
                .endm

;-----------------------------------------------------------------------------
; FUNCTION : LCD_WriteNibble
; PURPOSE  : Writes one nibble to the LCD module
;-----------------------------------------------------------------------------
;
; Description
;-------------------------------------
; Writes one 4-bit value (uiData bits 7-4) to the LCD module with uiRS
; specifying the instruction register (0), or the data register (1).
;
; Arguments:
;-------------------------------------
; R0: uint8_t uiRS
; R1: uint8_t uiData
;
; Return:
;-------------------------------------
; None
;
;
                .text

LCD_WriteNibble:
                push    {lr,r0-r3}

                ; Pre-load base addresses
                mov32   r2,GPIO_PORTA_BASE
                mov32   r3,GPIO_PORTE_BASE

                ; Determine register select
                lsl     r0,#3
                and     r0,r0,#LCD_RS

                ; Configure RS and a write cycle
                strb    r0,[r3, #(GPIO_O_DATA + ((LCD_RS | LCD_RW) << 2))]

                ; tSU1:  40 ns
                nop
                nop
                nop

                ; Drive E-strobe high
                movw    r0,#LCD_E
                strb    r0,[ r3, #(GPIO_O_DATA + (LCD_E << 2))]

                ; Make all bus signals outputs
                ldrb    r0,[r2, #GPIO_O_DIR]
                orr     r0,#LCD_BUS
                strb    r0,[r2, #GPIO_O_DIR]

                ; Place data on the bus
                strb    r1,[r2, #(GPIO_O_DATA + (LCD_BUS << 2))]

                ; tW  : 230 ns
                ; tSU2:  80 ns
                nop
                nop
                nop
                nop

                nop
                nop
                nop
                nop

                nop

                ; Bring E-strobe low
                movw    r0,#0
                strb    r0,[r3, #(GPIO_O_DATA + (LCD_E << 2))]

                ; Change data bus signals back to inputs
                ldrb    r0,[r2, #GPIO_O_DIR]
                bic     r0,#LCD_BUS
                strb    r0,[r2, #GPIO_O_DIR]

                nop

                pop     {pc,r0-r3}

;-----------------------------------------------------------------------------
; FUNCTION : LCD_ReadNibble
; PURPOSE  : Reads one nibble from the LCD module
;-----------------------------------------------------------------------------
;
; Description
;-------------------------------------
; Reads one 4-bit value (uiData bits 7-4) from the LCD module with uiRS
; specifying the instruction register (0), or the data register (1).
;
; Arguments:
;-------------------------------------
; R0: uint8_t uiRS
;
;
; Return:
;-------------------------------------
; R0: uint8_t uiData
;
;
                .text

LCD_ReadNibble:
                push    {lr,r1-r2}

                ; Pre-load base addresses
                mov32   r1,GPIO_PORTA_BASE
                mov32   r2,GPIO_PORTE_BASE

                ; Determine register select
                lsl     r0,#3
                and     r0,r0,#LCD_RS
                orr     r0,r0,#LCD_RW

                ; Configure RS and a read cycle
                strb    r0,[r2, #(GPIO_O_DATA + ((LCD_RS | LCD_RW) << 2))]

                ; tSU:   40 ns
                nop
                nop
                nop

                ; Drive E-strobe high
                movw    r0,#LCD_E
                strb    r0,[r2, #(GPIO_O_DATA + (LCD_E << 2))]

                ; tW  : 230 ns
                ; tD  : 120 ns
                nop
                nop
                nop
                nop

                nop
                nop
                nop
                nop

                nop
                nop
                nop
                nop

                nop
                nop

                ; Read data
                ldrb    r0,[r1, #(GPIO_O_DATA + (LCD_BUS << 2))]

                ; Bring E-strobe low
                movw    r1,#0
                strb    r1,[r2, #(GPIO_O_DATA + (LCD_E << 2))]
                nop

                pop     {pc,r1-r2}

                .end

;-----------------------------------------------------------------------------
; END LCDA.ASM
;-----------------------------------------------------------------------------
