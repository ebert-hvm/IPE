#include <Motors.h>
#include <Button.h>


int button_timer,
    position_timer,
    play_timer,
    cur_timer,
    cur_state,
    play_cnt;

Motors *motors;
Button *button;

void setup()
{
    Serial.begin(9600);
    button = new Button;
    motors = new Motors;

    cur_state = learn;
    button_timer = millis();
    position_timer = millis();
    cur_timer = millis();
    play_timer = millis();
    play_cnt = 0;
}

void loop()
{
    // The Loop function works as a finite state machine
    switch (cur_state)
    {
    case learn:
        // In learn state, it periodically updates servos position and the current
        // state according with the potenciometer readings and button state respectively
        if (cur_timer - position_timer > motors->update_position_interval)
        {
            position_timer = cur_timer;
            motors->update_position();
        }
        if (cur_timer - button_timer > button->update_interval)
        {
            button_timer = cur_timer;
            cur_state = button->update();
        }
        break;
    case play:
    // in play state, it periodically updates the current state according to button state
    // and makes the servos execute one saved move in loop
        if (cur_timer - button_timer > button->update_interval)
        {
            button_timer = cur_timer;
            if (button->is_pressed() or motors->position_index == 0)
            {
                cur_state = learn;
                button_timer = millis() + 1000; // 1s buffer
                Serial.print("Going to learn\n");
                play_cnt = 0;
                break;
            }
        }
        if (cur_timer - play_timer > play_interval)
        {
            Serial.print("Moving ");
            Serial.println(play_cnt);
            motors->play(play_cnt);
            play_timer = millis();
            play_cnt = 1 + play_cnt % (motors->position_index);
        }
        break;
    case reset:
        // This state reset the positions counting and change the current state to learn
        motors->reset();
        Serial.print("Positions reset\n");
        cur_state = learn;
        button_timer = millis() + 1000;
        break;
    case save_position:
        // This state saves the current position and change the current state to learn
        Serial.print("Saving current position...\n");
        motors->save_position();
        cur_state = learn;
        break;
    }
    cur_timer = millis(); // updates time count
}
