#include <DFRobot_SHT3x.h>
//DFRobot_SHT3x sht3x(&Wire,/*address=*/0x45,/*RST=*/4);
DFRobot_SHT3x sht3x;

/**
* Calculate the dew point in degrees celsius
* @param air_temperature, relative_humidity
* @return dew point in degrees celsius
*/
double dew_point(double air_temperature, double relative_humidity) {
  // Safety Check for log function since log(0) = -inf
  if (relative_humidity <= 0.0) {
    relative_humidity = 0.001;
  } else if (relative_humidity > 100.0) {
    relative_humidity = 100.0; 
  }

  // Magnus Coefficients
  double a = 17.625;
  double b = 243.04;

  // alpha(T, RH)
  double alpha = log(relative_humidity / 100) + ((a * air_temperature) / (b + air_temperature));

  // Return dew point
  return (b * alpha) / (a - alpha);
}

/**
* Convert temperature value in celsius to fahrenheit
* @param celsius_value 
* @return fahrenheit temperature
*/
double c_to_f(double celsius_temperature) {
  return celsius_temperature * 9.0 / 5.0 + 32.0;
}

void setup() {
  Serial.begin(9600);
  //Initialize the chip to detect if it can communicate properly.
  while (sht3x.begin() != 0) {
    Serial.println("Failed to initialize the chip, please confirm the chip connection");
    delay(1000);
  }
  
  /**
   * readSerialNumber Read the serial number of the chip
   * @return Return 32-digit serial number
   */
  Serial.print("chip serial number: ");
  Serial.println(sht3x.readSerialNumber());
  /**
   * softReset Send command resets via IIC, enter the chip's default mode single-measure mode, 
   * turn off the heater, and clear the alert of the ALERT pin.
   * @return Read the status register to determine whether the command was executed successfully, 
   * and return true indicates success.
   */
   if(!sht3x.softReset()){
     Serial.println("Failed to reset the chip");
   }
  /**
   * pinReset Reset through the chip's reset pin, enter the chip's default mode single-measure mode, 
   * turn off the heater, and clear the alert of the ALERT pin.
   * @return The status register has a data bit that detects whether the chip has been reset, 
   * and return true indicates success.
   * @note When using this API, the reset pin of the chip nRESET should be connected to RST (default to pin4) of arduino.
   */
  //if(!sht3x.pinReset()){
    //Serial.println("Failed to reset the chip");
  //}
  /**
   * heaterEnable() Turn on the heater inside the chip so that the sensor can have accurate humidity data even in humid environment.
   * @return Read the status register to determine whether the command was executed successfully, and return true indicates success.
   * @NOTE Heaters should be used in wet environment, and other cases of use will result in incorrect readings.
   */
  //if(!sht3x.heaterEnable()){
    // Serial.println("Failed to turn on the heater");
  //}
  /**
   * startPeriodicMode Enter cycle measurement mode and set repeatability and read frequency.
   * @param measureFreq Read the eMeasureFrequency_t data frequency.
   * @note  Selectable parameters:
               eMeasureFreq_.5Hz,   /**the chip collects data in every 2s
               eMeasureFreq_1Hz,   /**the chip collects data in every 1s 
               eMeasureFreq_2Hz,   /**the chip collects data in every 0.5s 
               eMeasureFreq_4Hz,   /**the chip collects data in every 0.25s 
               eMeasureFreq_10Hz   /**the chip collects data in every 0.1s 
   * @param repeatability Read the repeatability of temperature and humidity data, the default parameter is eRepeatability_High.
   * @note  Optional parameters:
               eRepeatability_High /**In high repeatability mode, the humidity repeatability is 0.10%RH, the temperature repeatability is 0.06°C
               eRepeatability_Medium,/**In medium repeatability mode, the humidity repeatability is 0.15%RH, the temperature repeatability is 0.12°C.
               eRepeatability_Low, /**In low repeatability mode, the humidity repeatability is0.25%RH, the temperature repeatability is 0.24°C
   * @return Read the status of the register to determine whether the command was executed successfully, and return true indicates success
   */          
  if(!sht3x.startPeriodicMode(sht3x.eMeasureFreq_1Hz)){
    Serial.println("Failed to enter the periodic mode");
  }
  Serial.println("------------------Read data in cycle measurement mode-----------------------");
}

void loop() {
  //Store temperature and relative humidity for the current timestamp
  double temp = sht3x.getTemperatureC();
  double relative_humidity = sht3x.getHumidityRH();

  //Print temperature to serial output
  Serial.print(temp);
  Serial.print("");
  Serial.print(",");

  //Print relative_humidity to serial output
  Serial.print(relative_humidity);
  Serial.print("");
  Serial.print(",");

  //Calculate and store dew point for current timestamp and print to serial output 
  double dp = dew_point(temp, relative_humidity);
  Serial.print(dp);
  Serial.print("");
  Serial.print(",");

  //Convert dew point to fahrenheit and print to serial output
  Serial.print(c_to_f(dp));
  Serial.println("");

  //Please adjust the frequency of reading according to the frequency of the chip collection data.
  //The frequency to read data must be greater than the frequency to collect the data, otherwise the returned data will go wrong.
  delay(30000); //Delay (in milliseconds) before data collection begins [30000 for 5 minutes]
  if(millis() > 10000 && millis() < 10200){
    /**
     * stopPeriodicMode() Exit from the cycle read data
     * @return Read the status of the register to determine whether the command was executed successfully, 
     * and return true indicates success.
     */
    sht3x.stopPeriodicMode();
    Serial.println("Exited from the cycle measurement mode, enter the single measurement mode");
  }
}

