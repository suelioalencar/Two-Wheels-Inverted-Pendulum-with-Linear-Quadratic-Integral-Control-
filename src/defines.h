// Declaraçao dos Pinos do Encoder
#define L_A 17 //->17
#define L_B 5 //->5
#define R_A 26 //->26
#define R_B 13 //->13

//Declaração dos Pinos do Motor 
#define L_PWM 25 //->25
#define L_IN1 14 //->14
#define L_IN2 12 //->12
#define R_PWM 27 //->27
#define R_IN1 18 //->18
#define R_IN2 19 //->19

#define PWMGAIN 21.5 //V  -< PWM = |250|/|12|


//Declaração das constantes matemáticas usadas 
#define DEGTORAD 0.01745329
#define encoder_ratio 56
#define radius 0.0325
#define backward 0
#define forward 1
#define stop 2

//Pino Padrão do ESP32
#define PIN_WIFI_LED 2 
//Variáveis usadas para a contagem do tempo das tarefas
TickType_t momentodeinicio;
TickType_t usertime;

double q,qant=0; //Variáveis usadas na integral 


//Vetor de ganhos do LQI: 
//            |poslin|,|vellin|,|theta|,|thetadot|, |q|
float k1[5] ={-100.493,-69.900,-83.636,-9.314,0.107};

//Vetores dos ganhos do LQR (rotação) para ambas as rodas
float kzl[3] = {-25.495,-1.568};
float kzr[2] = {25.495,1.570};

//Declaração dos estados
double phi,theta=0,thetadot=0,x,xant=0,xdot,delta,deltadot;

//Variáveis auxiliares
float d_angle,d_gyro_x,d_gyro_z,angle_ant=0,anglez_ant = 0,stop_flag=0; 

//Variáveis usadas na contagem dos pulsos do Encoder 
double counter_r = 0, counter_l=0,avgcounter;

//Constante de tempo do controlados (8 ms)
float dt = 0.008;

//Variáveis usadas para mudar a referência de posição linear e rotação
double zsetpoint,ref;

//Variáveis para controlar as velocidades das rodas
int L_pwm,R_pwm;


String MAC;  // String para armazenar o endereço MAC do Esp

//Credenciais do Wifi
String sta_ssid = "Suelioifibra24Ghz";     
String sta_password = "81909556";        

//Caso o Esp32 não consiga se conectar à WIFI, ele cria um AP
String AcessPoint_Password = "LACIFMA2021";


// Declaração das variáveis usadas para o controle WIFI
uint8_t OSCpage;
uint8_t OSCnewMessage;
double OSCfader[4];

const char* PARAM_FADER1 = "fader1";
const char* PARAM_FADER2 = "fader2";
const char* PARAM_FADER3 = "fader3";
const char* PARAM_FADER4 = "fader4";



//Declaração de todas as funções que será usadas no código
void L_Run(int pwm,char dir);
void R_Run(int pwm,char dir);
void Tarefa_de_controle( void * pvParameters );
void Tarefa_wifi( void * pvParameters );
void Set_Pwm();
void resetCar();
void IRAM_ATTR encoder_change_L();
void IRAM_ATTR encoder_change_R();
char Turn_Off(float angle);


