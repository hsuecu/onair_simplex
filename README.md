# OnAir

OnAir is a project to demonstrate low bit rate, data transmission
over ultrasonic audio band

## Description

OnAir is a simple and small application of data transmission over
near ultrasonic audio band of 18khz-19khz. The idea was to mimic 
the way old weather satellite communicate to ground stations.
For my case NOAA-15 satellite's modus operandi.

In real world, all this happens on electromagnatic sectrum. Using
various forms of modulation like ASK, FSK, PSK and many more. For 
example mobile phones use some mix of phase shift or amplitude 
shift to make 64 combinations, these are then interpreted into 
binary. It's just a rough estimate of how a mobile phone use EM
wave to send and receive data.

So, OnAir uses near ultrasonic audio band to transfer data over 
audio channel. It implementation can be pointed out as.

1. PCM audio data from microphone which is sampled @384khz is 
passed to DFT to filter two frequency component.
2. Q 18.8khz and ~Q 19khz , their DFT magitudes are passed to 
convert into streams of 1's and 0's using Differential Encoding
over Q and ~Q values.
3. these streams of 1's and 0's are then converted into custom
morse encoding which mapps to particular nibble.
4. these nibbles were transmitted from MSB to LSB order.
5. then simply convert the hex stored in string form to hex 
values.

data transfer over near ultrasonic audio band

## Usage
### Transmission
```bash
cat <filename> | ./ONAIR | aplay -t raw -f S32 -r 384000 -c 1
```
