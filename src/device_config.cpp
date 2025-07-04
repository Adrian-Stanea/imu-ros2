#include "adi_imu/device_config.h"
#include "adi_imu/adis_data_access.h"
#include <stdexcept>
#include <unordered_map>

DeviceConfig::DeviceConfig(const std::string& name, enum adis_device_id id, const std::string& family)
    : name_(name), id_(id), family_(family) {
    // Initialize with default values
    // Device-specific values will be set through setters
}

bool DeviceConfig::supportsFeature(const std::string& feature) const {
    static const std::unordered_map<std::string, bool DeviceCapabilities::*> feature_map = {
        {"delta_burst", &DeviceCapabilities::has_delta_burst},
        {"temperature", &DeviceCapabilities::has_temperature},
        {"gyro_fail_detection", &DeviceCapabilities::has_gyro_fail_detection},
        {"accel_fail_detection", &DeviceCapabilities::has_accel_fail_detection},
        {"mag_compensation", &DeviceCapabilities::has_mag_compensation},
        {"bias_correction", &DeviceCapabilities::has_bias_correction},
        {"crc", &DeviceCapabilities::has_crc},
        {"timestamp", &DeviceCapabilities::has_timestamp}
    };

    auto it = feature_map.find(feature);
    if (it != feature_map.end()) {
        return capabilities_.*(it->second);
    }
    return false;
}

uint16_t DeviceConfig::getRegisterAddress(const std::string& register_name) const {
    static const std::unordered_map<std::string, uint16_t DeviceRegisters::*> register_map = {
        {"diag_stat", &DeviceRegisters::diag_stat_addr},
        {"range_model", &DeviceRegisters::range_model_addr},
        {"msc_ctrl", &DeviceRegisters::msc_ctrl_addr},
        {"glob_cmd", &DeviceRegisters::glob_cmd_addr},
        {"filt_ctrl", &DeviceRegisters::filt_ctrl_addr},
        {"dec_rate", &DeviceRegisters::dec_rate_addr},
        {"sync_scale", &DeviceRegisters::sync_scale_addr},
        {"burst_cmd", &DeviceRegisters::burst_cmd_addr},
        {"prod_id", &DeviceRegisters::prod_id_addr},
        {"serial_num", &DeviceRegisters::serial_num_addr}
    };

    auto it = register_map.find(register_name);
    if (it != register_map.end()) {
        return registers_.*(it->second);
    }
    throw std::invalid_argument("Unknown register name: " + register_name);
}

double DeviceConfig::getScaleFactor(const std::string& measurement_type) const {
    static const std::unordered_map<std::string, double DeviceScales::*> scale_map = {
        {"delta_angle", &DeviceScales::delta_angle_scale},
        {"delta_velocity", &DeviceScales::delta_velocity_scale},
        {"gyro", &DeviceScales::gyro_scale},
        {"accel", &DeviceScales::accel_scale},
        {"temp", &DeviceScales::temp_scale}
    };

    auto it = scale_map.find(measurement_type);
    if (it != scale_map.end()) {
        return scales_.*(it->second);
    }
    throw std::invalid_argument("Unknown measurement type: " + measurement_type);
}
