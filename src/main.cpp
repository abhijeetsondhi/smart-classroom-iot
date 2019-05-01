#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <string>
#include <vector>
#include <set>
#include <iterator>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

using namespace std;
int scanTime = 5; 
BLEScan* pBLEScan;
const char* ssid = "LAPTOP-P4HHO4P1";
const char* password = "Abcd1233";
const char* awsEndpoint = "a1y1zh2yxjgx9j-ats.iot.us-west-2.amazonaws.com";
const char* topic = "arn:aws:iot:us-west-2:977366396705:thing/Node1";
const char* topic2 = "arn:aws:iot:us-west-2:977366396705:thing/Node2";
const char* subscribeTopic = topic;
const char* subscribeTopic2 = topic2;
const char* certificate_pem_crt = \

"-----BEGIN CERTIFICATE-----\n" \
"MIIDWTCCAkGgAwIBAgIUBsewCJxgia2dMzhtZAU5IxZ7bkEwDQYJKoZIhvcNAQEL\n" \
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n" \
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MDMyMDE5NTIz\n" \
"OFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n" \
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMMfNn3D0dPmYVYreh6A\n" \
"cqrg1YpYXaqrC53AuPw+iDfNkW9B6LoBzWXBciSpoPjgJcZWpGnY+9f853fZrnye\n" \
"OUw/xWau3TJ+heDhIZNqOv/8KQwnw6whKFbg2LHczwd53WD8MkJJPGSa1GY2Mg2J\n" \
"N00J6uOwclkTkEEEY72I/TLgV8LJGwrnKhKaN0KWOFY0HFZAxAOOo21+lCjGPfVd\n" \
"4RItH2YmKNfRVag1kCEdSN7NNheE91VsXre4iUnT9iuQJ8FToxSnHsZrQSzCy0UF\n" \
"1xiqq8NLQRDP9hMdUVhQ9EdzUM0KgYRZbeoj0tOlYbxN5/lIUMDO0kwpmIjlhjyJ\n" \
"wocCAwEAAaNgMF4wHwYDVR0jBBgwFoAUD6uPeDiyIfzyJp/PzPU2aCFmSLkwHQYD\n" \
"VR0OBBYEFD/Owq2NpN4rA4U91l3LnfGV7wvEMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n" \
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBZzXVJ9kVPMwpRUG07zPmTmosB\n" \
"9qezOGYAD+4lX5mFUBDtSKzRGVGuWFTjlyB6K9a2A/NuvCzdsP7uOrfmkaQHVirY\n" \
"nLuhOk7nsgKKxcAc/AiuzkU43iOjJQu0SSgYVkwl5qBp5TAbC9HV3behbnbVsmwk\n" \
"JfQZHFup4tvHotrNFOPvauTWzYYFQO7QSSkgGC6HIFg62sU+44Ue05WohFTnDH48\n" \
"CFGDWmFpOjBwtrmVDBrzJZ3hhPdBUVZLLSyRVwuaMA9A4qNnDl2XEV67fDqxdceh\n" \
"i2AYkrz7FSG4bCUF1nl19CpzFIdwLQaKz+5XY0n3bUR9+TjOM4zLULxrINIe\n" \
"-----END CERTIFICATE-----\n";

// xxxxxxxxxx-private.pem.key
const char* private_pem_key = \

"-----BEGIN RSA PRIVATE KEY-----\n" \
"MIIEowIBAAKCAQEAwx82fcPR0+ZhVit6HoByquDVilhdqqsLncC4/D6IN82Rb0Ho\n" \
"ugHNZcFyJKmg+OAlxlakadj71/znd9mufJ45TD/FZq7dMn6F4OEhk2o6//wpDCfD\n" \
"rCEoVuDYsdzPB3ndYPwyQkk8ZJrUZjYyDYk3TQnq47ByWROQQQRjvYj9MuBXwskb\n" \
"CucqEpo3QpY4VjQcVkDEA46jbX6UKMY99V3hEi0fZiYo19FVqDWQIR1I3s02F4T3\n" \
"VWxet7iJSdP2K5AnwVOjFKcexmtBLMLLRQXXGKqrw0tBEM/2Ex1RWFD0R3NQzQqB\n" \
"hFlt6iPS06VhvE3n+UhQwM7STCmYiOWGPInChwIDAQABAoIBABc1+1Fynd9LJIEP\n" \
"ovNF7F4PaNeQM3Q97J/TcS3wclvRBF/rwzd9R2W3YVCjLRbt8xBo9h3xReQnnZFQ\n" \
"P8VrPP2pCF5yprV+/4YEwe27UVFqOafMgyZSu226SYy+65HEGWWUGpZwB9Mf5+xQ\n" \
"vhuGGyH68Aotzyxxe1OApizBwvzuJaqISMRYPb5hRB3zQ9JmsLOxJn4uutjp+PcX\n" \
"O026FCa3CUpOeug8ssjKpyekFku1zJ5zadclr3FWSb+vxkamzoBzK+nu29dGUkRb\n" \
"PEHHrSw/T0iK6dHK+LdnLMTtDW2tVZbtLWI653opxWbg+r9+ysDoMrjGgGcG8CGi\n" \
"dNiaKQECgYEA8hhy+cLcY/MgIoLWma9EzwmKZMPnqqt7bn3h6DpDz7w597GIU47F\n" \
"P9xNzXLS7zRcj7b8ofUdPS6bkemmQ1Qm1oPeO8iHOsvkK/tr5cjv1LEeg/fYMwKf\n" \
"HsyFXpKVidNz1O4M6OCkqAPLY/Gme+ro310hr6dajvPjvXLXW+ULUeECgYEAzlQb\n" \
"cJiGsv3AqkAY4Q9oCqCVdoT27f+4N11jqknVT3Adcw5V8fYtviM0mgS+RKgSl8oB\n" \
"S/dgYlKtew3nXLpY/f0sfh2qzMqDnfiMEqK0c3+zLn+WtFISjFQgSIpNZikJwNIo\n" \
"qy3gUzJbEwxX5jJHqzr8nBOA1XyMBiZjl6fH8WcCgYEAk70r3RN3X19tSuJBiAQX\n" \
"n6kY1alf5R1E9AbKJ+f2nf0l+uxSX3uPEAunRys9YlC389Qdk9B7iWy572+A0TG3\n" \
"ic1rf1w+ejkXA2gpTk8zXL7B+pLzUx3UcP0qnq4hp7xswZKMOkls8kNuiY1cpd6T\n" \
"QkwZVJJyhy562pAMsL20DMECgYBM3kjayfhk9uSwiPC76GF6lBka7q6QPJUFC2X2\n" \
"K0utLLfMBas/Wdfw3mzadKnuKi/DHR8ejir6dpLrJWLsyQAeoBPTaHoZkdk3IUIO\n" \
"L0/LN+gpip86ztjaLv1YCS6MwP1BOQqtu4m3dSs5DYAx4smQoYk7HIkDAEOqhuLT\n" \
"vU5gQQKBgD2oIRIp78TPpTtr2HpYtYkhpjQ+BXR3spsARgtNxHs2IuwgsvvuBWi9\n" \
"GTUGFssS4fyyRX04BcEJ6v4KFwlH1nuFYWZcm3CRy4PWTTe6scPtkWp99q7nQysv\n" \
"iYqVPgLtQNL5o583O4GpUW2plBFZBAOQ/y5Y54l8zjFpzFNoRjLL\n" \
"-----END RSA PRIVATE KEY-----\n";

