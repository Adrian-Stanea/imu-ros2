#include "adi_imu/device_handler.h"
#include "adi_imu/device_registry.h"
#include <stdexcept>
#include <chrono>
#include <thread>

// For IIO operations - we'll need to include the actual IIO headers
extern "C" {
#include <iio.h>
}

BaseDeviceHandler::BaseDeviceHandler(std::shared_ptr<DeviceConfig> config)
    : config_(config), iio_context_(nullptr), iio_device_(nullptr) {
    if (!config_) {
        throw std::invalid_argument("Device configuration cannot be null");
    }
}

std::shared_ptr<DeviceConfig> BaseDeviceHandler::getConfig() const {
    return config_;
}

std::string BaseDeviceHandler::getDeviceName() const {
    return config_ ? config_->getName() : "";
}

std::string BaseDeviceHandler::getDeviceFamily() const {
    return config_ ? config_->getFamily() : "";
}

std::string BaseDeviceHandler::getLastError() const {
    return last_error_;
}

bool BaseDeviceHandler::setIIOContext(struct iio_context* context, struct iio_device* device) {
    if (!context || !device) {
        setLastError("Invalid IIO context or device");
        return false;
    }

    iio_context_ = context;
    iio_device_ = device;

    return validateDeviceConnection();
}

bool BaseDeviceHandler::validateDeviceConnection() const {
    if (!iio_device_) {
        setLastError("IIO device not set");
        return false;
    }

    // Try to read product ID to verify connection
    uint16_t product_id = const_cast<BaseDeviceHandler*>(this)->readRegister(config_->getRegisterAddress("prod_id"));
    if (product_id == 0) {
        const_cast<BaseDeviceHandler*>(this)->setLastError("Failed to read product ID - device not responding");
        return false;
    }

    return true;
}

bool BaseDeviceHandler::waitForDeviceReady(int timeout_ms) {
    auto start_time = std::chrono::steady_clock::now();
    auto timeout_duration = std::chrono::milliseconds(timeout_ms);

    while (std::chrono::steady_clock::now() - start_time < timeout_duration) {
        if (validateDeviceConnection()) {
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    setLastError("Device ready timeout");
    return false;
}

uint16_t BaseDeviceHandler::readRegister(uint16_t reg_addr) {
    if (!iio_device_) {
        setLastError("IIO device not initialized");
        return 0;
    }

    // This is a simplified implementation
    // In reality, you'd use the actual IIO library functions
    // For now, we'll return a placeholder

    // TODO: Implement actual IIO register read
    // Example:
    // uint16_t value = 0;
    // int ret = iio_device_reg_read(iio_device_, reg_addr, &value);
    // if (ret < 0) {
    //     setLastError("Failed to read register: " + std::to_string(reg_addr));
    //     return 0;
    // }
    // return value;

    return 0; // Placeholder
}

bool BaseDeviceHandler::writeRegister(uint16_t reg_addr, uint16_t value) {
    if (!iio_device_) {
        setLastError("IIO device not initialized");
        return false;
    }

    // This is a simplified implementation
    // In reality, you'd use the actual IIO library functions

    // TODO: Implement actual IIO register write
    // Example:
    // int ret = iio_device_reg_write(iio_device_, reg_addr, value);
    // if (ret < 0) {
    //     setLastError("Failed to write register: " + std::to_string(reg_addr));
    //     return false;
    // }
    // return true;

    return true; // Placeholder
}

double BaseDeviceHandler::convertToPhysicalUnits(int32_t raw_data, const std::string& measurement_type) {
    try {
        double scale = config_->getScaleFactor(measurement_type);
        return static_cast<double>(raw_data) * scale;
    } catch (const std::exception& e) {
        setLastError("Failed to convert to physical units: " + std::string(e.what()));
        return 0.0;
    }
}

bool BaseDeviceHandler::checkDeviceCapability(const std::string& capability) const {
    return config_ ? config_->supportsFeature(capability) : false;
}

uint16_t BaseDeviceHandler::getRegisterAddress(const std::string& register_name) const {
    try {
        return config_->getRegisterAddress(register_name);
    } catch (const std::exception& e) {
        return 0;
    }
}

double BaseDeviceHandler::getScaleFactor(const std::string& measurement_type) const {
    try {
        return config_->getScaleFactor(measurement_type);
    } catch (const std::exception& e) {
        return 1.0;
    }
}

void BaseDeviceHandler::setLastError(const std::string& error) const {
    last_error_ = error;
}
