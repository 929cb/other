/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
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

#ifndef __USB_DISK_SDCARD_H__
#define __USB_DISK_SDCARD_H__ 1

/*******************************************************************************
* Definitions
******************************************************************************/
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif

#if defined(__GIC_PRIO_BITS)
#define USB_DEVICE_INTERRUPT_PRIORITY (25U)
#else
#define USB_DEVICE_INTERRUPT_PRIORITY (3U)
#endif
/*! @brief enable the write task. 1U supported, 0U not supported . if this macro is enable ,the
USB_DEVICE_CONFIG_USE_TASK macro should also be enable.*/
#define USB_DEVICE_MSC_USE_WRITE_TASK (0U)

/* Length of Each Logical Address Block */
#define LOGICAL_UNIT_SUPPORTED (1U)

#define USB_DEVICE_MSC_WRITE_BUFF_NUM (3U)
/*buffer size for sd card example. the larger the buffer size ,the faster the data transfer speed is ,*/
/*the block size should be multiple of 512, the least value is 512*/
#define USB_DEVICE_MSC_WRITE_BUFF_SIZE (512U)
#define USB_DEVICE_MSC_READ_BUFF_SIZE (512U)

#define USB_DEVICE_SDCARD_BLOCK_SIZE_POWER (9U)
#define USB_DEVICE_MSC_ADMA_TABLE_WORDS (8U)
typedef struct _usb_msc_buffer_struct
{
    uint32_t offset; /*!< Offset of the block need to access*/
    uint32_t size;   /*!< Size of the transfered data*/
    struct _usb_msc_buffer_struct *next;
    uint8_t buffer[USB_DEVICE_MSC_WRITE_BUFF_SIZE]; /*!< Buffer address of the transferred data*/
} usb_msc_buffer_struct_t;

typedef struct _usb_msc_struct
{
    usb_device_handle deviceHandle;
    class_handle_t mscHandle;
    uint8_t diskLock;
    uint8_t readWriteError;
    uint8_t currentConfiguration;
    uint8_t speed;
    uint8_t attach;
} usb_msc_struct_t;

#endif /* _USB_MSC_DISK_H_ */
