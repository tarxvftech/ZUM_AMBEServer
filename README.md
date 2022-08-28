# ZUM Radio AMBE Server board open firmware

## What
This is a fully-open firmware for the ZUM Radio AMBE Server board, which
provides an ESP32 (with USB serial, Ethernet, WiFi, and Bluetooth connectivity) 
access to an AMBE3000 voice codec chip from DVSI.

The point of this hardware and software is to make the various AMBE
voice codecs accessible to various programs that might need them, such as
cross-mode software (e.g. to bridge DMR and D-STAR to each other or other
modes), soft network clients for these modes, and that sort of thing.

AMBE itself is proprietary, patent-encumbered, and all kinds
of other things.  It's also legacy, and there are now fully-open
alternatives. Alas, AMBE is built into several protocols in wide use,
hence the need for this hardware and software.

## Who
Expected audience is mostly amateur radio operators with some developer experience.
Releases and firmware upgrade information for non-developers available [TODO: HERE]().
For non-developer support, please see [the ZUM Radio support options](https://zumradio.com/support.html).

## Why
The [original firmware](https://zumradio.com/downloads.html#precompiled)
was not made open source. This firmware, instead, focuses on being open,
and maybe trying to do some new things. 

Open is strictly better as it allows for fewer bugs, more features,
more innovation, and ultimately prevents hardware from becoming useless
through the simple passage of time (or people).


## How

### Compile this firmware from Linux
```
python -m venv env
source env/bin/activate
pip install -U platformio
pio upgrade
pio run
```
PlatformIO's `pio run` _should_ auto-install the required dependencies
and build an image.

Flash the board over USB serial with `pio run -t upload` (no button
presses needed or anything) and watch the serial output with `pio device
monitor`.

Manual flashing: TODO (notes below from what PIO does, but not yet recommended):
```
esptool.py --chip esp32 --port "/dev/ttyUSB0" --baud 460800 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size 4MB 
0x1000 /root/.platformio/packages/framework-arduinoespressif32/tools/sdk/esp32/bin/bootloader_dio_40m.bin  \
0x8000 /app/.pio/build/lolin32/partitions.bin  \
0xe000 /root/.platformio/packages/framework-arduinoespressif32/tools/partitions/boot_app0.bin  \
0x10000 .pio/build/lolin32/firmware.bin \
```

### Network Flashing
You can compile the firmware with ArduinoOTA. Flash with `pio run -t upload --upload-port IP_ADDRESS_HERE`. (Untested in Docker). This is not working right now (suspect the flash partition layout is incompatible with OTA) and is disabled by default.



### Compile this firmware from Windows (GUI)
TODO? (No Windows around, here, so please fill in! ~MM)

### Use this firmware's services / Client software
TODO (Not there yet!)
