#include <iostream>
#include <chrono>
#include <math.h>
#include <thread>
#include <string>
#include <Transmitter.hpp>

void showUsage();


int main(int argc, char **argv) {
    Transmitter::sptr sdr = Transmitter::make();
    showUsage();
    Transmitter::TX_PARAMS params{};
    params.freq = 30e6;
    params.gain = 70;
    params.sample_rate = 10e6;
    params.st = Transmitter::SOURCE_TRIANGLE;

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

void showUsage(){
    std::cout << "command\t\tparam\t\tdetail\n"
     << "start\t\tNone\t\tStart\n"
     << "stop\t\tNone\t\tStop\n"
     << "freq\t\t10e6\t\tSet Frequency\n"
     << "gain\t\t50\t\tSet Gain\n"
     << "fs\t\t10e6\t\tSet TxRate\n"
     << "tri\t\tNone\t\tSet Triangle Wave\n"
     << "square\t\tNone\t\tSet Square Wave\n"
     << "wave\t\tNone\t\tSet Const Wave" << std::endl;
}

