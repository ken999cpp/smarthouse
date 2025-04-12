#include "smarthouse.h"
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
            delete *it;
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
void Coordinator::writeToFile(const std::string& str)
{
    std::ofstream fout(str);
    if (!fout.is_open())
    {
        throw std::runtime_error("Can't open the file");
    }
    for (auto& dev: devices)
    {
        fout << *dev << '\n';
    }
    if (fout.fail())
    {
        throw std::runtime_error("Error while writing to the file");
    }
}
void Coordinator::readFromFile(const std::string& str)
{
    std::ifstream fin(str);
    if (!fin.is_open())
    {
        throw std::runtime_error("Can't open the file");
    }
    for (auto& dev: devices)
    {
        dev -> setCoordinator(nullptr);
        delete dev;
    }
    devices.clear();
    nextId = 1;
    std::string line;
    while (std::getline(fin, line))
    {
        if (line.empty())
        {
            continue;
        }
        std::istringstream iss(line);
        std::string idToken, idStr, word1, word2, name, status, type;
        iss >> idToken >> idStr >> word1;
        if (word1 == "Security")
        {
            iss >> word2;
            type = word1 + " " + word2;
        }
        else
        {
            type = word1;
        }
        if (idToken != "[ID:" || word1.empty())
        {
            continue;
        }
        if (!std::isdigit(idStr[0]))
        {
            continue;
        }
        std::getline(iss, name, '"');
        std::getline(iss, name, '"');
        std::string restOfLine;
        std::getline(iss, restOfLine);
        if (restOfLine.find("(ON)") != std::string::npos)
        {
            status = "ON";
        }
        else if (restOfLine.find("(OFF)") != std::string::npos)
        {
            status = "OFF";
        }
        else
        {
            continue;
        }
        Device* device = nullptr;
        if (type == "Light")
        {
            device = new Light(name);
        }
        else if (type == "Thermostat")
        {
                
            device = new Thermostat(name);
        }
        else if (type == "Security Camera")
        {
            
            device = new SecurityCamera(name);
        }
        if (!device)
        {
            continue;
        }
        if (status == "ON")
        {
            device -> turnOn();
        }
        else
        {
            device -> turnOff();
        }
        connectDevice(device);
        if (fin.fail())
        {
            throw std::runtime_error("Error while reading from the file");
        }
    }
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
