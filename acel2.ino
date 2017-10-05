/*  ********************************************* 
 *  SparkFun_ADXL345_Example
 *  Triple Axis Accelerometer Breakout - ADXL345 
 *  Hook Up Guide Example 
 *  
 *  Utilizing Sparkfun's ADXL345 Library
 *  Bildr ADXL345 source file modified to support 
 *  both I2C and SPI Communication
 *  
 *  E.Robert @ SparkFun Electronics
 *  Created: Jul 13, 2016
 *  Updated: Sep 06, 2016
 *  
 *  Development Environment Specifics:
 *  Arduino 1.6.11
 *  
 *  Hardware Specifications:
 *  SparkFun ADXL345
 *  Arduino Uno
 *  *********************************************/
#include "Arduino.h"

#ifndef ADXL345_h
#define ADXL345_h

/*************************** REGISTER MAP ***************************/
#define ADXL345_DEVID      0x00    // Device ID
#define ADXL345_RESERVED1   0x01    // Reserved. Do Not Access. 
#define ADXL345_THRESH_TAP    0x1D    // Tap Threshold. 
#define ADXL345_OFSX      0x1E    // X-Axis Offset. 
#define ADXL345_OFSY      0x1F    // Y-Axis Offset.
#define ADXL345_OFSZ      0x20    // Z- Axis Offset.
#define ADXL345_DUR       0x21    // Tap Duration.
#define ADXL345_LATENT      0x22    // Tap Latency.
#define ADXL345_WINDOW      0x23    // Tap Window.
#define ADXL345_THRESH_ACT    0x24    // Activity Threshold
#define ADXL345_THRESH_INACT  0x25    // Inactivity Threshold
#define ADXL345_TIME_INACT    0x26    // Inactivity Time
#define ADXL345_ACT_INACT_CTL 0x27    // Axis Enable Control for Activity and Inactivity Detection
#define ADXL345_THRESH_FF   0x28    // Free-Fall Threshold.
#define ADXL345_TIME_FF     0x29    // Free-Fall Time.
#define ADXL345_TAP_AXES    0x2A    // Axis Control for Tap/Double Tap.
#define ADXL345_ACT_TAP_STATUS  0x2B    // Source of Tap/Double Tap
#define ADXL345_BW_RATE     0x2C    // Data Rate and Power mode Control
#define ADXL345_POWER_CTL   0x2D    // Power-Saving Features Control
#define ADXL345_INT_ENABLE    0x2E    // Interrupt Enable Control
#define ADXL345_INT_MAP     0x2F    // Interrupt Mapping Control
#define ADXL345_INT_SOURCE    0x30    // Source of Interrupts
#define ADXL345_DATA_FORMAT   0x31    // Data Format Control
#define ADXL345_DATAX0      0x32    // X-Axis Data 0
#define ADXL345_DATAX1      0x33    // X-Axis Data 1
#define ADXL345_DATAY0      0x34    // Y-Axis Data 0
#define ADXL345_DATAY1      0x35    // Y-Axis Data 1
#define ADXL345_DATAZ0      0x36    // Z-Axis Data 0
#define ADXL345_DATAZ1      0x37    // Z-Axis Data 1
#define ADXL345_FIFO_CTL    0x38    // FIFO Control
#define ADXL345_FIFO_STATUS   0x39    // FIFO Status

#define ADXL345_BW_1600     0xF     // 1111   IDD = 40uA
#define ADXL345_BW_800      0xE     // 1110   IDD = 90uA
#define ADXL345_BW_400      0xD     // 1101   IDD = 140uA
#define ADXL345_BW_200      0xC     // 1100   IDD = 140uA
#define ADXL345_BW_100      0xB     // 1011   IDD = 140uA 
#define ADXL345_BW_50     0xA     // 1010   IDD = 140uA
#define ADXL345_BW_25     0x9     // 1001   IDD = 90uA
#define ADXL345_BW_12_5       0x8     // 1000   IDD = 60uA 
#define ADXL345_BW_6_25     0x7     // 0111   IDD = 50uA
#define ADXL345_BW_3_13     0x6     // 0110   IDD = 45uA
#define ADXL345_BW_1_56     0x5     // 0101   IDD = 40uA
#define ADXL345_BW_0_78     0x4     // 0100   IDD = 34uA
#define ADXL345_BW_0_39     0x3     // 0011   IDD = 23uA
#define ADXL345_BW_0_20     0x2     // 0010   IDD = 23uA
#define ADXL345_BW_0_10     0x1     // 0001   IDD = 23uA
#define ADXL345_BW_0_05     0x0     // 0000   IDD = 23uA


 /************************** INTERRUPT PINS **************************/
#define ADXL345_INT1_PIN    0x00    //INT1: 0
#define ADXL345_INT2_PIN    0x01    //INT2: 1


 /********************** INTERRUPT BIT POSITION **********************/
