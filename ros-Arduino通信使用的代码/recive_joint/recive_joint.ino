//这是用于接收上位机传输的角度，并且执行的demo
#include <Arm7Bot.h>
Arm7Bot Arm;

double get_angles[6] = {110, 115, 65, 90, 90, 90};// 定义一个浮点数数组
String data; // 定义字符串变量

void setup()
{
  SerialUSB.begin(9600);//设置波特率为9600，这里要跟软件设置相一致。当接入特定设备（如：蓝牙）时，我们也要跟其他设备的波特率达到一致。
  // initial 7Bot Arm
  Arm.initialMove();
}
void loop()
{
   //get_angles=SerialUSB.read();//读取PC 机发送给Arduino 的指令或字符//注意是SerialUSB.read()   
   
   #define BUFFER_SIZE 128 // 定义字符数组大小
   #define STRING_SIZE 128 // 定义字符串数组大小

   char buffer [BUFFER_SIZE]; // 定义字符数组
   //String data; // 定义字符串变量
   String strings [STRING_SIZE]; // 定义字符串数组

   while (SerialUSB.available () > 0) { // 检查是否有可用数据
        int index = 0; // 定义索引变量
        while (true) { // 循环读取数据
            int val = SerialUSB.read (); // 读取一个字节
            if((char)val == 'R')break;
            if (val == -1 || val == '\n' || index >= BUFFER_SIZE - 1) { // 判断是否结束
                break; // 跳出循环
            }
            buffer [index] = (char) val; // 把字节转换为字符并存储到字符数组中
            index++; // 索引递增
        }
        buffer [index] = '\0'; // 在字符数组末尾添加空字符

        data = ""; // 清空字符串变量
        for (int i = 0; i < index; i++) { // 循环遍历字符数组
            if (buffer [i] == '\0' || data.length () >= STRING_SIZE - 1) { // 判断是否结束
                break; // 跳出循环
            }
            data += buffer [i]; // 把字符追加到字符串末尾
        }
/*
        char *ptr = strtok (data, ","); // 把字符串按逗号分割并返回第一个子字符串的指针
        index = 0; // 重置索引变量
        while (ptr != NULL) { // 循环分割字符串
            if (index >= STRING_SIZE) { // 判断是否超出范围
                break; // 跳出循环
            }
            strings [index] = ptr; // 把指针赋值给字符串数组中的元素
            index++; // 索引递增
            ptr = strtok (NULL, ","); // 继续分割并返回下一个子字符串的指针
        }
*/
        if(data.length()<6)
          SerialUSB.print("WOW\n");
        else
        {
          int len = -1;
          for (int i = 0; i < 6; i++) 
          { // 循环遍历字符串数组
            int sum = 0;
            bool flag = true;
            len++;
            while(data[len]!=',')
            {
               if(data[len]!='.'&&flag)
                  sum = sum * 10 + data[len] - '0';
               if(data[len] == '.')flag = false;
               len++;
            }
            get_angles [i] = (double)sum ;// 把字符串转换为浮点数并赋值给浮点数数组中的元素
          }
          SerialUSB.print ("Converted angles: ");
          for (int i = 0; i < 6; i++) { // 循环打印浮点数数组中的元素
            SerialUSB.print (get_angles [i]);
            if (i < 6 - 1) {
                SerialUSB.print (", ");
              }
           }
            SerialUSB.println (); // 换行     
        }
        //SerialUSB.print ("Received data: "); 
        //SerialUSB.println (data); // 打印接收到的数据
   }
     // change speed to 30
     Arm.maxSpeed[0] = 30;
     // Move to pose
     Arm.move(get_angles);
}
