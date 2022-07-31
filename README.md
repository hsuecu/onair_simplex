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
2. ~Q 18.8khz and Q 19khz , their DFT magitudes are passed to 
convert into streams of 1's and 0's using Differential Encoding
over Q and ~Q values.
3. these streams of 1's and 0's are then converted into custom
morse encoding which mapps to particular nibble.
4. these nibbles were transmitted from MSB to LSB order.
5. then simply convert the hex stored in string form to hex 
values.

data transfer over near ultrasonic audio band

## Usage
It uses pipeling to ease on programmer's end to remove necessity
of making IO rodust and error free. As transmission gets over one
of the part stops working stopping whole operation. 
Remember what they say, it's not a bug its a feature.
So, assuming you are using linux environment and you have `aplay`
and `parec` preinstalled in your system. 
### Build
just a simple shell script. run it by
```bash
sh build.sh
```
### Transmission
so the ascii file you want to send.
```bash
cat <filename> | ./onair_upload | aplay -t raw -f S32 -r 384000 -c 1
```
### Recevier
```bash
parec --channels 1 --rate 384000 --formate s32 | ./onair_dft | ./onair_signal | ./onair_decoder | python3 onair_convert.py
```

#### Side Note
1. onair_dft return stream of DFT of 19khz Q and 18.8khz ~Q in pairs.
2. onair_signal returns the compaired value of Q and ~Q. i.e
if Q > ~Q then 1 otherwise 0
3. onair_decoder return stream of custom morse encoding when recevier get
a sync flg and ends its reception on getting fin flg.

