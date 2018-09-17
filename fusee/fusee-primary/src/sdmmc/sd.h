/*
 *  include/linux/mmc/sd.h
 *
 *  Copyright (C) 2005-2007 Pierre Ossman, All Rights Reserved.
 *  Copyright (C) 2018 CTCaer
 *  Copyright (c) 2018 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */

#ifndef LINUX_MMC_SD_H
#define LINUX_MMC_SD_H

/* SD commands                           type  argument     response */
/* class 0 */
/* This is basically the same command as for MMC with some quirks. */
#define SD_SEND_RELATIVE_ADDR     3   /* bcr                     R6  */
#define SD_SEND_IF_COND           8   /* bcr  [11:0] See below   R7  */
#define SD_SWITCH_VOLTAGE         11  /* ac                      R1  */

/* class 10 */
#define SD_SWITCH                 6   /* adtc [31:0] See below   R1  */

/* class 5 */
#define SD_ERASE_WR_BLK_START    32   /* ac   [31:0] data addr   R1  */
#define SD_ERASE_WR_BLK_END      33   /* ac   [31:0] data addr   R1  */

/* Application commands */
#define SD_APP_SET_BUS_WIDTH        6   /* ac   [1:0] bus width    R1  */
#define SD_APP_SD_STATUS            13  /* adtc                    R1  */
#define SD_APP_SEND_NUM_WR_BLKS     22  /* adtc                    R1  */
#define SD_APP_OP_COND              41  /* bcr  [31:0] OCR         R3  */
#define SD_APP_SET_CLR_CARD_DETECT  42  /* ac   [0] set cd         R1  */
#define SD_APP_SEND_SCR             51  /* adtc                    R1  */

/* OCR bit definitions */
#define SD_OCR_S18R             (1 << 24)                   /* 1.8V switching request */
#define SD_ROCR_S18A            SD_OCR_S18R                 /* 1.8V switching accepted by card */
#define SD_OCR_XPC              (1 << 28)                   /* SDXC power control */
#define SD_OCR_CCS              (1 << 30)                   /* Card Capacity Status */
#define SD_OCR_VDD_LOW          (1 << 7)                    /* SD: Reserved for Low Voltage Range */
#define SD_OCR_VDD_20_21        (1 << 8)
#define SD_OCR_VDD_21_22        (1 << 9)
#define SD_OCR_VDD_22_23        (1 << 10)
#define SD_OCR_VDD_23_24        (1 << 11)
#define SD_OCR_VDD_24_25        (1 << 12)
#define SD_OCR_VDD_25_26        (1 << 13)
#define SD_OCR_VDD_26_27        (1 << 14)
#define SD_OCR_VDD_27_28        (1 << 15)
#define SD_OCR_VDD_28_29        (1 << 16)
#define SD_OCR_VDD_29_30        (1 << 17)
#define SD_OCR_VDD_30_31        (1 << 18)
#define SD_OCR_VDD_31_32        (1 << 19)
#define SD_OCR_VDD_32_33        (1 << 20)
#define SD_OCR_VDD_33_34        (1 << 21)
#define SD_OCR_VDD_34_35        (1 << 22)
#define SD_OCR_VDD_35_36        (1 << 23)

/*
 * SD_SWITCH argument format:
 *
 *      [31] Check (0) or switch (1)
 *      [30:24] Reserved (0)
 *      [23:20] Function group 6
 *      [19:16] Function group 5
 *      [15:12] Function group 4
 *      [11:8] Function group 3
 *      [7:4] Function group 2
 *      [3:0] Function group 1
 */

/*
 * SD_SEND_IF_COND argument format:
 *
 *  [31:12] Reserved (0)
 *  [11:8] Host Voltage Supply Flags
 *  [7:0] Check Pattern (0xAA)
 */

/*
 * SCR field definitions
 */

#define SD_SCR_SPEC_VER_0       0   /* Implements system specification 1.0 - 1.01 */
#define SD_SCR_SPEC_VER_1       1   /* Implements system specification 1.10 */
#define SD_SCR_SPEC_VER_2       2   /* Implements system specification 2.00-3.0X */
#define SD_SCR_BUS_WIDTH_1      1
#define SD_SCR_BUS_WIDTH_4      4
#define SD_SCR_CMD20_SUPPORT    1
#define SD_SCR_CMD23_SUPPORT    2

/*
 * SD bus widths
 */
#define SD_BUS_WIDTH_1      0
#define SD_BUS_WIDTH_4      2

/*
 * SD bus speed modes
 */
#define UHS_SDR12_BUS_SPEED     0
#define HIGH_SPEED_BUS_SPEED    1
#define UHS_SDR25_BUS_SPEED     1
#define UHS_SDR50_BUS_SPEED     2
#define UHS_SDR104_BUS_SPEED    3
#define UHS_DDR50_BUS_SPEED     4
#define SD_MODE_HIGH_SPEED      (1 << HIGH_SPEED_BUS_SPEED)
#define SD_MODE_UHS_SDR12       (1 << UHS_SDR12_BUS_SPEED)
#define SD_MODE_UHS_SDR25       (1 << UHS_SDR25_BUS_SPEED)
#define SD_MODE_UHS_SDR50       (1 << UHS_SDR50_BUS_SPEED)
#define SD_MODE_UHS_SDR104      (1 << UHS_SDR104_BUS_SPEED)
#define SD_MODE_UHS_DDR50       (1 << UHS_DDR50_BUS_SPEED)

/*
 * SD bus driver types
 */
#define SD_DRIVER_TYPE_B    0x01
#define SD_DRIVER_TYPE_A    0x02
#define SD_DRIVER_TYPE_C    0x04
#define SD_DRIVER_TYPE_D    0x08

/*
 * SD bus current limits
 */
#define SD_SET_CURRENT_LIMIT_200    0
#define SD_SET_CURRENT_LIMIT_400    1
#define SD_SET_CURRENT_LIMIT_600    2
#define SD_SET_CURRENT_LIMIT_800    3
#define SD_SET_CURRENT_NO_CHANGE    (-1)
#define SD_MAX_CURRENT_200          (1 << SD_SET_CURRENT_LIMIT_200)
#define SD_MAX_CURRENT_400          (1 << SD_SET_CURRENT_LIMIT_400)
#define SD_MAX_CURRENT_600          (1 << SD_SET_CURRENT_LIMIT_600)
#define SD_MAX_CURRENT_800          (1 << SD_SET_CURRENT_LIMIT_800)

/*
 * SD_SWITCH mode
 */
#define SD_SWITCH_CHECK     0
#define SD_SWITCH_SET       1

/*
 * SD_SWITCH function groups
 */
#define SD_SWITCH_GRP_ACCESS    0

/*
 * SD_SWITCH access modes
 */
#define SD_SWITCH_ACCESS_DEF    0
#define SD_SWITCH_ACCESS_HS     1

#endif /* LINUX_MMC_SD_H */