#define ADXL345_INT_DATA_READY_BIT    0x07
#define ADXL345_INT_SINGLE_TAP_BIT    0x06
#define ADXL345_INT_DOUBLE_TAP_BIT    0x05
#define ADXL345_INT_ACTIVITY_BIT    0x04
#define ADXL345_INT_INACTIVITY_BIT    0x03
#define ADXL345_INT_FREE_FALL_BIT   0x02
#define ADXL345_INT_WATERMARK_BIT   0x01
#define ADXL345_INT_OVERRUNY_BIT    0x00

#define ADXL345_DATA_READY        0x07
#define ADXL345_SINGLE_TAP        0x06
#define ADXL345_DOUBLE_TAP        0x05
#define ADXL345_ACTIVITY        0x04
#define ADXL345_INACTIVITY        0x03
#define ADXL345_FREE_FALL       0x02
#define ADXL345_WATERMARK       0x01
#define ADXL345_OVERRUNY        0x00


 /****************************** ERRORS ******************************/
#define ADXL345_OK      1   // No Error
#define ADXL345_ERROR   0   // Error Exists

#define ADXL345_NO_ERROR  0   // Initial State
#define ADXL345_READ_ERROR  1   // Accelerometer Reading Error
#define ADXL345_BAD_ARG   2   // Bad Argument


class ADXL345
{
public:
  bool status;          // Set When Error Exists 

  byte error_code;        // Initial State
  double gains[3];        // Counts to Gs
  
  ADXL345();
  ADXL345(int CS);
  void powerOn();
  void readAccel(int* xyx);
  void readAccel(int* x, int* y, int* z);
  void get_Gxyz(double *xyz);
  
  void setTapThreshold(int tapThreshold);
  int getTapThreshold();
  void setAxisGains(double *_gains);
  void getAxisGains(double *_gains);
  void setAxisOffset(int x, int y, int z);
  void getAxisOffset(int* x, int* y, int*z);
  void setTapDuration(int tapDuration);
  int getTapDuration();
  void setDoubleTapLatency(int doubleTapLatency);
  int getDoubleTapLatency();
  void setDoubleTapWindow(int doubleTapWindow);
  int getDoubleTapWindow();
  void setActivityThreshold(int activityThreshold);
  int getActivityThreshold();
  void setInactivityThreshold(int inactivityThreshold);
  int getInactivityThreshold();
  void setTimeInactivity(int timeInactivity);
  int getTimeInactivity();
  void setFreeFallThreshold(int freeFallthreshold);
  int getFreeFallThreshold();
  void setFreeFallDuration(int freeFallDuration);
  int getFreeFallDuration();
  
  bool isActivityXEnabled();
  bool isActivityYEnabled();
  bool isActivityZEnabled();
  bool isInactivityXEnabled();
  bool isInactivityYEnabled();
  bool isInactivityZEnabled();
  bool isActivityAc();
  bool isInactivityAc();
  void setActivityAc(bool state);
  void setInactivityAc(bool state);
  
  bool getSuppressBit();
  void setSuppressBit(bool state);
  bool isTapDetectionOnX();
  void setTapDetectionOnX(bool state);
  bool isTapDetectionOnY();
  void setTapDetectionOnY(bool state);
  bool isTapDetectionOnZ();
  void setTapDetectionOnZ(bool state);
  void setTapDetectionOnXYZ(bool stateX, bool stateY, bool stateZ);
  
  void setActivityX(bool state);
  void setActivityY(bool state);
  void setActivityZ(bool state);
  void setActivityXYZ(bool stateX, bool stateY, bool stateZ);
  void setInactivityX(bool state);
  void setInactivityY(bool state);
  void setInactivityZ(bool state);
  void setInactivityXYZ(bool stateX, bool stateY, bool stateZ);
  
  bool isActivitySourceOnX();
  bool isActivitySourceOnY();
  bool isActivitySourceOnZ();
  bool isTapSourceOnX();
  bool isTapSourceOnY();
  bool isTapSourceOnZ();
  bool isAsleep();
  
  bool isLowPower();
  void setLowPower(bool state);
  double getRate();
  void setRate(double rate);
  void set_bw(byte bw_code);
  byte get_bw_code();  
  
  bool triggered(byte interrupts, int mask);
  
  byte getInterruptSource();
  bool getInterruptSource(byte interruptBit);
  bool getInterruptMapping(byte interruptBit);
  void setInterruptMapping(byte interruptBit, bool interruptPin);
  bool isInterruptEnabled(byte interruptBit);
  void setInterrupt(byte interruptBit, bool state);
  void setImportantInterruptMapping(int single_tap, int double_tap, int free_fall, int activity, int inactivity);
  void InactivityINT(bool status);
  void ActivityINT(bool status);
  void FreeFallINT(bool status);
  void doubleTapINT(bool status);
  void singleTapINT(bool status);
  
