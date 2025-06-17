#include "log.h"

int main() {
    vk::MetricsStorage storage;
    vk::MetricsLogger logger(storage, "output.txt", std::chrono::milliseconds(1000));
    auto cpu = logger.create<int>("CPU", 0);
    auto https = logger.create<double>("HTTPS", 0);
    for(int i = 0; i < 10; ++i){
        cpu->set(i + std::rand());
        https->set(i + 0.7 + std::rand());
    }
    return 0;
}