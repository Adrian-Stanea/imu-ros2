/*******************************************************************************
 *   @file   adis1654x_handler.h
 *   @brief  Device handler for ADIS1654x family devices
 ********************************************************************************/

#ifndef ADIS1654X_HANDLER_H
#define ADIS1654X_HANDLER_H

#include "adi_imu/device_handler.h"

namespace adi_imu {

/**
 * @brief Device handler for ADIS1654x family IMU devices
 * - ADIS16545-1, ADIS16545-2, ADIS16545-3
 * - ADIS16547-1, ADIS16547-2, ADIS16547-3
 */
class ADIS1654xHandler : public BaseDeviceHandler {
public:
    explicit ADIS1654xHandler(std::shared_ptr<DeviceConfig> config);
    ~ADIS1654xHandler() override = default;

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
    static constexpr double MAX_SAMPLE_FREQUENCY = 2100.0;
    static constexpr int FLASH_MEMORY_ENDURANCE = 10000;
};

} // namespace adi_imu

#endif // ADIS1654X_HANDLER_H
