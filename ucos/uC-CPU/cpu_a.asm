;********************************************************************************************************
;                                               uC/CPU
;                                    CPU CONFIGURATION & PORT LAYER
;
;                          (c) Copyright 2004-2008; Micrium, Inc.; Weston, FL
;
;               All rights reserved.  Protected by international copyright laws.
;
;               uC/CPU is provided in source form for FREE evaluation, for educational
;               use or peaceful research.  If you plan on using uC/CPU in a commercial
;               product you need to contact Micrium to properly license its use in your
;               product.  We provide ALL the source code for your convenience and to
;               help you experience uC/CPU.  The fact that the source code is provided
;               does NOT mean that you can use it without paying a licensing fee.
;
;               Knowledge of the source code may NOT be used to develop a similar product.
;
;               Please help us continue to provide the Embedded community with the finest
;               software available.  Your honesty is greatly appreciated.
;********************************************************************************************************


;********************************************************************************************************
;
;                                            CPU PORT FILE
;
;                                            ARM-Cortex-M3
;                                            IAR C Compiler
;
; Filename      : cpu_a.asm
; Version       : V1.20
; Programmer(s) : JJL
;********************************************************************************************************


;********************************************************************************************************
;                                           PUBLIC FUNCTIONS
;********************************************************************************************************

        EXPORT  IntDisAll
        EXPORT  IntEnAll




;********************************************************************************************************
;                                      CODE GENERATION DIRECTIVES
;********************************************************************************************************

        AREA |.text|, CODE, READONLY, ALIGN=2
		THUMB
		REQUIRE8
		PRESERVE8

;$PAGE
;********************************************************************************************************
;                                    DISABLE and ENABLE INTERRUPTS
;
; Description: Disable/Enable interrupts.
;
; Prototypes : void  CPU_IntDis(void);
;              void  CPU_IntEn (void);
;********************************************************************************************************

IntDisAll
        CPSID   I
        BX      LR


IntEnAll
        CPSIE   I
        BX      LR



        END

