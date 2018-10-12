//
// Created by linwei on 2018/9/11.
//

#ifndef LIME_LIMEDEVICE_HPP
#define LIME_LIMEDEVICE_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <lime/LimeSuite.h>

class LimeDevice {
private:
    //Device structure, should be initialize to NULL
    lms_device_t *_device;

    lms_stream_t _tx_stream;                 //stream structure

    //should be large enough to hold all detected devices
    lms_info_str_t _list[8];

    int _device_num;

    double _freq;//center freq

    double _sample_rate;//fs

    float _gain;

public:
    typedef boost::shared_ptr<LimeDevice> sptr;

    LimeDevice();

    ~LimeDevice();

    int start();

    int stop();

    int send(short *samples, size_t size, unsigned int timeout);

    void setTxSampleRate(double sample_rate);

    void setTxCenterFreq(double freq);

    void setTxGain(float gain);

    static LimeDevice::sptr make();
};


#endif //LIME_LIMEDEVICE_HPP
