#ifndef DEVICE_REGISTRY_H
#define DEVICE_REGISTRY_H

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "device_config.h"

/**
 * @brief Device Registry
 * Manages device configurations and provides runtime lookup
 */
class DeviceRegistry {
public:
    /**
     * @brief Get device configuration by device name
     * @param device_name Name of the device (e.g., "adis16470")
     * @return Shared pointer to device configuration, nullptr if not found
     */
    static std::shared_ptr<DeviceConfig> getDeviceConfig(const std::string& device_name);

    /**
     * @brief Get device configuration by device ID
     * @param device_id Device ID enumeration
     * @return Shared pointer to device configuration, nullptr if not found
     */
    static std::shared_ptr<DeviceConfig> getDeviceConfig(enum adis_device_id device_id);

    /**
     * @brief Get all available device names
     * @return Vector of all registered device names
     */
    static std::vector<std::string> getAvailableDevices();

    /**
     * @brief Get all devices in a specific family
     * @param family Device family name (e.g., "adis1646x")
     * @return Vector of device configurations in the family
     */
    static std::vector<std::shared_ptr<DeviceConfig>> getDevicesByFamily(const std::string& family);

    /**
     * @brief Check if a device is supported
     * @param device_name Name of the device
     * @return True if device is supported, false otherwise
     */
    static bool isDeviceSupported(const std::string& device_name);

    /**
     * @brief Get device family from device name
     * @param device_name Name of the device
     * @return Device family name, empty string if not found
     */
    static std::string getDeviceFamily(const std::string& device_name);

    /**
     * @brief Initialize the registry with all supported devices
     * This is called automatically on first access
     */
    static void initialize();

    /**
     * @brief Register a new device configuration
     * @param config Device configuration to register
     * @return True if registration successful, false if device already exists
     */
    static bool registerDevice(std::shared_ptr<DeviceConfig> config);

private:
    // Registry storage
    static std::map<std::string, std::shared_ptr<DeviceConfig>> device_configs_;
    static std::map<enum adis_device_id, std::shared_ptr<DeviceConfig>> device_id_configs_;
    static std::map<std::string, std::vector<std::shared_ptr<DeviceConfig>>> family_configs_;

    // Initialization state
    static bool initialized_;

    // Private initialization methods
    static void initializeAdis1646xFamily();
    static void initializeAdis1647xFamily();
    static void initializeAdis1650xFamily();
    static void initializeAdis1654xFamily();
    static void initializeAdis1655xFamily();
    static void initializeAdis1657xFamily();

    // Helper methods
    static std::shared_ptr<DeviceConfig> createDeviceConfig(
        const std::string& name,
        enum adis_device_id id,
        const std::string& family
    );

    static void configureDevice(
        std::shared_ptr<DeviceConfig> config,
        const DeviceCapabilities& capabilities,
        const DeviceRegisters& registers,
        const DeviceScales& scales,
        const DevicePerformance& performance
    );
};

#endif // DEVICE_REGISTRY_H
