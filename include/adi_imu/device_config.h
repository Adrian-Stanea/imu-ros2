#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

#include <cstdint>
#include <string>
#include <memory>
#include "adis_data_access.h"

/**
 * @brief Device capabilities structure
 * Defines what features a specific ADIS device supports
 */
struct DeviceCapabilities {
    bool has_delta_burst;           // Supports delta angle/velocity burst read
    bool has_temperature;           // Has temperature sensor
    bool has_gyro_fail_detection;   // Supports gyroscope failure detection
    bool has_accel_fail_detection;  // Supports accelerometer failure detection
    bool has_mag_compensation;      // Supports magnetic compensation
    bool has_bias_correction;       // Supports bias correction
    bool has_crc;                   // Supports CRC checking
    bool has_timestamp;             // Supports timestamp

    // Additional capabilities for runtime detection
    bool has_accelerometer;         // Has accelerometer channels
    bool has_gyroscope;             // Has gyroscope channels
    bool has_magnetometer;          // Has magnetometer channels
    bool has_delta_velocity;        // Has delta velocity channels
    bool has_delta_angle;           // Has delta angle channels
    bool has_pressure;              // Has pressure sensor
    bool has_diagnostics;           // Has diagnostic channels

    DeviceCapabilities() :
        has_delta_burst(false),
        has_temperature(false),
        has_gyro_fail_detection(false),
        has_accel_fail_detection(false),
        has_mag_compensation(false),
        has_bias_correction(false),
        has_crc(false),
        has_timestamp(false),
        has_accelerometer(false),
        has_gyroscope(false),
        has_magnetometer(false),
        has_delta_velocity(false),
        has_delta_angle(false),
        has_pressure(false),
        has_diagnostics(false) {}
};

/**
 * @brief Device register addresses
 * Contains device-specific register addresses
 */
struct DeviceRegisters {
    uint16_t diag_stat_addr;
    uint16_t range_model_addr;
    uint16_t msc_ctrl_addr;
    uint16_t glob_cmd_addr;
    uint16_t filt_ctrl_addr;
    uint16_t dec_rate_addr;
    uint16_t sync_scale_addr;
    uint16_t burst_cmd_addr;
    uint16_t prod_id_addr;
    uint16_t serial_num_addr;

    DeviceRegisters() :
        diag_stat_addr(0x02),
        range_model_addr(0x04),
        msc_ctrl_addr(0x06),
        glob_cmd_addr(0x08),
        filt_ctrl_addr(0x0A),
        dec_rate_addr(0x0C),
        sync_scale_addr(0x0E),
        burst_cmd_addr(0x7C),
        prod_id_addr(0x7E),
        serial_num_addr(0x20) {}
};

/**
 * @brief Device scaling factors
 * Contains device-specific scaling factors for data conversion
 */
struct DeviceScales {
    double delta_angle_scale;       // rad/LSB
    double delta_velocity_scale;    // m/s/LSB
    double gyro_scale;             // rad/s/LSB
    double accel_scale;            // m/s²/LSB
    double temp_scale;             // °C/LSB
    double temp_offset;            // °C offset

    DeviceScales() :
        delta_angle_scale(1.0),
        delta_velocity_scale(1.0),
        gyro_scale(1.0),
        accel_scale(1.0),
        temp_scale(1.0),
        temp_offset(0.0) {}
};

/**
 * @brief Device performance specifications
 * Contains device-specific performance characteristics
 */
struct DevicePerformance {
    double max_sampling_freq;       // Hz
    double max_gyro_range;          // rad/s
    double max_accel_range;         // m/s²
    uint32_t flash_memory_endurance; // write cycles
    double startup_time;            // seconds

    DevicePerformance() :
        max_sampling_freq(2000.0),
        max_gyro_range(2000.0),
        max_accel_range(392.0),
        flash_memory_endurance(10000),
        startup_time(0.5) {}
};

/**
 * @brief Complete device configuration
 * Encapsulates all device-specific information
 */
class DeviceConfig {
public:
    DeviceConfig(const std::string& name, enum adis_device_id id, const std::string& family);

    // Constructor for runtime detection
    DeviceConfig() = default;

    // Getters
    const std::string& getName() const { return name_; }
    enum adis_device_id getId() const { return id_; }
    const std::string& getFamily() const { return family_; }
    const DeviceCapabilities& getCapabilities() const { return capabilities_; }
    const DeviceRegisters& getRegisters() const { return registers_; }
    const DeviceScales& getScales() const { return scales_; }
    const DevicePerformance& getPerformance() const { return performance_; }

    // Runtime detection getters
    const std::string& getContextString() const { return context_string_; }
    const std::string& getDeviceName() const { return device_name_; }
    const std::string& getTriggerName() const { return trigger_name_; }
    double getSampleRateHz() const { return sample_rate_hz_; }
    bool isBurstModeEnabled() const { return enable_burst_mode_; }
    bool isDiagnosticsEnabled() const { return enable_diagnostics_; }
    int getBufferSize() const { return buffer_size_; }

    // Setters for configuration
    void setCapabilities(const DeviceCapabilities& caps) { capabilities_ = caps; }
    void setRegisters(const DeviceRegisters& regs) { registers_ = regs; }
    void setScales(const DeviceScales& scales) { scales_ = scales; }
    void setPerformance(const DevicePerformance& perf) { performance_ = perf; }

    // Runtime detection setters
    void setContextString(const std::string& ctx) { context_string_ = ctx; }
    void setDeviceName(const std::string& name) { device_name_ = name; }
    void setTriggerName(const std::string& trigger) { trigger_name_ = trigger; }
    void setSampleRateHz(double rate) { sample_rate_hz_ = rate; }
    void setBurstModeEnabled(bool enabled) { enable_burst_mode_ = enabled; }
    void setDiagnosticsEnabled(bool enabled) { enable_diagnostics_ = enabled; }
    void setBufferSize(int size) { buffer_size_ = size; }

    // Utility methods
    bool supportsFeature(const std::string& feature) const;
    uint16_t getRegisterAddress(const std::string& register_name) const;
    double getScaleFactor(const std::string& measurement_type) const;

private:
    std::string name_;
    enum adis_device_id id_;
    std::string family_;
    DeviceCapabilities capabilities_;
    DeviceRegisters registers_;
    DeviceScales scales_;
    DevicePerformance performance_;

    // Runtime detection fields
    std::string context_string_;        // IIO context string
    std::string device_name_;           // IIO device name
    std::string trigger_name_;          // IIO trigger name
    double sample_rate_hz_ = 100.0;     // Desired sample rate
    bool enable_burst_mode_ = true;     // Enable burst mode
    bool enable_diagnostics_ = true;    // Enable diagnostics
    int buffer_size_ = 1024;            // Buffer size
};

#endif // DEVICE_CONFIG_H
