#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX, TX pins for Bluetooth module

// Known vulnerable devices and configurations
const char* VULNERABLE_SERVICES[] = {"0x1800", "0x1801", "0x180F"}; // Example vulnerable services
const char* DEFAULT_PINS[] = {"0000", "1234", "1111", "9999", "0123", "1122", "8888"};
const char* VULNERABLE_FIRMWARE[] = {"1.1.1.1", "0.0.0.0", "2.0.1"};

const char* KNOWN_EXPLOITS[] = {
  "BlueBorne", "KNOB Attack", "BIAS Attack", "SweynTooth"
};
const char* ENCRYPTION_MODES[] = {
  "E0", "E1", "AES-CCM", "None"
};
const int NUM_EXPLOITS = sizeof(KNOWN_EXPLOITS) / sizeof(KNOWN_EXPLOITS[0]);
const int NUM_MODES = sizeof(ENCRYPTION_MODES) / sizeof(ENCRYPTION_MODES[0]);

const int NUM_SERVICES = sizeof(VULNERABLE_SERVICES) / sizeof(VULNERABLE_SERVICES[0]);
const int NUM_PINS = sizeof(DEFAULT_PINS) / sizeof(DEFAULT_PINS[0]);
const int NUM_FIRMWARE = sizeof(VULNERABLE_FIRMWARE) / sizeof(VULNERABLE_FIRMWARE[0]);

// Scan results storage
#define MAX_DEVICES 10
char* found_devices[MAX_DEVICES];
int device_count = 0;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  Serial.println("BlueSec - Bluetooth Security Assessment Tool");
  Serial.println("-------------------------------------------");
  delay(1000);
  
  // Initialize Bluetooth module
  BTSerial.println("AT+INIT");
  delay(1000);
}

void scanDevices() {
  Serial.println("Scanning for nearby Bluetooth devices...");
  BTSerial.println("AT+INQ");
  delay(5000);
  
  while (BTSerial.available()) {
    String response = BTSerial.readString();
    parseDevices(response);
  }
}

void parseDevices(String response) {
  // Parse inquiry response and store found devices
  int index = 0;
  while (response.indexOf("+INQ:") >= 0 && device_count < MAX_DEVICES) {
    int start = response.indexOf("+INQ:") + 5;
    int end = response.indexOf("\r\n", start);
    String device = response.substring(start, end);
    
    // Extract MAC address
    found_devices[device_count] = strdup(device.c_str());
    device_count++;
    
    response = response.substring(end + 2);
  }
}

void checkEncryption(const char* device_mac) {
  Serial.println("Checking encryption...");
  BTSerial.print("AT+ENCR=");
  BTSerial.println(device_mac);
  delay(2000);
  
  String response = "";
  while (BTSerial.available()) {
      response += (char)BTSerial.read();
  }
  
  if (response.indexOf("None") != -1) {
      Serial.println("WARNING: No encryption detected!");
  } else {
      Serial.print("Encryption: ");
      Serial.println(response);
  }
}

void checkSignalStrength(const char* device_mac) {
  Serial.println("Checking signal strength...");
  BTSerial.print("AT+RSSI=");
  BTSerial.println(device_mac);
  delay(1000);
  
  String response = "";
  while (BTSerial.available()) {
      response += (char)BTSerial.read();
  }
  
  int rssi = response.toInt();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
  
  if (rssi > -50) {
      Serial.println("WARNING: Device extremely close - potential evil twin attack");
  }
}

void scanForEvilTwins() {
  Serial.println("Checking for duplicate devices...");
  
  for (int i = 0; i < device_count; i++) {
      for (int j = i + 1; j < device_count; j++) {
          if (strcmp(found_devices[i], found_devices[j]) == 0) {
              Serial.println("WARNING: Potential evil twin detected!");
              Serial.print("Duplicate MAC address: ");
              Serial.println(found_devices[i]);
          }
      }
  }
}

