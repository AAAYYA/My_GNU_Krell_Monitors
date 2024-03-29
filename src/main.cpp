/*
** EPITECH PROJECT, 2024
** B-PDG-300-PAR-3-1-PDGRUSH3-guillaume.houriez
** File description:
** main
*/

#include "../include/RAMModule.hpp"
#include "../include/CPUModule.hpp"
#include "../include/MonitorCore.hpp"
#include "../include/SystemInfoModule.hpp"
#include "../include/NcursesDisplay.hpp"
#include "../include/TemperatureModule.hpp"
#include "../include/NetworkModule.hpp"
#include "../include/DiskModule.hpp"
#include "../include/ProcessModule.hpp"
#include "../include/DateTimeModule.hpp"
#include "../include/BatteryModule.hpp"
#include "../include/OSModule.hpp"
#include "../include/DisplaySfml.hpp"

#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
    Krell::MonitorCore monitor;
    std::unique_ptr<Krell::IDisplay> display = std::make_unique<Krell::NcursesDisplay>();
    int key;

    if (argc > 1) {
        MyGkrellM displaySFML;
        std::string arg = argv[1];
        if (arg == "-g") {
            displaySFML.run();
            return 0;
        }
    }

    monitor.addModule(std::make_unique<Krell::SystemInfoModule>());
    monitor.addModule(std::make_unique<Krell::DateTimeModule>());
    monitor.addModule(std::make_unique<Krell::BatteryModule>());
    monitor.addModule(std::make_unique<Krell::OSModule>());
    monitor.addModule(std::make_unique<Krell::CPUModule>());
    monitor.addModule(std::make_unique<Krell::RAMModule>());
    monitor.addModule(std::make_unique<Krell::NetworkModule>());
    monitor.addModule(std::make_unique<Krell::TemperatureModule>());
    monitor.addModule(std::make_unique<Krell::DiskModule>());
    monitor.addModule(std::make_unique<Krell::ProcessModule>());
    monitor.setDisplay(std::make_unique<Krell::NcursesDisplay>());

    while (true && key != 27) {
        monitor.updateAndDisplay();
        sleep(1);
        key = getch();
    }
    return 0;
}
