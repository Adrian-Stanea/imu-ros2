#include "adi_imu/device_registry.h"
#include "adi_imu/adis_data_access.h"
#include <algorithm>
#include <stdexcept>

// Static member initialization
std::map<std::string, std::shared_ptr<DeviceConfig>> DeviceRegistry::device_configs_;
std::map<enum adis_device_id, std::shared_ptr<DeviceConfig>> DeviceRegistry::device_id_configs_;
std::map<std::string, std::vector<std::shared_ptr<DeviceConfig>>> DeviceRegistry::family_configs_;
bool DeviceRegistry::initialized_ = false;

std::shared_ptr<DeviceConfig> DeviceRegistry::getDeviceConfig(const std::string& device_name) {
    if (!initialized_) {
        initialize();
    }

    auto it = device_configs_.find(device_name);
    return (it != device_configs_.end()) ? it->second : nullptr;
}

std::shared_ptr<DeviceConfig> DeviceRegistry::getDeviceConfig(enum adis_device_id device_id) {
    if (!initialized_) {
        initialize();
    }

    auto it = device_id_configs_.find(device_id);
    return (it != device_id_configs_.end()) ? it->second : nullptr;
}

std::vector<std::string> DeviceRegistry::getAvailableDevices() {
    if (!initialized_) {
        initialize();
    }

    std::vector<std::string> devices;
    devices.reserve(device_configs_.size());

    for (const auto& pair : device_configs_) {
        devices.push_back(pair.first);
    }

    return devices;
}

std::vector<std::shared_ptr<DeviceConfig>> DeviceRegistry::getDevicesByFamily(const std::string& family) {
    if (!initialized_) {
        initialize();
    }

    auto it = family_configs_.find(family);
    return (it != family_configs_.end()) ? it->second : std::vector<std::shared_ptr<DeviceConfig>>();
}

bool DeviceRegistry::isDeviceSupported(const std::string& device_name) {
    if (!initialized_) {
        initialize();
    }

    return device_configs_.find(device_name) != device_configs_.end();
}

std::string DeviceRegistry::getDeviceFamily(const std::string& device_name) {
    auto config = getDeviceConfig(device_name);
    return config ? config->getFamily() : "";
}

void DeviceRegistry::initialize() {
    if (initialized_) {
        return;
    }

    // Clear existing configurations
    device_configs_.clear();
    device_id_configs_.clear();
    family_configs_.clear();

    // Initialize all device families
    initializeAdis1646xFamily();
    initializeAdis1647xFamily();
    initializeAdis1650xFamily();
    initializeAdis1654xFamily();
    initializeAdis1655xFamily();
    initializeAdis1657xFamily();

    initialized_ = true;
}

bool DeviceRegistry::registerDevice(std::shared_ptr<DeviceConfig> config) {
    if (!config) {
        return false;
    }

    const std::string& name = config->getName();
    enum adis_device_id id = config->getId();
    const std::string& family = config->getFamily();

    // Check if device already exists
    if (device_configs_.find(name) != device_configs_.end()) {
        return false;
    }

    // Register in all maps
    device_configs_[name] = config;
    device_id_configs_[id] = config;
    family_configs_[family].push_back(config);

    return true;
}

std::shared_ptr<DeviceConfig> DeviceRegistry::createDeviceConfig(
    const std::string& name,
    enum adis_device_id id,
    const std::string& family) {

    return std::make_shared<DeviceConfig>(name, id, family);
}

void DeviceRegistry::configureDevice(
    std::shared_ptr<DeviceConfig> config,
    const DeviceCapabilities& capabilities,
    const DeviceRegisters& registers,
    const DeviceScales& scales,
    const DevicePerformance& performance) {

    config->setCapabilities(capabilities);
    config->setRegisters(registers);
    config->setScales(scales);
    config->setPerformance(performance);
}

