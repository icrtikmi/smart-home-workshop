#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266TelegramBOT.h>

// Initialize Wifi connection to the router
char ssid[] = "Your SSID";         // your network SSID (name)
char pass[] = "Your password";     // your network key

// Initialize Telegram BOT
#define BOTtoken "Your token"  //token of FlashledBOT
#define BOTname "Your bot name"
#define BOTusername "Your bot username end with bot word"

TelegramBOT bot(BOTtoken, BOTname, BOTusername);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

// initialize digital pin output.
#define lampu D1 //gpio 5

int ledStatus = 0;

/********************************************
 * EchoMessages - function to Echo messages *
 ********************************************/

void Bot_ExecMessages() {
  for (int i = 1; i < bot.message[0][0].toInt() + 1; i++)      {
    // Kamar Depan
    if (bot.message[i][5] == "/onLampu") {
      digitalWrite(lampu, HIGH);
      ledStatus = 1;
      bot.sendMessage(bot.message[i][4], "Lampu menyala", "");
    }

    if (bot.message[i][5] == "/offLampu") {
      digitalWrite(lampu, LOW);
      ledStatus = 0;
      bot.sendMessage(bot.message[i][4], "Lampu mati", "");
    }
    // end

    if (bot.message[i][5] == "/status") {
      String infoLampu;
      if(ledStatus){infoLampu = "menyala";}else{infoLampu = "mati";}
      String welcome = "Info status lampu.%0A";
      welcome += "=========================%0A%0A";
      welcome += "Status Lampu "+ infoLampu +".%0A";
      bot.sendMessage(bot.message[i][4], welcome, "");
    }

    if (bot.message[i][5] == "/start") {
      String welcome = "Welcome to Workshop ICRT 2019.%0A";
      welcome += "IKMI Cirebon Robotic Team.%0A";
      welcome += "=========================%0A%0A";
      welcome += "1. /onLampu %26 /offLampu untuk mengontrol lampu.%0A%0A";
      welcome += "2. /status untuk melihat status lampu.%0A%0A";
      bot.sendMessage(bot.message[i][4], welcome, "");
    }
  }
  bot.message[0][0] = "";   // All messages have been replied - reset new messages
}

void setup() {
  Serial.begin(9600);

  // attempt to connect to Wifi network:
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  
  // launch Bot functionalities
  bot.begin();

  pinMode(lampu, OUTPUT);
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    bot.getUpdates(bot.message[0][1]);   // launch API GetUpdates up to xxx message
    Bot_ExecMessages();   // reply to message with Echo
    Bot_lasttime = millis();
  }
}
