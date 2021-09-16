# RT-Thread FoC motor control in voltage mode sample

## What is this?
---
This WIP sample is an FoC vector control in voltage mode sample app to be used in RT-Thread, it should
be compatible with any platform that have a 3-PWM output plus an I2C sensor for encoder reading.

## Hardware required: 
---

To run this sample you need any hardware that has an L6234 BLDC motor driver IC, one recomendation
is the STMicro IHM07M1 development board: https://www.st.com/en/ecosystems/x-nucleo-ihm07m1.html

Just wire the motor to the Bridge output plus the I2C encoder sensor to the MCU I2C ports, also 
wire your target MCU 3-PWM output to the 3-PWM inputs of the devboard / driver. Note don't forget 
tying the EN inputs of L6234 to the VCC to allow the bridge to work. 

## Software requirements:
---

To run this sample you must install the RT-thread correctly, just pick your flavour, I preferred
the manual way by cloning the repository direct and by doing the setup.

After that just pick your favorite BSP that supports PWM plus I2C device drivers, go to 
`bsp/<your_platform>/<board>/` , and inside of the folder, just replace the applications folder
with this directory, the SConscript file will find this directory automagically.

## Building the sample:
---

Just do the build command as any regular RT-Thread application:

```
$ scons --menuconfig       # configure rt-thread as the first time and installs env tool
$ source ~/.env/env.sh     # prepare envtool 
$ pkgs --update            # This may be required depending of the BSP. e.g. NRF devices
$ scons                    # this will build the application

```

## Usage:
---

The build process will produce .elf and .bin files, the first one can be used 
along your debug probe (like jlink or cmsis-dap) to debug the code. The later
you can flash direct to your target flash. The motor should start immediately 
with slow velocity command.

## Support:
---

Please mind this is an experimental project and have no guarantees of proper work,
in any case I will be glad to help you putting the motor to spin. reach me 
out at: ryukokki.felipe@gmail.com

---