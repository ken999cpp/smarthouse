#include "smarthouse.h"
#include <algorithm>
Device::Device(std::string name): name(std::move(name)) {}
void Device::setCoordinator(Coordinator* coord)
{
    coordinator = coord;
}
void Device::setId(int newId)
{
    id = newId;
}
int Device::getId() const
{
    return id;
}
Coordinator* Device::getCoordinator() const
{
    return coordinator;
}
std::string Device::getType() const
{
    return "Generic Device";
}
void Device::turnOn()
{
    isOn = true;
}
void Device::turnOff()
{
    isOn = false;
}
std::ostream& operator<<(std::ostream& os, const Device& device)
{
    os << "[ID: " << device.id << "] " << device.getType() << " \"" << device.name << "\" (" << (device.isOn ? "ON" : "OFF") << ")";
    return os;
}
void Device::printInfo() const
{
    std::cout << "Device ID: " << id << std::endl
    << "Type: " << getType() << std::endl
    << "Name: " << name << std::endl
    << "Status: " << (isOn ? "ON" : "OFF") << std::endl;
}
Light::Light(std::string name): Device(std::move(name)) {}

std::string Light::getType() const
{
    return "Light";
}
void Light::turnOn()
{
    isOn = true;
    brightness = 50;
}
void Light::printInfo() const
{
    Device::printInfo();
    std::cout << "Brightness: " << brightness << "%" << std::endl;
}
void Light::setBrightness(int level)
{
    brightness = (level < 0) ? 0 : (level > 100) ? 100 : level;
}
Light& Light::operator+=(int val)
{
    setBrightness(brightness + val);
    return *this;
}
Light& Light::operator-=(int val)
{
    setBrightness(brightness - val);
    return *this;   
}
Thermostat::Thermostat(std::string name): Device(std::move(name)) {}
std::string Thermostat::getType() const
{
    return "Thermostat";
}
void Thermostat::turnOn()
{
    isOn = true;
    currentTemp = 20;
}
void Thermostat::printInfo() const
{
    Device::printInfo();
    std::cout << "Temp: " << currentTemp << "°C" << std::endl;
}
void Thermostat::setTemp(int temp)
{
    currentTemp = temp;
}
Thermostat& Thermostat::operator++()
{
    currentTemp++;
    return *this;
}
Thermostat& Thermostat::operator--()
{
    currentTemp--;
    return *this;
}
SecurityCamera::SecurityCamera(std::string name): Device(std::move(name)) {}
std::string SecurityCamera::getType() const
{
    return "Security Camera";
}
void SecurityCamera::turnOn()
{
    isOn = true;
    isRecording = true;
}
void SecurityCamera::printInfo() const
{
    Device::printInfo();
    std::cout << "Recording: " << (isRecording ? "Yes" : "No") << std::endl;
}
void Coordinator::connectDevice(Device* device)
{
    device -> setId(nextId++);
    device -> setCoordinator(this);
    devices.push_back(device);
}
void Coordinator::disconnectDevice(int id)
{
    for (auto it = devices.begin(); it != devices.end(); it++)
    {
        if ((*it)->getId() == id)
        {
            devices.erase(it);
            *it = nullptr;
            break;
        }
    }
}
void Coordinator::turnAllOn()
{
    for (auto& dev: devices)
    {
        dev -> turnOn();   
    }
}
void Coordinator::turnAllOff()
{
    for (auto& dev: devices)
    {
        dev -> turnOff();   
    }
}
void Coordinator::printDevices() const
{
    for (auto& dev: devices)
    {
        std::cout << *dev << std::endl;
    }
}
void Coordinator::printSummart() const
{
    for (auto& dev: devices)
    {
        dev -> printInfo();
        std::cout << std::endl;
    }
}
Device* Coordinator::getDeviceById(int id) const
{
    for (auto it = devices.begin(); it != devices.end(); it++)
    {
        if ((*it) -> getId() == id)
        {
            return *it;
        }
    }
    return nullptr;
}
Coordinator::~Coordinator()
{
    for (auto& dev: devices)
    {
        dev -> setCoordinator(nullptr);
        delete dev;
    }
    devices.clear();
}