// Device family initialization methods (placeholder implementations)
void DeviceRegistry::initializeAdis1646xFamily() {
    // ADIS16465 variants
    auto adis16465_1 = createDeviceConfig("adis16465-1", ADIS16465_1, "adis1646x");
    auto adis16465_2 = createDeviceConfig("adis16465-2", ADIS16465_2, "adis1646x");
    auto adis16465_3 = createDeviceConfig("adis16465-3", ADIS16465_3, "adis1646x");

    // ADIS16467 variants
    auto adis16467_1 = createDeviceConfig("adis16467-1", ADIS16467_1, "adis1646x");
    auto adis16467_2 = createDeviceConfig("adis16467-2", ADIS16467_2, "adis1646x");
    auto adis16467_3 = createDeviceConfig("adis16467-3", ADIS16467_3, "adis1646x");

    // Configure common capabilities for 1646x family
    DeviceCapabilities capabilities;
    capabilities.has_delta_burst = true;
    capabilities.has_temperature = true;
    capabilities.has_gyro_fail_detection = true;
    capabilities.has_accel_fail_detection = true;
    capabilities.has_crc = true;

    // Configure common registers for 1646x family
    DeviceRegisters registers;
    // Use default values for now

    // Configure common scales for 1646x family (placeholder values)
    DeviceScales scales;
    scales.delta_angle_scale = 1.0e-6;
    scales.delta_velocity_scale = 1.0e-6;
    scales.gyro_scale = 1.0e-5;
    scales.accel_scale = 1.0e-3;
    scales.temp_scale = 0.1;

    // Configure common performance for 1646x family
    DevicePerformance performance;
    performance.max_sampling_freq = 2000.0;
    performance.max_gyro_range = 2000.0;
    performance.max_accel_range = 392.0;

    // Apply configurations and register devices
    std::vector<std::shared_ptr<DeviceConfig>> configs = {
        adis16465_1, adis16465_2, adis16465_3,
        adis16467_1, adis16467_2, adis16467_3
    };

    for (auto& config : configs) {
        configureDevice(config, capabilities, registers, scales, performance);
        registerDevice(config);
    }
}

void DeviceRegistry::initializeAdis1647xFamily() {
    // ADIS16470
    auto adis16470 = createDeviceConfig("adis16470", ADIS16470, "adis1647x");

    // ADIS16475 variants
    auto adis16475_1 = createDeviceConfig("adis16475-1", ADIS16475_1, "adis1647x");
    auto adis16475_2 = createDeviceConfig("adis16475-2", ADIS16475_2, "adis1647x");
    auto adis16475_3 = createDeviceConfig("adis16475-3", ADIS16475_3, "adis1647x");

    // ADIS16477 variants
    auto adis16477_1 = createDeviceConfig("adis16477-1", ADIS16477_1, "adis1647x");
    auto adis16477_2 = createDeviceConfig("adis16477-2", ADIS16477_2, "adis1647x");
    auto adis16477_3 = createDeviceConfig("adis16477-3", ADIS16477_3, "adis1647x");

    // Configure capabilities for 1647x family
    DeviceCapabilities capabilities;
    capabilities.has_delta_burst = true;
    capabilities.has_temperature = true;
    capabilities.has_gyro_fail_detection = true;
    capabilities.has_accel_fail_detection = true;
    capabilities.has_bias_correction = true;
    capabilities.has_crc = true;

    // Configure registers, scales, and performance (placeholder)
    DeviceRegisters registers;
    DeviceScales scales;
    DevicePerformance performance;

    std::vector<std::shared_ptr<DeviceConfig>> configs = {
        adis16470, adis16475_1, adis16475_2, adis16475_3,
        adis16477_1, adis16477_2, adis16477_3
    };

    for (auto& config : configs) {
        configureDevice(config, capabilities, registers, scales, performance);
        registerDevice(config);
    }
}

void DeviceRegistry::initializeAdis1650xFamily() {
    // ADIS16500, ADIS16501
    auto adis16500 = createDeviceConfig("adis16500", ADIS16500, "adis1650x");
    auto adis16501 = createDeviceConfig("adis16501", ADIS16501, "adis1650x");

    // ADIS16505 variants
    auto adis16505_1 = createDeviceConfig("adis16505-1", ADIS16505_1, "adis1650x");
    auto adis16505_2 = createDeviceConfig("adis16505-2", ADIS16505_2, "adis1650x");
    auto adis16505_3 = createDeviceConfig("adis16505-3", ADIS16505_3, "adis1650x");

    // ADIS16507 variants
    auto adis16507_1 = createDeviceConfig("adis16507-1", ADIS16507_1, "adis1650x");
    auto adis16507_2 = createDeviceConfig("adis16507-2", ADIS16507_2, "adis1650x");
    auto adis16507_3 = createDeviceConfig("adis16507-3", ADIS16507_3, "adis1650x");

    // Configure capabilities for 1650x family
    DeviceCapabilities capabilities;
    capabilities.has_delta_burst = true;
    capabilities.has_temperature = true;
    capabilities.has_gyro_fail_detection = true;
    capabilities.has_accel_fail_detection = true;
    capabilities.has_mag_compensation = true;
    capabilities.has_bias_correction = true;
    capabilities.has_crc = true;

    // Configure registers, scales, and performance (placeholder)
    DeviceRegisters registers;
    DeviceScales scales;
    DevicePerformance performance;

    std::vector<std::shared_ptr<DeviceConfig>> configs = {
        adis16500, adis16501, adis16505_1, adis16505_2, adis16505_3,
        adis16507_1, adis16507_2, adis16507_3
    };

    for (auto& config : configs) {
        configureDevice(config, capabilities, registers, scales, performance);
        registerDevice(config);
    }
}

