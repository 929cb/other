/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_pxp.h"
#include "fsl_elcdif.h"

#include "pin_mux.h"
#include "fsl_gpio.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_ELCDIF LCDIF
#define APP_PXP PXP

#define APP_IMG_HEIGHT 272
#define APP_IMG_WIDTH 480
#define APP_HSW 41
#define APP_HFP 4
#define APP_HBP 8
#define APP_VSW 10
#define APP_VFP 4
#define APP_VBP 2
#define APP_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)

/* Display. */
#define LCD_DISP_GPIO GPIO5
#define LCD_DISP_GPIO_PIN 9
/* Back light. */
#define LCD_BL_GPIO GPIO1
#define LCD_BL_GPIO_PIN 8

/*
 * Frame buffer data alignment.
 * The PXP input buffer, output buffer, and LCDIF frame buffer address 64B align.
 */
#define FRAME_BUFFER_ALIGN 64


#define APP_BPP 4U /* Use 24-bit RGB888 format. */

/* The pixel format. */
typedef struct _pixel
{
    uint32_t B : 8;
    uint32_t G : 8;
    uint32_t R : 8;
    uint8_t : 8;
} pixel_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_InitInputBuffer(void);
static void APP_InitLcdif(void);
static void APP_InitPxp(void);
static void APP_DitherOrdered(void);
static void APP_DitherQuantOnly(void);
static void APP_DitherPassThrough(void);
static void APP_UpdateFrame(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_BufferLcd[2][APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static pixel_t s_BufferPxp[APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);

/* PXP Output buffer config. */
static pxp_output_buffer_config_t outputBufferConfig;

/* Ordered dithering index matrix. */
static uint8_t s_matrix8[8][8];

/* Active frame buffer index for LCD. */
static uint8_t s_lcdActiveFbIdx = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize the LCD_DISP. */
void BOARD_InitLcd(void)
{
    volatile uint32_t i = 0x100U;

    gpio_pin_config_t config = {
        kGPIO_DigitalOutput, 0,
    };

    /* Reset the LCD. */
    GPIO_PinInit(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, &config);

    GPIO_WritePinOutput(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 0);

    while (i--)
    {
    }

    GPIO_WritePinOutput(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 1);

    /* Backlight. */
    config.outputLogic = 1;
    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);
}

void BOARD_InitLcdifPixelClock(void)
{
    /*
     * The desired output frame rate is 60Hz. So the pixel clock frequency is:
     * (480 + 41 + 4 + 18) * (272 + 10 + 4 + 2) * 60 = 9.2M.
     * Here set the LCDIF pixel clock to 9.3M.
     */

    /*
     * Initialize the Video PLL.
     * Video PLL output clock is OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 93MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 31, .postDivider = 8, .numerator = 0, .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_Lcdif1PreMux, 2);

    CLOCK_SetDiv(kCLOCK_Lcdif1PreDiv, 4);

    CLOCK_SetDiv(kCLOCK_Lcdif1Div, 1);

    /*
     * 000 derive clock from divided pre-muxed lcdif1 clock
     * 001 derive clock from ipp_di0_clk
     * 010 derive clock from ipp_di1_clk
     * 011 derive clock from ldb_di0_clk
     * 100 derive clock from ldb_di1_clk
     */
    CLOCK_SetMux(kCLOCK_Lcdif1Mux, 0);
}



int main(void)
{
    BOARD_InitPins();
    BOARD_LCDIF_ConfigurePins();
    BOARD_BootClockRUN();
    BOARD_InitLcdifPixelClock();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    BOARD_InitLcd();

    PRINTF("\r\nPXP dither example start...\r\n");

    APP_InitInputBuffer();
    APP_InitPxp();
    APP_InitLcdif();

    while (1)
    {
        APP_DitherPassThrough();
        APP_DitherQuantOnly();
        APP_DitherOrdered();
    }
}

static void APP_InitLcdif(void)
{
    const elcdif_rgb_mode_config_t config = {
        .panelWidth = APP_IMG_WIDTH,
        .panelHeight = APP_IMG_HEIGHT,
        .hsw = APP_HSW,
        .hfp = APP_HFP,
        .hbp = APP_HBP,
        .vsw = APP_VSW,
        .vfp = APP_VFP,
        .vbp = APP_VBP,
        .polarityFlags = APP_POL_FLAGS,
        .bufferAddr = (uint32_t)s_BufferLcd[s_lcdActiveFbIdx],
        .pixelFormat = kELCDIF_PixelFormatRGB888,
        .dataBus = kELCDIF_DataBus24Bit,
    };

    ELCDIF_RgbModeInit(APP_ELCDIF, &config);

    ELCDIF_RgbModeStart(APP_ELCDIF);
}

static void APP_InitPxp(void)
{
    PXP_Init(APP_PXP);

    /* Output config. */
    outputBufferConfig.pixelFormat = kPXP_OutputPixelFormatRGB888;
    outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
    outputBufferConfig.buffer1Addr = 0U;
    outputBufferConfig.pitchBytes = APP_IMG_WIDTH * APP_BPP;
    outputBufferConfig.width = APP_IMG_WIDTH;
    outputBufferConfig.height = APP_IMG_HEIGHT;

    /* PS configure. */
    const pxp_ps_buffer_config_t psBufferConfig = {
        .pixelFormat = kPXP_PsPixelFormatRGB888,
        .swapByte = false,
        .bufferAddr = (uint32_t)s_BufferPxp,
        .bufferAddrU = 0U,
        .bufferAddrV = 0U,
        .pitchBytes = APP_IMG_WIDTH * APP_BPP,
    };

    PXP_SetProcessSurfaceBackGroundColor(APP_PXP, 0U);

    PXP_SetProcessSurfaceBufferConfig(APP_PXP, &psBufferConfig);
    PXP_SetProcessSurfacePosition(APP_PXP, 0U, 0U, APP_IMG_WIDTH - 1U, APP_IMG_HEIGHT - 1U);

    /* Disable AS. */
    PXP_SetAlphaSurfacePosition(APP_PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);

    /* Disable CSC1, it is enabled by default. */
    PXP_EnableCsc1(APP_PXP, false);

    PXP_EnableDither(APP_PXP, true);

    PXP_SetProcessBlockSize(APP_PXP, kPXP_BlockSize8);

    /* Set the Ordered dithering index matrix. */
    PXP_SetInternalRamData(APP_PXP, kPXP_RamDither0Lut, 8 * 8, (uint8_t *)s_matrix8, 0U);
    PXP_SetInternalRamData(APP_PXP, kPXP_RamDither1Lut, 8 * 8, (uint8_t *)s_matrix8, 0U);
    PXP_SetInternalRamData(APP_PXP, kPXP_RamDither2Lut, 8 * 8, (uint8_t *)s_matrix8, 0U);
}

static void APP_DitherPassThrough(void)
{
    const pxp_dither_config_t config = {.enableDither0 = 1,
                                        .enableDither1 = 1,
                                        .enableDither2 = 1,
                                        .ditherMode0 = kPXP_DitherPassThrough,
                                        .ditherMode1 = kPXP_DitherPassThrough,
                                        .ditherMode2 = kPXP_DitherPassThrough,
                                        .quantBitNum = 1,
                                        .lutMode = kPXP_DitherLutOff,
                                        .idxMatrixSize0 = kPXP_DitherMatrix8,
                                        .idxMatrixSize1 = kPXP_DitherMatrix8,
                                        .idxMatrixSize2 = kPXP_DitherMatrix8,
                                        .enableFinalLut = 0};

    PRINTF("Press any key to see the original picture...\r\n");
    GETCHAR();

    PXP_SetDitherConfig(APP_PXP, &config);

    APP_UpdateFrame();
}

static void APP_DitherQuantOnly(void)
{
    const pxp_dither_config_t config = {.enableDither0 = 1,
                                        .enableDither1 = 1,
                                        .enableDither2 = 1,
                                        .ditherMode0 = kPXP_DitherQuantOnly,
                                        .ditherMode1 = kPXP_DitherQuantOnly,
                                        .ditherMode2 = kPXP_DitherQuantOnly,
                                        .quantBitNum = 4,
                                        .lutMode = kPXP_DitherLutOff,
                                        .idxMatrixSize0 = kPXP_DitherMatrix8,
                                        .idxMatrixSize1 = kPXP_DitherMatrix8,
                                        .idxMatrixSize2 = kPXP_DitherMatrix8,
                                        .enableFinalLut = 0};

    PRINTF("Press any key to see the quantized picture...\r\n");
    GETCHAR();

    PXP_SetDitherConfig(APP_PXP, &config);

    APP_UpdateFrame();
}

static void APP_DitherOrdered(void)
{
    const pxp_dither_config_t config = {.enableDither0 = 1,
                                        .enableDither1 = 1,
                                        .enableDither2 = 1,
                                        .ditherMode0 = kPXP_DitherOrdered,
                                        .ditherMode1 = kPXP_DitherOrdered,
                                        .ditherMode2 = kPXP_DitherOrdered,
                                        .quantBitNum = 4,
                                        .lutMode = kPXP_DitherLutOff,
                                        .idxMatrixSize0 = kPXP_DitherMatrix8,
                                        .idxMatrixSize1 = kPXP_DitherMatrix8,
                                        .idxMatrixSize2 = kPXP_DitherMatrix8,
                                        .enableFinalLut = 0};

    PRINTF("Press any key to see the ordered dithering picture...\r\n");
    GETCHAR();

    PXP_SetDitherConfig(APP_PXP, &config);

    APP_UpdateFrame();
}

static void APP_InitInputBuffer(void)
{
    uint32_t i, j;
    uint8_t R = 0;
    uint8_t G = 0;

    const uint8_t matrix4[4][4] = {
        {0, 8, 2, 10}, {12, 4, 14, 6}, {3, 11, 1, 9}, {15, 7, 13, 5},
    };

    memset(s_BufferLcd, 0x0U, sizeof(s_BufferLcd));
    memset(s_BufferPxp, 0x0U, sizeof(s_BufferPxp));

    /* Init the input buffer. */
    for (i = 0; i < APP_IMG_HEIGHT; i++)
    {
        for (j = 0; j < APP_IMG_WIDTH; j++)
        {
            s_BufferPxp[i][j].R = R;
            s_BufferPxp[i][j].G = G;
            G++;
        }
        G = 0;
        R++;
    }

    /* Init the ordered dithering input matrix. */
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            s_matrix8[i][j] = 4 * matrix4[i][j];
            s_matrix8[i][j + 4] = 4 * matrix4[i][j] + 2;
            s_matrix8[i + 4][j] = 4 * matrix4[i][j] + 3;
            s_matrix8[i + 4][j + 4] = 4 * matrix4[i][j] + 1;
        }
    }
}

static void APP_UpdateFrame(void)
{
    s_lcdActiveFbIdx ^= 1;
    outputBufferConfig.buffer0Addr = (uint32_t)s_BufferLcd[s_lcdActiveFbIdx];
    PXP_SetOutputBufferConfig(APP_PXP, &outputBufferConfig);

    /* Start PXP process and wait for complete. */
    PXP_ClearStatusFlags(APP_PXP, kPXP_CompleteFlag);
    PXP_Start(APP_PXP);
    while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(APP_PXP)))
    {
    }

    /* Set the new frame buffer to LCD and wait for active. */
    ELCDIF_SetNextBufferAddr(APP_ELCDIF, (uint32_t)s_BufferLcd[s_lcdActiveFbIdx]);
    ELCDIF_ClearInterruptStatus(APP_ELCDIF, kELCDIF_CurFrameDone);
    while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(APP_ELCDIF)))
    {
    }
}
