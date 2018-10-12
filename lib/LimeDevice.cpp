//
// Created by linwei on 2018/9/11.
//

#include "LimeDevice.hpp"
#include <iostream>
#include <LimeDevice.hpp>


int LimeDevice::start() {
    _device_num = LMS_GetDeviceList(this->_list);
    if (_device_num <= 0) return -1;

    std::cout << "开始发送" << std::endl;
    if (LMS_Open(&_device, _list[0], NULL)) {
        //error
        throw "打开设备失败 LMS_Open()";
    }


    if (LMS_Init(_device) != 0) {
        throw "打开设备失败 LMS_Init()";
    }

    if (LMS_EnableChannel(_device, LMS_CH_TX, 0, true) != 0) {
        std::cerr << "打开通道失败 " << __FILE__ << __LINE__ << __FUNCTION__ << std::endl;
        throw "打开通道设备";
    }

    // set param
    if (LMS_SetSampleRate(_device, _sample_rate, 0) != 0) {
        throw "设置采样率失败";
    }
    if (LMS_SetLOFrequency(_device, LMS_CH_TX, 0, _freq) != 0) {
        throw "设置频率出错";
    }
    if (LMS_SetAntenna(_device, LMS_CH_TX, 0, LMS_PATH_TX1) != 0) {
        throw "设置天线失败";
    }
    if (LMS_SetNormalizedGain(_device, LMS_CH_TX, 0, _gain) != 0) {
        throw "设置发射增益失败";
    }

    _tx_stream.channel = 0;                  //channel number
    _tx_stream.fifoSize = 20480 * 1024;          //fifo size in samples
    _tx_stream.throughputVsLatency = 1;    //0 min latency, 1 max throughput
    _tx_stream.dataFmt = lms_stream_t::LMS_FMT_I12; //short samples max (1<<10)-1
    _tx_stream.isTx = true;                  //TX channel

    if (LMS_SetupStream(_device, &_tx_stream) < 0) {
        throw "setup stream error";
    }

    //Start streaming
    if (LMS_StartStream(&_tx_stream) < 0) {
        throw "start stream error";
    }

    return 0;
}

int LimeDevice::stop() {
    //Stop streaming
    LMS_StopStream(&_tx_stream);

    LMS_DestroyStream(_device, &_tx_stream);

    if (LMS_EnableChannel(_device, LMS_CH_TX, 0, false) != 0) {
        std::cerr << "关闭通道失败 " << __FILE__ << __LINE__ << __FUNCTION__ << std::endl;
    }

    LMS_Close(_device);
    _device = NULL;
    return 0;
}

void LimeDevice::setTxSampleRate(double sample_rate) {
    this->_sample_rate = sample_rate;
}

void LimeDevice::setTxCenterFreq(double freq) {
    this->_freq = freq;
}


void LimeDevice::setTxGain(float gain) {
    this->_gain = gain;
}

LimeDevice::sptr LimeDevice::make() {
    return LimeDevice::sptr(new LimeDevice());
}

LimeDevice::LimeDevice() {
    std::cout << "LimeDevice()" << std::endl;
}

LimeDevice::~LimeDevice() {
    std::cout << "~LimeDevice()" << std::endl;
}

int LimeDevice::send(short *tx_buffer, size_t tx_size, unsigned int timeout) {
    int ret = LMS_SendStream(&_tx_stream, tx_buffer, tx_size, nullptr, timeout);
    return ret;
//    return 0;
}
