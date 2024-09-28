#include <SoftwareSerial.h>


SoftwareSerial BTSerial(10, 11); // RX, TX ports for Bluetooth module

const char* c_passwords[] = {"0000", "1234", "6953", "1111"}; //propagate as needed
const char* devices[] = {"DEVICE_MAC_ADDRESS"}; //replace with target device mac addresses
const char* VFirmware_versions[] = {"1.1.1.1", "0.0.0.0"}; //replace with unsafe known vulnerable versions
const int pin_count = sizeof(c_passwords)/sizeof(c_passwords[0]);
const int device_count = sizeof(devices)/sizeof(devices[0]);
const int num_versions = sizeof(VFirmware_versions)/sizeof(VFirmware_versions[0]);

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.println("Connected to Bluetooth Module ");
  delay(1000);
  
}

void password_check(const char* device_mac){
  Serial.print("Attempting to connect to device: ");
  Serial.println(device_mac);

  for (int m = 0; m < pin_count; m++){
    BTSerial.print("AT+CONN");
    BTSerial.print(device_mac);
    BTSerial.print(",");
    BTSerial.print(c_passwords[m]);
    BTSerial.print("\r\n");

    delay(2000);
    String response = "";
    while (BTSerial.available()){
      char c = BTSerial.read();
      const char* target = "OK";

      response += c;
    }
    Serial.println(response);
      

    if (response.indexOf("OK") != -1){
      Serial.print("Connected to device: ");
      Serial.print(device_mac);
      Serial.print("with");
      Serial.print(c_passwords[m]);
      return;
      }
    else{
        Serial.print("Pin: ");
        Serial.print(c_passwords[m]);
        Serial.println(" failed");
      }
    delay(1000);
    
  }
}

void firmware_check(const char* device_mac){
  BTSerial.print("AT+FIRMWARE_VERSION");
  delay(2000);
  String firmware_version = "";
  while (BTSerial.available()){
    char c = BTSerial.read();
    firmware_version += c;
  }
  Serial.println(firmware_version);
  for (int k = 0; k < num_versions; k++){
    if (firmware_version == VFirmware_versions[k]){
      Serial.print("unsafe version detected: ");
      Serial.print(VFirmware_versions[k]);
      
    }
  }
  Serial.println("firmware check passed!");
  
  
}

void loop() {
  Serial.println("Beginning Security Assessment...");
  for (int i = 0; i < device_count; i++){
    password_check(devices[i]);
    Serial.println("password check passed!");
    firmware_check(devices[i]);
    
  }
  
}
