//Declaração das bibliotecas que serão usadas
#include <analogWrite.h>
#include <Arduino.h>
#include <I2Cdev.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <stdio.h>
#include "defines.h"
#include "index.h"

 
//Criação de um objeto ASyncWebServer na porta 80
AsyncWebServer server(80);


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
      



unsigned long previousMillis = 0;

MPU6050 mpu6050(Wire);

void setup()
{
    //Inicia a porta serial com um BaudRate de 115200
    Serial.begin(115200);

    /*Os pinos de leitura dos puldos do Encoder devem
    ser inicializados como PULL-UP, para garantir que os 
    estados lógicos sejam precisos*/
    pinMode(L_A, INPUT_PULLUP);
    pinMode(R_A, INPUT_PULLUP);
    
    //Criação das Interrupções para contar os pulsos dos encoders 
    attachInterrupt(digitalPinToInterrupt(L_A),encoder_change_L,CHANGE);
    attachInterrupt(digitalPinToInterrupt(R_A),encoder_change_R,CHANGE);

    //Inicialização dos pinfos do motor como saída
    pinMode(L_IN1,OUTPUT);
    pinMode(L_IN2,OUTPUT);
    pinMode(R_IN1,OUTPUT);
    pinMode(R_IN2,OUTPUT);
    pinMode(L_PWM,OUTPUT);
    pinMode(R_PWM,OUTPUT);
    
    //Coloca-se todos em nível lógigo baixo inicialmente
    digitalWrite(L_IN1,LOW);
    digitalWrite(L_IN2,LOW);
    digitalWrite(R_IN1,LOW);
    digitalWrite(R_IN1,LOW);
    analogWrite(L_PWM,0);
    analogWrite(R_PWM,0);
    
    //inicia-se a MPU6050
    Wire.begin();
    mpu6050.begin();

    //Defini-se aqui se quer calcular o offset do giroscópio
    mpu6050.calcGyroOffsets(true);
    


  // Caso o Esp32 seja usado como acess-point, ele terá um nome a ser definido + ]
  // O endereço MAC do Chip
  char chip_id[15];
  snprintf(chip_id, 15, "%04X", (uint16_t)(ESP.getEfuseMac()>>32));
  String hostname = "esp32-Self-Balancing-Robot-By-LAC_IFMA-" + String(chip_id);

  Serial.println();
  Serial.println("Hostname: "+hostname);

  
  //Rotina para conectar ao WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(sta_ssid.c_str(), sta_password.c_str());
  Serial.println("");
  Serial.print("Connecting to: ");
  Serial.println(sta_ssid);
  Serial.print("Password: ");
  Serial.println(sta_password);

  //Caso não seja possível, espera 5 segundos e tenta novamente
  unsigned long currentMillis = millis();
  previousMillis = currentMillis;
  while (WiFi.status() != WL_CONNECTED && currentMillis - previousMillis <= 5000) {
    delay(500);
    Serial.print(".");
    currentMillis = millis();
  }

  // Se o ESP32 conectou ao WIFI, é mostrado o endereço IP
  // Se não for possível conectar, configura o ESP32 com acess-point e mostra o IP
  IPAddress myIP;
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("*WiFi-STA-Mode*");
    Serial.print("IP: ");
    myIP=WiFi.localIP();
    Serial.println(myIP);
    digitalWrite(PIN_WIFI_LED, HIGH);    //Apenas pisca o led para indicar que está conectado
    delay(2000);
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hostname.c_str(),AcessPoint_Password.c_str());
    myIP = WiFi.softAPIP();
    Serial.println("");
    Serial.println("WiFi failed connected to " + sta_ssid);
    Serial.println("");
    Serial.println("*WiFi-AP-Mode*");
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    digitalWrite(PIN_WIFI_LED, LOW);   // Apenas pisca o led para indicar que o AP foi criado
    delay(2000);
  }


  // Envia um request para o <ESP_IP>/?fader=<inputValue>
    server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputValue;
    String inputMessage;
    OSCnewMessage = 1;
    
    // Obtém os valores para frente/trás
    if (request->hasParam(PARAM_FADER1)) {
      OSCpage = 1;
      inputValue = request->getParam(PARAM_FADER1)->value();
      inputMessage = PARAM_FADER1;
      OSCfader[0] = inputValue.toDouble();
      Serial.printf("fader 1: %f \n",OSCfader[0]);
    }
    // Obtém os valores para direita/esquerda
    else if (request->hasParam(PARAM_FADER2)) {
      OSCpage = 1;
      inputValue = request->getParam(PARAM_FADER2)->value();
      inputMessage = PARAM_FADER2;
      OSCfader[1] = inputValue.toDouble();
      Serial.printf("fader 2: %f \n",OSCfader[1]);
    }
    else if (request->hasParam(PARAM_FADER3)) {
      OSCpage = 1;
      inputValue = request->getParam(PARAM_FADER3)->value();
      inputMessage = PARAM_FADER3;
      OSCfader[2] = inputValue.toDouble();
    }
    // Obtém os valores double para as referências (rastreio definido)
    else if (request->hasParam(PARAM_FADER4)) {
      OSCpage = 1;
      inputValue = request->getParam(PARAM_FADER4)->value();
      inputMessage = PARAM_FADER4;
      OSCfader[3] = inputValue.toDouble();
    }
    else {
      inputValue = "No message sent";
    }
    request->send_P(200, "text/html", index_html); //Envia a página HTML criada para o servidor
  });

  server.onNotFound (notFound);   
  server.begin();                           // Inicia o Servidor

 

  // Valores inicias dos FADERs
  OSCfader[0] = 0.0;
  OSCfader[1] = 0.0;
  OSCfader[2] = 41;
  OSCfader[3] = 121;

  
  ArduinoOTA.begin();   // Inicia o OTA (possibilidade de fazer upload do código via WIFI)

  //Criação das tarefas de controle e Wifi, uma em cada núcleo
  xTaskCreatePinnedToCore(Tarefa_de_controle, " Tarefa_de_controle", 1500, NULL, 1, NULL, 0);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(Tarefa_wifi, "Tarefa_wifi", 4500, NULL, 3, NULL, 1);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
   }

