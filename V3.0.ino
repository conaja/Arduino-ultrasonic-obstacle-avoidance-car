/*the SR[0] is the left sensor
  the SR[1] is the middle sensor
  the ST[2] is the right sensor
*/
#define LEFT 0
#define MID 1
#define RIGHT 2
#define Touch A1

struct SR04
{
    int Trig, Echo;
};

struct SR04 SR[3] = {2, 3, 4, 5, 6, 7};

const int IN1 = 8, IN2 = 9, IN3 = 12, IN4 = 13;
const int pwm1 = 10, pwm2 = 11;
const int turntime = 250;

float getdistance(int number);
void goahead();
void halfv();
void turn();
void stopp();

void setup()
{
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(pwm1, OUTPUT);
    pinMode(pwm2, OUTPUT);
    pinMode(Touch, INPUT);

    digitalWrite(pwm1, HIGH);
    digitalWrite(pwm2, HIGH);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    for (int i = 0; i < 3; i++)
    {
        pinMode(SR[i].Trig, OUTPUT);
        pinMode(SR[i].Echo, INPUT);
    }

    Serial.begin(9600); //设置波特率
}

void loop()
{
    if (digitalRead(Touch))
    {
        digitalWrite(pwm1, LOW);
        digitalWrite(pwm2, LOW);

        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        while (1){
            delay(1000);
        }
        
    }

    float n = getdistance(MID);

    Serial.print("Distance:");

    Serial.print(n);

    Serial.print("cm");

    Serial.println();

    if (n > 50)
    {
        goahead();
    }
    else
    {
        if (n > 30)
        {
            halfv();
        }
        else
        {
            if (n > 10)
            {
                turn();
            }
            else
            {
                stopp();
            }
        }
    }
    delay(50);
}

float getdistance(int number)
{
    float cm, sum = 0;

    for (int i = 0; i < 3; i++)
    {
        digitalWrite(SR[number].Trig, LOW); //低高低电平发一个短时间脉冲去TrigPin
        delayMicroseconds(2);               // delayMicroseconds在更小的时间内延时准确
        digitalWrite(SR[number].Trig, HIGH);
        delayMicroseconds(10);
        digitalWrite(SR[number].Trig, LOW);         //通过这里控制超声波的发射
        cm = pulseIn(SR[number].Echo, HIGH) / 58.0; //将回波时间换算成cm
        if (cm < 0)
        {
            cm = 450;
        }
        sum += cm;
    }

    return sum / 3;
}

void goahead()
{
    digitalWrite(pwm1, HIGH);
    digitalWrite(pwm2, HIGH);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void halfv()
{
    analogWrite(pwm1, 128);
    analogWrite(pwm2, 128);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turn()
{
    float left = getdistance(LEFT), right = getdistance(RIGHT);
    if (left > right)
    {
        digitalWrite(pwm1, HIGH);
        digitalWrite(pwm2, HIGH);

        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    }
    else
    {
        digitalWrite(pwm1, HIGH);
        digitalWrite(pwm2, HIGH);

        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    }
    delay(turntime);

    goahead();
}

void stopp()
{
    digitalWrite(pwm1, HIGH);
    digitalWrite(pwm2, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    for (int i = 0; i < 3; i++)
    {
        digitalWrite(A0, HIGH);
        delay(100);
        digitalWrite(A0, LOW);
        delay(100);
    }

    for (int i = 0; i < 3; i++)
    {
        digitalWrite(A0, HIGH);
        delay(200);
        digitalWrite(A0, LOW);
        delay(200);
    }

    for (int i = 0; i < 3; i++)
    {
        digitalWrite(A0, HIGH);
        delay(100);
        digitalWrite(A0, LOW);
        delay(100);
    }
    delay(100);
}
