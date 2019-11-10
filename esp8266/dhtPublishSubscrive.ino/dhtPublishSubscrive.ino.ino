#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define wifi_ssid "ssid"
#define wifi_password "pass"

#define mqtt_server "192.168.1.53"
#define mqtt_user ""
#define mqtt_password ""
#define REPORT_INTERVAL 10 //60 secondi di report interval

//definizione topic
#define temperature_topic "nodemcu1/temperature"
#define humidity_topic "nodemcu1/humidity"
#define status_topic "nodemcu1/caldaia/status"
#define availability_topic "nodemcu1/caldaia/availability"
#define command_topic "nodemcu1/caldaia/command"

#define DHTPIN D4 // Pin a cui è collegato il DHT
#define DHTTYPE DHT22
char message_buff[100];

//Creazione oggetti
DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient clientMqtt(espClient);

void setup() {
  Serial.begin(9600);     //Porta di debug
  setup_wifi();           //call setup wificonnesssione wifi
  clientMqtt.setServer(mqtt_server, 1883);    //connessione mqtt
  clientMqtt.setCallback(callback);
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
      Serial.println("Connesso MQTT e sottoscritto ai topic command_topic..");
      clientMqtt.subscribe(command_topic);
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
  float h = dht.readHumidity();
  //get temperature
  float t = dht.readTemperature();


  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print(" | humidita : ");
  Serial.println(h);

  clientMqtt.publish(temperature_topic, String(t).c_str());   //put mqtt
  clientMqtt.publish(humidity_topic, String(h).c_str());      //put mqtt

  Serial.println("PutMqtt completata temperatura");



  int cnt = REPORT_INTERVAL;

  Serial.println("wait REPORT_INTERVAL..");

  while (cnt--)
    delay(1000);

}




void callback(char* topic, byte* payload, unsigned int length) {

  int i = 0;

  Serial.println("Message recu =>  topic: " + String(topic));
  Serial.print(" | lunghezza: " + String(length, DEC));

  // create character buffer with ending null terminator (string)
  for (i = 0; i < length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';

  String msgString = String(message_buff);

  Serial.println("Payload: " + msgString);


  if ( msgString == "ON" ) {
    Serial.println("Accendi caldaia");
    clientMqtt.publish(status_topic, "ON");
  } else if ( msgString == "OFF" ) {
    Serial.println("Spegni caldaia");
    clientMqtt.publish(status_topic, "OFF");
  }
}
