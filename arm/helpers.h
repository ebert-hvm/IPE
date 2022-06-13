#ifndef helpers_h
#define helpers_h

typedef enum servo_index
{
    base,
    whrist,
    grip,
    servo_count
} servo_index;

typedef enum state
{
    learn,
    play,
    reset,
    save_position
} state;

#endif