void checkVulnerabilities(const char* device_mac) {
  Serial.println("Checking for known vulnerabilities...");
  
  // Check for specific Bluetooth versions vulnerable to known exploits
  BTSerial.print("AT+VERSION=");
  BTSerial.println(device_mac);
  delay(2000);
  
  String response = "";
  while (BTSerial.available()) {
      response += (char)BTSerial.read();
  }
  
  for (int i = 0; i < NUM_EXPLOITS; i++) {
      if (isVulnerableToExploit(response, KNOWN_EXPLOITS[i])) {
          Serial.print("WARNING: Potentially vulnerable to ");
          Serial.println(KNOWN_EXPLOITS[i]);
      }
  }
}

bool isVulnerableToExploit(String version, const char* exploit) {
  if (strcmp(exploit, "BlueBorne") == 0 && version.indexOf("4.0") != -1) {
      return true;
  }
  if (strcmp(exploit, "KNOB Attack") == 0 && version.indexOf("5.0") != -1) {
      return true;
  }
  if (strcmp(exploit, "BIAS Attack") == 0 && version.indexOf("4.2") != -1) {
      return true;
  }
  return false;
}

void checkSecurity(const char* device_mac) {
  Serial.print("\nAssessing device: ");
  Serial.println(device_mac);
  
  checkServices(device_mac);
  checkDefaultPins(device_mac);
  checkFirmware(device_mac);
  checkEncryption(device_mac);  // New
  checkSignalStrength(device_mac);  // New
  checkVulnerabilities(device_mac);  // New
}

void generateReport() {
  Serial.println("\n=== Security Assessment Report ===");
  Serial.print("Devices scanned: ");
  Serial.println(device_count);
  
  for (int i = 0; i < device_count; i++) {
      Serial.print("\nDevice ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(found_devices[i]);
  }
  
  scanForEvilTwins();  // New
  
  Serial.println("\nAssessment complete!");
  Serial.println("===========================");
}

void checkServices(const char* device_mac) {
  Serial.println("Checking for vulnerable services...");
  BTSerial.print("AT+SDAP=");
  BTSerial.println(device_mac);
  delay(2000);
  
  String response = "";
  while (BTSerial.available()) {
    response += (char)BTSerial.read();
  }
  
  for (int i = 0; i < NUM_SERVICES; i++) {
    if (response.indexOf(VULNERABLE_SERVICES[i]) != -1) {
      Serial.print("WARNING: Vulnerable service found: ");
      Serial.println(VULNERABLE_SERVICES[i]);
    }
  }
}

void checkDefaultPins(const char* device_mac) {
  Serial.println("Testing default PINs...");
  
  for (int i = 0; i < NUM_PINS; i++) {
    BTSerial.print("AT+PAIR=");
    BTSerial.print(device_mac);
    BTSerial.print(",");
    BTSerial.println(DEFAULT_PINS[i]);
    delay(2000);
    
    String response = "";
    while (BTSerial.available()) {
      response += (char)BTSerial.read();
    }
    
    if (response.indexOf("OK") != -1) {
      Serial.print("WARNING: Device accepts default PIN: ");
      Serial.println(DEFAULT_PINS[i]);
      return;
    }
  }
}

void checkFirmware(const char* device_mac) {
  Serial.println("Checking firmware version...");
  BTSerial.print("AT+VERSION=");
  BTSerial.println(device_mac);
  delay(2000);
  
  String response = "";
  while (BTSerial.available()) {
    response += (char)BTSerial.read();
  }
  
  for (int i = 0; i < NUM_FIRMWARE; i++) {
    if (response.indexOf(VULNERABLE_FIRMWARE[i]) != -1) {
      Serial.print("WARNING: Vulnerable firmware version detected: ");
      Serial.println(VULNERABLE_FIRMWARE[i]);
      return;
    }
  }
}



void loop() {
  Serial.println("\nStarting new security assessment scan...");
  
  // Clear previous results
  device_count = 0;
  for (int i = 0; i < MAX_DEVICES; i++) {
    if (found_devices[i] != NULL) {
      free(found_devices[i]);
      found_devices[i] = NULL;
    }
  }
  
  scanDevices();
  
  // Assess each found device
  for (int i = 0; i < device_count; i++) {
    checkSecurity(found_devices[i]);
  }
  
  generateReport();
  
  delay(30000); // pause 30 seconds
}
