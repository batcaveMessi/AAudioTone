#include <aaudio/AAudio.h>
#include <iostream>
#include "AAudioEngine.h"
#include <android/log.h>



using namespace std;
AAudioEngine::AAudioEngine() : mPhase(0.0), mGlobalFormat(AAUDIO_FORMAT_PCM_FLOAT) {}

aaudio_result_t AAudioEngine::createPlayback(jint mFormat) {
    aaudio_result_t playStreamReturn, initStreamReturn;
    if(mFormat == 4) {
        mFormat = AAUDIO_FORMAT_PCM_FLOAT;
    } else if(mFormat == 2){
        mFormat = AAUDIO_FORMAT_PCM_I16;
    }
    mGlobalFormat = mFormat;
    initStreamReturn = initStream(mFormat);
    if(initStreamReturn == AAUDIO_OK){
        playStreamReturn = playStream(stream_);
        if(playStreamReturn == AAUDIO_OK)
            return playStreamReturn;
        return playStreamReturn;
    }
    return initStreamReturn;
}

aaudio_data_callback_result_t myCallback(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames) {
    // Write samples directly into the audioData array.
    AAudioEngine::generateSineWave(audioData, numFrames);
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

void AAudioEngine::generateSineWave(void *pVoid, int32_t frames) {
	/* sampleValue = abs( AAudioEngine::amplitude - mPhase % (2*AAudioEngine::amplitude)) 
		Could be used for triangular waves.
	*/
	
    float *floatData = (float *) pVoid;
    for (int i = 0; i < frames; ++i) {
        float sampleValue = AAudioEngine::amplitude * sinf((float)mPhase);
        for (int j = 0; j < AAudioEngine::mChannelCount; j++) {
            floatData[i * AAudioEngine::mChannelCount + j] = sampleValue;
        }
        mPhase += AAudioEngine::mPhaseIncrement;
        if (mPhase >= AAudioEngine::kTwoPi){
            mPhase -= AAudioEngine::kTwoPi;
        }
    }
}

aaudio_result_t AAudioEngine::initStream(int mFormat) {
    AAudioStreamBuilder  *builder = nullptr;
    aaudio_result_t createBuilder = AAudio_createStreamBuilder(&builder);
    if(createBuilder != AAUDIO_OK){
        __android_log_print(ANDROID_LOG_DEBUG,"AAUDIOENGINE","%s", AAudio_convertResultToText(createBuilder));
        return createBuilder;
    }
    AAudioStreamBuilder_setFormat(builder, mFormat);
    AAudioStreamBuilder_setSampleRate(builder, 44100);
    AAudioStreamBuilder_setSharingMode(builder,AAUDIO_SHARING_MODE_EXCLUSIVE);
    AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE_NONE);
    AAudioStreamBuilder_setDataCallback(builder, myCallback, nullptr);

    return reqOpenStream(builder);
}

aaudio_result_t AAudioEngine::reqOpenStream(AAudioStreamBuilder *builder) {
    aaudio_result_t openStream = AAudioStreamBuilder_openStream(builder, &stream_);
    AAudioStreamBuilder_delete(builder);
    if(openStream != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_DEBUG,"AAUDIOENGINE","%s", AAudio_convertResultToText(openStream));
        return openStream;
    }
    return openStream;
}

aaudio_result_t AAudioEngine::deleteStream() {
    if(stream_ != nullptr){
        AAudioStreamBuilder_deleteStreamBuilder(stream_);
        stream_ = nullptr;
        return AAUDIO_OK;
    }
    return AAUDIO_ERROR_INVALID_STATE;
}

aaudio_result_t AAudioEngine::playStream(AAudioStream *stream_) {
    aaudio_result_t startStream_ = AAudioStream_requestStart(stream_);
    if(startStream_ != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_DEBUG,"AAUDIOENGINE","%s", AAudio_convertResultToText(startStream_));
        return startStream_;
    }
    return startStream_;
}

bool AAudioEngine::stopPlayBack() {
    if(stream_ != nullptr){
        AAudioStream_requestStop(stream_);
        AAudioStream_close(stream_);
        stream_ = nullptr;
        return true;
    }
    return false;
}
