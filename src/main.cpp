#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "vulkan.h"
#include "vk_platform.h"
#include "utils.hpp"

int main(int, char**) {

    vke::printCollapsed("Hello", "World", 2.2);

    vke::print("Vulkan Test");

    uint32_t extensionsCount = 0;

    if (auto res = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr)) {
        vke::print("Could not get the number of Instance extensions.", res);
        return 1;
    }

    std::vector<VkExtensionProperties> availableExtensions(extensionsCount);

    if (auto res = vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, availableExtensions.data())) {
        vke::print("Could not enumerate Instance extensions.", res);
        return 1;
    }

    std::vector<const char*> desiredExtensions = {"VK_KHR_display", "VK_KHR_surface"};

    auto isExtensionAvailable = [&availableExtensions](const char* extName) {
        auto isSameExtension = [&](const VkExtensionProperties& ext){ return std::string(extName) == ext.extensionName; };
        return std::find_if(availableExtensions.begin(), availableExtensions.end(), isSameExtension) != availableExtensions.end();
    };

    if (!std::all_of(desiredExtensions.begin(), desiredExtensions.end(), isExtensionAvailable)) {
        vke::print("Couldn't found extension.");
        return 1;
    }

    vke::print("Extensions found.");
    
    VkApplicationInfo appInfo;

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = "Vulkan Jerry";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Test Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_MAKE_VERSION(1, 1, 0);

    VkInstanceCreateInfo instanceCreateInfo;
    
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledLayerCount = 0;
    instanceCreateInfo.ppEnabledLayerNames = nullptr;
    instanceCreateInfo.enabledExtensionCount = desiredExtensions.size();
    if (desiredExtensions.empty()) {
        instanceCreateInfo.ppEnabledExtensionNames = nullptr;
    }
    else {
        instanceCreateInfo.ppEnabledExtensionNames = desiredExtensions.data();
    }

    VkInstance instance;

    if (auto res = vkCreateInstance(&instanceCreateInfo, nullptr, &instance)) {
        vke::print("Could not create Instance.", res);
        return 1;
    }

    vke::print("Instance created.");

    uint32_t devicesCount;

    if (auto res = vkEnumeratePhysicalDevices(instance, &devicesCount, nullptr)) {
        vke::print("Could not get physical devices.", res);
        return 1;
    }

    std::vector<VkPhysicalDevice> availableDevices(devicesCount);

    if (auto res = vkEnumeratePhysicalDevices(instance, &devicesCount, availableDevices.data())) {
        vke::print("Could not get physical devices. ", res);
        return 1;
    }

    if (!devicesCount) {
        vke::print("Could not get any physical device.");
    }

    for (auto& device : availableDevices) {

        VkPhysicalDeviceProperties deviceProps;

        vkGetPhysicalDeviceProperties(device, &deviceProps);

        vke::print("API Version: ", deviceProps.apiVersion);
        vke::print("Drive Version: ", deviceProps.driverVersion);
        vke::print("Vendor ID: ", deviceProps.vendorID);
        vke::print("Device Type: ", deviceProps.deviceType);
        vke::print("Device Name: ", deviceProps.deviceName);

        uint32_t queueFamiliesCount;

        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, nullptr);

        if(!queueFamiliesCount) {
            vke::print("Could not get number of queue family properties");
            return 1;
        }

        std::vector<VkQueueFamilyProperties> queueFamProps(queueFamiliesCount);

        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamiliesCount, queueFamProps.data());

        if (!queueFamiliesCount) {
            vke::print("Could not get any queue family properties");
            return 1;
        }

        vke::print("Number of queue family properties retreived:", queueFamiliesCount);
    }

    vkDestroyInstance(instance, nullptr);

    return 0;
}