# pragma once

#include "RingBuffer.hpp"

#include <inttypes.h>
#include <string>
#include <stdio.h>
#include <memory>
#include <thread>
#include <unistd.h>

#define MAX_SESSION               10    // Client sessions.
#define MAX_PCM_INTERNAL_BUF    5000    // Auxliar internal frame buffers.
#define PCM_FRAME                320    // Frame for incomming PCM buffer stream from decoded opus data.

class wimic_Callback;

namespace WIMIC {
    const wimic_Callback &get_instance();
}

class wimic_Callback
{
public:
    wimic_Callback();
    ~wimic_Callback(void);

    void init(std::shared_ptr<RingBuffer<int16_t>> out_buf);
    void update_audio(int target,
                      int sessionId,
                      int sequenceNumber,
                      int16_t *pcm_data,
                      uint32_t pcm_data_size);

    static wimic_Callback *get_instance()
    {
        return _instance;
    }

private:

    static wimic_Callback *_instance;
    static bool _initialized;

    static bool _started;
    static uint8_t _chan_speak_cnt;

    static std::shared_ptr<RingBuffer<int16_t>> _out_buf;
    static std::shared_ptr<RingBuffer<int16_t>> _out_buf_tmp[MAX_SESSION];

    static int16_t _pcmbuf_out[MAX_PCM_INTERNAL_BUF];
    static int16_t _pcmbuf_in[MAX_PCM_INTERNAL_BUF];
    static int16_t _pcmbuf_aux[MAX_PCM_INTERNAL_BUF];

    static void *_timer_buf(void *arg);
    static int16_t mix_pcm(int16_t pcm_one, int16_t pcm_two);
};
