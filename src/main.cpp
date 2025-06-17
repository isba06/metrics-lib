#include "log.h"

int main() {
    vk::MetricsStorage storage;
    vk::MetricsLogger logger(storage, "output.txt", std::chrono::milliseconds(1000));
    auto cpu = logger.create<int>("CPU", 0);
    auto https = logger.create<double>("HTTPS", 0);
    cpu->set(1);
    https->set(1.1);
    //logger.run();
    //std::shared_ptr<vk::BaseMetric> p = std::make_shared<vk::Metric>("CPU", 0);
    //storage.add(std::make_shared<vk::Metric<int>>("CPU", 0));
    //storage.add(std::make_shared<vk::Metric<int>>("HTTPS", 0));

    auto st = storage.get_metrics();
    for(const auto& [_, v] : st){
        std::cout << v->get_name() << " " << v->get_value() << std::endl;
    }
    //ml.event({{"CPU", 1}});
    return 0;
}