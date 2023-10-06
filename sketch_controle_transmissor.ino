// CLIENTE
#include <SPI.h>
#include <RH_NRF24.h>
#include <RF24.h>

const int limiteX_baixo = 300;
const int limiteX_alto = 340;

const int limiteY_baixo = 313;
const int limiteY_alto = 353;

// Singleton instance of the radio driver
RH_NRF24 nrf24;

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(A0, INPUT);  // EIXO X
  pinMode(A1, INPUT);  // EIXO Y

  Serial.begin(9600);

  while (!Serial)
    ;  // wait for serial port to connect. Needed for Leonardo only
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(2))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}
void loop() {
  int butA, butB, butC, butD;
  int eixoX, eixoY;
  butA = digitalRead(2);  // frente
  butB = digitalRead(3);  // direita
  butC = digitalRead(4);  // tras
  butD = digitalRead(5);  // esquerda

  eixoX = analogRead(A0);
  eixoY = analogRead(A1);

  Serial.print("Eixo X:");
  Serial.println(eixoX);
  Serial.print("Eixo Y:");
  Serial.println(eixoY);

  uint8_t zero[] = "0";    //frente
  uint8_t um[] = "1";      //atras
  uint8_t dois[] = "2";    //direita
  uint8_t tres[] = "3";    //esquerda
  uint8_t quatro[] = "4";  //parado
  uint8_t data[] = "4";


  if (butA == 0) {
    *data = *zero;
  } else if (butB == 0) {
    *data = *dois;
  } else if (butC == 0) {
    *data = *um;
  } else if (butD == 0) {
    *data = *tres;
  }

  if (eixoX >= limiteX_baixo && eixoX <= limiteX_alto && eixoY >= limiteY_baixo && eixoY <= limiteY_alto) {
    *data = *quatro;
  } 
  if (eixoY > limiteY_alto) {
    *data = *zero;
  }
  if (eixoY < limiteY_baixo) {
    *data = *um;
  }
  if (eixoX > limiteX_alto) {
    *data = *dois;
  }
  if (eixoX < limiteX_baixo) {
    *data = *tres;
  }


  Serial.println("\nTransmitindo mensagem...");  // print na console serial
  nrf24.send(data, sizeof(data));




  /*
  nrf24.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (nrf24.waitAvailableTimeout(1)) {
    // Should be a reply message for us now
    if (nrf24.recv(buf, &len)) {
      Serial.print("Recebi resposta: ");
      Serial.println((char*)buf);
    } else {
      Serial.println("recv failed");
    }
  } else {
    Serial.println("Sem resposta, nrf24_server está rodando?");
  }
  delay(500);
  */
}