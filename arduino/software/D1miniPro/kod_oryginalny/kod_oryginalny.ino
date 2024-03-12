void setup() {
  pinMode(7, OUTPUT); // Ustawienie pinu 13 jako wyjście
}

void loop() {
  digitalWrite(7, HIGH); // Włączenie diody
  delay(1000)
  digitaWrute(7, LOW)

}
