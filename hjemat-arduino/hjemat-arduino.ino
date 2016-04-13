byte data[4];
byte id = 0x2;

byte handshakeMessage[] = { B00000101, 0x0, 0x0, 0x10 };

byte productID[] = { 0x0, 0x0, 0x10 };

int ledPin = 13;

int ledValue = 0;

byte comPing      = 0x1;
byte comBack      = 0x2;
byte comGet       = 0x3;
byte comSet       = 0x4;
byte comReturn    = 0x5;

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
        
        if (kom == comSet)
        {
          byte dataType =  data[1];
          int value = data[2];
          value = value << 8;
          value = value | data[3];

          handleDataChange(dataType, value);

          returnValue(dataType);
        }
        else if (kom == comGet)
        {
          byte dataID =  data[1];

          returnValue(dataID);
        }
        else if (kom == comPing)
        {
          byte header = createHeader(comBack);
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

void returnValue(byte dataID)
{
  short value;
  
  if (dataID == 0x1)
  {
    value = ledValue;
  }
  
  byte header = createHeader(comReturn);
  byte returnMessage[4] = { header, dataID, (byte)(value >> 8), (byte)value };

  Serial.write(returnMessage, 4);
}

void handleDataChange(byte dataType, int value) {
  if (dataType == 1){
    ledValue = value == 0 ? LOW : HIGH;
    digitalWrite(ledPin, ledValue);
  }
}


