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

// Bosch BMM350 magnetometer (I2C). Setup and compensation come from Bosch's
// BMM350 SensorAPI (lib/main/BoschSensortec/BMM350-Sensor-API); this file
// supplies the bus callbacks and a non-blocking read.

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "platform.h"

#if defined(USE_MAG_BMM350)

#include "common/axis.h"
#include "common/maths.h"
#include "drivers/bus.h"
#include "drivers/time.h"

#include "compass.h"
#include "compass_bmm350.h"

#include "bmm350.h"

// Every BMM350 I2C read starts with two dummy bytes (datasheet 7.2.2).
#define BMM350_DATA_READ_LEN (BMM350_DUMMY_BYTES + BMM350_MAG_TEMP_DATA_LEN)

static struct bmm350_dev bmm350;
static uint8_t dataBuf[BMM350_DATA_READ_LEN];

static BMM350_INTF_RET_TYPE bmm350BusRead(uint8_t reg, uint8_t *data, uint32_t len, void *intf)
{
    return busReadRegisterBuffer((extDevice_t *)intf, reg, data, len) ? BMM350_INTF_RET_SUCCESS : BMM350_E_COM_FAIL;
}

static BMM350_INTF_RET_TYPE bmm350BusWrite(uint8_t reg, const uint8_t *data, uint32_t len, void *intf)
{
    // Registers auto-increment, so a burst equals byte-by-byte writes.
    for (uint32_t i = 0; i < len; i++) {
        if (!busWriteRegister((extDevice_t *)intf, reg + i, data[i])) {
            return BMM350_E_COM_FAIL;
        }
    }
    return BMM350_INTF_RET_SUCCESS;
}

// Serves the API's data read from the buffer the non-blocking transfer filled.
static BMM350_INTF_RET_TYPE bmm350BufferRead(uint8_t reg, uint8_t *data, uint32_t len, void *intf)
{
    UNUSED(intf);
    if (reg != BMM350_REG_MAG_X_XLSB || len > sizeof(dataBuf)) {
        return BMM350_E_COM_FAIL;
    }
    memcpy(data, dataBuf, len);
    return BMM350_INTF_RET_SUCCESS;
}

static void bmm350Delay(uint32_t us, void *intf)
{
    UNUSED(intf);
    delayMicroseconds(us);
}

static bool bmm350Init(magDev_t *mag)
{
    extDevice_t *dev = &mag->dev;

    busDeviceRegister(dev);

    bmm350.intf_ptr = dev;
    bmm350.read = bmm350BusRead;
    bmm350.write = bmm350BusWrite;
    bmm350.delay_us = bmm350Delay;

    // Soft reset, OTP compensation download and magnetic reset, then 100 Hz
    // with 2x averaging (the most averaging the API allows at 100 Hz besides 4x).
    if (bmm350_init(&bmm350) != BMM350_OK
        || bmm350_set_odr_performance(BMM350_DATA_RATE_100HZ, BMM350_AVERAGING_2, &bmm350) != BMM350_OK
        || bmm350_enable_axes(BMM350_X_EN, BMM350_Y_EN, BMM350_Z_EN, &bmm350) != BMM350_OK
        || bmm350_set_powermode(BMM350_NORMAL_MODE, &bmm350) != BMM350_OK) {
        return false;
    }

    mag->magOdrHz = 100;
    return true;
}

static int16_t bmm350ToAdc(float microTesla)
{
    // 0.1 uT (1 mG) per count, so Earth's field reads about 250-650.
    return (int16_t)constrainf(microTesla * 10.0f, INT16_MIN, INT16_MAX);
}

static bool bmm350Read(magDev_t *mag, int16_t *magData)
{
    static bool pendingRead = true;
    extDevice_t *dev = &mag->dev;

    if (pendingRead) {
        if (busReadRegisterBufferStart(dev, BMM350_REG_MAG_X_XLSB, dataBuf, sizeof(dataBuf))) {
            pendingRead = false;
        }
        return false;
    }

    if (busBusy(dev, NULL)) {
        return false;
    }
    pendingRead = true;

    struct bmm350_mag_temp_data out;
    bmm350.read = bmm350BufferRead;
    const int8_t rslt = bmm350_get_compensated_mag_xyz_temp_data(&out, &bmm350);
    bmm350.read = bmm350BusRead;

    if (rslt != BMM350_OK) {
        return false;
    }

    magData[X] = bmm350ToAdc(out.x);
    magData[Y] = bmm350ToAdc(out.y);
    magData[Z] = bmm350ToAdc(out.z);

    return true;
}

bool bmm350Detect(magDev_t *mag)
{
    extDevice_t *dev = &mag->dev;

    if (dev->bus->busType != BUS_TYPE_I2C) {
        return false;
    }

    if (dev->busType_u.i2c.address == 0) {
        dev->busType_u.i2c.address = BMM350_I2C_ADSEL_SET_LOW;
    }

    uint8_t id[BMM350_DUMMY_BYTES + 1];
    if (!busReadRegisterBuffer(dev, BMM350_REG_CHIP_ID, id, sizeof(id)) || id[BMM350_DUMMY_BYTES] != BMM350_CHIP_ID) {
        return false;
    }

    mag->init = bmm350Init;
    mag->read = bmm350Read;

    return true;
}

#endif // USE_MAG_BMM350
