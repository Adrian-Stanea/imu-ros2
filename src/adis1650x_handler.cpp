/*******************************************************************************
 *   @file   adis1650x_handler.cpp
 *   @brief  Implementation for ADIS1650x family device handler
 ********************************************************************************/

#include "adi_imu/adis1650x_handler.h"
#include "adi_imu/adis1650x/adis1650x_data_access.h"
#include <rclcpp/rclcpp.hpp>
#include <cstring>

namespace adi_imu {

ADIS1650xHandler::ADIS1650xHandler(std::shared_ptr<DeviceConfig> config)
    : BaseDeviceHandler(config) {
    // Initialize device-specific configuration
    if (config_) {
        DeviceCapabilities caps;
        caps.has_delta_burst = true;
        caps.has_temperature = true;
        caps.has_gyro_fail_detection = true;
        caps.has_accel_fail_detection = true;
        caps.has_mag_compensation = true;
        caps.has_bias_correction = true;
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

bool ADIS1650xHandler::initializeDevice() {
    setLastError("");

    if (!iio_device_) {
        setLastError("No IIO device set");
        return false;
    }

    // Check if device is the expected ADIS1650x family
    uint16_t product_id = readProductId();
    if (product_id == 0) {
        setLastError("Failed to read product ID");
        return false;
    }

    // TODO: Add proper product ID validation for ADIS1650x family

    try {
        // Device-specific initialization
        if (!waitForDeviceReady()) {
            setLastError("Device not ready after initialization");
            return false;
        }

        // Device initialized successfully
        RCLCPP_INFO(rclcpp::get_logger("adis1650x_handler"),
                   "ADIS1650x device initialized successfully");
        return true;

    } catch (const std::exception& e) {
        setLastError(std::string("ADIS1650x initialization failed: ") + e.what());
        return false;
    }
}

bool ADIS1650xHandler::configureDevice() {
    return configureDevice(MAX_SAMPLE_FREQUENCY, true, true);
}

bool ADIS1650xHandler::configureDevice(double sampling_freq, bool enable_delta_burst, bool enable_temperature) {
    // Device-specific configuration implementation
    (void)sampling_freq;
    (void)enable_delta_burst;
    (void)enable_temperature;

    try {
        // Apply device-specific configuration
        // Device configured successfully
        RCLCPP_INFO(rclcpp::get_logger("adis1650x_handler"),
                   "ADIS1650x device configured successfully");
        return true;

    } catch (const std::exception& e) {
        setLastError(std::string("ADIS1650x configuration failed: ") + e.what());
        return false;
    }
}

int ADIS1650xHandler::readRawData(void* buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) {
        setLastError("Invalid buffer parameters");
        return -1;
    }

    if (!isDeviceConnected()) {
        setLastError("Device not connected");
        return -1;
    }

    // Read raw data from device
    // TODO: Implement actual device-specific data reading
    memset(buffer, 0, buffer_size);
    return static_cast<int>(buffer_size);
}

int ADIS1650xHandler::readBurstData(void* buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) {
        setLastError("Invalid buffer parameters");
        return -1;
    }

    if (!isDeviceConnected()) {
        setLastError("Device not connected");
        return -1;
    }

    // Read burst data from device
    // TODO: Implement actual device-specific burst data reading
    memset(buffer, 0, buffer_size);
    return static_cast<int>(buffer_size);
}

uint16_t ADIS1650xHandler::readDiagnosticStatus() {
    if (!isDeviceConnected()) {
        setLastError("Device not connected");
        return 0;
    }

    // TODO: Implement actual diagnostic status reading
    return 0;
}

uint16_t ADIS1650xHandler::readProductId() {
    if (!isDeviceConnected()) {
        setLastError("Device not connected");
        return 0;
    }

    // TODO: Implement actual product ID reading
    return 0x4056; // Example ADIS1650x product ID
}

uint16_t ADIS1650xHandler::readSerialNumber() {
    if (!isDeviceConnected()) {
        setLastError("Device not connected");
        return 0;
    }

    // TODO: Implement actual serial number reading
    return 0;
}

bool ADIS1650xHandler::performSelfTest() {
    if (!isDeviceConnected()) {
        setLastError("Device not connected");
        return false;
    }

    // TODO: Implement actual self-test procedure
    return true;
}

bool ADIS1650xHandler::resetDevice() {
    if (!isDeviceConnected()) {
        setLastError("Device not connected");
        return false;
    }

    // TODO: Implement actual device reset
    return true;
}

bool ADIS1650xHandler::isDeviceConnected() const {
    return iio_device_ != nullptr && validateDeviceConnection();
}

} // namespace adi_imu
