/***************************************************************************/ /**
 *   @file   hardware_detector.h
 *   @brief  Hardware detection and capability querying for ADI IMU devices.
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

#ifndef HARDWARE_DETECTOR_H
#define HARDWARE_DETECTOR_H

#include <optional>
#include <string>
#include <vector>
#include "adi_imu/device_config.h"

struct iio_context;
struct iio_device;

namespace adi_imu {

/**
 * @brief Device family enumeration
 */
enum class DeviceFamily {
    ADIS1646X_FAMILY,
    ADIS1647X_FAMILY,
    ADIS1650X_FAMILY,
    ADIS1654X_FAMILY,
    ADIS1655X_FAMILY,
    ADIS1657X_FAMILY,
    UNKNOWN
};

/**
 * @brief Hardware detection result containing device information
 */
struct HardwareInfo {
    DeviceFamily device_family;
    std::string device_name;
    std::string iio_device_name;
    std::string trigger_name;
    DeviceCapabilities capabilities;

    HardwareInfo()
        : device_family(DeviceFamily::UNKNOWN) {}
};

/**
 * @brief Device mapping structure for device detection
 */
struct DeviceMapping {
    std::string iio_name;
    DeviceFamily family;
    std::string display_name;
};

/**
 * @brief Hardware detector for ADI IMU devices
 */
class HardwareDetector {
public:
    /**
     * @brief Constructor
     */
    HardwareDetector();

    /**
     * @brief Destructor
     */
    ~HardwareDetector();

    /**
     * @brief Initialize IIO context for hardware detection
     * @param context_string IIO context string (e.g., "local:" or "ip:192.168.1.100")
     * @return true if successful, false otherwise
     */
    bool initializeContext(const std::string& context_string);

    /**
     * @brief Detect available ADI IMU hardware
     * @return HardwareInfo structure if device found, std::nullopt otherwise
     */
    std::optional<HardwareInfo> detectHardware();

private:
    /**
     * @brief Find trigger device for a given IIO device
     * @param dev IIO device pointer
     * @param device_name Device name
     * @return trigger device name if found, empty string otherwise
     */
    std::string findTriggerDevice(struct iio_device* dev, const std::string& device_name);

    /**
     * @brief Query device capabilities by examining IIO channels
     * @param dev IIO device pointer
     * @return DeviceCapabilities structure
     */
    DeviceCapabilities queryDeviceCapabilities(struct iio_device* dev);

    struct iio_context* m_iio_context;
};

} // namespace adi_imu

#endif // HARDWARE_DETECTOR_H
