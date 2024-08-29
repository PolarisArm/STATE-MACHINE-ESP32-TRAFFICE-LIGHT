#include <Arduino.h>


int red = 2;
int yellow = 17;
int green  = 21;
int blue = 22;
class MainClass;

class BASECLASS // THe ABSTRACT CLASS
{
  public:
    
    virtual void nextState(MainClass* mc) = 0;
    virtual void display(int LED) = 0;
    virtual ~BASECLASS() {};
};

class REDLIGHT : public BASECLASS{ // MEMBER CLASS
  public:
    void nextState(MainClass* mc) override;
     void display(int LED){
      digitalWrite(LED,HIGH);
     };
};

class YELLOWLIGHT : public BASECLASS{
  public:
    void nextState(MainClass* mc) override;
   void display(int LED){
      digitalWrite(LED,HIGH);
     };
};

class GREENLIGHT : public BASECLASS{
  public:
    void nextState(MainClass* mc) override;
     void display(int LED){
      digitalWrite(LED,HIGH);
     };

};

class MainClass{
  protected:
    BASECLASS *activestate;
    REDLIGHT *redlight = new REDLIGHT;
    GREENLIGHT *greenlight = new GREENLIGHT;
    YELLOWLIGHT *yellowlight = new YELLOWLIGHT;

    int RED,YELLOW,GREEN;
    unsigned long previous, current = 0;
    unsigned long interval = 500;
  
  public:
    MainClass(int _RED, int _YELLOW, int _GREEN) : RED(_RED),YELLOW(_YELLOW),GREEN(_GREEN)
    {
      pinMode(RED,OUTPUT);
      pinMode(YELLOW,OUTPUT);
      pinMode(GREEN,OUTPUT);

      activestate = redlight;
      activestate->display(RED);
      
    }

    void update()
    {
      current = millis();
      if(current - previous >= interval)
      {
        activestate->nextState(this);
        previous = current;
      }
    }
    void setState(BASECLASS *changedState)
    {
     
        activestate = changedState;
    }



    BASECLASS* getRedState()
    {
      return redlight;
    }
    BASECLASS* getYellowState()
    {
      return yellowlight;
    }
    BASECLASS* getGreenState()
    {
      return greenlight;
    }

    int getRedpin()
    {
      return RED;
    }
    int getGreenpin()
    {
      return GREEN;
    }
    int getYellowpin()
    {
      return YELLOW;
    }



    ~MainClass()
    {
      delete(redlight);
      delete(yellowlight);
      delete(greenlight); 

    }
};

void REDLIGHT::nextState(MainClass *mc)
{
  digitalWrite(mc->getRedpin(),LOW);
  mc->setState(mc->getYellowState());
  mc->getYellowState()->display(mc->getYellowpin());
}

void YELLOWLIGHT::nextState(MainClass *mc)
{
  digitalWrite(mc->getYellowpin(),LOW);

  mc->setState(mc->getGreenState());
  mc->getYellowState()->display(mc->getGreenpin());

}

void GREENLIGHT::nextState(MainClass *mc)
{
  digitalWrite(mc->getGreenpin(),LOW);

  mc->setState(mc->getRedState());
  mc->getYellowState()->display(mc->getRedpin());

}

MainClass trafficlight(red,yellow,green);
void setup()
{
  Serial.begin(115200);

  

}



void loop()
{
  trafficlight.update();
}


