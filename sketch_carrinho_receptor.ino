//SERVIDOR
#include <SPI.h>
#include <RH_NRF24.h>

// Singleton instance of the radio driver
RH_NRF24 nrf24;

// carrinho parado
void fullStop() {

  digitalWrite(7, LOW);
  digitalWrite(6, LOW);

  digitalWrite(5, LOW);
  digitalWrite(4, LOW);

  digitalWrite(3, LOW);
  digitalWrite(2, LOW);

  digitalWrite(A0, LOW);
  //digitalWrite(1, LOW);
  digitalWrite(9, LOW);
}

void frente() {

  digitalWrite(7, HIGH);  
  digitalWrite(6, LOW);

  digitalWrite(5, HIGH);
  digitalWrite(4, LOW);

  digitalWrite(3, HIGH);
  digitalWrite(2, LOW);  

  digitalWrite(A0, HIGH);
  digitalWrite(9, LOW);
}

void atras() {

  digitalWrite(7, LOW);  
  digitalWrite(6, HIGH);

  digitalWrite(5, LOW);
  digitalWrite(4, HIGH);

  digitalWrite(3, LOW);
  digitalWrite(2, HIGH);  

  digitalWrite(A0, LOW);
  digitalWrite(9, HIGH);
}
void esquerda() {

  digitalWrite(7, HIGH); // ATRAS DIREITA 
  digitalWrite(6, LOW);

  digitalWrite(5, LOW); // ATRAS ESQUERDA
  digitalWrite(4, HIGH);

  digitalWrite(3, HIGH); // FRENTE DIREITA
  digitalWrite(2, LOW);  

  digitalWrite(A0, LOW); // FRENTE ESQUERDA
  digitalWrite(9, HIGH); 
}
void direita() {
  digitalWrite(7, LOW); // ATRAS DIREITA 
  digitalWrite(6, HIGH);

  digitalWrite(5, HIGH); // ATRAS ESQUERDA
  digitalWrite(4, LOW);

  digitalWrite(3, LOW); // FRENTE DIREITA
  digitalWrite(2, HIGH);  

  digitalWrite(A0, HIGH); // FRENTE ESQUERDA
  digitalWrite(9, LOW); 
}

void setup() {

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

  // Inputs de Polaridade do Motor
  pinMode(9, OUTPUT);
  pinMode(7, OUTPUT);  
  pinMode(6, OUTPUT);  
  pinMode(5, OUTPUT);  

  pinMode(4, OUTPUT);  
  pinMode(3, OUTPUT);  
  pinMode(2, OUTPUT);  
  pinMode(A0, OUTPUT);  
 

}

void loop() {
  // CODIGO DO RECEPTOR
  //
  //
  int valorRecebido;
  if (nrf24.available()) {
    // Should be a message for us now
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (nrf24.recv(buf, &len)) {
      //      NRF24::printBuffer("request: ", buf, len);
      Serial.print("got request: ");
      Serial.println((char*)buf);
      // carrinho controle
      valorRecebido = atoi(buf);
      int but0, but1, but2, but3, butAtual;
      butAtual = valorRecebido;

      switch (butAtual) {
        case 0:
          frente();
          break;

        case 1:
          atras();
          break;

        case 2:
          direita();
          break;

        case 3:
          esquerda();
          break;

        case -1:
          fullStop();
          break;

        default:
          fullStop();
          break;
      }

      // Send a reply
      uint8_t data[] = "Funcionando, hi";
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      Serial.println("Sent a reply");
    } else {
      Serial.println("recv failed");
    }
  }

  // CODIGO DO CARRINHO
  //
  //
  /*
  int but0, but1, but2, but3, butAtual;

  butAtual = valorRecebido;

  
  switch (butAtual) {
    case 0:
      frente();
      break;

    case 1:
      atras();
      break;

    case 2:
      direita();
      break;

    case 3:
      esquerda();
      break;

    case -1:
      fullStop();
      break;

    default:
      fullStop();
      break;
  }
  */
}