void DeviceRegistry::initializeAdis1654xFamily() {
    // ADIS16545 variants
    auto adis16545_1 = createDeviceConfig("adis16545-1", ADIS16545_1, "adis1654x");
    auto adis16545_2 = createDeviceConfig("adis16545-2", ADIS16545_2, "adis1654x");
    auto adis16545_3 = createDeviceConfig("adis16545-3", ADIS16545_3, "adis1654x");

    // ADIS16547 variants
    auto adis16547_1 = createDeviceConfig("adis16547-1", ADIS16547_1, "adis1654x");
    auto adis16547_2 = createDeviceConfig("adis16547-2", ADIS16547_2, "adis1654x");
    auto adis16547_3 = createDeviceConfig("adis16547-3", ADIS16547_3, "adis1654x");

    // Configure capabilities for 1654x family
    DeviceCapabilities capabilities;
    capabilities.has_delta_burst = true;
    capabilities.has_temperature = true;
    capabilities.has_gyro_fail_detection = true;
    capabilities.has_accel_fail_detection = true;
    capabilities.has_crc = true;

    // Configure registers, scales, and performance (placeholder)
    DeviceRegisters registers;
    DeviceScales scales;
    DevicePerformance performance;

    std::vector<std::shared_ptr<DeviceConfig>> configs = {
        adis16545_1, adis16545_2, adis16545_3,
        adis16547_1, adis16547_2, adis16547_3
    };

    for (auto& config : configs) {
        configureDevice(config, capabilities, registers, scales, performance);
        registerDevice(config);
    }
}

void DeviceRegistry::initializeAdis1655xFamily() {
    // ADIS16550
    auto adis16550 = createDeviceConfig("adis16550", ADIS16550, "adis1655x");

    // Configure capabilities for 1655x family
    DeviceCapabilities capabilities;
    capabilities.has_delta_burst = true;
    capabilities.has_temperature = true;
    capabilities.has_gyro_fail_detection = true;
    capabilities.has_accel_fail_detection = true;
    capabilities.has_crc = true;

    // Configure registers, scales, and performance (placeholder)
    DeviceRegisters registers;
    DeviceScales scales;
    DevicePerformance performance;

    configureDevice(adis16550, capabilities, registers, scales, performance);
    registerDevice(adis16550);
}

void DeviceRegistry::initializeAdis1657xFamily() {
    // ADIS16575 variants
    auto adis16575_2 = createDeviceConfig("adis16575-2", ADIS16575_2, "adis1657x");
    auto adis16575_3 = createDeviceConfig("adis16575-3", ADIS16575_3, "adis1657x");

    // ADIS16576 variants
    auto adis16576_2 = createDeviceConfig("adis16576-2", ADIS16576_2, "adis1657x");
    auto adis16576_3 = createDeviceConfig("adis16576-3", ADIS16576_3, "adis1657x");

    // ADIS16577 variants
    auto adis16577_2 = createDeviceConfig("adis16577-2", ADIS16577_2, "adis1657x");
    auto adis16577_3 = createDeviceConfig("adis16577-3", ADIS16577_3, "adis1657x");

    // Configure capabilities for 1657x family
    DeviceCapabilities capabilities;
    capabilities.has_delta_burst = true;
    capabilities.has_temperature = true;
    capabilities.has_gyro_fail_detection = true;
    capabilities.has_accel_fail_detection = true;
    capabilities.has_timestamp = true;
    capabilities.has_crc = true;

    // Configure registers, scales, and performance (placeholder)
    DeviceRegisters registers;
    DeviceScales scales;
    DevicePerformance performance;

    std::vector<std::shared_ptr<DeviceConfig>> configs = {
        adis16575_2, adis16575_3, adis16576_2, adis16576_3,
        adis16577_2, adis16577_3
    };

    for (auto& config : configs) {
        configureDevice(config, capabilities, registers, scales, performance);
        registerDevice(config);
    }
}
