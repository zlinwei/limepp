#include <iostream>
#include <chrono>
#include <math.h>
#include <thread>
#include <string>
#include <Transmitter.hpp>


int main(int argc, char **argv) {
    Transmitter::sptr sdr = Transmitter::make();

    Transmitter::TX_PARAMS params{};
    params.freq = 500e6;
    params.gain = 70;
    params.sample_rate = 2e6;
    params.st = Transmitter::SOURCE_CONST;

    sdr->setTxParams(params);

    std::string command;
    for (;;) {
        std::cin >> command;
        if (command == "exit") {
            break;
        }

        if (command == "gain") {
            float gain;
            std::cin >> gain;
            params.gain = gain;
            sdr->setTxParams(params);
            continue;
        }

        if (command == "freq") {
            double freq;
            std::cin >> freq;
            params.freq = freq;
            sdr->setTxParams(params);
            continue;
        }

        if (command == "fs") {
            double fs;
            std::cin >> fs;
            params.sample_rate = fs;
            sdr->setTxParams(params);
            continue;
        }

        if (command == "tri") {
            params.st = Transmitter::SOURCE_TRIANGLE;
            sdr->setTxParams(params);
            continue;
        }

        if (command == "square") {
            params.st = Transmitter::SOURCE_SQUARE;
            sdr->setTxParams(params);
            continue;
        }

        if (command == "wave") {
            params.st = Transmitter::SOURCE_CONST;
            sdr->setTxParams(params);
            continue;
        }

        if (command == "start") {
            sdr->Start();
            continue;
        }

        if (command == "stop") {
            sdr->Stop();
            continue;
        }


    }

    sdr->Close();

    return 0;
}

