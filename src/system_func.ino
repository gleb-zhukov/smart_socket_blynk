BLYNK_WRITE(V0)
{
  int value = param.asInt();
  switch (value) {
    case 1:
      {
        digitalWrite(12, HIGH);
        EEPROM.put(3570, 1);
        EEPROM.commit();
      }
      break;
    case 2:
      {
        digitalWrite(12, LOW);
        EEPROM.put(3570, 0);
        EEPROM.commit();
      }
      break;
    case 3:
      {
        blynk_control(4, 1);
      }
      break;
    case 4:
      {
        blynk_control(5, 1);
      }
      break;
    case 5:
      {
        blynk_control(4, 2);
      }
      break;
    case 6:
      {
        blynk_control(5, 2);
      }
      break;
    case 7:
      {
        blynk_control(4, 3);
      }
      break;
    case 8:
      {
        blynk_control(5, 3);
      }
      break;
    case 9:
      {
        blynk_control(4, 4);
      }
      break;
    case 10:
      {
        blynk_control(5, 4);
      }
      break;
    case 11:
      {
        blynk_control(4, 5);
      }
      break;
    case 12:
      {
        blynk_control(5, 5);
      }
      break;
    case 13:
      {
        blynk_control(4, 6);
      }
      break;
    case 14:
      {
        blynk_control(5, 6);
      }
      break;
    case 15:
      {
        blynk_control(4, 7);
      }
      break;
    case 16:
      {
        blynk_control(5, 7);
      }
      break;
    case 17:
      {
        blynk_control(4, 8);
      }
      break;
    case 18:
      {
        blynk_control(5, 8);
      }
      break;
    case 19:
      {
        blynk_control(4, 9);
      }
      break;
    case 20:
      {
        blynk_control(5, 9);
      }
      break;
    case 21:
      {
        blynk_control(4, 10);
      }
      break;
    case 22:
      {
        blynk_control(5, 10);
      }
      break;
  }
}


BLYNK_WRITE(V1)
{
  uint8_t r = param[0].asInt();  //red
  uint8_t g = param[1].asInt();  //green
  uint8_t b = param[2].asInt();  //blue
  uint8_t device_mac_addr[20][6]; //mac-адрес устройства
  EEPROM.get(2152, device_mac_addr);
  send_packet(device_mac_addr[5], 7, r, g, b);
}

BLYNK_WRITE(V2)
{
  uint8_t device_mac_addr[20][6]; //mac-адрес устройства
  EEPROM.get(2152, device_mac_addr);
  uint8_t br = param[0].asInt();
  send_packet(device_mac_addr[5], 6, br);
}


BLYNK_WRITE(V3)
{
  uint8_t r = param[0].asInt();  //red
  uint8_t g = param[1].asInt();  //green
  uint8_t b = param[2].asInt();  //blue
  uint8_t device_mac_addr[20][6]; //mac-адрес устройства
  EEPROM.get(2152, device_mac_addr);
  send_packet(device_mac_addr[6], 7, r, g, b);
}

BLYNK_WRITE(V4)
{
  uint8_t device_mac_addr[20][6]; //mac-адрес устройства
  EEPROM.get(2152, device_mac_addr);
  uint8_t br = param[0].asInt();
  send_packet(device_mac_addr[6], 6, br);
}



void blynk_control(int func_command, int device_num) {
  uint8_t device_mac_addr[20][6]; //mac-адрес устройства
  EEPROM.get(2152, device_mac_addr);
  send_packet(device_mac_addr[device_num], func_command);
}


String device_menu(uint8_t *device_type_arr) {
  String menu;
  char device_name[10][41];
  EEPROM.get(100, device_name);
  for (byte i = 0; i < 20; i++) {
    if (device_type_arr[i]) {
      menu += device_name[i];
      menu += " \n";
    }
  }
  menu += "Назад";
  return (menu);
}

String rooms() {
  String menu;
  char room_name[10][41];
  EEPROM.get(2454, room_name);
  for (byte i = 0; i < 10; i++) {
    if (strlen(room_name[i])) {
      menu += room_name[i];
      menu += " \n";
    }
  }
  return (menu);

}

