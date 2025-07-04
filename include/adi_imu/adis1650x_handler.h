/*******************************************************************************
 *   @file   adis1650x_handler.h
 *   @brief  Device handler for ADIS1650x family devices
 ********************************************************************************/

#ifndef ADIS1650X_HANDLER_H
#define ADIS1650X_HANDLER_H

#include "adi_imu/device_handler.h"

namespace adi_imu {

/**
 * @brief Device handler for ADIS1650x family IMU devices
 * - ADIS16500, ADIS16501
 * - ADIS16505-1, ADIS16505-2, ADIS16505-3
 * - ADIS16507-1, ADIS16507-2, ADIS16507-3
 */
class ADIS1650xHandler : public BaseDeviceHandler {
public:
    explicit ADIS1650xHandler(std::shared_ptr<DeviceConfig> config);
    ~ADIS1650xHandler() override = default;

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

#endif // ADIS1650X_HANDLER_H
