#include "log.h"

int main()
{
    // loglib::Log log("output_metrics.txt");
    // std::cout << "a" << 1;
    // log << std::make_pair<std::string, int>("CPU", 1) << std::make_pair<std::string, float>("CPU", 1.1);
    // MetricsLogger metrics_logger;
    // metrics_logger.log("CPU", 1);
    vk::MetricsStorage storage;
    std::shared_ptr<vk::Metric<int>> cpu("CPU", 0);
    storage.add(cpu);
    vk::MetricsLogger ml("output.txt");
    return 0;
}