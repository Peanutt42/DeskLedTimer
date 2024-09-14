#include <LedControl.h>

#define TOUCH_SENSOR_PIN 7

#define BUZZ_PIN 4

#define DIN_PIN 11
#define CLK_PIN 13
#define CS_PIN 6

LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN);
bool clock_ticking = false;
unsigned long long clock_start = 0;

bool last_touch_state = false;
unsigned long long touch_start = 0;
#define LONG_TOUCH_DURATION_MILLIS 1000 // 1s

enum Timer {
  Learn30,
  Pause5,
  EarlGrey,
  _COUNT
} timer = Timer::Learn30;

unsigned long long get_timer_duration(Timer& timer) {
  switch (timer) {
    case Timer::Learn30: return 1800000; //(30 * 60 * 1000)
    case Timer::Pause5: return 300000; //(5 * 60 * 1000)
    case Timer::EarlGrey: return 180000; //(3 * 60 * 1000)
  }
}

// generated with python script: "python generate_clock_led_percentages.py"
double clock_led_percentages[8][8] = {
	{ 0.125, 0.09871577164437327, 0.0644405292045783, 0.02258361765043329, 0.9774163823495667, 0.9355594707954217, 0.9012842283556267, 0.875, },
	{ 0.15128422835562672, 0.125, 0.08601043481131533, 0.03141647909450054, 0.9685835209054995, 0.9139895651886847, 0.875, 0.8487157716443733, },
	{ 0.1855594707954217, 0.16398956518868468, 0.125, 0.051208191174783355, 0.9487918088252166, 0.875, 0.8360104348113153, 0.8144405292045783, },
	{ 0.2274163823495667, 0.21858352090549937, 0.1987918088252166, 0.125, 0.875, 0.8012081911747834, 0.7814164790945005, 0.7725836176504333, },
	{ 0.27258361765043326, 0.28141647909450057, 0.30120819117478337, 0.375, 0.625, 0.6987918088252166, 0.7185835209054995, 0.7274163823495667, },
	{ 0.3144405292045783, 0.3360104348113153, 0.375, 0.44879180882521663, 0.5512081911747834, 0.625, 0.6639895651886847, 0.6855594707954217, },
	{ 0.3487157716443733, 0.375, 0.4139895651886847, 0.4685835209054994, 0.5314164790945006, 0.5860104348113153, 0.625, 0.6512842283556267, },
	{ 0.375, 0.4012842283556267, 0.4355594707954217, 0.47741638234956674, 0.5225836176504333, 0.5644405292045783, 0.5987157716443733, 0.625, },
};

void setup() {
  pinMode(TOUCH_SENSOR_PIN, INPUT_PULLUP);

  pinMode(BUZZ_PIN, OUTPUT);

  lc.shutdown(0, false);
  lc.setIntensity(0, 3); // 0..15
  lc.clearDisplay(0);
}

void buzz() {
  tone(BUZZ_PIN, 300, 150);
}

void start_clock() {
  clock_ticking = true;
  clock_start = millis();
  lc.clearDisplay(0);
  buzz();
}

void stop_clock() {
  clock_ticking = false;
  lc.clearDisplay(0);
  buzz();
}

void set_timer(Timer new_timer) {
  timer = new_timer;
  lc.clearDisplay(0);
}

void check_touch_sensor() {
  bool touch_state = digitalRead(TOUCH_SENSOR_PIN);

  // pressed
  if (!last_touch_state && touch_state) {
    touch_start = millis();
    if (clock_ticking) {
      stop_clock();
    }
  }

  // released
  if (touch_state && !clock_ticking) {
    if ((millis() - touch_start) >= LONG_TOUCH_DURATION_MILLIS) {
      start_clock();
    }
  }

  if (!clock_ticking && last_touch_state && !touch_state) {
    set_timer(Timer((int)timer + 1));
    if (timer == Timer::_COUNT) {
      set_timer(Timer(0));
    }
  }  

  last_touch_state = touch_state;
}

