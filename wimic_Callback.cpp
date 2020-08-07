/*
   WiMic remote wireless microphone server/client.
   Copyright (c) 2020 Hiroshi Takey F. <htakey@gmail.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "wimic_Callback.h"
#include "typedef_ext.h"
#include <speex/speex_resampler.h>
#include <math.h>
#include <string.h>

bool wimic_Callback::_initialized = false;
wimic_Callback *wimic_Callback::_instance = nullptr;

bool wimic_Callback::_started = false;
std::shared_ptr<RingBuffer<int16_t>> wimic_Callback::_out_buf;
std::shared_ptr<RingBuffer<int16_t>> wimic_Callback::_out_buf_tmp[MAX_SESSION];
uint16_t wimic_Callback::_chan_speak_cnt = 0;

int16_t wimic_Callback::_pcmbuf_out[MAX_PCM_INTERNAL_BUF] = {0};
int16_t wimic_Callback::_pcmbuf_in[MAX_PCM_INTERNAL_BUF] = {0};

extern wm_system_status_t wmsystem_status;

wimic_Callback::wimic_Callback()
{
}

wimic_Callback::~wimic_Callback() {
    _started = false;
    _initialized = false;
    usleep(50000);
}

void wimic_Callback::_resampler_float(uint16_t inputSr, uint16_t outputSr, uint16_t channels, uint16_t frames, int16_t* in_data, uint16_t &out_length, int16_t* out_data, bool resample_float)
{
    typedef std::numeric_limits<int16_t> rlimit;
	int resampler_err = 0;
	uint32_t total_samples_written = 0;

	/* It's improvised, we solve it with a memory storage
     * allocator or something like that.
	 */
	float *inbuffer = nullptr;
	float *outbuffer = nullptr;
	float *outbuffer_feed = nullptr;
	int16_t inbuffer_int[MAX_PCM_INTERNAL_BUF];
	int16_t outbuffer_int[MAX_PCM_INTERNAL_BUF];
	int16_t outbuffer_feed_int[MAX_PCM_INTERNAL_BUF];
#ifndef __ARM_EABI__
    static int16_t echo_buffer[MAX_PCM_INTERNAL_BUF];
#endif // __ARM_EABI__
    static int16_t in_out_datatmp[MAX_PCM_INTERNAL_BUF];

	uint16_t output_frames = (channels * (outputSr / 1000) * 20 * (frames / 320));
	uint16_t input_frames = (channels * (inputSr / 1000) * 20 * (frames / 320));

	const uint32_t expected_samples_written = output_frames;

	if (inputSr == outputSr) {
        for (uint16_t i = 0; i < input_frames; i++) {
            out_data[i] = in_data[i];
        }
#ifndef __ARM_EABI__ // we do not make echo canceller on eabi meanwhile.
        _update_echo_canceller(&in_out_datatmp[0], &out_data[0], &echo_buffer[0], outputSr, output_frames);
#endif
        out_length = output_frames;
        return;
	}

	SpeexResamplerState *resampler_state = speex_resampler_init(channels, inputSr, outputSr, SPEEX_RESAMPLER_QUALITY_DEFAULT, &resampler_err);
	speex_resampler_skip_zeros(resampler_state);

    for (uint16_t i = 0; i < input_frames; i++) {
        if (resample_float) {
            if (!inbuffer) {
                inbuffer = new float[MAX_PCM_INTERNAL_BUF];
            }
            if (!outbuffer) {
                outbuffer = new float[MAX_PCM_INTERNAL_BUF];
            }
            if (!outbuffer_feed) {
                outbuffer_feed = new float[MAX_PCM_INTERNAL_BUF];
            }
            inbuffer[i] = (float)in_data[i] / rlimit::max();
        } else {
            inbuffer_int[i] = in_data[i];
        }
    }

	uint32_t in_processed = input_frames;
	uint32_t out_processed = output_frames;

	if (resample_float) {
        speex_resampler_process_float(resampler_state, 0, inbuffer, &in_processed, outbuffer, &out_processed);
	} else {
	    speex_resampler_process_int(resampler_state, 0, inbuffer_int, &in_processed, outbuffer_int, &out_processed);
	}

	total_samples_written += out_processed;

    for (uint16_t i = 0; i < total_samples_written; i++) {
        if (resample_float) {
            out_data[i] = (int16_t)outbuffer[i] * rlimit::max();
        } else {
            out_data[i] = outbuffer_int[i];
        }
    }

    const uint16_t samplesneeded = (uint16_t)(expected_samples_written - total_samples_written);

    in_processed = samplesneeded;
    out_processed = samplesneeded;

    if (resample_float) {
        speex_resampler_process_float(resampler_state, 0, inbuffer + (input_frames - samplesneeded), &in_processed, outbuffer_feed, &out_processed);
    } else {
        speex_resampler_process_int(resampler_state, 0, inbuffer_int + (input_frames - samplesneeded), &in_processed, outbuffer_feed_int, &out_processed);
    }

    uint16_t cntbuf = 0;
    for (uint16_t i = (output_frames - samplesneeded); i < output_frames; i++) {
        if (resample_float) {
            out_data[i] = (int16_t)outbuffer_feed[cntbuf] * rlimit::max();
        } else {
            out_data[i] = outbuffer_feed_int[cntbuf];
        }
        cntbuf++;
    }

