# Smart-Healthcare-Adopting-MQTT-

## IMPLEMENTATION DETAILS
An ESP8266 act as the control unit is used to send the data measured by the sensors to the server and receive commands from the doctor. The sensor used are temperature sensor LM35, pressure sensor, heart rate sensor which sends continuously the temperature of the patient. The ESP8266 is collecting the data from the sensor connected to it then it connects to a free server called Mosquitto which is an open source (EPL/EDL licensed) message broker that implements the MQTT protocol. By using free google support tool called MQTTLens which supports MQTT communications, where we can use it to send and watch the data from computer. To watch the data of nodes from mobile we used an application called MyMQTT a free android app which can publish or subscribe the topics.

![Picture1](https://github.com/Ahmed00Gaber/Smart-Healthcare-Adopting-MQTT-/assets/113920593/b7633e98-27fe-4612-b6f7-d5d0ae8ea9c1)

![Picture2](https://github.com/Ahmed00Gaber/Smart-Healthcare-Adopting-MQTT-/assets/113920593/7735c10f-97a3-4173-bbc8-c5e481785d10)
