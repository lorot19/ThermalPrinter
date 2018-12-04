/*
 * NO LIBRARY image printing for standard ESC/POS printer also known as EPSON script.
 * This sketch is using just standard Arduino.h library and raw commands described
 * by ESC/POS® Command Reference
 * https://reference.epson-biz.com/modules/ref_escpos/index.php?content_id=2
 * 
 * Some printers like SparkFun or AdaFruit thermal printers are using newer commands
 * which should not work on older ESP/POS printers. This is reason why libraries doesnt work
 * for all printers. This sketch is written for GSerial GP-5890XIII but it should be 
 * compatible with all ESC/POS printers as it is using only standart and some obsolete commands.
 * 
 * Author: Tomas Lorinc, 2018
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "logo.h"
#include "qrcode.h"

const char* ssid = "IoT_Network";
const char* password = "griswold";
const char* mqtt_server = "192.168.1.1";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

/*
 * WiFi and MQTT stuff copied from PubSubClient.h library
 * distribued under MIT license
 * Source: https://github.com/knolleary/pubsubclient
 */
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
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


void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
     Serial.println("Technicka Univerzita v Kosiciach");
     Serial.write(10); // line feed
     Serial.println("KATEDRA");
     Serial.println("TEORETICKEJ A PRIEMYSELNEJ");
     Serial.println("ELEKTROTECHNIKY");
      Serial.write(10); // line feed
      printLogo();
      if(msg!="") 
      {
        Serial.println(msg);
        Serial.println("moze studovat na KTPE");
      }
      Serial.write(10); // line feed
      Serial.println("Viac info:");
      Serial.write(10); // line feed
      printQR();
      Serial.println("www.ktpe.fei.tuke.sk");
      Serial.write(10); // line feed
      Serial.println("Dotaznik");
      Serial.println("www.bit.ly/ktpedotaznik");

      Serial.write(10); // line feed
      Serial.write(10); // line feed
      Serial.write(10); // line feed
      Serial.write(10); // line feed
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266_printer-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      Serial.write(10); // line feed
      Serial.write(10); // line feed
      Serial.write(10); // line feed
      Serial.write(10); // line feed
      client.subscribe("form/newsubmit");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*
 * This function is used for initialisation default printer settings. 
 * Some of the commands are obsolete in actual version of ESC/POS script
 */
void initPrinter()
{
 Serial.write(27); //ESC
 Serial.write(64); // INITIALIZE
 Serial.write(27); // ESC
 Serial.write(97); // FONT SELECTION
 Serial.write(49); // FONT B
}

/*
 * This functions are used for logo printing. Logos iare stored in header files
 * as byte array. Each pixel contain 8bit of data. All pixels are stored
 * from left to right and from up to down direction.
 * GS v 0 0 is ESC/POS command header for bitimage printing.
 * According to the datasheet xL xH (yL yH) are two bytes representing
 * number of pixels in axis. It means that for example 
 * 
 * 160px is represented as
 *           xH       xL
 * BIN  0000 0000 1010 0000
 * DEC  0         160
 * HEX  0         A0
 *          but   
 * 240px is represented as
 *          xH       xL
 * BIN 0000 0001 0001 1000
 * DEC 1         18
 * HEX 1         24
 * 
 * However there might be some bug or missunderstanding because of very 
 * chaotic datasheet. The X value must be divided by 8 otherwise picture 
 * will be missmatched. But Y axis stay untouched.
 * Example:
 * 160px x 210px will be represented as 160/8 and 210
 * Serial.write(160/8); // xL
 * Serial.write(byte(0)); // xH
 * Serial.write(210); // yL
 * Serial.write(byte(0)); // yH
 * 
 * After that we are sending all bytes of image from array. There are no ending command
 * however in case of reset during printing, which is very frequent in high density images
 * it is recommended to use 1ms delay after each byte to get enought time for RX/TX.
 * 
 */
void printLogo(){
 Serial.write(29); // GS
 Serial.write(118); // v
 Serial.write(48); // 0
 Serial.write(48); // m
 Serial.write(18); // xL
 Serial.write(byte(0)); // xH
 Serial.write(144); // yL
 Serial.write(byte(0)); // yH
 for(int i=0; i<sizeof(ktpe); i++)
 {
  Serial.write(ktpe[i]);
  delay(1);  // optional in case of high density images
 }
 Serial.write(10); // line feed
}

void printQR()
{
 Serial.write(29); // GS
 Serial.write(118); // v
 Serial.write(48); // 0
 Serial.write(48); // m
 Serial.write(20); // xL
 Serial.write(byte(0)); // xH
 Serial.write(160); // yL
 Serial.write(byte(0)); // yH

 for(int i=0; i<sizeof(qr_scan); i++)
 {
  Serial.write(qr_scan[i]);
  delay(1);
 }
 Serial.write(10); // line feed
}

/*
 * Setup loop
 * Printer and printer initialisation.
 * It is not reccomended to put a lot of printing commands in setup loop otherwise
 * system watchdog will reset ESP8266. In case of arduino this issue was not seen.
 * 
 */

void setup() 
{
 Serial.begin(9600); // to write to our new printer
 initPrinter();
 setup_wifi();
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
}


/*
 * Main loop
 * used just for printing label
 */
 
void loop()
{

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
