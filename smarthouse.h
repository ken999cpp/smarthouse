#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <sstream>
#pragma once
class Coordinator;
class Device
{
    protected:
        int id = -1;
        std::string name;
        bool isOn = false;
        Coordinator* coordinator = nullptr;
    public:
        Device() = delete;
        Device(std::string name);
        virtual ~Device() = default;
        void setCoordinator(Coordinator* coord);
        void setId(int newId);
        int getId() const;
        Coordinator* getCoordinator() const;
        virtual std::string getType() const;
        virtual void turnOn();
        void turnOff();
        friend std::ostream& operator<<(std::ostream& os, const Device& device);
        virtual void printInfo() const;
};
class Light: public Device
{
    private:
        int brightness = 0;
    public:
        Light(std::string name);
        virtual std::string getType() const override;
        virtual void turnOn() override;
        virtual void printInfo() const override;
        void setBrightness(int level);
        Light& operator+=(int val);
        Light& operator-=(int val);
};
class Thermostat: public Device
{

    private:
        int currentTemp;
    public:
    Thermostat(std::string name);
        virtual std::string getType() const override;
        virtual void turnOn() override;
        virtual void printInfo() const override;
        void setTemp(int temp);
        Thermostat& operator++();
        Thermostat& operator--();
};
class SecurityCamera: public Device
{
    private:
        bool isRecording = false;
    public:
    SecurityCamera(std::string name);
        virtual std::string getType() const override;
        virtual void turnOn() override;
        virtual void printInfo() const override;
};
class Coordinator
{
    private:
        std::vector<Device*> devices;
        int nextId = 1;
    public:
        Coordinator() = default;
        Coordinator(const Coordinator&) = delete;
        Coordinator& operator=(const Coordinator&) = delete;
        void connectDevice(Device* device);
        void disconnectDevice(int id);
        void turnAllOn();
        void turnAllOff();
        void printDevices() const;
        void printSummart() const;
        Device* getDeviceById(int id) const;
        void writeToFile(const std::string&);
        void readFromFile(const std::string&);
        ~Coordinator();
};