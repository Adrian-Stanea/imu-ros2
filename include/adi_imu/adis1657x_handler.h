/*******************************************************************************
 *   @file   adis1657x_handler.h
 *   @brief  Device handler for ADIS1657x family devices
 ********************************************************************************/

#ifndef ADIS1657X_HANDLER_H
#define ADIS1657X_HANDLER_H

#include "adi_imu/device_handler.h"

namespace adi_imu {

/**
 * @brief Device handler for ADIS1657x family IMU devices
 * - ADIS16575-2, ADIS16575-3
 * - ADIS16576-2, ADIS16576-3
 * - ADIS16577-2, ADIS16577-3
 */
class ADIS1657xHandler : public BaseDeviceHandler {
public:
    explicit ADIS1657xHandler(std::shared_ptr<DeviceConfig> config);
    ~ADIS1657xHandler() override = default;

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
    static constexpr double MAX_SAMPLE_FREQUENCY = 4100.0;
    static constexpr int FLASH_MEMORY_ENDURANCE = 100000;
};

} // namespace adi_imu

#endif // ADIS1657X_HANDLER_H