#ifndef __ARM_EABI__ // we do not make echo canceller on eabi meanwhile.
    _update_echo_canceller(&in_out_datatmp[0], &out_data[0], &echo_buffer[0], outputSr, output_frames);
#endif // __ARM_EABI__
    out_length = output_frames;
    speex_resampler_destroy(resampler_state);
}

void wimic_Callback::_update_echo_canceller(int16_t *in_out_datatmp, int16_t *out_data, int16_t *echo_buffer, uint16_t outputSr, uint16_t output_frames)
{
#ifndef __ARM_EABI__ // we do not make echo canceller on eabi meanwhile.

    for (uint16_t i = 0; i < output_frames; i++) {
        in_out_datatmp[i] = out_data[i];
    }

    uint32_t sampling_rate_echo;
    SpeexEchoState *echo_state = speex_echo_state_init(output_frames, output_frames * 5);
    sampling_rate_echo = outputSr;
    speex_echo_ctl(echo_state, SPEEX_ECHO_SET_SAMPLING_RATE, &sampling_rate_echo);
    speex_echo_cancellation(echo_state, in_out_datatmp, echo_buffer, out_data);

    for (uint16_t i = 0; i < output_frames; i++) {
        echo_buffer[i] = out_data[i];
    }

	speex_echo_state_destroy(echo_state);

#endif // __ARM_EABI__
}

void wimic_Callback::init(std::shared_ptr<RingBuffer<int16_t>> out_buf)
{
    if (_initialized) {
        printf("Wimic Callback already initialized!\n");
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
        _chan_speak_cnt = _chan_speak_cnt | (1 << (uint16_t)sessionId);
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
    }

    while (_started) {
        uint8_t spkcnt = 0;
        uint16_t remainbuf = 0;
        for (uint8_t sessid = 0; sessid < MAX_SESSION; sessid++) {
            uint16_t chantmp = _chan_speak_cnt;
            if ((chantmp >> (uint16_t)sessid) & 0x01) {
                spkcnt++;
                if (spkcnt > 1) {
#if DEBUG_WIMIC == 1
                    printf("ch: %d cnt: %d\n", sessid, spkcnt);
#endif // DEBUG_WIMIC
                    break;
                }
            }
        }

        for (uint8_t i1 = 0; i1 < MAX_SESSION; i1++) {
            remainbuf = _out_buf_tmp[i1]->getRemaining();
            if (remainbuf >= PCM_FRAME) {
                uint16_t ret = _out_buf_tmp[i1]->top(_pcmbuf_in, 0, PCM_FRAME);
                for (uint16_t i = 0; i < PCM_FRAME; ++i) {
                    _pcmbuf_out[i] = (int16_t)mix_pcm(_pcmbuf_out[i], _pcmbuf_in[i]);
                }
            }

            if (remainbuf < PCM_FRAME) {
                _chan_speak_cnt = _chan_speak_cnt & ~(0x01 << (uint16_t)i1);
            }
        }

        static int16_t pcmout[MAX_PCM_INTERNAL_BUF];
        uint16_t nrframe = 0;

        if ((remainbuf >= PCM_FRAME) || (spkcnt >= 1)) {
            _resampler_float(16000, 16000, 1, PCM_FRAME, _pcmbuf_out, nrframe, pcmout);
            _out_buf->push(pcmout, 0, nrframe);
            for (uint16_t i = 0; i < MAX_PCM_INTERNAL_BUF; i++) {
                _pcmbuf_out[i] = 0;
                pcmout[i] = 0;
                _pcmbuf_in[i] = 0;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(PCM_FRAME / 16));
    }

    _started = false;

    printf("Exit timer buf\n");
    if (wmsystem_status.silent_mode) {
        exit(1);
    }
    pthread_exit(NULL);
}

inline int16_t wimic_Callback::mix_pcm(int16_t pcm_one, int16_t pcm_two)
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

void wimic_Callback::stop()
{
    _started = false;
    _initialized = false;
}

void wimic_Callback::_resampler(uint16_t inputSr, uint16_t outputSr, uint16_t channels, uint16_t frames, int16_t* data, uint16_t &out_length, int16_t* out_data)
{
	int err;
	auto resampler = speex_resampler_init(channels, inputSr, outputSr, SPEEX_RESAMPLER_QUALITY_VOIP, &err);

	spx_uint32_t out_len = (channels * (outputSr / 1000.0) * 20 * 2);
	spx_uint32_t in_len = (channels * frames);

	spx_int16_t *data_in = data;
	spx_int16_t *data_out = out_data;
    speex_resampler_process_int(resampler, 0, data_in, &in_len, data_out, &out_len);
#if DEBUG_WIMIC == 1
	printf("written %d - readed %d Lenframes: %d\n", (uint32_t)out_len, (uint32_t)in_len, (uint32_t)(channels * frames));
#endif // DEBUG_WIMIC

	out_length = (uint16_t)out_len;
	speex_resampler_destroy(resampler);
}

const wimic_Callback &WIMIC::get_instance()
{
    static const wimic_Callback wimic_callback;
    return wimic_callback;
}