void loop() {
  check_touch_sensor();

  if (clock_ticking) {
    unsigned long long now = millis();
    unsigned long long duration = now - clock_start;
    unsigned long long timer_duration = get_timer_duration(timer);
    if (duration >= timer_duration) {
      stop_clock();
      return;
    }

    double clock_percentage = (double)duration / (double)timer_duration;
    for (int row = 0; row < 8; row++) {
      for (int column = 0; column < 8; column++) {
        bool led_state = clock_percentage < clock_led_percentages[row][column];
        lc.setLed(0, row, column, led_state);
      }
    }
  }
  else {
    switch (timer) {
      case Timer::Learn30:
        lc.setLed(0, 1, 0, true);
        lc.setLed(0, 1, 1, true);
        lc.setLed(0, 1, 2, true);
        lc.setLed(0, 1, 4, true);
        lc.setLed(0, 1, 5, true);
        lc.setLed(0, 1, 6, true);

        lc.setLed(0, 2, 2, true);
        lc.setLed(0, 2, 4, true);
        lc.setLed(0, 2, 6, true);

        lc.setLed(0, 3, 1, true);
        lc.setLed(0, 3, 2, true);
        lc.setLed(0, 3, 4, true);
        lc.setLed(0, 3, 6, true);

        lc.setLed(0, 4, 2, true);
        lc.setLed(0, 4, 4, true);
        lc.setLed(0, 4, 6, true);

        lc.setLed(0, 5, 0, true);
        lc.setLed(0, 5, 1, true);
        lc.setLed(0, 5, 2, true);
        lc.setLed(0, 5, 4, true);
        lc.setLed(0, 5, 5, true);
        lc.setLed(0, 5, 6, true);
        break;

      case Timer::Pause5:
        lc.setLed(0, 0, 1, true);
        lc.setLed(0, 0, 2, true);
        lc.setLed(0, 0, 3, true);
        lc.setLed(0, 0, 4, true);
        lc.setLed(0, 0, 5, true);

        lc.setLed(0, 1, 1, true);

        lc.setLed(0, 2, 1, true);

        lc.setLed(0, 3, 1, true);
        lc.setLed(0, 3, 2, true);
        lc.setLed(0, 3, 3, true);
        lc.setLed(0, 3, 4, true);
        lc.setLed(0, 3, 5, true);

        lc.setLed(0, 4, 5, true);
        
        lc.setLed(0, 5, 5, true);

        lc.setLed(0, 6, 1, true);
        lc.setLed(0, 6, 2, true);
        lc.setLed(0, 6, 3, true);
        lc.setLed(0, 6, 4, true);
        lc.setLed(0, 6, 5, true);
        break;

      case Timer::EarlGrey:
        lc.setLed(0, 0, 0, true);
        lc.setLed(0, 0, 1, true);
        lc.setLed(0, 0, 2, true);
        lc.setLed(0, 0, 3, true);
        lc.setLed(0, 0, 4, true);
        lc.setLed(0, 0, 5, true);
        lc.setLed(0, 0, 6, true);
        lc.setLed(0, 0, 7, true);
        lc.setLed(0, 1, 0, true);
        lc.setLed(0, 1, 1, true);
        lc.setLed(0, 1, 2, true);
        lc.setLed(0, 1, 3, true);
        lc.setLed(0, 1, 4, true);
        lc.setLed(0, 1, 5, true);
        lc.setLed(0, 1, 6, true);
        lc.setLed(0, 1, 7, true);

        lc.setLed(0, 2, 3, true);
        lc.setLed(0, 2, 4, true);

        lc.setLed(0, 3, 3, true);
        lc.setLed(0, 3, 4, true);
        
        lc.setLed(0, 4, 3, true);
        lc.setLed(0, 4, 4, true);

        lc.setLed(0, 5, 3, true);
        lc.setLed(0, 5, 4, true);

        lc.setLed(0, 6, 3, true);
        lc.setLed(0, 6, 4, true);

        lc.setLed(0, 7, 3, true);
        lc.setLed(0, 7, 4, true);
        break;
    }
  }
}
