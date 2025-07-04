/*******************************************************************************
 *   @file   adis1646x_handler.h
 *   @brief  Device handler for ADIS1646x family devices
 *   @author Generated for runtime device detection
 *******************************************************************************
 * Copyright 2023(c) Analog Devices, Inc.
 *
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
 ******************************************************************************/

#ifndef ADIS1646X_HANDLER_H
#define ADIS1646X_HANDLER_H

#include "adi_imu/device_handler.h"
#include "adi_imu/adis1646x/adis1646x_data_access.h"

namespace adi_imu {

/**
 * @brief Device handler for ADIS1646x family IMU devices
 *
 * Handles specific capabilities and configurations for ADIS1646x devices:
 * - ADIS16465-1, ADIS16465-2, ADIS16465-3
 * - ADIS16467-1, ADIS16467-2, ADIS16467-3
 * - ADIS16470
 * - ADIS16475-1, ADIS16475-2, ADIS16475-3
 */
class ADIS1646xHandler : public BaseDeviceHandler {
public:
    /**
     * @brief Constructor for ADIS1646x handler
     * @param config Device configuration
     */
    explicit ADIS1646xHandler(std::shared_ptr<DeviceConfig> config);

    /**
     * @brief Destructor
     */
    ~ADIS1646xHandler() override = default;

    // DeviceHandler interface implementation
    bool initializeDevice() override;
    bool configureDevice() override;
    bool configureDevice(double sampling_freq, bool enable_delta_burst, bool enable_temperature) override;
    int readRawData(void* buffer, size_t buffer_size) override;
    int readBurstData(void* buffer, size_t buffer_size) override;
    uint16_t readDiagnosticStatus() override;
    uint16_t readProductId() override;
    uint16_t readSerialNumber() override;
    bool performSelfTest() override;
    bool resetDevice() override;
    bool isDeviceConnected() const override;

private:
    // Device-specific constants
    static constexpr double MAX_SAMPLE_FREQUENCY = ADIS_MAX_SAMP_FREQ;
    static constexpr int FLASH_MEMORY_ENDURANCE = ADIS_FLS_MEM_ENDURANCE;
};

} // namespace adi_imu

#endif // ADIS1646X_HANDLER_H
