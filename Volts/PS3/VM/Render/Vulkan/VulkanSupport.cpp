#include "VulkanSupport.h"

#if OS_WINDOWS
#   define VKLOAD(Handle, Name) { Name = (decltype(Name))GetProcAddress(Handle, #Name); }
#elif OS_LINUX
#   define VKLOAD(Handle, Name) { Name = (decltype(Name))dlsym(Handle, #Name); }
#endif

#define VKDECLARE(Name) CAT(PFN_, Name) Name

namespace Volts::PS3::RSX::VulkanSupport
{
    using namespace Cthulhu;
    // on windows DLL files are HMODULES
    // on linux they're void*

    const char* ExtensionNames[] = {
            VK_KHR_SURFACE_EXTENSION_NAME,
#if OS_WINDOWS
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif OS_LINUX
            VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#endif
        };


    void MakeInfo(VkApplicationInfo& App, VkInstanceCreateInfo& Create)
    {
        App.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        App.pApplicationName = "Volts";
        App.applicationVersion = VK_MAKE_VERSION(VMAJOR, VMINOR, VPATCH);
        App.pEngineName = "RSX";
        App.engineVersion = VK_MAKE_VERSION(VMAJOR, VMINOR, VPATCH);
        App.apiVersion = VK_API_VERSION_1_0;

        Create.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        Create.pApplicationInfo = &App;

        Create.enabledLayerCount = 0;
        Create.ppEnabledLayerNames = nullptr;
    }

    VkInstance MakeInstance()
    {
        VkApplicationInfo App = {};
        VkInstanceCreateInfo Create = {};
        MakeInfo(App, Create);

        Create.enabledExtensionCount = sizeof(ExtensionNames) / sizeof(const char*);
        Create.ppEnabledExtensionNames = ExtensionNames;

        VkInstance Instance;
        if(vkCreateInstance(&Create, nullptr, &Instance) != VK_SUCCESS)
        {
            // TODO: log error
            return nullptr;
        }

        return Instance;
    }

    VkInstance SimpleInstance()
    {
        VkApplicationInfo App = {};
        VkInstanceCreateInfo Create = {};
        MakeInfo(App, Create);

        Create.enabledExtensionCount = 0;
        Create.ppEnabledExtensionNames = nullptr;

        VkInstance Instance;
        if(vkCreateInstance(&Create, nullptr, &Instance) != VK_SUCCESS)
        {
            // TODO: log error
            return nullptr;
        }

        return Instance;
    }

    Array<VkExtensionProperties> Extensions;

    bool InitExtensions()
    {
        U32 Count;
        if(vkEnumerateInstanceExtensionProperties(nullptr, &Count, nullptr) != VK_SUCCESS)
            return false;

        Extensions.Reserve(Count);
        vkEnumerateInstanceExtensionProperties(nullptr, &Count, Extensions.Data());

        return true;
    }

    bool Supported(const char* Name)
    {
        for(auto Ext : Extensions)
        {
            if(CString::Compare(Ext.extensionName, Name) == 0)
                return true;
        }

        return false;
    }

    Cthulhu::Array<Cthulhu::String> Devices(VkInstance Instance)
    {
        U32 Count = 0;
        if(vkEnumeratePhysicalDevices(Instance, &Count, nullptr) != VK_SUCCESS)
            return {};

        if(Count == 0)
            return {};

        auto* Devices = new VkPhysicalDevice[Count];

        vkEnumeratePhysicalDevices(Instance, &Count, Devices);

        Array<Cthulhu::String> Ret;

        for(U32 I = 0; I < Count; I++)
        {
            VkPhysicalDeviceProperties Prop = {};
            vkGetPhysicalDeviceProperties(Devices[I], &Prop);
            MessageBox(nullptr, Prop.deviceName, "Vulkan", 0);
            // TODO: this is kinda busted for some reason
            Ret.Append(String::FromPtr(CString::Duplicate(Prop.deviceName)));
        }

        delete[] Devices;

        return Ret;
    }
}

#undef VKDECLARE
#undef VKLOAD