#define EN 8 //步进电机使能端，低电平有效
#define X_DIR 5 //X轴步进电机方向控制
#define Y_DIR 7 //y轴步进电机方向控制
#define Z_DIR 6 //z轴步进电机方向控制
#define X_STP 2 //x轴步进控制
#define Y_STP 4 //y轴步进控制
#define Z_STP 3 //z轴步进控制

/*
//函数：step 功能：控制步进电机方向，步数。
//参数：dir 方向控制, dirPin对应步进电机的DIR引脚，stepperPin 对应步进电机的step引
脚，steps 步进的步数
//无返回值
*/

void step(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite(dirPin, dir);
  delay(50);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(800); 
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(800); 
  }

}
void setup(){//将步进电机用到的IO管脚设置成输出
  Serial.begin(9600);
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);
  pinMode(Z_DIR, OUTPUT); pinMode(Z_STP, OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
}

int cur_x = 15000;
int cur_y = 15000;

int x_c = 240;
int y_c = 320;

int init_x = 15000;
int init_y = 15000;

int X_dist = 240; // 竖直
int Y_dist = 320; // 水平
bool X_dir = true;
bool Y_dir = true;
bool iX_dir = false;
bool iY_dir = false;

int c = 25;
int wide = 500;

void loop(){
  while(Serial.available()>0){
    char val = Serial.read();
    if(val=='r'){
      int x = cur_x;
      int y = cur_y;
      Serial.print(y);
      Serial.print(" ");
      Serial.print(x);
      Serial.println();
      X_dist = x - init_x;
      Y_dist = y - init_y;
      if(X_dist > 0){
        X_dir = true;
        iX_dir = false;
      }else{
        X_dir = false;
        iX_dir = true;
        X_dist = -X_dist;
      }
      if(Y_dist > 0){
        Y_dir = true;
        iY_dir = false;
      }else{
        Y_dir = false;
        iY_dir = true;
        Y_dist = -Y_dist;
      } 
      cur_x = init_x;
      x_c = init_x;
      y_c = init_y;
      cur_y = init_y;
      Serial.print(Y_dist);
      Serial.print(" ");
      Serial.print(X_dist);
      Serial.println();
      step(X_dir, X_DIR, X_STP, X_dist);
      step(Y_dir, Y_DIR, Y_STP, Y_dist);
    }
    if(val=='i'){
      int x = -1;
      int y = -1;
      int x_out = 0;
      int y_out = 0;
      for(int i=0;i<3;i++){
        while(1){
          y = (int)Serial.read()-48;     
          if(y>=0) break;
        }
        //Serial.print(x);
        y_out += y;
        y_out *= 10;
      }
      y_out /= 10;
      
      for(int i=0;i<3;i++){
        while(1){
          x = (int)Serial.read()-48;     
          if(x>=0) break;
        }
        //Serial.print(y);
        x_out += x;
        x_out *= 10;
      }
      x_out /= 10;
      Serial.print(y_out);
      Serial.print(" ");
      Serial.print(x_out);
      Serial.println();
      X_dist = x_out-x_c;
      Y_dist = y_out-y_c;

      x_c = x_out;
      y_c = y_out;

      X_dist = (int)X_dist;
      Y_dist = (int)Y_dist;
      Serial.print(X_dist);
      Serial.print(" ");
      Serial.print(Y_dist);
      Serial.println();

      if(X_dist > 0){
        X_dir = false;
        iX_dir = true;
        X_dist = c*X_dist;
      }else{
        X_dir = true;
        iX_dir = false;
        X_dist = -c*X_dist;
        //Serial.println("-");
      }
      if(Y_dist > 0){
        Y_dir = false;
        iY_dir = true;
        Y_dist = c*Y_dist;
      }else{
        Y_dir = true;
        iY_dir = false;
        Y_dist = -c*Y_dist;
        //Serial.println("-");
      } 
    }

    if(val=='o'){
      // Serial.print(Y_dist);
      // Serial.print(" ");
      // Serial.print(X_dist);
      // Serial.println();
      // Serial.println(X_dir);
      step(X_dir, X_DIR, X_STP, X_dist);
      if(X_dir) cur_x -= X_dist;
      else cur_x += X_dist;
      step(Y_dir, Y_DIR, Y_STP, Y_dist);
      if(Y_dir) cur_y -= Y_dist;
      else cur_y += Y_dist;
      delay(2000);

      step(true, Z_DIR, Z_STP, 1700); 
      delay(1000);

      step(false, Y_DIR, Y_STP, 250);
      step(true, X_DIR, X_STP, 1300);
      step(true, Y_DIR, Y_STP, 500);
      delay(500);
      step(false, X_DIR, X_STP, 1300);
      delay(500);
      step(false, Y_DIR, Y_STP, 250);

      step(false, Z_DIR, Z_STP, 1700);
      delay(1000);
      
      delay(1500);

      Serial.print(y_c);
      Serial.print(" ");
      Serial.println(x_c);

      Serial.print(cur_y);
      Serial.print(" ");
      Serial.println(cur_x);

      // step(iY_dir, Y_DIR, Y_STP, Y_dist);
      // if(iY_dir) cur_y -= Y_dist;
      // else cur_y += Y_dist;
      // step(iX_dir, X_DIR, X_STP, X_dist);
      // if(iX_dir) cur_x -= X_dist;
      // else cur_x += X_dist;
      // delay(1000);
    }
  }
}
