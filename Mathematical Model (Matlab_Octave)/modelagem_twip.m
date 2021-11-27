%Modelagem do TWIP

pkg load control %descomentar essa linha para octave. 
                  % comentar para matlab


%% --- Parametros do motor  ---
% baseado em 
% https://fkeng.blogspot.com/2019/03/theory-and-design-of-two-wheels-self.html

% dados retirados do datasheet

Tstall = 0.8237585;        %N-m - stall torque
RPMnoload = 178;           %RPM  sem carga para Vin=12V
wnl = (RPMnoload*2*pi)/60; %Velocidade de rotação do eixo de saída da caixa 
                            %...de engrenagens sem carga em rad/s.
                            
Inl = 0.12;                  %A - Corrente sem carga.
Vin = 12;                    %V -Tensão de operação do motor.
Istall = 1;                  %A - corrente de armadura stall.
Ng = 56;                     %Relação de engrenagens.
Dg = 0.00001;                %kgm/rad/sec - Perda viscosa de engrenagem.
La = 0;                      %Assumindo que a indutância da armadura é 0.

Ra = Vin/Istall;             %Resistência da Armadura.
Kt = (Tstall/(Vin*Ng))*Ra;   %Constante de torque da Armadura.
Kb = (Vin-Ra*Inl)/(wnl*Ng);  %Constante de força eletromotriz da Armadura.

%% --- Parametros do driver ---
Kmd = 1.0;                   %Assume-se que é igual a 1.

%% --- Parametros da Roda ---
R = 0.0325;                   %m - Radio da roda.
Mw=0.04;                      %Kg - Massa da roda
Ia = 0.5*Mw*R*R;              %kgm2 - Momento de inércia do conjunto da roda
                              % com referência ao centro (estimado).

%% Parametros do Pendulo                             
Mb = 0.867;                   %Kg - Massa do pendulo 
d = 0.05;                     %M - Distância até o centro de massa (estimada).
L = 0.18;                     %M - Distância entre as rodas
g = 9.81;                     %M/s2 - Constante gravitacional
Ix=(Mb*L^2)/12;               %kgm2 - Momento de inércia do chassiem relação ao 
                              % eixo X (estimado).
Iz = 3*Ix;                    %kgm2 - Momento de inércia do chassiem relação ao 
                              % eixo Z (estimado).
 

%% Equações para o subsistema pêndulo (sem a dinâmica do motor DC)

beta = (Mb*R^2+2*Mw*R^2 + 2*Ia);
rho = ((Mb+2*Mw)*R^2 + 2*Ia)*Ix + 2*Mb*d^2*(Mw*R^2+Ia);

A43 = (Mb*g*d*beta)/rho;
B4 = -(Mb*d*R+beta)/rho;

gamma = (Mb*d^2 +Ix)*(Mb*R^2+2*Mw*R^2+2*Ia)-(Mb*d*R)^2;

A23 = -(Mb^2*d^2*g*R^2)/gamma;
B2 = (R*(Mb*d^2+Ix+Mb*d*R))/gamma;
A = [0 1 0 0;
    0 0 A23 0 ;
    0 0 0 1;
    0 0 A43 0];
B = [0;
     B2;
     0;
     B4];

%% Constantes do Motor DC 
Cm1 = Ng*Kt/Ra;
Cm2 = Cm1*Kb;
%% Dinâmica do motor DC 
Adc  = [0 0 0 0;
           0 -Cm2/(R*Ng) 0 Cm2;
           0 0 0 0;
           0 -Cm2/(R*Ng) 0 Cm2];   
A1 = Adc(:,1).*B;
A2 = Adc(:,2).*B;
A3 = Adc(:,3).*B;
A4 = Adc(:,4).*B;

%% Modelo final do subsistema pêndulo
Af = ([A1 A2 A3 A4])+A; % Matriz de estados do LQR - Concanenação da matriz 
                            % dada a dinâmica do motor com a matriz sem a dinâmica 
Bf = B*Cm1;             %Matriz de entrada final
Ci = [1 0 0 0];         %Matriz de saída, com somente a variável que a integral atuará.

%% Aumento do tamanho do sistema adicionando o erro (integral)
Atil = [Af zeros(4,1)]; % Adiciona-se, inicialmente, uma coluna de zeros à matriz 
                           % de entradas
Ctil = [Ci 0];          % Adiciona-se uma coluna de zeros (nesse caso, apenas um)
                          % À matriz de saída
Atil = [Atil;-Ctil];    %Concacane-se ambas, para obter a matriz de estados do LQI
Btil = [Bf;0];          %Adicina-se uma linha de zeros (nesse caso, apenas um)
                            %à matrz de entradas do LQR. Obtendo assim a matriz de 
                                  %entradas do LQI.

%% Cálculo do LQI                                  
Rlqi = 0.1; %Matriz R do LQI
Qi = diag([1000 200 50 0 0.05]); %Matriz Q do LQI

Klqi = lqr(Atil,Btil,Qi,Rlqi);  %Calcula a matriz de ganhos do LQI

%printa os resultados
fprintf("\nLQI:\n%.3f,%.3f,%.3f,%.3f,%.3f\n\n",Klqi(1),Klqi(2),Klqi(3),Klqi(4),Klqi(5));

%% Modelagem do subsistema rotação
alfa = 2*(Mw+Ia/R^2)*L^2 + Iz; 
B2z = L/(R*alfa);               
Ar = [0 1;                      %Matriz de estados do subsistema rotação 
      0 (L/(R*Ng))*(B2z*Cm2)];      %(roda direita)
                                    
Br = [0; B2z*Cm1];             %Matriz de entradas do subsistema rotação
                                     %(roda direita)
Al = [0 1;                     %Matriz de estados do subsistema rotação 
      0 -(L/(R*Ng))*(B2z*Cm2)];     %(roda esquerda)
Bl = [0; -B2z*Cm1];             %Matriz de entradas do subsistema rotação
                                    %(roda esquerda)

%% Cálculo do LQR para o subsistema rotação
Qz = diag([65 .1]);             %Matriz Q para o LQR do subsistema rotação
Rz = .1;                        %Matriz R para o LQR do subsistema rotação  
Kzr = lqr(Ar,Br,Qz,Rz);         %Calculo do ganho para a roda direita
Kzl = lqr(Al,Bl,Qz,Rz);         %Calculo do ganho para a roda esquerda

%printa os resultados
fprintf("LQR rotação:\n Roda Direita:\n");
fprintf("%.3f,",Kzr);
fprintf("\nRoda esquerda:\n");
fprintf("%.3f,",Kzl);
fprintf("\n");