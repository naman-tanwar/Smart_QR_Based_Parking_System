#include <esp_now.h>
#include <WiFi.h>
#include <HardwareSerial.h>

HardwareSerial SerialPort(2); // use UART2


// Must match the sender structure
typedef struct struct_message {
    int parkID;
    int parkStatus;
} struct_message;

struct_message myData;
struct_message currentFree;

void Initilize_free() // status = 0 --> free
{
  currentFree.parkID = 0; // 0 means invalid/NULL parking
  currentFree.parkStatus = 0;
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  Serial.println("ondata");
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("ID: ");
  Serial.println(myData.parkID);
  Serial.print("Status: ");
  Serial.println(myData.parkStatus);
  Serial.println();

  if(myData.parkStatus == 0) //if parking area free
  {
    currentFree.parkID = myData.parkID;
    currentFree.parkStatus = 0;
  }
  
  if(myData.parkID == currentFree.parkID)
  {
    if(myData.parkStatus ==1) // if the parking data stored in currentparking is not free then empty the free variable
    {
      currentFree.parkID = 0; 
      currentFree.parkStatus = 0;
    }
  }
  SerialPort.print(currentFree.parkID);
  Serial.println();
  Serial.println();
  Serial.print("Free Parking INFO: ");
  Serial.print("ID: ");
  Serial.println(currentFree.parkID);
  Serial.print("Status: ");
  Serial.println(currentFree.parkStatus);
  Serial.println();

  Serial.println();
  Serial.println();
 }




void setup() {
  SerialPort.begin(15200, SERIAL_8N1, 16, 17); 
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(500);
  Initilize_free();

  Serial.println("st");
 WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
        Serial.println("ESP NOW failed to initialize");
        while (1);
    }
    Serial.println("init");

    esp_now_register_recv_cb(OnDataRecv);
    
}
 
void loop() {

}
