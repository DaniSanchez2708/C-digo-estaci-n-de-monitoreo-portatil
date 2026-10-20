#include <DHT.h>

// Programa para guardar mediciones en Excel (formato CSV)
// MQ-135: AO -> A0
// DHT11: DAT -> D4
// ARD-371: Vo -> A1, LED control -> D7
// Todos comparten 5V y GND.

const int PIN_GAS = A0;
const int PIN_POLVO = A1;
const int PIN_LED_POLVO = 7;
const int PIN_DHT = 4;

DHT dht(PIN_DHT, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(PIN_LED_POLVO, OUTPUT);
  digitalWrite(PIN_LED_POLVO, HIGH);  // LED apagado entre lecturas
  delay(2000);

  // La primera linea sera el encabezado del CSV.
  Serial.println("Tiempo_s,MQ135_raw,Temperatura_C,Humedad_pct,ARD371_raw,Voltaje_polvo_V");
}

void loop() {
  int gas = analogRead(PIN_GAS);
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  long suma = 0;
  for (int i = 0; i < 20; i++) {
    digitalWrite(PIN_LED_POLVO, LOW);
    delayMicroseconds(280);
    suma += analogRead(PIN_POLVO);
    delayMicroseconds(40);
    digitalWrite(PIN_LED_POLVO, HIGH);
    delayMicroseconds(9680);
  }

  float polvo = suma / 20.0;
  float voltaje = polvo * 5.0 / 1023.0;

  Serial.print(millis() / 1000UL);
  Serial.print(",");
  Serial.print(gas);
  Serial.print(",");

  if (!isnan(temperatura)) Serial.print(temperatura, 1);
  Serial.print(",");
  if (!isnan(humedad)) Serial.print(humedad, 1);
  Serial.print(",");

  Serial.print(polvo, 2);
  Serial.print(",");
  Serial.println(voltaje, 3);

  delay(1800);  // intervalo total cercano a 2 segundos
}
