#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <format>
#include <concepts>
#include <unordered_map>
#include <variant>
#include <vector>

namespace vk {

    template<class T>
    requires std::integral<T> || std::floating_point<T>
    struct Metric{
        T num;
        std::string name;
    };

class MetricsLogger {
public:
    explicit MetricsLogger(const std::string& filename){
        m_file.open(filename);
        if(!m_file.is_open())
            throw std::runtime_error("Failed to open log file: " + filename);
    }
    ~MetricsLogger(){
        m_file.close();
    }

    void event(const std::initializer_list<std::pair<std::string, int>>& metrics) {

         auto now = std::chrono::system_clock::now();
         auto epoch = std::chrono::system_clock::to_time_t(now);
        // m_file << now;
        // for(const auto& metric : metrics){
        //     m_file << " \"" <<  metric.first << "\" " << metric.second;
        // }

        for(const auto& metric : metrics){
            m_storage[epoch].push_back(metric);
        }
    }
    void log() {

    }

    void event_and_log(){

    }

private:
    template<typename T>
    static constexpr bool is_valid_pair() {
        return std::is_same_v<
            std::pair<std::string, typename T::second_type>, T
        > && std::is_arithmetic_v<typename T::second_type>;
    }
private:
    std::ofstream m_file;
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> m_storage;
};

} //namespace loglib