
int val;//定义变量val
float joint_angles[6];// 定义一个浮点数数组

void setup()
{
  SerialUSB.begin(9600);//设置波特率为9600，这里要跟软件设置相一致。当接入特定设备（如：蓝牙）时，我们也要跟其他设备的波特率达到一致。
  pinMode(LED_BUILTIN,OUTPUT);//设置数字13 口为输出接口，Arduino 上我们用到的I/O 口都要进行类似这样的定义。
}
void loop()
{
  val=SerialUSB.read();//读取PC 机发送给Arduino 的指令或字符，并将该指令或字符赋给val    //注意是SerialUSB.read()
  if(val=='R')//判断接收到的指令或字符是否是“R”。
  { 
   SerialUSB.println("Hello World!");
   digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
   delay(1000);                      // wait for a second
   digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
   delay(1000);  
   for (int i = 0; i < 6; i++) 
    { 
     SerialUSB.println(joint_angles[i]); // 在串口监视器上打印数组
    }     
  }
}
