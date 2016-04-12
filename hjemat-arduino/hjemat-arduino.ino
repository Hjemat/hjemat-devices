byte data[4];
byte id = 0x2;

byte handshakeMessage[] = { B00000101, 0x0, 0x0, 0x10 };

byte productID[] = { 0x0, 0x0, 0x10 };

int ledPin = 13;

void setup() {
  Serial.begin(9600, SERIAL_8O1);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (Serial.available() == 0)
    return;
    
  Serial.readBytes(data, 4);

  if (data[0] != 0){
    if (id == 0x0) {
      if (data[0] == B00000011) {
        Serial.write(handshakeMessage, 4);
      }
      else if (data[0] == B00000101) {
        id = data[1];
      }
    }
    else {
      if (data[0] >> 3 == id){
        byte kom = data[0] & B00000111;
        
        if (kom == B101)
        {
          byte dataType =  data[1];
          int value = data[2];
          value = value << 8;
          value = value | data[3];

          handleDataChange(dataType, value);

          byte header = createHeader(B110);
          byte confirmationMessage[4] = { header, data[1], data[2], data[3]};

          Serial.write(confirmationMessage, 4);
        }
        else if (kom == B100)
        {
          byte header = createHeader(B111);
          byte pingback[4] = { header, productID[0], productID[1], productID[2] };

          Serial.write(pingback, 4);
        }
      }
    }
  }
}

byte createHeader(byte command)
{
  byte header = id << 3;
  header = header | command;
  return header;
}

void handleDataChange(byte dataType, int value) {
  if (dataType == 1){
    digitalWrite(ledPin, value == 0 ? LOW : HIGH);
  }
}


