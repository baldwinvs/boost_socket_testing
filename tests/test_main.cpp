#define CATCH_CONFIG_RUNNER
#include <catch2/catch_session.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    Catch::Session session;

    // Optional: configure here
    Catch::ConfigData configData {};
    configData.listTests = true;

    std::cout << "== Listing Catch2 tests ==" << std::endl;
    session.useConfigData(configData);
    session.run();

    configData = {};
    configData.showDurations = Catch::ShowDurations::Always; // show test durations
    configData.verbosity = Catch::Verbosity::High;

    // Do any setup here (e.g. global init, logging)
    std::cout << "== Running Catch2 tests ==" << std::endl;
    session.useConfigData(configData);
    session.run();

    // Do any teardown here
    return EXIT_SUCCESS;
}