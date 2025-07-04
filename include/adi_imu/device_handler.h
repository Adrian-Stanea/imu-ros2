#ifndef DEVICE_HANDLER_H
#define DEVICE_HANDLER_H

#include <memory>
#include <string>
#include <cstdint>
#include <vector>
#include "device_config.h"

// Forward declarations
struct iio_device;
struct iio_context;

namespace adi_imu {

/**
 * @brief Abstract Device Handler Interface
 * Provides a common interface for all ADIS device handlers
 */
class DeviceHandler {
public:
    virtual ~DeviceHandler() = default;

    /**
     * @brief Initialize the device
     * @return True if initialization successful, false otherwise
     */
    virtual bool initializeDevice() = 0;

    /**
     * @brief Configure the device with default settings
     * @return True if configuration successful, false otherwise
     */
    virtual bool configureDevice() = 0;

    /**
     * @brief Configure the device with custom settings
     * @param sampling_freq Desired sampling frequency in Hz (0 for default)
     * @param enable_delta_burst Enable delta burst mode
     * @param enable_temperature Enable temperature measurement
     * @return True if configuration successful, false otherwise
     */
    virtual bool configureDevice(
        double sampling_freq,
        bool enable_delta_burst,
        bool enable_temperature
    ) = 0;

    /**
     * @brief Read raw data from the device
     * @param buffer Buffer to store raw data
     * @param buffer_size Size of the buffer
     * @return Number of bytes read, -1 on error
     */
    virtual int readRawData(void* buffer, size_t buffer_size) = 0;

    /**
     * @brief Read burst data from the device
     * @param buffer Buffer to store burst data
     * @param buffer_size Size of the buffer
     * @return Number of bytes read, -1 on error
     */
    virtual int readBurstData(void* buffer, size_t buffer_size) = 0;

    /**
     * @brief Read device diagnostic status
     * @return Diagnostic status value, 0 if no errors
     */
    virtual uint16_t readDiagnosticStatus() = 0;

    /**
     * @brief Read device product ID
     * @return Product ID value
     */
    virtual uint16_t readProductId() = 0;

    /**
     * @brief Read device serial number
     * @return Serial number value
     */
    virtual uint16_t readSerialNumber() = 0;

    /**
     * @brief Perform device self-test
     * @return True if self-test passed, false otherwise
     */
    virtual bool performSelfTest() = 0;

    /**
     * @brief Reset the device
     * @return True if reset successful, false otherwise
     */
    virtual bool resetDevice() = 0;

    /**
     * @brief Get device configuration
     * @return Shared pointer to device configuration
     */
    virtual std::shared_ptr<DeviceConfig> getConfig() const = 0;

    /**
     * @brief Get device name
     * @return Device name string
     */
    virtual std::string getDeviceName() const = 0;

    /**
     * @brief Get device family
     * @return Device family string
     */
    virtual std::string getDeviceFamily() const = 0;

    /**
     * @brief Check if device is connected and responsive
     * @return True if device is connected, false otherwise
     */
    virtual bool isDeviceConnected() const = 0;

    /**
     * @brief Get last error message
     * @return Last error message string
     */
    virtual std::string getLastError() const = 0;

    /**
     * @brief Set IIO context and device
     * @param context IIO context
     * @param device IIO device
     * @return True if set successfully, false otherwise
     */
    virtual bool setIIOContext(struct iio_context* context, struct iio_device* device) = 0;

protected:
    /**
     * @brief Read register value
     * @param reg_addr Register address
     * @return Register value, 0 on error
     */
    virtual uint16_t readRegister(uint16_t reg_addr) = 0;

    /**
     * @brief Write register value
     * @param reg_addr Register address
     * @param value Value to write
     * @return True if write successful, false otherwise
     */
    virtual bool writeRegister(uint16_t reg_addr, uint16_t value) = 0;

    /**
     * @brief Convert raw data to physical units
     * @param raw_data Raw data value
     * @param measurement_type Type of measurement (e.g., "gyro", "accel")
     * @return Converted value in physical units
     */
    virtual double convertToPhysicalUnits(int32_t raw_data, const std::string& measurement_type) = 0;
};

/**
 * @brief Base implementation of DeviceHandler
 * Provides common functionality for all device handlers
 */
class BaseDeviceHandler : public DeviceHandler {
public:
    explicit BaseDeviceHandler(std::shared_ptr<DeviceConfig> config);
    virtual ~BaseDeviceHandler() = default;

    // Common implementations
    std::shared_ptr<DeviceConfig> getConfig() const override;
    std::string getDeviceName() const override;
    std::string getDeviceFamily() const override;
    std::string getLastError() const override;
    bool setIIOContext(struct iio_context* context, struct iio_device* device) override;

    // Common utility methods
    bool validateDeviceConnection() const;
    bool waitForDeviceReady(int timeout_ms = 1000);

protected:
    // Protected members
    std::shared_ptr<DeviceConfig> config_;
    struct iio_context* iio_context_;
    struct iio_device* iio_device_;
    mutable std::string last_error_;

    // Protected utility methods
    uint16_t readRegister(uint16_t reg_addr) override;
    bool writeRegister(uint16_t reg_addr, uint16_t value) override;
    double convertToPhysicalUnits(int32_t raw_data, const std::string& measurement_type) override;

    // Helper methods
    bool checkDeviceCapability(const std::string& capability) const;
    uint16_t getRegisterAddress(const std::string& register_name) const;
    double getScaleFactor(const std::string& measurement_type) const;
    void setLastError(const std::string& error) const;
};
} // namespace adi_imu
#endif // DEVICE_HANDLER_H
