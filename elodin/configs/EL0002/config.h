/*
 * This file is part of Betaflight.
 *
 * Betaflight is free software. You can redistribute this software
 * and/or modify this software under the terms of the GNU General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later
 * version.
 *
 * Betaflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

// Elodin EL0002 (Aleph FC) rev 6: STM32H747XIH6, Betaflight on the M7 core.

#pragma once

#define FC_TARGET_MCU       STM32H757   // H747 is the H757 without crypto

#define BOARD_NAME          EL0002
#define MANUFACTURER_ID     ELOD

#define USE_H7_LDO_SUPPLY               // VDDSMPS grounded
#define SYSTEM_HSE_MHZ      24
#define USE_USB_CLOCK_PLL3

#define TARGET_IO_PORTI     0xffff
#define TARGET_IO_PORTJ     0xffff
#define TARGET_IO_PORTK     0xffff

#define USE_SPI_DEVICE_5
#define USE_I2C_DEVICE_4

#define USE_ACC
#define USE_GYRO
#define USE_ACCGYRO_BMI270
#define USE_BARO
#define USE_BARO_BMP581                 // U15; the BMP384 alternate uses the BMP388 driver
#define USE_BARO_BMP388
#define USE_MAG
#define USE_MAG_BMM350                  // U19; external compasses on J7 (I2C4) via mag_i2c_device

#define SPI5_SCK_PIN        PK0
#define SPI5_SDI_PIN        PJ11
#define SPI5_SDO_PIN        PJ10
#define GYRO_1_SPI_INSTANCE SPI5
#define GYRO_1_CS_PIN       PK1
#define GYRO_1_EXTI_PIN     PG7
// Board frame: forward is the JST row edge (J2-J10), USB-C at the rear, top side up.
#define GYRO_1_ALIGN        CW180_DEG   // U18 BMI270: +X toward USB-C, +Y toward J12

#define I2C3_SCL_PIN        PH7
#define I2C3_SDA_PIN        PH8
#define I2C4_SCL_PIN        PH11
#define I2C4_SDA_PIN        PH12
#define BARO_I2C_INSTANCE   I2CDEV_3
#define MAG_I2C_INSTANCE    I2CDEV_3
#define MAG_ALIGN           CW270_DEG_FLIP  // U19 BMM350: +X toward J12, +Y toward USB-C, +Z down

#define UART1_TX_PIN        PA9         // carrier
#define UART1_RX_PIN        PA10
#define UART2_TX_PIN        PD5         // J14 telemetry
#define UART2_RX_PIN        PD6
#define UART3_TX_PIN        PD8         // debug probe console 1
#define UART3_RX_PIN        PD9
#define UART4_TX_PIN        PD1         // onboard ExpressLRS
#define UART4_RX_PIN        PI9
#define UART6_TX_PIN        PC6         // debug probe console 2
#define UART6_RX_PIN        PC7
#define UART7_TX_PIN        PF7         // J4 GPS
#define UART7_RX_PIN        PF6
#define UART8_TX_PIN        PE1         // J5 RC receiver
#define UART8_RX_PIN        PE0

#define SERIALRX_UART       SERIAL_PORT_UART4
#define SERIALRX_PROVIDER   SERIALRX_CRSF
#define MSP_UART            SERIAL_PORT_USART3
#define GPS_UART            SERIAL_PORT_UART7

// ExpressLRS BOOT (GPIO9) and EN, idle high
#define PINIO1_PIN          PG5
#define PINIO1_CONFIG       129
#define PINIO2_PIN          PG6
#define PINIO2_CONFIG       129

#define MOTOR1_PIN          PE9         // J6
#define MOTOR2_PIN          PE11
#define MOTOR3_PIN          PE13
#define MOTOR4_PIN          PE14
#define MOTOR5_PIN          PD12
#define MOTOR6_PIN          PD13
#define MOTOR7_PIN          PD14
#define MOTOR8_PIN          PD15
#define SERVO1_PIN          PA5         // J9
#define SERVO2_PIN          PB3
#define SERVO3_PIN          PA2
#define SERVO4_PIN          PA3
#define SERVO5_PIN          PI5
#define SERVO6_PIN          PI6
#define SERVO7_PIN          PI7
#define SERVO8_PIN          PI2

#define TIMER_PIN_MAPPING \
    TIMER_PIN_MAP( 0, PE9,  1,  0) \
    TIMER_PIN_MAP( 1, PE11, 1,  1) \
    TIMER_PIN_MAP( 2, PE13, 1,  2) \
    TIMER_PIN_MAP( 3, PE14, 1,  3) \
    TIMER_PIN_MAP( 4, PD12, 1,  4) \
    TIMER_PIN_MAP( 5, PD13, 1,  5) \
    TIMER_PIN_MAP( 6, PD14, 1,  6) \
    TIMER_PIN_MAP( 7, PD15, 1,  7) \
    TIMER_PIN_MAP( 8, PA5,  1, -1) \
    TIMER_PIN_MAP( 9, PB3,  1, -1) \
    TIMER_PIN_MAP(10, PA2,  1, -1) \
    TIMER_PIN_MAP(11, PA3,  1, -1) \
    TIMER_PIN_MAP(12, PI5,  1, -1) \
    TIMER_PIN_MAP(13, PI6,  1, -1) \
    TIMER_PIN_MAP(14, PI7,  1, -1) \
    TIMER_PIN_MAP(15, PI2,  1, -1)

#define ADC1_DMA_OPT        8
#define ADC3_DMA_OPT        12          // internal Vref, temperature
#define TIMUP1_DMA_OPT      9
#define TIMUP4_DMA_OPT      10
#define TIMUP8_DMA_OPT      11

#define LED0_PIN            PA15        // green
#define LED1_PIN            PI0         // blue
#define LED2_PIN            PE3         // red
#define LED0_INVERTED
#define LED1_INVERTED
#define LED2_INVERTED

#define USE_ADC_PA0_C
#define ADC_VBAT_PIN        PA0         // PA0_C, 147k/10k divider
#define DEFAULT_VOLTAGE_METER_SOURCE    VOLTAGE_METER_ADC
#define DEFAULT_VOLTAGE_METER_SCALE     157

#define USE_SDCARD
#define USE_SDCARD_SDIO
#define USE_SDIO_PULLUP
#define SDIO_DEVICE         SDIODEV_1
#define SDIO_USE_4BIT       1
#define SDIO_CK_PIN         PC12
#define SDIO_CMD_PIN        PD2
#define SDIO_D0_PIN         PC8
#define SDIO_D1_PIN         PC9
#define SDIO_D2_PIN         PC10
#define SDIO_D3_PIN         PC11
#define SDIO_CLOCK_DIV      4           // 198 MHz / 8 = 24.75 MHz; the default 1 gives UHS cards 99 MHz and writes fail
#define DEFAULT_BLACKBOX_DEVICE         BLACKBOX_DEVICE_SDCARD
