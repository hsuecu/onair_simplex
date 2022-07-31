#include <iostream>
#include <vector>
#include <complex>
#include <string.h>

using namespace std;

const int SAMPLE_RATE = 384000; // compartible for a 192khz rated mic
const float FREQ = 19000; // the target frequency to look for
const float FREQl = 18800;
const int WINDOW = 1920;//384;
const int RATIO = SAMPLE_RATE / WINDOW;

int main(int argc, const char * argv[]) {

    // storaing unit for 16 bit audio pcm 
    // signed values
    int32_t audio_buffer[SAMPLE_RATE + WINDOW] = {0};
    // making a caching unit for the 
    // e ^ j * 2 * pi * freq
    // for N samples

    complex<float> map[WINDOW];
    complex<float> maplower[WINDOW];
    do{
        float offset = (2 * M_PI * FREQ) / SAMPLE_RATE;
        float offsetlower = (2 * M_PI * FREQl) / SAMPLE_RATE;
        float curr_b = 0.0f;
        float curr_bl = 0.0f;
        for(int i = 0; i < WINDOW; i++) {
            map[i] = exp(complex<float>(0, curr_b));
            maplower[i] = exp(complex<float>(0, curr_bl));
            curr_b += offset;
            curr_bl += offsetlower;
        }
    }while(0);

    // making the io looper
    // working at 44100 hz of sampling frequency
    // with a sliding window alogorithm
    // window of 441
    // sliding @ 147
    complex<float> freq_x(0.0f, 0.0f);
    complex<float> freq_xl(0.0f, 0.0f);
    do{
        if(fread(audio_buffer + WINDOW, sizeof(int32_t) * SAMPLE_RATE, 1, stdin) != 1) break;
        for(int k = 0; k < SAMPLE_RATE; k+=96) {
            freq_x = {0, 0};
            freq_xl = {0, 0};
            for(int i = 0; i < WINDOW; i++) {
                freq_x += (float)audio_buffer[k+i] * map[i]; // first the 19000 hz signal
                freq_xl += (float)audio_buffer[k+i] * maplower[i]; // then 18800 hx lower signal
            }
            cout << abs(freq_x) / SAMPLE_RATE << endl;
            cout << abs(freq_xl) / SAMPLE_RATE << endl;
        }
        memcpy(audio_buffer, audio_buffer + SAMPLE_RATE, sizeof(int32_t) * WINDOW);
    }while(1);
    
    return 0;
}