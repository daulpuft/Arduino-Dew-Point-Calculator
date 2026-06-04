# Arduino-Dew-Point-Calculator
A small Arduino sketch using a sensor capable of reading temperature and relative humidity to return dew point to serial output.

# Serial Output
The dew_point calculator is intended to be used alongside Excel's Data Streamer. The output is organized into the following comma separated list:

| Timestamp | Temperature(C) | Relative Humidity(%) | Dew Point(C) | Dew Point(F) |
| --------- | -------------- | -------------------- | ------------ | ------------ |

# INSTALLATION
1. Download and install the official [Arduino IDE](https://www.arduino.cc/en/software/)
2. Download and extract the [DFRobot repository](https://github.com/DFRobot/DFRobot_SHT3x/tree/master)
3. Place the extracted file inside: **"...\Documents\Arduino\libraries"**
4. Download and extract *this* repository. **The "dew_point.ino" file must be place inside a folder called "dew_point"**
5. Open **"dew_point.ino"** in the Arduino IDE, verify, and upload to a connected Arduino.
6. Verify data is being output correctly by opening 'Tools/Serial Monitor' or press 'Ctrl-Shift-M'

