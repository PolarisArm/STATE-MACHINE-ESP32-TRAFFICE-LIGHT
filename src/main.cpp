#include <Arduino.h>

class MAINCLASS;

class BASE{
  public:
    virtual void nextState(MAINCLASS *mc) = 0;
    virtual void display(int LED);
    virtual  ~BASE(){ };

};

class OFFSTATE : public BASE{
  public:
    void nextState(MAINCLASS *mc) override;
    void display(int LED) override
    {
      ledcWrite(LED, 0);
    }
};

class LOWSTATE : public BASE{
  public:
   void nextState(MAINCLASS *mc) override;
    void display(int LED) override
    {
      ledcWrite(LED, 65);
    }
};

class MIDSTATE : public BASE{
  public:
    void nextState(MAINCLASS *mc) override;
    void display(int LED) override
    {
      ledcWrite(LED, 150);
    }
};

class HIGHSTATE : public BASE{
  public:
    void nextState(MAINCLASS *mc) override;
    void display(int LED) override
    {
      ledcWrite(LED, 250);
    }
};


class MAINCLASS{
  protected:
    BASE *activeState;
    
    OFFSTATE *offState = new OFFSTATE;
    LOWSTATE *lowState = new LOWSTATE;
    MIDSTATE *midState = new MIDSTATE;
    HIGHSTATE *highState = new HIGHSTATE;

    unsigned long previous, current = 0;
    unsigned long interval = 500;

    int _LED;
    public:
      MAINCLASS(int LED):_LED(LED)
      {
        ledcAttachPin(_LED, 0);
        Serial.println("MAIN CLASS HAS BEEN CREATED");
        Serial.println("OFFSTATE");
        activeState = offState;
        activeState->display(_LED);
       
      }

      void changeState(BASE *newState)
      {
        activeState = newState;
      }

      void update()
      {
        current = millis();
        if(current - previous >= interval)
        {
          activeState->nextState(this);
          activeState->display(_LED);
          previous = current;
        }
      }

      BASE* getOffState()
      {
        return offState;
      }

       BASE* getLowState()
      {
        return lowState;
      }

       BASE* getMidState()
      {
        return midState;
      }

       BASE* getHighState()
      {
        return highState;
      }

      ~MAINCLASS()
      {
        delete(offState);
        delete(lowState);
        delete(midState);
        delete(highState);
        Serial.println("MAIN CLASS IS DELETED");
      }


};





void OFFSTATE::nextState(MAINCLASS *mc)
{
  mc->changeState(mc->getLowState());
}

void LOWSTATE::nextState(MAINCLASS *mc)
{
  mc->changeState(mc->getMidState());
}

void MIDSTATE::nextState(MAINCLASS *mc)
{
  mc->changeState(mc->getHighState());
}

void HIGHSTATE::nextState(MAINCLASS *mc)
{
  mc->changeState(mc->getOffState());
}

int led = 2;
MAINCLASS* mc;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mc = new MAINCLASS(led);

}
void loop() {
mc->update();
}

