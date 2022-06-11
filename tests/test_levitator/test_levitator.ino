//#include <PrintStream.h>
byte TP = 0b10101010; // Every other port receives the inverted signal
int t_1, t_0, interval;
void setup()
{
    Serial.begin(115200);
    DDRD = 0b11111111; // Set all analog ports to be outputs

    // Initialize Timer1
    noInterrupts(); // Disable interrupts

    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    OCR2A = 200; // Set compare register (16MHz / 200 = 80kHz square wave -> 40kHz full wave)

    TCCR2B |= (1 << WGM22);  // CTC mode
    TCCR2B |= (1 << CS20);   // Set prescaler to 1 ==> no prescaling
    TIMSK2 |= (1 << OCIE2A); // Enable compare timer interrupt
    interrupts(); // Enable interrupts
    t_0 = micros();
    t_1 = micros();
}
ISR(TIMER2_COMPA_vect)
{
    t_1 = micros();
    interval = t_1 - t_0;

    PORTD = TP; // Send the value of TP to the outputs
    TP = ~TP;   // Invert TP for the next run
    t_0 = micros();
}
void loop()
{
    Serial.println(interval);
    // Nothing left to do here :)
}