String main_menu() {
  String menu;
  menu += rooms();
  menu += "Настройки \xE2\x9A\x99 \n"
          "У меня проблема \xF0\x9F\x91\x80";
  return (menu);
}

void set_relay() {
  bool pin = digitalRead(12);
  bool pin_state = !pin;
  digitalWrite(12, pin_state);
  EEPROM.put(3570, pin_state);
  EEPROM.commit();
}

void first_start() {
  uint32_t userid[10] = {}; //для хранения ID пользователей
  uint8_t device_type[10] = {}; //флаг включения/отключения пункта меню в клавиатуре устройства освещения
  char device_name[20][41] = {}; //название устройств
  uint8_t device_mac_addr[20][6] = {}; //mac-адрес устройств
  char room_name[10][41] = {}; //название комнат
  char room_device[10][20] = {}; //для связки комнат с устройствами (какое устройство к какой комнате относится)
  char token[60] = {};  //Telegram token
  char ssid[35] = {};
  char pass[65] = {};
  //записываем стандартные значения всех данных в EEPROM
  EEPROM.put(3550, mode_flag);
  EEPROM.put(3580, userid);
  EEPROM.put(87, device_type);
  EEPROM.put(100, device_name);
  EEPROM.put(2152, device_mac_addr);
  EEPROM.put(2454, room_name);
  EEPROM.put(2866, room_device);
  EEPROM.put(3368, token);
  EEPROM.put(3430, ssid);
  EEPROM.put(3467, pass);
  EEPROM.commit(); //для окончания записи
  delay(100);
  Serial.println(F("first_start OK"));
}


void reset_settings() {
  EEPROM.write(1, 0); //записываем ключ для сброса всех данных в EEPROM
  EEPROM.commit();
  delay(100);
  ESP.restart();
}


void update_firm() {
  {

    Serial.println("пробуем обновиться");
    BearSSL::WiFiClientSecure client;

    BearSSL::X509List x509(serverCACert);
    client.setTrustAnchors(&x509);

    setClock();

    ESPhttpUpdate.rebootOnUpdate(false); // remove automatic update

    t_httpUpdate_return ret = ESPhttpUpdate.update(client, "https://grib-tech.ru/update/socket_v" + String(NEXT_FIRMWARE_VERSION) + ".bin");

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        if (mode_flag == 2) {
          WiFi.disconnect();
        }
        else {
          bool check_firm = false;
          EEPROM.put(3, check_firm);
          EEPROM.commit();
        }
        break;
      case HTTP_UPDATE_NO_UPDATES:
        if (mode_flag == 2) {
          WiFi.disconnect();
        }
        else {
          bool check_firm = false;
          EEPROM.put(3, check_firm);
          EEPROM.commit();
        }
        break;
      case HTTP_UPDATE_OK:
        if (mode_flag == 2) {
          delay(1000);
          ESP.restart();
        }
        else {
          bool check_firm = false;
          EEPROM.put(3, check_firm);
          EEPROM.commit();
          delay(1000);
          ESP.restart();
        }
        break;
    }
  }
}


void connect_wifi() {
  char ssid[35];
  char pass[65];
  EEPROM.get(3430, ssid);
  EEPROM.get(3467, pass);
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASS);
  WiFi.begin(ssid, pass); //подключаемся к точке досупа
  uint8_t count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    count ++;
    if (count > 20) {
      break;
    }
  }
}

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
    for (uint8_t i = 0; i < n; i++) {
      if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
        return WiFi.channel(i);
      }
    }
  }
  return 0;
}


void set_channel() {

  int32_t channel;
  channel = getWiFiChannel(WIFI_SSID);

  if (channel >= 1) {
    wifi_promiscuous_enable(1);
    wifi_set_channel(channel);
    wifi_promiscuous_enable(0);
  }

  else {
    char ssid[35];
    EEPROM.get(3430, ssid);
    channel = getWiFiChannel(ssid);
    if (channel >= 1) {
      wifi_promiscuous_enable(1);
      wifi_set_channel(channel);
      wifi_promiscuous_enable(0);
    }
  }
}


uint8_t setClock() {

  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    yield();
    now = time(nullptr);
  }
  struct tm * timeinfo;
  timeinfo = localtime(&now);

  uint8_t hour = timeinfo->tm_hour;

  return hour;
}
