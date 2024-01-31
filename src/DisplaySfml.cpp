/*
** EPITECH PROJECT, 2024
** B-PDG-300-PAR-3-1-PDGRUSH3-guillaume.houriez
** File description:
** DisplaySfml
*/

#include "../include/DisplaySfml.hpp"

MyGkrellM::MyGkrellM()
{
    window.create(sf::VideoMode(800, 600), "MyGkrellM");
    font.loadFromFile("assets/Roboto-Black.ttf");

    text.setFont(font);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color(255, 204, 153));
    text.setPosition(10.f, 10.f);

    scrollbar.setSize(sf::Vector2f(10.f, static_cast<float>(window.getSize().y)));
    scrollbar.setFillColor(sf::Color(100, 100, 100));
    scrollbar.setPosition(window.getSize().x - 10.f, 0.f);

    scrollPosition = 0.0f;
}

MyGkrellM::~MyGkrellM()
{
    this->window.close();
}

void MyGkrellM::run()
{
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void MyGkrellM::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta > 0) {
                scrollPosition -= 20.0f;
            } else if (event.mouseWheelScroll.delta < 0) {
                scrollPosition += 20.0f;
            }

            scrollPosition = std::max(0.0f, std::min(scrollPosition, textHeight - window.getSize().y));

            sf::View view = window.getView();
            view.reset(sf::FloatRect(0, scrollPosition, window.getSize().x, window.getSize().y));
            window.setView(view);
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (sf::FloatRect(scrollbar.getPosition(), scrollbar.getSize()).contains(event.mouseButton.x, event.mouseButton.y)) {
                    isScrolling = true;
                    offset = event.mouseButton.y - scrollbar.getPosition().y;
                }
            }
        } else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                isScrolling = false;
            }
        } else if (event.type == sf::Event::MouseMoved) {
            if (isScrolling) {
                scrollbar.setPosition(scrollbar.getPosition().x, event.mouseMove.y - offset);
                scrollbar.setPosition(scrollbar.getPosition().x, std::max(0.f, std::min(scrollbar.getPosition().y, window.getSize().y - scrollbar.getSize().y)));
                scrollPosition = scrollbar.getPosition().y / (window.getSize().y - scrollbar.getSize().y) * (textHeight - window.getSize().y);

                sf::View view = window.getView();
                view.reset(sf::FloatRect(0, scrollPosition, window.getSize().x, window.getSize().y));
                window.setView(view);
            }
        }
    }
}

void MyGkrellM::update()
{
    std::ostringstream oss;
    oss << "System Information:" << std::endl;

        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) == 0) {
            oss << "Hostname: " << hostname << std::endl;
        } else {
            oss << "Failed to get hostname." << std::endl;
        }

        uid_t uid = geteuid();
        struct passwd *pw = getpwuid(uid);
        if (pw != nullptr) {
            oss << "Username: " << pw->pw_name << std::endl;
        } else {
            oss << "Failed to get username." << std::endl;
        }

        std::time_t now = std::time(nullptr);
        struct tm *localTime = std::localtime(&now);

        oss << "Date and Time: " << localTime->tm_year + 1900 << '-'
            << localTime->tm_mon + 1 << '-'
            << localTime->tm_mday << ' '
            << localTime->tm_hour << ':'
            << localTime->tm_min << ':'
            << localTime->tm_sec << std::endl;

        std::ifstream tempFile("/sys/class/thermal/thermal_zone0/temp");
        if (tempFile.is_open()) {
            int temp;
            tempFile >> temp;
            tempFile.close();
            oss << "CPU Temperature: " << static_cast<float>(temp) / 1000.0f << "C" << std::endl;
        } else {
            oss << "Failed to read CPU temperature." << std::endl;
        }

        std::ifstream batteryFile("/sys/class/power_supply/BAT0/capacity");
        if (batteryFile.is_open()) {
            int batteryLevel;
            batteryFile >> batteryLevel;
            batteryFile.close();
            oss << "Battery Level: " << batteryLevel << "%" << std::endl;
        } else {
            oss << "Failed to read battery level." << std::endl;
        }

        float networkLoad = getNetworkLoad();
        oss << "Network Load: " << networkLoad << "%" << std::endl;

        std::ifstream cpuInfo("/proc/cpuinfo");
        int processorCount = 0;
        std::string line;
        if (cpuInfo.is_open()) {
            while (getline(cpuInfo, line)) {
                if (line.find("stepping") != std::string::npos || line.find("initial apicid") != std::string::npos) {
                    continue;
                }

                if (line.find("processor") != std::string::npos && processorCount > 0) {
                    break;
                }

                oss << line << std::endl;

                if (line.find("processor") != std::string::npos) {
                    processorCount++;
                }
            }
            cpuInfo.close();
        } else {
            oss << "Failed to read CPU info." << std::endl;
        }

        std::ifstream memInfo("/proc/meminfo");
        if (memInfo.is_open()) {
            while (getline(memInfo, line)) {
                oss << line << std::endl;
            }
            memInfo.close();
        } else {
            oss << "Failed to read memory info." << std::endl;
        }

        text.setString(oss.str());

        textHeight = text.getLocalBounds().height;

        if (textHeight > window.getSize().y) {
            float maxScroll = textHeight - window.getSize().y;
            scrollPosition = std::min(scrollPosition, maxScroll);

            float scrollbarHandleHeight = (window.getSize().y / textHeight) * window.getSize().y;
            scrollbar.setSize(sf::Vector2f(10.f, scrollbarHandleHeight));

            float scrollbarHandlePosition = (scrollPosition / maxScroll) * (window.getSize().y - scrollbarHandleHeight);
            scrollbar.setPosition(window.getSize().x - 10.f, scrollbarHandlePosition);

            sf::View view = window.getView();
            view.reset(sf::FloatRect(0, scrollPosition, window.getSize().x, window.getSize().y));
            window.setView(view);
        } else {
            scrollbar.setSize(sf::Vector2f(0.f, 0.f));
        }
}

void MyGkrellM::render()
{
    window.clear(sf::Color(51, 51, 51));
    window.draw(text);
    window.draw(scrollbar);
    window.display();
}

float MyGkrellM::getNetworkLoad() {
    std::ifstream networkFile("/proc/net/dev");
    std::string line;
    std::string interfaceName = "eth0";

    if (networkFile.is_open()) {
        while (getline(networkFile, line)) {
            if (line.find(interfaceName) != std::string::npos) {
                std::istringstream iss(line);
                std::string token;

                for (int i = 0; i < 8; ++i) {
                    iss >> token;
                }

                float received, dropped;
                iss >> received;
                iss >> dropped;

                float total = received + dropped;
                float loadPercentage = (total > 0) ? (received / total) * 100.0f : 0.0f;
                return loadPercentage;
            }
        }
        networkFile.close();
    }

    return 0.0f;
}
