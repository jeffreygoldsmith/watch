//
// Class to control buttons
//

//
// Function to detect amount of time a button has been pressed.
//
void pressTime(int pin, unsigned long *pressTime)
{
  if (!digitalRead(pin))
  {
    if (!*pressTime)
      *pressTime = millis();
  } else {
    *pressTime = 0;
  }
}

//
// Input::Input() -- Class constructor
//
Input::Input(Row rows)
{
  this.rows = rows;
}


//
// Input::Init() -- Initialization method
//
void Input::Init()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}


//
// Input::TakeInput() -- Take readings from buttons
//
void Input::TakeInput()
{
  //
  // Check for button presses.
  //
  if (digitalRead(A0) && !button1Prev && isFirstB1)
  {
    if (millis() - pressMill1 >= 1000)
    {
      isSet = !isSet;
      Serial.println("Hold 1");
    } else {
      if (isSet)
      {
        row = (row + 1) % 3;
      }
      Serial.println("Press 1");
    }
    isFirstB1 = false;
  } else {
    isFirstB1 = true;
  }

  if (digitalRead(A1) && !button2Prev && isFirstB2)
  {
    if (millis() - pressMill2 >= 1000)
    {
      Serial.println("Hold 2");
    } else {
      if (isSet)
      {
        tempTime[row] = (tempTime[row] + 1) % timeOverflowAmount[row];
      }
      Serial.println("Press 2");
    }
    isFirstB2 = false;
  } else {
    isFirstB2 = true;
  }

  pressTime(A0, &pressMill1);
  pressTime(A1, &pressMill2);

  button1Prev = digitalRead(A0); // Update lagging values
  button2Prev = digitalRead(A1);
}