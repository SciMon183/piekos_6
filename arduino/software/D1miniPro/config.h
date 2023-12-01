// Plik konfiguracyjny config.h

// Plik musi znajdować się w tym samym folderze, co plik
// D1MiniPro.ino.

// *******************************************************
// ********************* DANE WI-FI **********************
// *******************************************************

// Nazwa sieci Wi-Fi 2,4 GHz (SSID).
// Uwaga! Wemos D1 Pro nie połączy się z siecią 5 GHz.
const char *WIFI_SSID = "ssid";

// Hasło do sieci Wi-Fi
const char *WIFI_PASSWORD = "hasl;o";

// *******************************************************
// ****************** USTAWIENIA STACJI ******************
// *******************************************************

// Wprowadź napięcie mierzone na woltomierzu cyfrowym oraz
// napięcie zgłaszane przez Wemos D1 Pro.
// Jeśli nie wykonałeś procedury kalibracji, nie zmieniaj 
// poniższych wartości.
const float dmmVoltage = 4.45;
const float adcVoltage = 4.45;

// Wysokość stacji w metrach nad poziom morza.
// Dane o wysokości n.p.m. można sprawdzić na stronie
// https://www.freemaptools.com/elevation-finder.htm
// wskazując na mapie lokalizację swojej stacji.
const float STATION_ELEV = 197; // np.: 127.5

// Okres aktualizacji danych w sekundach.
// Musi być dłuższy niż 15 sekund.
// Sugerowane 360, 600 lub 900 sekund.
const long SLEEP_INTERVAL = 30; // np.: 360

// *******************************************************
// ****************** RPi MySQL Server *******************
// *******************************************************

// Adres IP serwera RPi z bazą MySQL
const char IOT_SERVER[] = "127.0.0.1"; // np.: 192.168.0.10

// Port HTTP
const char IOT_SERVER_PORT = 80;

// Nazwa tabeli w bazie danych meteo
const char table_name[] = "S_01"; // np.: S_01

// Klucz API
const char write_api_key[] = "CxRc86WQAd"; // np.: CxRc86WQAd