#include "log.h"

int main()
{
    // loglib::Log log("output_metrics.txt");
    // std::cout << "a" << 1;
    // log << std::make_pair<std::string, int>("CPU", 1) << std::make_pair<std::string, float>("CPU", 1.1);
    // MetricsLogger metrics_logger;
    // metrics_logger.log("CPU", 1);
    vk::MetricsLogger metrics_logger("output.txt");
    metrics_logger.event({{"CPU", 1}, {"HTTPS", 2}});
    return 0;
}