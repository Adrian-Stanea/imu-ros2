/***************************************************************************/ /**
 *   @file   hardware_detector.cpp
 *   @brief  Implementation for hardware detection and capability querying.
 *   @author Generated for ADI IMU ROS2 refactoring
 ********************************************************************************
 * Copyright 2023(c) Analog Devices, Inc.

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#include "adi_imu/hardware_detector.h"
#include "adi_imu/device_config.h"
#include "adi_imu/iio_wrapper.h"
#include <rclcpp/rclcpp.hpp>
#include <iio.h>
#include <algorithm>
#include <cctype>

namespace adi_imu {

HardwareDetector::HardwareDetector() : m_iio_context(nullptr) {}

HardwareDetector::~HardwareDetector() {
    if (m_iio_context) {
        iio_context_destroy(m_iio_context);
        m_iio_context = nullptr;
    }
}

bool HardwareDetector::initializeContext(const std::string& context_string) {
    if (m_iio_context) {
        RCLCPP_INFO(rclcpp::get_logger("hardware_detector"), "IIO context already exists.");
        return false;
    }

    if (context_string == "local:") {
        m_iio_context = iio_create_local_context();
    } else {
        m_iio_context = iio_create_context_from_uri(context_string.c_str());
    }

    if (!m_iio_context) {
        RCLCPP_ERROR(rclcpp::get_logger("hardware_detector"), "Failed to create IIO context");
        return false;
    }

    iio_context_set_timeout(m_iio_context, 5000);
    RCLCPP_INFO(rclcpp::get_logger("hardware_detector"), "Successfully initialized IIO context");
    return true;
}

std::optional<HardwareInfo> HardwareDetector::detectHardware() {
    if (!m_iio_context) {
        RCLCPP_ERROR(rclcpp::get_logger("hardware_detector"), "IIO context not initialized");
        return std::nullopt;
    }

    // Define supported devices and their families
    std::vector<DeviceMapping> supported_devices = {
        // ADIS1646X family
        {"adis16465-1", DeviceFamily::ADIS1646X_FAMILY, "ADIS16465-1"},
        {"adis16465-2", DeviceFamily::ADIS1646X_FAMILY, "ADIS16465-2"},
        {"adis16465-3", DeviceFamily::ADIS1646X_FAMILY, "ADIS16465-3"},
        {"adis16467-1", DeviceFamily::ADIS1646X_FAMILY, "ADIS16467-1"},
        {"adis16467-2", DeviceFamily::ADIS1646X_FAMILY, "ADIS16467-2"},
        {"adis16467-3", DeviceFamily::ADIS1646X_FAMILY, "ADIS16467-3"},

        // ADIS1647X family
        {"adis16470", DeviceFamily::ADIS1647X_FAMILY, "ADIS16470"},
        {"adis16475-1", DeviceFamily::ADIS1647X_FAMILY, "ADIS16475-1"},
        {"adis16475-2", DeviceFamily::ADIS1647X_FAMILY, "ADIS16475-2"},
        {"adis16475-3", DeviceFamily::ADIS1647X_FAMILY, "ADIS16475-3"},
        {"adis16477-1", DeviceFamily::ADIS1647X_FAMILY, "ADIS16477-1"},
        {"adis16477-2", DeviceFamily::ADIS1647X_FAMILY, "ADIS16477-2"},
        {"adis16477-3", DeviceFamily::ADIS1647X_FAMILY, "ADIS16477-3"},

        // ADIS1650X family
        {"adis16500", DeviceFamily::ADIS1650X_FAMILY, "ADIS16500"},
        {"adis16501", DeviceFamily::ADIS1650X_FAMILY, "ADIS16501"},
        {"adis16505-1", DeviceFamily::ADIS1650X_FAMILY, "ADIS16505-1"},
        {"adis16505-2", DeviceFamily::ADIS1650X_FAMILY, "ADIS16505-2"},
        {"adis16505-3", DeviceFamily::ADIS1650X_FAMILY, "ADIS16505-3"},
        {"adis16507-1", DeviceFamily::ADIS1650X_FAMILY, "ADIS16507-1"},
        {"adis16507-2", DeviceFamily::ADIS1650X_FAMILY, "ADIS16507-2"},
        {"adis16507-3", DeviceFamily::ADIS1650X_FAMILY, "ADIS16507-3"},

        // ADIS1654X family
        {"adis16545-1", DeviceFamily::ADIS1654X_FAMILY, "ADIS16545-1"},
        {"adis16545-2", DeviceFamily::ADIS1654X_FAMILY, "ADIS16545-2"},
        {"adis16545-3", DeviceFamily::ADIS1654X_FAMILY, "ADIS16545-3"},
        {"adis16547-1", DeviceFamily::ADIS1654X_FAMILY, "ADIS16547-1"},
        {"adis16547-2", DeviceFamily::ADIS1654X_FAMILY, "ADIS16547-2"},
        {"adis16547-3", DeviceFamily::ADIS1654X_FAMILY, "ADIS16547-3"},

        // ADIS1655X family
        {"adis16550", DeviceFamily::ADIS1655X_FAMILY, "ADIS16550"},

        // ADIS1657X family
        {"adis16575-2", DeviceFamily::ADIS1657X_FAMILY, "ADIS16575-2"},
        {"adis16575-3", DeviceFamily::ADIS1657X_FAMILY, "ADIS16575-3"},
        {"adis16576-2", DeviceFamily::ADIS1657X_FAMILY, "ADIS16576-2"},
        {"adis16576-3", DeviceFamily::ADIS1657X_FAMILY, "ADIS16576-3"},
        {"adis16577-2", DeviceFamily::ADIS1657X_FAMILY, "ADIS16577-2"},
        {"adis16577-3", DeviceFamily::ADIS1657X_FAMILY, "ADIS16577-3"},
    };

    // Search for supported devices
    for (const auto& device_mapping : supported_devices) {
        struct iio_device* dev = iio_context_find_device(m_iio_context, device_mapping.iio_name.c_str());
        if (dev) {
            RCLCPP_INFO(rclcpp::get_logger("hardware_detector"), "Found device: %s", device_mapping.iio_name.c_str());

            // Check for trigger device
            std::string trigger_name = findTriggerDevice(dev, device_mapping.iio_name);
            if (trigger_name.empty()) {
                RCLCPP_WARN(rclcpp::get_logger("hardware_detector"), "No trigger found for device %s", device_mapping.iio_name.c_str());
                continue;
            }

            // Query device capabilities
            DeviceCapabilities capabilities = queryDeviceCapabilities(dev);

            // Create hardware info
            HardwareInfo hardware_info;
            hardware_info.device_family = device_mapping.family;
            hardware_info.device_name = device_mapping.display_name;
            hardware_info.iio_device_name = device_mapping.iio_name;
            hardware_info.trigger_name = trigger_name;
            hardware_info.capabilities = capabilities;

            RCLCPP_INFO(rclcpp::get_logger("hardware_detector"), "Detected hardware: %s (family: %d)",
                       hardware_info.device_name.c_str(), static_cast<int>(hardware_info.device_family));

            return hardware_info;
        }
    }

    RCLCPP_ERROR(rclcpp::get_logger("hardware_detector"), "No supported device found");
    return std::nullopt;
}

std::string HardwareDetector::findTriggerDevice(struct iio_device* dev, const std::string& device_name) {
    const std::string device_id = iio_device_get_id(dev);

    // Extract device number from device ID
    std::string device_number;
    for (char c : device_id) {
        if (std::isdigit(c)) {
            device_number += c;
        }
    }

    // Construct trigger name
    std::string trigger_name = device_name + "-dev" + device_number;

    // Check if trigger exists
    struct iio_device* trigger_dev = iio_context_find_device(m_iio_context, trigger_name.c_str());
    if (trigger_dev) {
        RCLCPP_INFO(rclcpp::get_logger("hardware_detector"), "Found trigger: %s", trigger_name.c_str());
        return trigger_name;
    }

    RCLCPP_WARN(rclcpp::get_logger("hardware_detector"), "Trigger %s not found", trigger_name.c_str());
    return "";
}

DeviceCapabilities HardwareDetector::queryDeviceCapabilities(struct iio_device* dev) {
    DeviceCapabilities capabilities;

    // Check for basic IMU channels
    capabilities.has_accelerometer = (iio_device_find_channel(dev, "accel_x", false) != nullptr);
    capabilities.has_gyroscope = (iio_device_find_channel(dev, "anglvel_x", false) != nullptr);
    capabilities.has_magnetometer = (iio_device_find_channel(dev, "magn_x", false) != nullptr);
    capabilities.has_temperature = (iio_device_find_channel(dev, "temp0", false) != nullptr);

    // Check for delta channels (velocity integration)
    capabilities.has_delta_velocity = (iio_device_find_channel(dev, "deltavelocity_x", false) != nullptr);
    capabilities.has_delta_angle = (iio_device_find_channel(dev, "deltaangl_x", false) != nullptr);

    // Check for pressure (barometer)
    capabilities.has_pressure = (iio_device_find_channel(dev, "pressure", false) != nullptr);

    // Check for diagnostic channels
    capabilities.has_diagnostics = (iio_device_find_channel(dev, "diag_stat", false) != nullptr);

    // Log detected capabilities
    RCLCPP_INFO(rclcpp::get_logger("hardware_detector"), "Device capabilities - Accel: %s, Gyro: %s, Mag: %s, Temp: %s, Delta: %s/%s, Pressure: %s, Diag: %s",
               capabilities.has_accelerometer ? "Yes" : "No",
               capabilities.has_gyroscope ? "Yes" : "No",
               capabilities.has_magnetometer ? "Yes" : "No",
               capabilities.has_temperature ? "Yes" : "No",
               capabilities.has_delta_velocity ? "Yes" : "No",
               capabilities.has_delta_angle ? "Yes" : "No",
               capabilities.has_pressure ? "Yes" : "No",
               capabilities.has_diagnostics ? "Yes" : "No");

    return capabilities;
}

} // namespace adi_imu
