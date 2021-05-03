#include <cstdio>
#include <iostream>

// Inludes common necessary includes for development using depthai library
#include "depthai/depthai.hpp"
void printSystemInformation(dai::SystemInformation info) {
    printf("Ddr used / total - %.2f / %.2f MiB\n", info.ddrMemoryUsage.used / (1024.0f * 1024.0f), info.ddrMemoryUsage.total / (1024.0f * 1024.0f));
    printf("Cmx used / total - %.2f / %.2f MiB\n", info.cmxMemoryUsage.used / (1024.0f * 1024.0f), info.cmxMemoryUsage.total / (1024.0f * 1024.0f));
    printf("LeonCss heap used / total - %.2f / %.2f MiB\n",
           info.leonCssMemoryUsage.used / (1024.0f * 1024.0f),
           info.leonCssMemoryUsage.total / (1024.0f * 1024.0f));
    printf("LeonMss heap used / total - %.2f / %.2f MiB\n",
           info.leonMssMemoryUsage.used / (1024.0f * 1024.0f),
           info.leonMssMemoryUsage.total / (1024.0f * 1024.0f));
    const auto& t = info.chipTemperature;
    printf("Chip temperature - average: %.2f, css: %.2f, mss: %.2f, upa0: %.2f, upa1: %.2f\n", t.average, t.css, t.mss, t.upa, t.dss);
    printf("Cpu usage - Leon OS: %.2f %%, Leon RT: %.2f %%\n", info.leonCssCpuUsage.average * 100, info.leonMssCpuUsage.average * 100);
    printf("----------------------------------------\n");
}

int main() {
    using namespace std;

    // Start defining a pipeline
    dai::Pipeline pipeline;

    auto sysLog = pipeline.create<dai::node::SystemLogger>();
    auto xout = pipeline.create<dai::node::XLinkOut>();

    xout->setStreamName("sysinfo");

    // Properties
    sysLog->setRate(1.0f);  // 1 hz updates

    // Linking
    sysLog->out.link(xout->input);

    // Connect to device
    dai::Device device;

    // Query device (before pipeline starts)
    dai::MemoryInfo ddr = device.getDdrMemoryUsage();
    printf("Ddr used / total - %.2f / %.2f MiB\n", ddr.used / (1024.0f * 1024.0f), ddr.total / (1024.0f * 1024.0f));

    dai::MemoryInfo cmx = device.getCmxMemoryUsage();
    printf("Cmx used / total - %.2f / %.2f MiB\n", cmx.used / (1024.0f * 1024.0f), cmx.total / (1024.0f * 1024.0f));

    // Start pipeline
    device.startPipeline(pipeline);

    // Output queue will be used to get the system info
    auto qSysInfo = device.getOutputQueue("sysinfo", 4, false);

    while(true) {
        auto sysInfo = qSysInfo->get<dai::SystemInformation>();
        printSystemInformation(*sysInfo);
    }
    return 0;
}
