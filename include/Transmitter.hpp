//
// Created by linwei on 2018/9/11.
//

#ifndef LIME_LIMETRANSMITTER_HPP
#define LIME_LIMETRANSMITTER_HPP


#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/mutex.hpp>
#include <queue>

#include "lime/LimeSuite.h"
#include "LimeDevice.hpp"


class Transmitter {
public:
    typedef boost::shared_ptr<Transmitter> sptr;

    enum MS_STATE {
        MS_IDLE = 0,
        MS_SENDING,
        MS_SET_PARAM
    };

    enum SOURCE_TYPE {
        SOURCE_FILE = 0,
        SOURCE_TRIANGLE,
        SOURCE_SQUARE,
        SOURCE_CONST,
        SOURCE_NORMAL
    };

    struct TX_PARAMS {
        double sample_rate;
        double freq;
        float gain;
        SOURCE_TYPE st;
    };

private:
    boost::mutex _mtx;

    std::queue<MS_STATE> _signals;

    //Find devices
    int n;

    TX_PARAMS _tx_params;

    SOURCE_TYPE _source_type;

    boost::shared_ptr<boost::thread> _work_thread;

    bool loop;

    bool sending;

    MS_STATE _state;

    static Transmitter::sptr _instance;

    LimeDevice::sptr _dev;

public:
    Transmitter();

    ~Transmitter();

    void Close();

    static Transmitter::sptr make();


    //给外部使用的控制指令
    void setTxParams(TX_PARAMS params);

    void Start();

    void Stop();

private:

    void popSignal();

    void addSignal(MS_STATE state);

    void Working();

    void processIDLE();

    void processSending();

    void processSetParam();



};


#endif //LIME_LIMETRANSMITTER_HPP
