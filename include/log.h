#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <concepts>
#include <unordered_map>
#include <atomic>
#include <memory>
#include <vector>
#include <mutex>
#include <thread>
#include <shared_mutex>

namespace vk
{

    class BaseMetric
    {
    public:
        virtual ~BaseMetric() = default;
        virtual std::string get_name() const  = 0;
        virtual std::string get_value() const = 0;
        virtual std::string get_value_reset() = 0;
    };

    template <class T>
        requires std::integral<T> || std::floating_point<T>
    class Metric : public BaseMetric
    {
    public:

        Metric(const std::string& name, const T& value) : m_name(name), m_value(value)
        {}

        std::string get_name() const noexcept override
        {
            return m_name;
        }
        
        std::string get_value() const override
        {
            return std::to_string(m_value);
        }

        void set(const T &value)
        {
            m_value.store(value, std::memory_order_relaxed);
        }

        std::string get_value_reset() override
        {
            return std::to_string(m_value.exchange(0));
        }

    private:
        std::string m_name;
        std::atomic<T> m_value;
    };

    class MetricsStorage
    {
    public:
        void add(const std::shared_ptr<BaseMetric> &metric)
        {
            std::unique_lock lock(m_mutex);
            m_metrics[metric->get_name()] = metric;
        }

        auto& get_metrics()
        {
            std::shared_lock lock(m_mutex);
            return m_metrics;
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<BaseMetric>> m_metrics;
        std::shared_mutex m_mutex;
    };

    class MetricsLogger
    {
    public:
        MetricsLogger(MetricsStorage& reg, const std::string& filename, std::chrono::milliseconds interval)
                        : m_storage(reg), m_interval(interval) 
        {
            m_file.open(filename);
            if (!m_file.is_open())
                throw std::runtime_error("Failed to open log file: " + filename);
            m_worker = std::thread([this] { run(); });
        }
        ~MetricsLogger()
        {
            running = false;
            if (m_worker.joinable()) m_worker.join();
            m_file.close();
        }

        template <class T>
        std::shared_ptr<Metric<T>> create(const std::string &name, const T& value)
        {
            auto m = std::make_shared<Metric<T>>(name, value);
            m_storage.add(m);
            return m;
        }

    private:
        void run()
        {
            while (running) {
                auto now = std::chrono::system_clock::now();
                //auto time_t_now = std::chrono::system_clock::to_time_t(now);
                //auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

                //std::stringstream ss;
                //ss << std::put_time(std::localtime(&time_t_now), "%F %T");
                //ss << '.' << std::setw(3) << std::setfill('0') << ms.count();
                m_file << now;
                for (auto& [_, metric]: m_storage.get_metrics()) {
                    m_file << " \"" << metric->get_name() << "\" " << metric->get_value_reset();
                }

                m_file << '\n';

                std::this_thread::sleep_for(m_interval);
            }
        }

    private:
        std::ofstream m_file;
        MetricsStorage& m_storage;
        std::shared_mutex m_mutex;
        std::thread m_worker;
        std::atomic<bool> running{true};
        std::chrono::milliseconds m_interval;
    };

} // namespace vk