void loop(){}

void Tarefa_wifi( void * pvParameters ){
usertime = xTaskGetTickCount();
while (1)
{
ArduinoOTA.handle();

if(OSCfader[0]==1) ref += 0.003;
if(OSCfader[0]==-1) ref -= 0.003;
if(OSCfader[1]==1) zsetpoint += 1*DEGTORAD;
if(OSCfader[1]==-1) zsetpoint -= 1*DEGTORAD;;

//Rotina para ir para uma determinada posição:
if((OSCfader[1]==0)&&(OSCfader[3]<=120)){
zsetpoint = OSCfader[3] *DEGTORAD;
}
else OSCfader[3]=121;

if((OSCfader[0]==0)&&(OSCfader[2]<=40)){
ref = OSCfader[2]*0.01;
}
else OSCfader[2]=41;

vTaskDelayUntil(&usertime, pdMS_TO_TICKS(20));
}
vTaskDelete(NULL);
}


//Tarefa de controle (LQR)
void Tarefa_de_controle( void * pvParameters )
{
  momentodeinicio = xTaskGetTickCount(); //Marca o tempo de início da chamada da função
  while(1){
  sei();
  mpu6050.update(); //Atualiza os dados da MPU6050
  //Obtém o ângulo (de equilíbrio) do robô em graus e converte pra Radianos
  theta = mpu6050.getAngleX() * DEGTORAD; 
  //Obtém a velocidade angular (de equilíbrio) do robô e converte pra Radianos
  thetadot  = mpu6050.getGyroX()* DEGTORAD; 
  //Obtém o ângulo (de rotação) do robô em graus e converte pra Radianos
  delta = (mpu6050.getAngleZ()) * DEGTORAD;
  //Obtém a velocidade angular (de rotação) do robô e converte pra Radianos
  deltadot = (mpu6050.getGyroZ()) * DEGTORAD;
  
  //Calcula a média da contagem dos pulsos de ambas as rodas
  avgcounter = (counter_l + counter_r) / 2;
  /*Calcula o ângulo da rota baseado na quantidade de pulsos, como se segue:
 https://www.dynapar.com/knowledge/applications/angle_encoders/
 a(graus) = count*360/PPR
 The encoder has 11 ppr per channel, and 2 channels.
 So PPR = 360/(22*56) = 45/154
 a(rad) = a(graus)*pi/180
 dist = a(rad)*r; */
  phi = avgcounter * 45 / 154 * DEGTORAD;
  //A posição linear é o ângulo x Raio
  x = phi * radius;
  //Se estima (derivada) a velocidade linear 
  xdot = (x - xant) / dt;
  //Calcula o erro da integral do LQI
  q = qant + (ref - x);

  //Passa os parâmetros atuais para os anterioes (para usar na derivada)
  qant = q;
  xant = x;

// A rotina abaixo apenas printa os estados ba porta serial
/*Serial.print(x);
  Serial.print(",");
  Serial.print(xdot);
  Serial.print(",");
  Serial.print(mpu6050.getAngleX());
  Serial.print(",");
  Serial.print(mpu6050.getGyroX());
  Serial.print(",");
  Serial.print(mpu6050.getAngleZ());
  Serial.print(",");
  Serial.println(mpu6050.getGyroZ()); */

  // Calcula os ganhos do LQI e multiplica pelo ganho em relação ao PWM 
  L_pwm= (-(k1[0]*x+k1[1]*xdot+k1[2]*theta+k1[3]*thetadot+k1[4]*q+kzl[0]*(delta-zsetpoint)+kzl[1]*deltadot))*PWMGAIN;
  R_pwm = (-(k1[0]*x+k1[1]*xdot+k1[2]*theta+k1[3]*thetadot+k1[4]*q+kzr[0]*(delta-zsetpoint)+kzr[1]*deltadot))*PWMGAIN;
  
  //Limita a velocidade com base nos limites do PWM
  R_pwm = constrain(R_pwm, -250, 250);  
  L_pwm = constrain(L_pwm, -250, 250);

  //Desliga o sistema caso o |ângulo| seja maior que 30 graus
  if (Turn_Off(mpu6050.getAngleX())) {
    stop_flag = 1;
    resetCar();
  } else {
    stop_flag = 0;
    Set_Pwm();
  }

  //Determina o tempo exato da tarefa. Nesse caso, 8ms.  
  vTaskDelayUntil(&momentodeinicio, pdMS_TO_TICKS(dt*1000));
}
}

