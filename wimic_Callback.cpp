#include "wimic_Callback.h"

bool wimic_Callback::_initialized = false;
wimic_Callback *wimic_Callback::_instance = nullptr;

bool wimic_Callback::_started = false;
std::shared_ptr<RingBuffer<int16_t>> wimic_Callback::_out_buf;
std::shared_ptr<RingBuffer<int16_t>> wimic_Callback::_out_buf_tmp[MAX_SESSION];
uint8_t wimic_Callback::_chan_speak_cnt = 0;

int16_t wimic_Callback::_pcmbuf_out[MAX_PCM_INTERNAL_BUF] = {0};
int16_t wimic_Callback::_pcmbuf_in[MAX_PCM_INTERNAL_BUF] = {0};
int16_t wimic_Callback::_pcmbuf_aux[MAX_PCM_INTERNAL_BUF] = {0};

wimic_Callback::wimic_Callback()
{
}

wimic_Callback::~wimic_Callback() {
    _started = false;
    usleep(50000);
}

void wimic_Callback::init(std::shared_ptr<RingBuffer<int16_t>> out_buf)
{
    if (_initialized) {
        return;
    }

    _out_buf = out_buf;

    for (uint8_t i = 0; i < MAX_SESSION; i++) {
        _out_buf_tmp[i] = std::make_shared<RingBuffer<int16_t>>((uint16_t)-1);

    }

    pthread_t timer_buf_thread;
    pthread_attr_t thread_attr_timer_buf;

    pthread_attr_init(&thread_attr_timer_buf);
    pthread_attr_setstacksize(&thread_attr_timer_buf, 2048);

    pthread_attr_setschedpolicy(&thread_attr_timer_buf, SCHED_FIFO);

    pthread_create(&timer_buf_thread, &thread_attr_timer_buf, wimic_Callback::_timer_buf, NULL);

    _initialized = true;
    _instance = this;
}

void wimic_Callback::update_audio(int target,
                                  int sessionId,
                                  int sequenceNumber,
                                  int16_t *pcm_data,
                                  uint32_t pcm_data_size)
{
    if(pcm_data != NULL) {
        _out_buf_tmp[sessionId]->push(pcm_data, 0, pcm_data_size);
        _chan_speak_cnt = _chan_speak_cnt | (1 << (uint8_t)sessionId);
    }
}

void *wimic_Callback::_timer_buf(void *arg)
{
    if (_started) {
        return NULL;
    }

    _started = true;

    for (uint16_t c1 = 0; c1 < MAX_PCM_INTERNAL_BUF; c1++) {
        _pcmbuf_in[c1] = 0;
        _pcmbuf_out[c1] = 0;
        _pcmbuf_aux[c1] = 0;
    }

    while (_started) {
        uint8_t spkcnt = 0;
        uint8_t sessid = 0;
        for (uint8_t i = 0; i < MAX_SESSION; i++) {
            if ((_chan_speak_cnt >> (uint8_t)i) & 0x01) {
                spkcnt++;
                sessid = i;
                if (spkcnt > 1) {
#if DEBUG_WIMIC == 1
                    printf("chan %d\n", spkcnt);
#endif // DEBUG_WIMIC
                    _chan_speak_cnt = 0;
                    break;
                }
                uint16_t ret = _out_buf_tmp[sessid]->top(_pcmbuf_out, 0, PCM_FRAME);
#if DEBUG_WIMIC == 1
                if (ret > 0) {
                    printf("One channel %d\n", sessid);
                }
#endif // DEBUG_WIMIC
            }
        }

        if (spkcnt > 1) {
            for (uint8_t i1 = 0; i1 < MAX_SESSION; i1++) {
                uint16_t ret = _out_buf_tmp[i1]->top(_pcmbuf_in, 0, PCM_FRAME);
                for (uint16_t i = 0; i < PCM_FRAME; ++i) {
                    _pcmbuf_out[i] = (int16_t)mix_pcm(_pcmbuf_aux[i], _pcmbuf_in[i]);
                }

                for (uint16_t i = 0; i < PCM_FRAME; ++i) {
                    _pcmbuf_aux[i] = _pcmbuf_in[i];
                }
            }
        }

        _out_buf->push(_pcmbuf_out, 0, PCM_FRAME);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    _started = false;

    printf("Exit timer buf\n");
    pthread_exit(NULL);
}

int16_t wimic_Callback::mix_pcm(int16_t pcm_one, int16_t pcm_two)
{
    const int32_t result(static_cast<int32_t>(pcm_one) + static_cast<int32_t>(pcm_two));
    typedef std::numeric_limits<int16_t> rlimit;
    if (rlimit::max() < result)
        return rlimit::max();
    else if (rlimit::min() > result)
        return rlimit::min();
    else
        return result;
}

const wimic_Callback &WIMIC::get_instance()
{
    static const wimic_Callback wimic_callback;
    return wimic_callback;
}
