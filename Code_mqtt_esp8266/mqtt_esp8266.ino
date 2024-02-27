#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//heartrate liberary
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables of heart rate
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
int Threshold = 550; 


PulseSensorPlayground pulseSensor;


// dr publish these drug according to input data
#define panadol "panadol"
#define antinal "antinal"
#define clartine "clartine"

//buffer of data
#define MSG_BUFFER_SIZE	(50)

//define networks data
const char* ssid = "Amr Gaber";
const char* password = "eN3&X7U#";
const char* mqtt_server = "91.121.93.94"; //test mosquitto
//.................................................................

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
// create an char of array
char msg[MSG_BUFFER_SIZE];
char msg2[MSG_BUFFER_SIZE];
//intial value of sensor for not exceeding normal measurments
int value = 0;
int value2=110;


//setup wifi 
void setup_wifi() {

  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//subscrbing from topics when dr send
void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
//pasring for data 
  if (strstr(topic, panadol)){
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(D0, LOW);   // Turn the LED on (Note that LOW is the voltage level

    } else {
      digitalWrite(D0, HIGH);  // Turn the LED off by making the voltage HIGH
    }    
  }
  else if ( strstr(topic, antinal)){
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
       if ((char)payload[0] == '0') {
      digitalWrite(D1, LOW);   // Turn the LED on (Note that LOW is the voltage level
    } else {
      digitalWrite(D1, HIGH);  // Turn the LED off by making the voltage HIGH
    }    
  }
  else if ( strstr(topic, clartine)){
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the LED if an 1 was received as first character
       if ((char)payload[0] == '0') {
      digitalWrite(D2, LOW);   // Turn the LED on (Note that LOW is the voltage level
    } else {
      digitalWrite(D2, HIGH);  // Turn the LED off by making the voltage HIGH
    }    
  }


}
//loop until reconnect
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("temp", " MQTT is connedted");
      client.publish("Heart_rate", " heartrate is being published");
      client.publish("Humidty", " heartrate is being published");
      // ... and resubscribe
      client.subscribe("panadol");
      client.subscribe("antinal"); 
      client.subscribe("clartine"); 
      

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(D0,OUTPUT);     // antinal
  pinMode(D1,OUTPUT);     // panadol
  pinMode(D2,OUTPUT);     // clartine
 
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(D2);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold); 

  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }

}

void read_sensor(int value) {
    value2+=7; 
    if(value2>= 130){
      delay(2000);
      value2=110;
    }  
}

void loop() {

  int myBPM = pulseSensor.getBeatsPerMinute();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  unsigned long now = millis();
  if (now - lastMsg > 2000) { //time tell send again using builtin millis
    lastMsg = now;
    value = analogRead(A0) * 0.32;
    
    read_sensor(value2);

    snprintf (msg2, MSG_BUFFER_SIZE, "Heart_rate = %ld", value2); 
    snprintf (msg, MSG_BUFFER_SIZE, "Temprature = %ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    Serial.println(msg2);
    client.publish("Heart_rate", msg2);    
    client.publish("temp", msg);   
    delay(1000);
  }
  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
 Serial.print("BPM: ");                        // Print phrase "BPM: " 
 Serial.println(myBPM);                        // Print the value inside of myBPM. 
}


}
