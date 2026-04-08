#ifndef AAUDIOPLAYER_AAUDIOENGINE_H
#define AAUDIOPLAYER_AAUDIOENGINE_H

#include <aaudio/AAudio.h>
#include <jni.h>
class AAudioEngine {

public:
    AAudioEngine();
    aaudio_result_t initStream(int );
    AAudioStream *stream_ = nullptr;

    aaudio_result_t reqOpenStream(AAudioStreamBuilder *pStruct);
    aaudio_result_t playStream(AAudioStream *stream_);
    aaudio_result_t createPlayback(jint i);
    bool stopPlayBack();

    static double constexpr frequency = 440.0;
    static int constexpr mSampleRate = 44100;
    static constexpr double amplitude = 0.1;
    static double constexpr kPI = M_PI;
    static double constexpr kTwoPi = kPI * 2;
    static int constexpr mChannelCount = 2;
    static double  constexpr mPhaseIncrement = frequency * kTwoPi / (double) mSampleRate;

    static void generateSineWave(void *pVoid, int32_t frames);

private:
    double mPhase = 0.0;
    int mGlobalFormat = AAUDIO_FORMAT_PCM_FLOAT;
};

#endif //AAUDIOPLAYER_AAUDIOENGINE_H
