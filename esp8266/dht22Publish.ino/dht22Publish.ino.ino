#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define wifi_ssid "<ssid>"
#define wifi_password "<pwd>"

#define mqtt_server "<mqttServer>"
#define mqtt_user ""
#define mqtt_password ""
#define REPORT_INTERVAL 20 //60 secondi di report interval


#define temperature_topic "nodemcu1/temperature"
#define humidity_topic "nodemcu1/humidity"

#define DHTPIN D4 // Pin a cui è collegato il DHT
#define DHTTYPE DHT22

//Creazione oggetti
DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient clientMqtt(espClient);

void setup() {
  Serial.begin(9600);     //Porta di debug
  setup_wifi();           //call setup wificonnesssione wifi
  clientMqtt.setServer(mqtt_server, 1883);    //connessione mqtt
  dht.begin();
}

//connessionedi rete wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.println("Connessione a.. ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connessione wifi stabilita");
  Serial.print("=> Indirizzo IP : ");
  Serial.print(WiFi.localIP());
}

//Riconnessione
void reconnect() {
  //Ripeti fino a quando non ottieni una riconnessione
  while (!clientMqtt.connected()) {
    Serial.println("Connessione mqtt MQTT...");
    if (clientMqtt.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("OK");
    } else {
      Serial.print("KO, errore : ");
      Serial.print(clientMqtt.state());
      Serial.println(" Attendi 5 secondi e riprova connessione");
      delay(5000);
    }
  }
}

void loop() {
  if (!clientMqtt.connected()) {
    reconnect();
  }
  Serial.print("Enter in loop ");
  clientMqtt.loop();

  //get umidita
  Serial.println("Start reading hum");
  float h = dht.readHumidity();
  Serial.println("Stop reading hum");
  //get temperature
  Serial.println("Start reading temp");
  float t = dht.readTemperature();
  Serial.println("Stop reading temp");

  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print(" | humidita : ");
  Serial.println(h);

  clientMqtt.publish(temperature_topic, String(t).c_str(), true);   //put mqtt
  clientMqtt.publish(humidity_topic, String(h).c_str(), true);      //put mqtt

  Serial.println("PutMqtt completata");

  

  int cnt = REPORT_INTERVAL;

  Serial.println("wait REPORT_INTERVAL..");

  while (cnt--)
    delay(1000);

}