// This key should be fine as is. It is just the root certificate.
const char* rootCA = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
"rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
"-----END CERTIFICATE-----\n";

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient);
void pubSubCheckConnect();
set<string> student_data;
void setup() {
  Serial.begin(9600);
  Serial.println("Scanning...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); 
  pBLEScan->setActiveScan(true); 
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); 
  delay(50);
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());

  Serial.print("Connecting to "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); 
  Serial.println(WiFi.localIP());

  wiFiClient.setCACert(rootCA);
  wiFiClient.setCertificate(certificate_pem_crt);
  wiFiClient.setPrivateKey(private_pem_key);
}

void loop() {
  static BLEAdvertisedDevice* myDevice;
  // put your main code here, to run repeatedly:
  pubSubCheckConnect();
  set<string> tempData;
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  myDevice = new BLEAdvertisedDevice(foundDevices.getDevice(1));
  for(int i=0;i<foundDevices.getCount();i++)
  {
      String x = foundDevices.getDevice(i).toString().c_str();
      if(isDigit(x.substring(x.indexOf(':')+2,x.indexOf(','))[0]))
      {
        tempData.insert(x.substring(x.indexOf(':')+2,x.indexOf(',')).c_str());
      }
  }
  if(tempData.size() > student_data.size())
  {
    for(int i=0;i<foundDevices.getCount();i++)
    {
      String x = foundDevices.getDevice(i).toString().c_str();
      String realData = x.substring(x.indexOf(':')+2,x.indexOf(','));
      if(tempData.find(realData.c_str()) != tempData.end() && student_data.find(realData.c_str()) == student_data.end())
      {
        StaticJsonBuffer<300> JSONbuffer;
        JsonObject& JSONencoder = JSONbuffer.createObject();
        JSONencoder["key_value"] = realData.substring(0,realData.indexOf(" "));
        JSONencoder["name"] = realData.substring(realData.indexOf(" ") + 1);
        char JSONmessageBuffer[100];
        JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
        boolean rc = pubSubClient.publish(topic,JSONmessageBuffer);
        Serial.print("Published, rc="); 
        Serial.println(JSONmessageBuffer);
        student_data.insert(realData.c_str());
      }
    }
  }
  else if(tempData.size() < student_data.size())
  {
      for(string f:student_data)
      {
        if(tempData.find(f) == tempData.end())
        {
            String x = f.c_str();
            StaticJsonBuffer<300> JSONbuffer;
            JsonObject& JSONencoder = JSONbuffer.createObject();
            JSONencoder["key_value"] = x.substring(0,x.indexOf(" "));
            char JSONmessageBuffer[100];
            JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
            boolean rc = pubSubClient.publish(topic2,JSONmessageBuffer);
            Serial.print("Published, rc="); 
            Serial.println(JSONmessageBuffer);
            student_data.erase(f);
            Serial.println(f.c_str());
            Serial.println("deleted");
        }
      }
  }
  pBLEScan->clearResults();
  delay(5000);
}


void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); 
  Serial.print(topic); 
  Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
}

void pubSubCheckConnect() {
  if (!pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); 
    Serial.print(awsEndpoint);
    while (!pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("ESPthingXXXX");
      delay(1000);
    }
    Serial.println(" connected");
    pubSubClient.subscribe(subscribeTopic);
  }
  pubSubClient.loop();
}
