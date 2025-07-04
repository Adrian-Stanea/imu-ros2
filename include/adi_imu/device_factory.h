#ifndef DEVICE_FACTORY_H
#define DEVICE_FACTORY_H

#include <memory>
#include <string>
#include "device_config.h"
#include "hardware_detector.h"

// Forward declarations
class DeviceHandler;

/**
 * @brief Device Factory
 * Creates device handlers based on device type or configuration
 */
class DeviceFactory {
public:
    /**
     * @brief Create device handler by device name
     * @param device_name Name of the device (e.g., "adis16470")
     * @return Unique pointer to device handler, nullptr if creation fails
     */
    static std::unique_ptr<DeviceHandler> createDeviceHandler(const std::string& device_name);

    /**
     * @brief Create device handler by device ID
     * @param device_id Device ID enumeration
     * @return Unique pointer to device handler, nullptr if creation fails
     */
    static std::unique_ptr<DeviceHandler> createDeviceHandler(enum adis_device_id device_id);

    /**
     * @brief Create device handler from device configuration
     * @param config Device configuration
     * @return Unique pointer to device handler, nullptr if creation fails
     */
    static std::unique_ptr<DeviceHandler> createDeviceHandler(std::shared_ptr<DeviceConfig> config);

    /**
     * @brief Create device handler by auto-detection
     * This method attempts to detect the connected device and create appropriate handler
     * @param context_string IIO context string for device detection
     * @return Unique pointer to device handler, nullptr if detection/creation fails
     */
    static std::unique_ptr<DeviceHandler> createDeviceHandlerAutoDetect(const std::string& context_string);

    /**
     * @brief Create handler for specific device family using enum
     * @param family Device family enum value
     * @param config Device configuration
     * @return Unique pointer to device handler, nullptr if creation fails
     */
    static std::unique_ptr<DeviceHandler> createHandlerForFamily(
        adi_imu::DeviceFamily family,
        std::shared_ptr<DeviceConfig> config
    );

private:
    /**
     * @brief Create handler for specific device family
     * @param family Device family name (e.g., "adis1646x")
     * @param config Device configuration
     * @return Unique pointer to device handler, nullptr if creation fails
     */
    static std::unique_ptr<DeviceHandler> createHandlerForFamily(
        const std::string& family,
        std::shared_ptr<DeviceConfig> config
    );

    /**
     * @brief Detect device type from IIO context
     * @param context_string IIO context string
     * @param detected_name Output parameter for detected device name
     * @param detected_id Output parameter for detected device ID
     * @return True if device detected successfully, false otherwise
     */
    static bool detectDeviceType(
        const std::string& context_string,
        std::string& detected_name,
        enum adis_device_id& detected_id
    );
};

#endif // DEVICE_FACTORY_H
