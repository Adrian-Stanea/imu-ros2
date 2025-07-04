/*******************************************************************************
 *   @file   adis1655x_handler.h
 *   @brief  Device handler for ADIS1655x family devices
 ********************************************************************************/

#ifndef ADIS1655X_HANDLER_H
#define ADIS1655X_HANDLER_H

#include "adi_imu/device_handler.h"

namespace adi_imu {

/**
 * @brief Device handler for ADIS1655x family IMU devices
 * - ADIS16550
 */
class ADIS1655xHandler : public BaseDeviceHandler {
public:
    explicit ADIS1655xHandler(std::shared_ptr<DeviceConfig> config);
    ~ADIS1655xHandler() override = default;

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

#endif // ADIS1655X_HANDLER_H
