#ifndef Motors_h
#define Motors_h

#include <Potentiometer.h>
#include <Servo.h>

#define PIN_SERVO_1 9
#define PIN_SERVO_2 10
#define PIN_SERVO_3 11
#define LIST_SIZE 50

extern const int pins_servo[servo_count],
    min_pwm[servo_count],
    max_pwm[servo_count],
    movement_frame_time,
    play_interval;
extern int home_position[servo_count],
           signal[servo_count],
           velocity;

class Motors
{
public:
    int update_position_interval;
    int position_index;
    Motors();
    void update_position();
    void save_position();
    void play(int cnt);
    void reset();

private:
    Potentiometer _pot;
    Servo _servos[servo_count];
    int _index, _pos_cnt;
    int _positions[LIST_SIZE][servo_count];
    int _diff[servo_count],
        _max_steps,
        _cnt,
        _steps[servo_count],
        _rem[servo_count];
    int _cur[servo_count];
};

#endif