  void getRangeSetting(byte* rangeSetting);
  void setRangeSetting(int val);
  bool getSelfTestBit();
  void setSelfTestBit(bool selfTestBit);
  bool getSpiBit();
  void setSpiBit(bool spiBit);
  bool getInterruptLevelBit();
  void setInterruptLevelBit(bool interruptLevelBit);
  bool getFullResBit();
  void setFullResBit(bool fullResBit);
  bool getJustifyBit();
  void setJustifyBit(bool justifyBit);
  void printAllRegister();
  
private:
  void writeTo(byte address, byte val);
  void writeToI2C(byte address, byte val);
  void writeToSPI(byte address, byte val);
  void readFrom(byte address, int num, byte buff[]);
  void readFromI2C(byte address, int num, byte buff[]);
  void readFromSPI(byte address, int num, byte buff[]);
  void setRegisterBit(byte regAdress, int bitPos, bool state);
  bool getRegisterBit(byte regAdress, int bitPos);  
  byte _buff[6] ;   //  6 Bytes Buffer
  int _CS = 10;
  bool I2C = true;
  unsigned long SPIfreq = 5000000;
};
void print_byte(byte val);
#endif




//#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library
#include <ESP8266WiFi.h> // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient


#define TOPICO_PUBLISH_GUITARRA   "MQTTSensorGuitarra"
#define ID_MQTT  "iot_guitarra" 


// MQTT 
const char* BROKER_MQTT = "test.mosquitto.org"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT

// WIFI
//const char* SSID = "NET_2G99407E"; // SSID / nome da rede WI-FI que deseja se conectar
//const char* PASSWORD = "D099407E"; // Senha da rede WI-FI que deseja se conectar
const char* SSID = "iPhone de Rafael"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "palmeiras"; // Senha da rede WI-FI que deseja se conectar

//VARIAVEIS
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
int x,y,z;   


/*********** COMMUNICATION SELECTION ***********/
/*    Comment Out The One You Are Not Using    */
//ADXL345 adxl = ADXL345(10);           // USE FOR SPI COMMUNICATION, ADXL345(CS_PIN);
ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION

/****************** INTERRUPT ******************/
/*      Uncomment If Attaching Interrupt       */
//int interruptPin = 2;                 // Setup pin 2 to be the interrupt pin (for most Arduino Boards)


void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}

void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    //MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

/******************** SETUP ********************/
/*          Configure ADXL345 Settings         */
void setup(){
  
  Serial.begin(9600);                 // Start the serial terminal
  Serial.println("SparkFun ADXL345 Accelerometer Hook Up Guide Example");
  Serial.println();
  initWiFi();
  initMQTT();
  
  adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(2);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity

  adxl.setSpiBit(1);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                      // Default: Set to 1
                                      // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library 
   
  adxl.setActivityXYZ(1, 1, 1);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
 
  adxl.setInactivityXYZ(0, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

  adxl.setTapDetectionOnXYZ(0, 0, 0); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)
 
  // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
  adxl.setTapThreshold(50);           // 62.5 mg per increment
  adxl.setTapDuration(15);            // 625 μs per increment
  adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
  adxl.setDoubleTapWindow(200);       // 1.25 ms per increment
 
  // Set values for what is considered FREE FALL (0-255)
  adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment
 
  // Setting all interupts to take place on INT1 pin
  //adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
                                                        // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
                                                        // This library may have a problem using INT2 pin. Default to INT1 pin.
  
  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);
  adxl.FreeFallINT(1);
  adxl.doubleTapINT(1);
  adxl.singleTapINT(1);
  
//attachInterrupt(digitalPinToInterrupt(interruptPin), ADXL_ISR, RISING);   // Attach Interrupt

}

void lerSensor(){

  // Accelerometer Readings
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z

  // Output Results to Serial
  /* UNCOMMENT TO VIEW X Y Z ACCELEROMETER VALUES */  
//  Serial.print(x);
//  Serial.print(", ");
  Serial.print(y);
  Serial.println(", ");
//  Serial.println(z); 

}

/****************** MAIN CODE ******************/
/*     Accelerometer Readings and Interrupt    */
void loop(){

  VerificaConexoesWiFIEMQTT();
  lerSensor();
  EnviaEstadoOutputMQTT();
  
  MQTT.loop();
  
}

//Função: envia ao Broker o estado atual do output 
//Parâmetros: nenhum
//Retorno: nenhum
void EnviaEstadoOutputMQTT(void)
{
     
     //Manda informação do ADXL345
     char result[4]; // Buffer big enough for 7-character float
     dtostrf(y, 4, 0, result);       
     MQTT.publish(TOPICO_PUBLISH_GUITARRA, result);
     Serial.print("result: ");
     Serial.println(result);

    delay(300);
}

//Função: verifica o estado das conexões WiFI e ao broker MQTT. 
//        Em caso de desconexão (qualquer uma das duas), a conexão
//        é refeita.
//Parâmetros: nenhum
//Retorno: nenhum
void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
    
     reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}


//--------------------------------------------------------------------------------
//                       ROTINAS PARA RECONECTAR AO WIFI E MQTT
//--------------------------------------------------------------------------------
//Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
//        em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
//Parâmetros: nenhum
//Retorno: nenhum
void reconnectMQTT() 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            //MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else 
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
 
//Função: reconecta-se ao WiFi
//Parâmetros: nenhum
//Retorno: nenhum
void reconectWiFi() 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
        
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
    
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
  
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
