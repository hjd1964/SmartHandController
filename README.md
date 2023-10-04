# OnStep Smart Hand Controller
The SHC adds dedicated hand controller functionality to an OnStep or OnStepX telescope controller. 
This firmware is designed to run on ESP32 (recommended) or Teensy3.2 or Teensy4.0 hardware.
It can communicate with OnStep over the ST4 guiding port (which automatically reconfigures itself for synchronous serial communications when it detects an SHC) or over WiFi instead if running on an ESP32 (recommended.)
The SHC hardware provides physical buttons that are easy to work by feel in the dark and an OLED display for convenient telescope control. 

The SHC software was initially based on [Charles Lemaire](https://pixelstelescopes.wordpress.com/)'s [TeenAstro fork](https://groups.io/g/TeenAstro/wiki/home).

# Features
You can review the SHC capabilities in its [User Manual](https://onstep.groups.io/g/main/wiki/28605) Wiki.

# Documentation
You can review recommended hardware and flashing instructions in the [Smart Hand Controller](https://groups.io/g/onstep/wiki/Smart-Hand-Controller) Wiki.

# License
The Smart Hand Controller is open source free software, licensed under the GPL.

See [LICENSE.txt](./LICENSE.txt) file.

# Primary Author
[Howard Dutton](http://www.stellarjourney.com), Charles Lemaire, and [Khalid Baheyeldin](https://baheyeldin.com)
