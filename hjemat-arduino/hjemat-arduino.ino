//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(2, 3);

byte data[4];
byte id = 0x2;

byte handshakeMessage[] = { B00000101, 0x0, 0x0, 0x10 };

byte productID[] = { 0x0, 0x0, 0x1 };

int ledPin = 13;

int ledValue = 0;

byte comPing      = 0x1;
byte comBack      = 0x2;
byte comGet       = 0x3;
byte comSet       = 0x4;
byte comReturn    = 0x5;

void setup() {
  Serial.begin(115200);
  //Serial.begin(115200);

  DDRB = DDRB | B00110000;
  PORTB = PORTB & B11011111;
}

void loop() {
  if (Serial.available() == 0)
    return;
    
  Serial.readBytes(data, 4);
  /*Serial.print(data[0], BIN);
  Serial.print(" | ");
  Serial.println(data[0] >> 3);*/

  if (data[0] != 0){
    if (id == 0x0) {
      if (data[0] == B00000011) {
        tWrite(handshakeMessage, 4);
      }
      else if (data[0] == B00000101) {
        id = data[1];
      }
    }
    else {
      if (data[0] >> 3 == id){
        //Serial.println("I've been summoned!");
        byte kom = data[0] & B00000111;
        
        if (kom == comSet)
        {
          //Serial.println("I've been told to do something");
          byte dataType =  data[1];
          int value = data[2];
          value = value << 8;
          value = value | data[3];

          handleDataChange(dataType, value);

          returnValue(dataType);
        }
        else if (kom == comGet)
        {
          //Serial.println("I've to tell something");
          byte dataID =  data[1];

          returnValue(dataID);
        }
        else if (kom == comPing)
        {
          //Serial.println("I've have to tell that I'm awake!");
          byte header = createHeader(comBack);
          byte pingback[4] = { header, productID[0], productID[1], productID[2] };
          delay(150);
          tWrite(pingback, 4);
        }
        else
        {
          //Serial.println("I've do not know that they want :(");
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

  tWrite(returnMessage, 4);
}

void handleDataChange(byte dataType, int value) {
  if (dataType == 1){
    ledValue = value == 0 ? LOW : HIGH;
    
    if (value == 0)
    {
      PORTB = PORTB & B11011111;
    }
    else
    {
      PORTB = PORTB | B00100000;
    }
  }
}

void tWrite(byte bytes[], int numBytes)
{
  //Serial.println("Writing fine data");
  PORTB = PORTB | B00100000;
  delay(5);
  Serial.write(bytes, numBytes);

  delay(5);
  PORTB = PORTB & B11011111;
}

void tWrite(byte bytes[])
{
  tWrite(bytes, 4);
}


