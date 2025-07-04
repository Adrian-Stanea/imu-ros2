#include "adi_imu/device_factory.h"
#include "adi_imu/device_handler.h"
#include "adi_imu/device_registry.h"
#include "adi_imu/adis_data_access.h"
#include "adi_imu/hardware_detector.h"
#include "adi_imu/adis1646x_handler.h"
#include "adi_imu/adis1647x_handler.h"
#include "adi_imu/adis1650x_handler.h"
#include "adi_imu/adis1654x_handler.h"
#include "adi_imu/adis1655x_handler.h"
#include "adi_imu/adis1657x_handler.h"
#include <memory>
#include <stdexcept>

using namespace adi_imu;

std::unique_ptr<DeviceHandler> DeviceFactory::createDeviceHandler(const std::string& device_name) {
    auto config = DeviceRegistry::getDeviceConfig(device_name);
    if (!config) {
        return nullptr;
    }

    return createDeviceHandler(config);
}

std::unique_ptr<DeviceHandler> DeviceFactory::createDeviceHandler(enum adis_device_id device_id) {
    auto config = DeviceRegistry::getDeviceConfig(device_id);
    if (!config) {
        return nullptr;
    }

    return createDeviceHandler(config);
}

std::unique_ptr<DeviceHandler> DeviceFactory::createDeviceHandler(std::shared_ptr<DeviceConfig> config) {
    if (!config) {
        return nullptr;
    }

    const std::string& family = config->getFamily();
    return createHandlerForFamily(family, config);
}

std::unique_ptr<DeviceHandler> DeviceFactory::createDeviceHandlerAutoDetect(const std::string& context_string) {
    std::string detected_name;
    enum adis_device_id detected_id;

    if (detectDeviceType(context_string, detected_name, detected_id)) {
        return createDeviceHandler(detected_id);
    }

    return nullptr;
}

std::unique_ptr<DeviceHandler> DeviceFactory::createHandlerForFamily(
    const std::string& family,
    std::shared_ptr<DeviceConfig> config) {

    if (family == "adis1646x") {
        return std::make_unique<ADIS1646xHandler>(config);
    } else if (family == "adis1647x") {
        return std::make_unique<ADIS1647xHandler>(config);
    } else if (family == "adis1650x") {
        return std::make_unique<ADIS1650xHandler>(config);
    } else if (family == "adis1654x") {
        return std::make_unique<ADIS1654xHandler>(config);
    } else if (family == "adis1655x") {
        return std::make_unique<ADIS1655xHandler>(config);
    } else if (family == "adis1657x") {
        return std::make_unique<ADIS1657xHandler>(config);
    }

    return nullptr;
}

std::unique_ptr<DeviceHandler> DeviceFactory::createHandlerForFamily(
    adi_imu::DeviceFamily family,
    std::shared_ptr<DeviceConfig> config) {

    switch (family) {
        case adi_imu::DeviceFamily::ADIS1646X_FAMILY:
            return std::make_unique<ADIS1646xHandler>(config);
        case adi_imu::DeviceFamily::ADIS1647X_FAMILY:
            return std::make_unique<ADIS1647xHandler>(config);
        case adi_imu::DeviceFamily::ADIS1650X_FAMILY:
            return std::make_unique<ADIS1650xHandler>(config);
        case adi_imu::DeviceFamily::ADIS1654X_FAMILY:
            return std::make_unique<ADIS1654xHandler>(config);
        case adi_imu::DeviceFamily::ADIS1655X_FAMILY:
            return std::make_unique<ADIS1655xHandler>(config);
        case adi_imu::DeviceFamily::ADIS1657X_FAMILY:
            return std::make_unique<ADIS1657xHandler>(config);
        case adi_imu::DeviceFamily::UNKNOWN:
        default:
            return nullptr;
    }
}

bool DeviceFactory::detectDeviceType(
    const std::string& /*context_string*/,
    std::string& detected_name,
    enum adis_device_id& detected_id) {

    // This is a placeholder implementation
    // In reality, this would:
    // 1. Create IIO context from context_string
    // 2. Enumerate devices
    // 3. Read product ID from each device
    // 4. Match product ID to known devices
    // 5. Return the matched device information

    // For now, we'll implement a simple string matching approach
    // This should be replaced with actual IIO device detection

    // TODO: Implement actual device detection using IIO
    // Example approach:
    // struct iio_context* ctx = iio_create_context_from_uri(context_string.c_str());
    // if (!ctx) return false;
    //
    // unsigned int nb_devices = iio_context_get_devices_count(ctx);
    // for (unsigned int i = 0; i < nb_devices; i++) {
    //     struct iio_device* dev = iio_context_get_device(ctx, i);
    //     const char* name = iio_device_get_name(dev);
    //     if (name && strstr(name, "adis")) {
    //         // Read product ID and match to known devices
    //         uint16_t prod_id = read_product_id(dev);
    //         // Match prod_id to device_id and name
    //         break;
    //     }
    // }
    // iio_context_destroy(ctx);

    // Placeholder: assume ADIS16470 for now
    detected_name = "adis16470";
    detected_id = ADIS16470;

    return true;
}
