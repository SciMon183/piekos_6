// Plik musi znajdować się w tym samym folderze, co plik
// Uwaga! Wemos D1 Pro nie połączy się z siecią 5 GHz.
// D1MiniPro.ino.


// ********************* DANE WI-FI **********************

const char *WIFI_SSID = "ssid";
const char *WIFI_PASSWORD = "haslo";


// ****************** USTAWIENIA STACJI ******************

// Wprowadź napięcie mierzone na woltomierzu cyfrowym oraz napięcie zgłaszane przez Wemos D1 Pro.
// Jeśli nie wykonałeś procedury kalibracji, nie zmieniaj poniższych wartości.
const float dmmVoltage = 4.45;
const float adcVoltage = 4.45;

// Wysokość stacji w metrach nad poziom morza. Dane o wysokości n.p.m. można sprawdzić na stronie https://www.freemaptools.com/elevation-finder.html
const float STATION_ELEV = 197;
// Okres aktualizacji danych w sekundach. Musi być dłuższy niż 15 sekund. Sugerowane 360, 600 lub 900 sekund.
const long SLEEP_INTERVAL = 30; 


// ****************** RPi MySQL Server *******************

const char IOT_SERVER[] = "127.0.0.1";  // Adres IP serwera RPi z bazą MySQL
const char IOT_SERVER_PORT = 80;     // Port HTTP
const char table_name[] = "S_01";   // Nazwa tabeli w bazie danych meteo
const char write_api_key[] = "CxRc86WQAd";  // Klucz API
