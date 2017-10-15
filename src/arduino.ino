const int steper1_dir = 30;
const int steper1_pul = 31;
const int steper2_dir = 32;
const int steper2_pul = 33;
const int steper3_dir = 34;
const int steper3_pul = 35;
const int signal_rev = 13;

const int std_speed = 400;
int dir1 = 0;
int steps1 = 0;
int dir2 = 0;
int steps2 = 0;
int dir3 = 0;
int steps3 = 0;
bool tag = 0;

int readSerial()
{
    int num = 0;
    if (Serial.available())
    {
        digitalWrite(signal_rev, 1);
        byte incomingByte = Serial.read();
        while (!Serial.available())
            ;
        while (incomingByte != '#')
        {
            if (incomingByte >= '0' && incomingByte <= '9')
                num = num * 10 + (incomingByte - '0');
            while (!Serial.available())
                ;
            incomingByte = Serial.read();
        }
    }
    digitalWrite(signal_rev, 0);
    return num;
}

void step_para(int dir1, int steps1, int dir2, int steps2, int dir3,
               int steps3)
{
    digitalWrite(steper1_dir, dir1);
    digitalWrite(steper2_dir, dir2);
    digitalWrite(steper3_dir, dir3);
    int max_steps = max(max(steps1, steps2), steps3);
    for (int i = 0; i < max_steps; i++)
    {
        if (i < steps1)
            digitalWrite(steper1_pul, 1);
        if (i < steps2)
            digitalWrite(steper2_pul, 1);
        if (i < steps3)
            digitalWrite(steper3_pul, 1);
        delayMicroseconds(std_speed);
        if (i < steps1)
            digitalWrite(steper1_pul, 0);
        if (i < steps2)
            digitalWrite(steper2_pul, 0);
        if (i < steps3)
            digitalWrite(steper3_pul, 0);
        delayMicroseconds(std_speed);
    }
}

void setup()
{
    // put your setup code here, to run once:
    pinMode(steper1_dir, OUTPUT);
    pinMode(steper1_pul, OUTPUT);
    pinMode(steper2_dir, OUTPUT);
    pinMode(steper2_pul, OUTPUT);
    pinMode(steper3_dir, OUTPUT);
    pinMode(steper3_pul, OUTPUT);
    Serial.begin(9600);
    delay(2000);
}

void loop()
{
    // put your main code here, to run repeatedly:
    while (!Serial.available())
        tag = 0;
    if (tag == 0)
    {
        delay(50);
        tag = 1;
    };
    if (Serial.available() < 11)
        delay(5);
    if (Serial.read() == '$')
    {
        dir1 = int(Serial.read() - '0');
        steps1 = readSerial();
        dir2 = int(Serial.read() - '0');
        steps2 = readSerial();
        dir3 = int(Serial.read() - '0');
        steps3 = readSerial();
        Serial.write('@');
        step_para(dir1, steps1, dir2, steps2, dir3, steps3);
    }
}
