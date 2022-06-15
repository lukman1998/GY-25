
int YPR[3];
unsigned char Re_buf[8], counter = 0;
unsigned char sign = 0;
int led = 13;
float t;
float r = 17;


void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.write(0XA5);
  Serial.write(0X52);    //初始化GY25,连续输出模式



}
//-------------------------------------------------------------
void loop() {
  if (sign)
  {
    sign = 0;
    if (Re_buf[0] == 0xAA && Re_buf[7] == 0x55)   //检查帧头，帧尾
    {
      YPR[0] = (Re_buf[1] << 8 | Re_buf[2]) / 100; //合成数据，去掉小数点后2位
      YPR[1] = (Re_buf[3] << 8 | Re_buf[4]) / 100;
      YPR[2] = (Re_buf[5] << 8 | Re_buf[6]) / 100;

      if (YPR[2] >= 0)
      {
        YPR[2] = YPR[2] - 179;
        YPR[2] = YPR[2] * (-1);
      }
      else if (YPR[2] < 0)
      {
        YPR[2] = YPR[2] + 179;
        YPR[2] = YPR[2] * (-1);
      }

      
      Serial.print("Pit: ");
      Serial.print(YPR[1]);
      Serial.print("   ||   ");

      t = r * sin(YPR[2] * 3.1412 / 180);
      Serial.print("Tinggi Roll :");
      Serial.print(t);
      Serial.print(" ||  ");

      Serial.print("Rol: ");
      Serial.println(YPR[2]);


      //delay(100);
    }
  }
}
//---------------------------------------------------------------
void serialEvent() {
  while (Serial.available()) {
    Re_buf[counter] = (unsigned char)Serial.read();
    if (counter == 0 && Re_buf[0] != 0xAA) return; // 检查帧头
    counter++;
    if (counter == 8)             //接收到数据
    {
      counter = 0;               //重新赋值，准备下一帧数据的接收
      sign = 1;
    }
  }
}
