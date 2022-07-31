#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>

using namespace std;

int SAMPLE_RATE = 384000;
int BIT_DEPTH = 32;
float PULSE_DURATION = 0.004; // 0.006
auto MAX_AMP = pow(2, BIT_DEPTH-1)-1;

class FMSineOscillator {
    float freq, offset_freq;
    float freq_lower, offset_freq_lower;
    float angle = 0.0f;
    float ampli = 0.0f;
public:
    FMSineOscillator(float f, float f_l, float a):freq(f), freq_lower(f_l), ampli(a){
        offset_freq = 2 * M_PI * freq / SAMPLE_RATE;
        offset_freq_lower = 2 * M_PI * freq_lower / SAMPLE_RATE;
    }
    enum f_s{
        UPPER = 1,
        LOWER
    };
    float process(f_s state) {
        angle = fmod(angle, 2 * M_PI);
        angle += state==UPPER?offset_freq:offset_freq_lower; // modulating the square wave pulse
        auto sample = ampli * sin(angle);
        return sample;
    }
};

class HexToPulseConverter {
    vector<vector<int>> map = {
        {1,0,0},               // 0 .
        {1,1,0,0},             // 1 -
        {1,0,1,0,0},           // 2 ..
        {1,0,1,1,0,0},         // 3 .-
        {1,1,0,1,0,0},         // 4 -.
        {1,1,0,1,1,0,0},       // 5 --
        {1,0,1,0,1,0,0},       // 6 ...
        {1,0,1,0,1,1,0,0},     // 7 ..-
        {1,0,1,1,0,1,0,0},     // 8 .-.
        {1,0,1,1,0,1,1,0,0},   // 9 .--
        {1,1,0,1,0,1,0,0},     // A -..
        {1,1,0,1,0,1,1,0,0},   // B -.-
        {1,1,0,1,1,0,1,0,0},   // C --.
        {1,1,0,1,1,0,1,1,0,0}, // D ---
        {1,0,1,0,1,0,1,0,0},   // E ....
        {1,0,1,0,1,0,1,1,0,0}, // F ...-
	    {0,0,0,0,0,0,0,0,0,0}, // SYNC
        {1,1,1,1,1,1,1,1,1,1}  // FIN
    };
public:
    vector<int> convert(char c) {
        vector<int> ret = {};
        ret = map[c / 16];
        auto temp = map[c % 16];
        for(auto itr: temp)
            ret.push_back(itr);
        return ret;
    } 
    vector<int> sync() {
        return map[16];
    }
    vector<int> fin() {
        return map[17];
    }
};

int main(int argc, const char ** argv) {

    if(argc >= 2) {
        PULSE_DURATION = atof(argv[1]);
    }

    int ratio = SAMPLE_RATE*PULSE_DURATION;
    FMSineOscillator audio(19000.0f, 18800.0f, 0.5f); //19000 18800
    HexToPulseConverter convertor;

    char buffer;

    auto sync = convertor.sync();
    for(int times = 0; times < 3; times++) {
        for(int j = 0; j < sync.size(); j++) {
            for(int k = 0; k < ratio; k++) {
                auto sample = audio.process(FMSineOscillator::f_s(sync[j]));
                int int_sample = static_cast<int>(sample * MAX_AMP);
                cout.write(reinterpret_cast<const char*>(&int_sample), 4);
            }
        }
    }

    while( (buffer = cin.get()) != EOF) {
        auto pulses = convertor.convert(buffer);
        for(int j = 0; j < pulses.size(); j++) {
            for(int k = 0; k < ratio; k++) {
                auto sample = audio.process(FMSineOscillator::f_s(pulses[j]));
                int int_sample = static_cast<int>(sample * MAX_AMP);
                cout.write(reinterpret_cast<const char*>(&int_sample), 4);
            }
        }
    }

    auto fin = convertor.fin();
    for(int times = 0; times < 3; times++) {
        for(int j = 0; j < fin.size(); j++) {
            for(int k = 0; k < ratio; k++) {
                auto sample = audio.process(FMSineOscillator::f_s(fin[j]));
                int int_sample = static_cast<int>(sample * MAX_AMP);
                cout.write(reinterpret_cast<const char*>(&int_sample), 4);
            }
        }
    }

    return 0;
}