//Contador dos Pulsos para a roda esquerda. 
//Isso é uma porta XOR. Para mais detalhes ler o capítulo 5 da monografia
void  IRAM_ATTR encoder_change_L()
{
    if(digitalRead(L_A)==LOW)
    {
        if(digitalRead(L_B)==HIGH){
            counter_l--;
           }
        else{
            counter_l++;
        }
    }
    else
    {
        if(digitalRead(L_B)==HIGH){
            counter_l++;
        }
        else{
            counter_l--;
        }
    }
}

/*O mesmo para a roda direita, mas com as direções trocadas (dada a posição física de ambas)*/
void IRAM_ATTR encoder_change_R()
{
    if(digitalRead(R_A)==LOW)
    {
        if(digitalRead(R_B)==HIGH){
            counter_r++;}
        else{
            counter_r--;}
    }
    else
    {
        if(digitalRead(R_B)==HIGH){
            counter_r--;}
        else{
            counter_r++;}
    }
}

void resetCar()
{
    L_Run(0,stop);
    R_Run(0,stop);
}

char Turn_Off(float angle)
{
    if(angle<-30||angle>30) return 1;
    return 0;
}

/*Função para determinar a direção das rodas com base nos valores dos ganhos
do controlador
*/
void Set_Pwm()
{
    if(L_pwm>0 && R_pwm > 0){ 
    L_Run(L_pwm,forward);
    R_Run(R_pwm,forward);
    }
    else if(L_pwm<0 && R_pwm < 0){ 
    L_Run(L_pwm,backward);
    R_Run(R_pwm,backward);
    }
    else if(L_pwm>0 && R_pwm < 0){ 
    L_Run(L_pwm,forward);
    R_Run(R_pwm,backward);
    }
    else if(L_pwm<0 && R_pwm > 0){ 
    L_Run(L_pwm,backward);
    R_Run(R_pwm,forward);
    }
}


/*Função para roda a roda direita.
Inicialmente se configura os pinos direcionais e depois
se escreve o valor do equivalente de velocidade dos ganhos do controlador
*/
void L_Run(int pwm,char dir)
{
    if(dir==forward)
    {
        digitalWrite(L_IN1,LOW);
        digitalWrite(L_IN2,HIGH);
    }
    else if(dir==backward)
    {
        digitalWrite(L_IN1,HIGH);
        digitalWrite(L_IN2,LOW);
    }
    else
    {
        digitalWrite(L_IN1,LOW);
        digitalWrite(L_IN2,LOW);
    }
    analogWrite(L_PWM,abs(pwm));
}
// O mesmo para a roda esquerda
void R_Run(int pwm,char dir)
{    
    if(dir==forward)
    {
        digitalWrite(R_IN1,HIGH);
        digitalWrite(R_IN2,LOW);
       
    }
    else if(dir==backward)
    {
        digitalWrite(R_IN1,LOW);
        digitalWrite(R_IN2,HIGH);
        
    }
    else
    {
        digitalWrite(R_IN1,LOW);
        digitalWrite(R_IN2,LOW);
    }
     analogWrite(R_PWM,abs(pwm));
}





