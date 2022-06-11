#include <Button.h>
//#include <helpers.h>
//#include <Motors.h>
//#include <Potentiometer.h>
//#include <PrintStream.h>

#include <Button.h>

Button * button;

void setup()
{
    Serial.begin(9600);
    button = new Button;
}

void loop()
{
    Serial.println(button->is_pressed());    
}
