class Relay{
  public:
    //properties
    int pin;
    int state;

    Relay(int p, bool off = true)
    {
      pin = p;
      pinMode(pin, OUTPUT);

      state = off ? LOW : HIGH;
      digitalWrite(pin, state);
    }


    //methods
    void On()
    {
      if (state == HIGH) return;

      state = HIGH;
      digitalWrite(pin, state);
    }

    void Off()
    {
      if (state == LOW) return;

      state = LOW;
      digitalWrite(pin, state);
    }

    void Reverse()
    {
      if (state == LOW)On();
      else Off();
    }
};
