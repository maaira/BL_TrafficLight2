#include "mbed.h"

InterruptIn button(p5);

DigitalOut red_(p6);
DigitalOut yellow_(p7);
DigitalOut green_(p8); // Representado pelo led azul

Ticker ticker;
Timeout timeout;

enum States
{
    OFF,
    RED,
    YELLOW,
    GREEN,
    ALLERT,
};

States state = RED;
States state_ = RED;

float button_timer = 0;
bool press = false;

//Trafic Light
void red();
void yellow();
void green();

//Button
void start_button();
void end_button();

void off();
void allert();
void revert_state();
void reset();

//Trafic Light
void red()
{
    reset();
    printf("State : Red\n");
    printf("__________________________________________________\n");
    red_ = 1;
    state = RED;
    timeout.attach(&green, 10);
}

void yellow()
{
    reset();
    printf("State : Yellow\n");
    printf("__________________________________________________\n");
    yellow_ = 1;
    state = YELLOW;
    timeout.attach(&red, 4);
}

void green()
{
    reset();
    printf("State : Green\n");
    printf("__________________________________________________\n");
    green_ = 1;
    state = GREEN;
    timeout.attach(&yellow, 20);
}

void blink()
{
    yellow_ = !yellow_;
}

//button
void start_button()
{
    button_timer = 0;
    press = true;
}

void end_button()
{
    printf("Botão pressionado por %.3fs\n----\n", button_timer);
    if (button_timer <= 3 && state == GREEN)
        yellow();
    else if (button_timer <= 10)
    {
	if (state == ALLERT) revert_state();
	else allert();
        
    }
    else
    {
	if (state == ALLERT) red();
	else off();
    }
    press = false;
}



void off()
{
    reset();
    printf("State : Off\n");
    printf("__________________________________________________\n");
    state = OFF;
}

void allert()
{
    reset();
    printf("State : Allert\n");
    printf("__________________________________________________\n");
    state_ = state;
    state = ALLERT;
    ticker.attach(&blink, 0.5);
}

void revert_state()
{
    reset();
    switch (state_)
    {
    case OFF:
        off();
        break;
    case RED:
        red();
        break;
    case YELLOW:
        yellow();
        break;
    case GREEN:
        green();
        break;
    default:
        break;
    }
}

void reset()
{
    red_ = 0;
    yellow_ = 0;
    green_ = 0;
    ticker.detach();
    timeout.detach();
}

int main()
{
    red();
    button.rise(&start_button);
    button.fall(&end_button);
    while (1)
    {
        if (press)
            button_timer += 0.1;
        wait(0.2);
    }
}
