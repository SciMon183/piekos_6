/* D1MiniPro.ino
   Wemos D1 Mini Pro - Stacja pogodowa zasilana panelem solarnym


   Ustaw monitor portu szeregowego na 115,200 baud
   
   Program opracowano na podstawie projektu Karl Berger (Berger Engineering) http://w4krl.com/

   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
   (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
   merge, publish, distribute, sublicense, and/or sell  copies of the Software, and to permit persons to whom the Software
   is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <Wire.h>               // [biblioteka wbudowana] I2C bus
#include <BME280I2C.h>          // [doinstaluj bibliotekę] Tyler Glenn https://github.com/finitespace/BME280
#include <BH1750.h>             // [doinstaluj bibliotekę] https://github.com/claws/BH1750
#include <ESP8266WiFi.h>        // [biblioteka wbudowana] ESP8266 WiFi

#include "config.h"             // plik konfiguracyjny do edycji (musi być w tym samym folderze co niniejszy plik)

// *******************************************************
// *********************** GLOBALS ***********************
// *******************************************************
String unitStatus = "";                         // for weather station status

// structure to hold sensor measurements in metric units
struct sensorData
{
  float stationPressure;          // measured station pressure in hPa
  float seaLevelPressure;         // calculated SLP
  float temperature;              // degrees Celsius
  float humidity;                 // relative humidity %
  unsigned int lightIntensity;    // lux
  float cellVoltage;              // volts
  long wifiRSSI;                  // dBm
} rawData;  					  // declare struct variable

// *******************************************************
// ***************** INSTANTIATE OBJECTS *****************
// *******************************************************
BME280I2C myBME280;     // barometric pressure / temperature / humidity sensor
BH1750 myBH1750;        // light intensity sensor
WiFiClient client;      // WiFi connection

// *******************************************************
// ************************ SETUP ************************
// *******************************************************
void setup()
{
  Serial.begin(115200);             // initialize the serial port
  pinMode(LED_BUILTIN, OUTPUT);     // set builtin LED for output

  // initialize BME280 pressure/temperature/humidity sensor
  while(!Serial) {} // Wait
  Wire.begin();
  while(!myBME280.begin())
  {
    Serial.println("Nie wykryto sensora BME/BMP280!");
    delay(1000);
  }
  switch(myBME280.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Wykryto sensor BME280. OK.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Wykryto sensor BMP280. Odczyt wilgotności niemożliwy.");
       break;
     default:
       Serial.println("Sensor NIEZNANY! Błąd!");
       blinkLED(4);                    // notify the user
  }

  // initialize BH1750 light sensor
  // the BH1750 library does not have a test for presence
  myBH1750.begin();

  logonToRouter();                  // logon to local Wi-Fi

  // load all sensor data into rawData struct
  rawData = readSensors();

  // send data to local serial port, ThingSpeak & APRS-IS
  printToSerialPort(rawData);       // display data to local serial monitor
  postToRPi(rawData);        // send data to RPi

  // all done, now go to sleep
  blinkLED(3);                      // notify the user
  enterSleep(SLEEP_INTERVAL);
} //setup()

// *******************************************************
// ************************ LOOP *************************
// *******************************************************
void loop()
{
  // there is nothing to do here
  // everything is done in setup()
} // loop()

// *******************************************************
// ***************** LOGON TO YOUR Wi-Fi *****************
// *******************************************************
void logonToRouter()
{
  String exitMessage = "";
  int count = 0;
  WiFi.begin(internet2.4,internet123);
  while ( WiFi.status() != WL_CONNECTED )
  {
    count++;
    // give up if more than 15 tries
    if ( count > 15 )
    {
      // display error code on serial monitor
      switch ( WiFi.status() )
      {
        case 1:
          exitMessage = "Sieć Wi-Fi niedostępna lub\nStacja zbyt daleko od punktu dostępowego lub\nNiepoprawny SSID lub hasło lub\nAccess Poin nie pracuje na częstotliwości 2.4GHz.";
          break;
        case 2:  // will never show this condition
          exitMessage = "Skanowanie sieci zakończone.";
          break;
        case 3:  // will never show this condition
          exitMessage = "Połączono.";
          break;
        case 4:
          exitMessage = "Błąd połączenia.";
          break;
        case 5:
          exitMessage = "Utracono połączenie.";
          break;
        case 6:
          exitMessage = "Rozłączono.";
          break;
      } // switch
      Serial.print("WiFi fail: ");
      Serial.println(exitMessage);
      blinkLED(5);                    // notify the user
      enterSleep(60);                 // retry after 1 minute
    } // if > 15
    // otherwise if < 15 blink LED and wait 500ms before checking connection
    blinkLED(1); // blink LED on each attempt to connect
    delay(500);  // one-half second delay between checks
    Serial.print("");
  } // while not connected
  // WiFi is sucesfully connected
  Serial.println("");                 // new line to show IP address
  Serial.print("Połączono z siecią Wi-Fi. Otrzymany adres IP: ");
  Serial.println(WiFi.localIP().toString()); // is toString necessary?
} // logonToRouter()

// *******************************************************
// ******** READ SENSORS INTO A SENSORDATA STRUCT ********
// *******************************************************
sensorData readSensors()
{
  sensorData tempData = {0};  // initialize temporary variable to hold readings

  // a fudgeFactor corrects for voltage divider component variation
  // as measured by user in teh calbration step
  float fudgeFactor = dmmVoltage / adcVoltage;
  // BME280 pressure unit - every function expects pressure to be hPa
  // BME280 temperature unit - every function expects temperature in Celsius
  
  int samples = 3;      // number of samples to take for average

  // declare and initialize temporary variables
  float t = 0;          // temperature C
  float h = 0;          // humidity %
  float sp = 0;         // station pressure mb or hPa
  float slp = 0;        // sea level pressure
  unsigned int li = 0;  // light intensity lux
  float cv = 0;         // cell voltage
  long ssi = 0;         // signal strength

  // read sensors multiple times and take average
  for (int i = 0; i < samples; i++)
  {
    // BME280 data read: (float pressure, float temperature, float humidity, tempUnit, presUnit)
    myBME280.read(sp, t, h, BME280::TempUnit_Celsius, BME280::PresUnit_hPa);

    // calculate the Sea Level Pressure from the station pressure
    slp = calculateSeaLevelPressure(t, sp, STATION_ELEV);

    // read light level in lux
    li = myBH1750.readLightLevel();

    // read analog voltage from the Analog to Digital Converter
    // on D1 Mini this is 0 - 1023 for voltages 0 to 3.2V
    // the D1M-WX1 has an external resistor to extend the range to 5.0 Volts
    cv = 5.0 * analogRead(A0) * fudgeFactor / 1023.0;

    // read the Wi-Fi signal strength
    ssi = WiFi.RSSI();

    // accumulate the values of each sensor
    tempData.temperature += t;
    tempData.humidity += h;
    tempData.stationPressure += sp;
    tempData.seaLevelPressure += slp;
    tempData.lightIntensity += li;
    tempData.cellVoltage += cv;
    tempData.wifiRSSI += ssi;

    delay(50);   // provide some delay to let sensors settle
  } // for()

  // divide the accumulated values by the number of samples
  // to get an average
  tempData.temperature /= (float)samples;
  tempData.humidity /= (float)samples;
  tempData.stationPressure /= (float)samples;
  tempData.seaLevelPressure /= (float)samples;
  tempData.lightIntensity /= (long)samples;
  tempData.cellVoltage /= (float)samples;
  tempData.wifiRSSI /= (int)samples;

  return tempData;
} // readSensors()

// *******************************************************
// ********** PRINT RAW DATA TO THE SERIAL PORT **********
// *******************************************************
void printToSerialPort(sensorData dataRaw)
{
  // '\t' is the C++ escape sequence for tab
  // header line
  Serial.println("\t°C\t%\thPa\tSLP hPa\tLux\tV");
  // data line
  Serial.print("Dane\t");
  Serial.print(dataRaw.temperature, 2);
  Serial.print("\t");
  Serial.print(dataRaw.humidity, 2);
  Serial.print("\t");
  Serial.print(dataRaw.stationPressure, 2);
  Serial.print("\t");
  Serial.print(dataRaw.seaLevelPressure, 2);
  Serial.print("\t");
  Serial.print(dataRaw.lightIntensity);
  Serial.print("\t");
  Serial.println(dataRaw.cellVoltage, 2);
  Serial.println("----------------------------------------------------");
} // printToSerialPort()

// *******************************************************
// ****************** SEND DATA TO RPi *******************
// *******************************************************
void postToRPi(sensorData data)
{
  // assemble and post the data
  if ( client.connect(IOT_SERVER, IOT_SERVER_PORT) == true )
  {
    Serial.println("Połączono z serwerem RPi.");

    // get the data to RPi
    client.print( "GET /espdata.php?");
    client.print("api_key=");
    client.print( write_api_key );
    client.print("&&");
    client.print("station_id=");
    client.print( table_name );
    client.print("&&");
    client.print("t=");
    client.print( data.temperature );
    client.print("&&");
    client.print("h=");
    client.print( data.humidity );
    client.print("&&");
    client.print("ap=");
    client.print( data.stationPressure );
    client.print("&&");
    client.print("rp=");
    client.print( data.seaLevelPressure );
    client.print("&&");
    client.print("li=");
    client.print( data.lightIntensity );
    client.print("&&");
    client.print("ps=");
    client.print( data.cellVoltage );
    client.print("&&");
    client.print("sl=");
    client.print( data.wifiRSSI );
    client.println( " HTTP/1.1");
    client.println( "Host: localhost" );
    client.println( "Content-Type: application/x-www-form-urlencoded" );
    client.println( "Connection: close" );
    client.println();
    client.println();
    Serial.println("Wysłano dane na serwer RPi.");
  }
  client.stop();
  Serial.println("Rozłączono z serwerem RPi.");
} // postToRPi()

// *******************************************************
// calculate relative sea level pressure from absolute station pressure in hPa
// temperature in °C, elevation in m
// http://www.sandhurstweather.org.uk/barometric.pdf
// http://keisan.casio.com/exec/system/1224575267
// *******************************************************
float calculateSeaLevelPressure(float celsius, float stationPressure, float elevation)
{
  float slP = stationPressure / pow(2.718281828, -(elevation / ((273.15 + celsius) * 29.263)));
  return slP;
} // calculateSeaLevelPressure()

// *******************************************************
// ******************** enterSleep ***********************
// *******************************************************
void enterSleep(long sleep)
{
  // sleep is in seconds
  Serial.print("Wejście w tryb głębokiego snu na: ");
  Serial.print(sleep);
  Serial.println(" sekund.");
  delay(200);                       // delay to let things settle
  // WAKE_RF_DEFAULT wakes the ESP8266 with WiFi enabled
  ESP.deepSleep(sleep * 1000000L, WAKE_RF_DEFAULT);
} // enterSleep()

// *******************************************************
// ******************** Blink LED ************************
// *******************************************************
// this flashes the onboard LED to indicate various internal messages
void blinkLED(int flashes)
{
  // set LED_BUILTIN pin to Output mode in setup()
  for (int i = 0; i < flashes; i++)
  {
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED *ON*
    delay(20);                       // short flash for low energy consumption
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED *OFF*
    delay(250);                      // time between flashes
  }
} // blinkLED()

// *******************************************************
// *********************** END ***************************
// *******************************************************