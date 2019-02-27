//#define DEBUG_HARD_SERIAL

#include <MIDI.h>
// #include <name.c>
#include <Bounce.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define LED 12   	                                                              // LED pin on Arduino board
const int channelTX = 1;                                                        //midi tx channel

#define BUTTON_1 1
#define BUTTON_2 2
#define BUTTON_3 3

#define LED1 4
#define LED2 5
#define LED3 6

Bounce button1 = Bounce(1, 5);                                                  // 5 = 5 ms debounce time
Bounce button2 = Bounce(2, 5);                                                  // which is appropriate for good
Bounce button3 = Bounce(3, 5);                                                  // quality mechanical pushbuttons

bool status1 = false;
bool status2 = false;
bool status3 = false;

#ifdef DEBUG_HARD_SERIAL
  #define HWSERIAL Serial2                                                      // pin 9 -RX, 10-TX
  #define SERIAL_SPEED 9600
#endif

void OnNoteOn(byte channel, byte note, byte velocity)                           //62, 110, channel
{
#ifdef DEBUG_HARD_SERIAL
  HWSERIAL.print("midi note on received: ");
  HWSERIAL.print(note); HWSERIAL.print(", "); HWSERIAL.println(velocity);
#endif

  if (note == 51) {status1 = true; digitalWrite(LED1, HIGH);}
  if (note == 52) {status2 = true; digitalWrite(LED2, HIGH);}
  if (note == 53) {status3 = true; digitalWrite(LED3, HIGH);}

#ifdef DEBUG_HARD_SERIAL
  HWSERIAL.print("Status: 1->"); HWSERIAL.print(status1); HWSERIAL.print(", 2->");
  HWSERIAL.print(status2); HWSERIAL.print(", 3->"); HWSERIAL.println(status3);
#endif
}

void OnNoteOff(byte channel, byte note, byte velocity)
{
#ifdef DEBUG_HARD_SERIAL
  HWSERIAL.print("midi note off received: ");
  HWSERIAL.print(note); HWSERIAL.print(", "); HWSERIAL.println(velocity);
#endif

if (note == 51) {status1 = false; digitalWrite(LED1, LOW);}
if (note == 52) {status2 = false; digitalWrite(LED2, LOW);}
if (note == 53) {status3 = false; digitalWrite(LED3, LOW);}

#ifdef DEBUG_HARD_SERIAL
  HWSERIAL.print("Status: 1->"); HWSERIAL.print(status1); HWSERIAL.print(", 2->");
  HWSERIAL.print(status2); HWSERIAL.print(", 3->"); HWSERIAL.println(status3);
#endif
}

void setup() {
  #ifdef DEBUG_HARD_SERIAL
    HWSERIAL.begin(SERIAL_SPEED);
    HWSERIAL.println("hardware serial initialized");
  #endif

  pinMode(LED, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);

  usbMIDI.begin();                                                             	// Launch MIDI with default options
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn) ;

  digitalWrite(LED, HIGH);                                                      // quick test indicator
  delay(200);
  digitalWrite(LED, LOW);
  delay(200);
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);
  delay(200);
  digitalWrite(LED, HIGH);

}

void loop() {
  button1.update();
  button2.update();
  button3.update();

  // Note On messages when each button is pressed
  if (button1.fallingEdge()) {
    #ifdef DEBUG_HARD_SERIAL
      HWSERIAL.println("button 1 pressed");
    #endif

    status1 = !status1;
    if (status1) usbMIDI.sendNoteOn(61, 99, channelTX);
    if (!status1) usbMIDI.sendNoteOff(61, 0, channelTX);

    #ifdef DEBUG_HARD_SERIAL
      HWSERIAL.print("status1: "); HWSERIAL.println(status1);
    #endif
  }

  if (button2.fallingEdge()) {
    #ifdef DEBUG_HARD_SERIAL
      HWSERIAL.println("button 2 pressed");
    #endif

    status2 = !status2;
    if (status2) usbMIDI.sendNoteOn(62, 99, channelTX);
    if (!status2) usbMIDI.sendNoteOff(62, 0, channelTX);

    #ifdef DEBUG_HARD_SERIAL
      HWSERIAL.print("status2: "); HWSERIAL.println(status2);
    #endif
  }

  if (button3.fallingEdge()) {
    #ifdef DEBUG_HARD_SERIAL
      HWSERIAL.println("button 3 pressed");
    #endif

    status3 = !status3;
    if (status3) usbMIDI.sendNoteOn(63, 99, channelTX);
    if (!status3) usbMIDI.sendNoteOff(63, 0, channelTX);

    #ifdef DEBUG_HARD_SERIAL
      HWSERIAL.print("status3: "); HWSERIAL.println(status3);
    #endif
  }


usbMIDI.read();

}
