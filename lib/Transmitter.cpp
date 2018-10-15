//
// Created by linwei on 2018/9/11.
//

#include <Transmitter.hpp>
#include <iostream>

#include "Transmitter.hpp"
#include <fstream>

Transmitter::sptr Transmitter::_instance = NULL;

Transmitter::Transmitter() {
    std::cout << "Transmitter()" << std::endl;
    this->_dev = LimeDevice::make();
    this->loop = true;
    this->n = 0;
    this->_state = MS_IDLE;

    this->_state = MS_IDLE;
    this->loop = true;
    this->_work_thread = boost::make_shared<boost::thread>(&Transmitter::Working, this);


}


void Transmitter::Working() {
    while (loop) {
        switch (_state) {
            case MS_IDLE:
                processIDLE();
                break;
            case MS_SENDING:
                processSending();
                break;
            case MS_SET_PARAM:
                processSetParam();
                break;
        }
    }
}

void Transmitter::Close() {
    this->loop = false;
}

void Transmitter::processIDLE() {
    popSignal();
    boost::this_thread::sleep(boost::posix_time::milliseconds(2));
}

void Transmitter::processSending() {

    this->sending = true;

    //Initialize data buffers
    const int tx_size = 1024; //1k samples period
    short tx_buffer[2 * tx_size];     //buffer to hold complex values (2*samples))

    const short max_data = (1 << 10) - 1;//

    //构造信号
    switch (_source_type) {
        case SOURCE_CONST: {
            std::cout << "发送单音" << std::endl;
            for (int i = 0; i < tx_size; ++i) {
                tx_buffer[2 * i] = max_data;
                tx_buffer[2 * i + 1] = max_data;
            }
            break;
        }
        case SOURCE_SQUARE: {
            std::cout << "发送方波" << std::endl;
            for (int i = 0; i < tx_size; ++i) {
                if (i < tx_size / 2) {
                    tx_buffer[2 * i] = max_data;
                    tx_buffer[2 * i + 1] = max_data;
                } else {
                    tx_buffer[2 * i] = 0;
                    tx_buffer[2 * i + 1] = 0;
                }

            }
            break;
        }
        case SOURCE_TRIANGLE: {
            std::cout << "发送三角波" << std::endl;

            for (int i = 0; i < tx_size; ++i) {
                tx_buffer[2 * i] = static_cast<short>(i * max_data / tx_size);
                tx_buffer[2 * i + 1] = static_cast<short>(i * max_data / tx_size);
            }
            break;
        }
        default: {
            std::cout << "发送全零" << std::endl;
            for (int i = 0; i < tx_size; ++i) {
                tx_buffer[2 * i] = 0;
                tx_buffer[2 * i + 1] = 0;
            }
        }
    }

    std::ofstream output("out.dat", std::ios::binary | std::ios::trunc);
    output.write((char *) tx_buffer, sizeof(short) * 2 * tx_size);
    output.close();


    this->_dev->start();
    //发送
    while (sending && loop) {
        this->_dev->send(tx_buffer, tx_size, 100);
//        while (this->_dev->send(tx_buffer, tx_size, 100) <= 0 && loop) {
//           // 没发出去重发,FIFO出去的速度不够快
//        }
    }

    this->_dev->stop();

    _state = MS_IDLE;
    std::cout << "结束发送" << std::endl;
}

void Transmitter::processSetParam() {
    std::cout << "设置参数" << std::endl;
    _dev->setTxCenterFreq(this->_tx_params.freq);
    _dev->setTxSampleRate(this->_tx_params.sample_rate);
    _dev->setTxGain(this->_tx_params.gain);

    this->_source_type = this->_tx_params.st;
    std::cout << "中心频率: " << this->_tx_params.freq / 1e6
              << " Mhz 采样率: " << this->_tx_params.sample_rate / 1e6
              << " Mhz 发送增益: " << this->_tx_params.gain
              << " db 发送类型: " << this->_source_type;
    std::cout << " 设置参数完毕" << std::endl;
    _state = MS_IDLE;
}


Transmitter::sptr Transmitter::make() {
    if (_instance == NULL)
        return Transmitter::sptr(new Transmitter());
    else
        return _instance;
}

Transmitter::~Transmitter() {
    if (_work_thread->joinable())
        _work_thread->join();
    this->Close();
    std::cout << "Transmittertter()" << std::endl;
}

void Transmitter::setTxParams(Transmitter::TX_PARAMS params) {
    this->_tx_params = params;
    this->addSignal(MS_SET_PARAM);
}

void Transmitter::popSignal() {
    _mtx.lock();
    if (this->_signals.size() > 0) {
        MS_STATE state = this->_signals.front();
        _state = state;//切换状态
        this->_signals.pop();
    }
    _mtx.unlock();
}

void Transmitter::addSignal(Transmitter::MS_STATE state) {
    _mtx.lock();
    this->_signals.push(state);
    _mtx.unlock();
}

void Transmitter::Start() {
    this->addSignal(MS_SENDING);
}

void Transmitter::Stop() {
    this->sending = false;
}
