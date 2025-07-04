/*******************************************************************************
 *   @file   adis1654x_handler.cpp
 *   @brief  Implementation for ADIS1654x family device handler
 ********************************************************************************/

#include "adi_imu/adis1654x_handler.h"
#include "adi_imu/adis1654x/adis1654x_data_access.h"
#include <cstring>

namespace adi_imu {

ADIS1654xHandler::ADIS1654xHandler(std::shared_ptr<DeviceConfig> config)
    : BaseDeviceHandler(config) {
    // Initialize device-specific configuration
    if (config_) {
        DeviceCapabilities caps;
        caps.has_delta_burst = true;
        caps.has_temperature = true;
        caps.has_gyro_fail_detection = true;
        caps.has_accel_fail_detection = true;
        caps.has_diagnostics = true;
        caps.has_accelerometer = true;
        caps.has_gyroscope = true;
        caps.has_delta_velocity = true;
        caps.has_delta_angle = true;
        config_->setCapabilities(caps);

        DevicePerformance perf;
        perf.max_sampling_freq = MAX_SAMPLE_FREQUENCY;
        perf.flash_memory_endurance = FLASH_MEMORY_ENDURANCE;
        config_->setPerformance(perf);
    }
}

bool ADIS1654xHandler::initializeDevice() {
    setLastError("");

    if (!iio_device_) {
        setLastError("No IIO device set");
        return false;
    }

    return true;
}

bool ADIS1654xHandler::configureDevice() {
    return configureDevice(0.0, true, true);
}

bool ADIS1654xHandler::configureDevice(double sampling_freq, bool enable_delta_burst, bool enable_temperature) {
    setLastError("");

    if (!iio_device_) {
        setLastError("No IIO device set");
        return false;
    }

    return true;
}

int ADIS1654xHandler::readRawData(void* buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) {
        setLastError("Invalid buffer parameters");
        return -1;
    }

    if (!iio_device_) {
        setLastError("No IIO device set");
        return -1;
    }

    return 0;
}

int ADIS1654xHandler::readBurstData(void* buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) {
        setLastError("Invalid buffer parameters");
        return -1;
    }

    if (!iio_device_) {
        setLastError("No IIO device set");
        return -1;
    }

    return 0;
}

uint16_t ADIS1654xHandler::readDiagnosticStatus() {
    if (!iio_device_) {
        setLastError("No IIO device set");
        return 0;
    }

    return 0;
}

uint16_t ADIS1654xHandler::readProductId() {
    if (!iio_device_) {
        setLastError("No IIO device set");
        return 0;
    }

    return 0;
}

uint16_t ADIS1654xHandler::readSerialNumber() {
    if (!iio_device_) {
        setLastError("No IIO device set");
        return 0;
    }

    return 0;
}

bool ADIS1654xHandler::performSelfTest() {
    if (!iio_device_) {
        setLastError("No IIO device set");
        return false;
    }

    return true;
}

bool ADIS1654xHandler::resetDevice() {
    if (!iio_device_) {
        setLastError("No IIO device set");
        return false;
    }

    return true;
}

bool ADIS1654xHandler::isDeviceConnected() const {
    return iio_device_ != nullptr && validateDeviceConnection();
}

} // namespace adi_imu
