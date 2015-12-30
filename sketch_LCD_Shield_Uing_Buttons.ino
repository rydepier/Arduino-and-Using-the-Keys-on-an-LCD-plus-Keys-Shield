
/*---------------------------------------------------------
ReadButtons
Detect the button pressed
*----------------------------------------------------------
Define pins used and button return values
---------------------------------------------------------*/
#define BUTTON_ADC_PIN A0 // A0 is the button ADC input
#define LCD_BACKLIGHT_PIN 3 // D3 controls LCD backlight

// ADC readings expected for the 5 buttons on the ADC input
// these values may need altering to suit your board
#define RIGHT_10BIT_ADC 0 // right button
#define UP_10BIT_ADC 101 // up button
#define DOWN_10BIT_ADC 259 // down button
#define LEFT_10BIT_ADC 410 // left button
#define SELECT_10BIT_ADC 640 // right button


#define BUTTONHYSTERESIS 10 // hysteresis for valid button sensing window
//return values for ReadButtons()
#define BUTTON_NONE 0
#define BUTTON_RIGHT 1
#define BUTTON_UP 2
#define BUTTON_DOWN 3
#define BUTTON_LEFT 4
#define BUTTON_SELECT 5

/*---------------------------------------------------------
Variables
-----------------------------------------------------------*/
byte buttonJustPressed = false; //this will be true after a ReadButtons() call if triggered
byte buttonJustReleased = false; //this will be true after a ReadButtons() call if triggered
byte buttonWas = BUTTON_NONE; //used by ReadButtons() for detection of button events
/*---------------------------------------------------------*/

void setup(){
Serial.begin(9600); // start the Serial to allow results to be shown
//button adc input
pinMode( BUTTON_ADC_PIN, INPUT ); //ensure A0 is an input
digitalWrite( BUTTON_ADC_PIN, LOW ); //ensure pullup is off on A0
}

void loop(){
byte button;
//get the latest button pressed, also the buttonJustPressed, buttonJustReleased flags
button = ReadButtons();
//show text label for the button pressed

switch( button )
{
case BUTTON_NONE:
{
break;
}
case BUTTON_RIGHT:
{
Serial.println( "RIGHT" );
break;
}
case BUTTON_UP:
{
Serial.println( "UP" );
break;
}
case BUTTON_DOWN:
{
Serial.println( "DOWN" );
break;
}
case BUTTON_LEFT:
{
Serial.println( "LEFT" );
break;
}
case BUTTON_SELECT:
{
Serial.println( "SELECT" );
break;
}
default:
{
break;
}
}

//clear the buttonJustPressed or buttonJustReleased flags, they've already done their job now.
if( buttonJustPressed ){
buttonJustPressed = false;
}
if( buttonJustReleased ) {
buttonJustReleased = false;
}
} // End main loop


/*--------------------------------------------------------------------------------------
  ReadButtons()
  Detect the button pressed and return the value
  Uses global values buttonWas, buttonJustPressed, buttonJustReleased.
--------------------------------------------------------------------------------------*/
byte ReadButtons()
{
   unsigned int buttonVoltage;
   byte button = BUTTON_NONE;   // return no button pressed if the below checks don't write to btn
   
   //read the button ADC pin voltage
   buttonVoltage = analogRead( BUTTON_ADC_PIN );
   //Serial.println(buttonVoltage); // un comment to print voltage
   //sense if the voltage falls within valid voltage windows
   if( buttonVoltage < ( RIGHT_10BIT_ADC + BUTTONHYSTERESIS ) )
   {
      button = BUTTON_RIGHT;
   }
   else if(   buttonVoltage >= ( UP_10BIT_ADC - BUTTONHYSTERESIS )
           && buttonVoltage <= ( UP_10BIT_ADC + BUTTONHYSTERESIS ) )
   {
      button = BUTTON_UP;
   }
   else if(   buttonVoltage >= ( DOWN_10BIT_ADC - BUTTONHYSTERESIS )
           && buttonVoltage <= ( DOWN_10BIT_ADC + BUTTONHYSTERESIS ) )
   {
      button = BUTTON_DOWN;
   }
   else if(   buttonVoltage >= ( LEFT_10BIT_ADC - BUTTONHYSTERESIS )
           && buttonVoltage <= ( LEFT_10BIT_ADC + BUTTONHYSTERESIS ) )
   {
      button = BUTTON_LEFT;
   }
   else if(   buttonVoltage >= ( SELECT_10BIT_ADC - BUTTONHYSTERESIS )
           && buttonVoltage <= ( SELECT_10BIT_ADC + BUTTONHYSTERESIS ) )
   {
      button = BUTTON_SELECT;
   }
   //handle button flags for just pressed and just released events
   if( ( buttonWas == BUTTON_NONE ) && ( button != BUTTON_NONE ) )
   {
      //the button was just pressed, set buttonJustPressed, this can optionally be used to trigger a once-off action for a button press event
      //it's the duty of the receiver to clear these flags if it wants to detect a new button change event
      buttonJustPressed  = true;
      buttonJustReleased = false;
   }
   if( ( buttonWas != BUTTON_NONE ) && ( button == BUTTON_NONE ) )
   {
      buttonJustPressed  = false;
      buttonJustReleased = true;
   }
   
   //save the latest button value, for change event detection next time round
   buttonWas = button;
   
   return( button );
}
