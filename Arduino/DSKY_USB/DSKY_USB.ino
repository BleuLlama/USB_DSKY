// DSKY_USB
//
//  A usb keyboard that can be used as a simulation of sorts of the Apollo
//  Block 2 AGC/DSKY keypad interface.
//
#include <Keyboard.h>
#include <EEPROM.h>

// Versioning info
#define kVERSION  "v1.1 2019-07-18 SDL"
//  1.1 - added key remapping
// 1.0 - initial version
//

// define/undef this to enable/require serial connection to run
#undef kDebugMode

// define to enable/disable keyboard output (for debugging)
#define kKeyboardOutput


// I built the matrix wrong, so we gotta reverse the rows and columns. oops.
//
//  these are wired as such:    (row pin)----(switch)---(diode)----(col pin)
//

// Row info for the keyboard matrix
#define kNRows (3)
unsigned int rowPins[kNRows] = { 7, 8, 9 };

// Column info for the keyboard matrix
#define kNCols (7)
unsigned int colPins[kNCols] = { A3, A2, A1, A0, 15, 14, 16 };

// Default mapping of keys
unsigned int keyCodes[kNRows][kNCols] = {
  { 'v',  '+',  '7',  '8',  '9',  'c',  'e' },
  { 'n',  '-',  '4',  '5',  '6',  'p',  'r' },
  { ' ',  '0',  '1',  '2',  '3',  'k',  ' ' },
};


// missing function keys
//
#define KEY_F13       0x68
#define KEY_F14       0x69
#define KEY_F15       0x6A
#define KEY_F16       0x6B
#define KEY_F17       0x6C
#define KEY_F18       0x6D
#define KEY_F19       0x6E
#define KEY_F20       0x6F
#define KEY_F21       0x70
#define KEY_F22       0x71
#define KEY_F23       0x72
#define KEY_F24       0x73
#define KEY_F24       0x73

// missing sound control keys
//
#define KEY_MUTE          0x7F
#define KEY_VOLUMEUP      0x80
#define KEY_VOLUMEDOWN    0x81

// Numeric keypad
//
#define KEY_KEYPAD_0      0x62
#define KEY_KEYPAD_1      0x59
#define KEY_KEYPAD_2      0x5A
#define KEY_KEYPAD_3      0x5B
#define KEY_KEYPAD_4      0x5C
#define KEY_KEYPAD_5      0x5D
#define KEY_KEYPAD_6      0x5E
#define KEY_KEYPAD_7      0x5F
#define KEY_KEYPAD_8      0x60
#define KEY_KEYPAD_9      0x61
#define KEY_KEYPAD_DECIMAL    0x63
#define KEY_KEYPAD_ENTER    0x58
#define KEY_KEYPAD_PLUS   0x57
#define KEY_KEYPAD_MINUS    0x56
#define KEY_KEYPAD_MULTIPLY   0x55
#define KEY_KEYPAD_DIVIDE   0x54
#define KEY_KEYPAD_NUMLOCK    0x53
#define KEY_KEYPAD_EQUALS   0x67

unsigned int keyCodes9[kNRows][kNCols] = {
  { 'v',  KEY_VOLUMEUP,     '7',              KEY_UP_ARROW,     '9',  'c',  'e' },
  { 'n',  KEY_VOLUMEDOWN,   KEY_LEFT_ARROW,   '5',              KEY_RIGHT_ARROW,  'p',  'r' },
  { ' ',  KEY_MUTE,         '1',              KEY_DOWN_ARROW,   '3',  'k',  ' ' },
};

// which key is our meta key?
//  hold + combine with other keys for extended behaviors
//  will also only send its keypress after 500ms (or if released sooner for 10ms)
#define kMetaKeyR (1)
#define kMetaKeyC (5)
bool metaDown = false;
int metaMode = 0;
int metaModeSame = 0;

// for programming:
int programMode = 0;      // 0 = inactive  1 = get key  2 = get value
int programR, programC;   // current key we're mapping

// mode use
int useBank = 0;

// for led patterns
//  0 - both off
//  1 - both on
//  11 - 100ms bounce
int ledMode = 0;
void LEDMode( int );

// storage for the current state of each key in the loop
unsigned char keyStates[kNRows][kNCols];
unsigned long keyDurations[kNRows][kNCols];

void setup() {

#ifdef kDebugMode
  Serial.begin( 115200 );
  while ( !Serial );  // 32u4/ProMicro/Leonardo needs to wait.
  Serial.println( "Connected to DSKY_USB in debug mode" );
  Serial.println( kVERSION );
#endif

  int i;
  for ( i = 0 ; i < kNRows ; i++ ) {
    pinMode( rowPins[i], INPUT_PULLUP );
  }

  for ( i = 0 ; i < kNCols ; i++ ) {
    pinMode( colPins[i], OUTPUT );
  }

  // EEPROM.read( address );
  // EEPROM.write( address, val );

  ResetKeyboard();
}

void ResetKeyboard()
{
#ifdef kKeyboardOutput
  Keyboard.releaseAll();
#endif
#ifdef kDebugMode
  Serial.println( "Release all keys" );
#endif

  for ( int r = 0 ; r < kNRows ; r++ ) {
    for ( int c = 0 ; c < kNCols ; c++ ) {
      keyStates[r][c] = 0;
      keyDurations[r][c] = 0;
    }
  }
  metaDown = false;
  useBank = 0;
}

char GetKeyCodeFor( int r, int c )
{
  if ( useBank == 0 ) {
    return keyCodes[r][c];
  }

/*
  if ( useBank == 9 ) {
    return keyCodes9[r][c];
  }
  */

  int eepromOffset = (useBank * 20) + (r * 7) + c;
  return EEPROM.read( eepromOffset );
}

char prgBuf[ 4 ];


void MetaPressed( int r, int c )
{
  int baseCode = keyCodes[r][c];

  // meta+number = change key bank code
  if ( baseCode >= '0' && baseCode <= '9' ) {
    useBank = baseCode - '0';
    return;
  }

  switch ( baseCode ) {
    case ( 'r' ):
      if ( metaModeSame == 3 ) {
        LEDMode( 1 );
        for ( int b = 1 ; b < 10 ; b++ ) {
          for ( int i = 0 ; i < 20; i++) {
            EEPROM.write( (b * 20) + i, 49 + b - 1 );
          }
        }
      }
      ResetKeyboard();
      // fudge some stuff
      metaDown = true;
      metaMode = baseCode;

      LEDMode( 0 );
      break;

    /*
    case ( '+' ):
      metaMode = 255;
      break;
    */

    case ( 'k' ):
      LEDMode( 1 );
      programMode = 1;
      prgBuf[0] = '0';
      prgBuf[1] = '0';
      prgBuf[2] = '0';
      prgBuf[3] = '\0';
      break;

    default:
      break;
  }
}


  typedef struct
  {
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keys[6];
  } xKeyReport;

  KeyReport  _keyReport;
  
void sendReport(KeyReport* keys)
{
  HID().SendReport(2,keys,sizeof(KeyReport));
}

size_t kPress(uint8_t k)
{
  uint8_t i;
  for (i = 0; i < 6; i++) {
      _keyReport.keys[i] = 0x00;
    }
    
  _keyReport.keys[0] = k;
  sendReport(&_keyReport);
  return 1;
}


// program mode:
//  0: get r/c for a key
//  1: get 3 digit code (roll digits through a 3 byte array
//  2: if 'c' clear, then exit to run mode
//  3: if 'e' enter, then store value to eeprom

void KeyPressed( int r, int c )
{
  int baseCode = keyCodes[r][c];

  if ( programMode == 1 ) {
    programR = r;
    programC = c;
    programMode = 2;
#ifdef kDebugMode
    Serial.print( "Reprogramming code: Bank=" );
    Serial.print( useBank );
    Serial.print( "  Key=" );
    Serial.print( programR );
    Serial.print( ", " );
    Serial.println( programC );
#endif
    return;
  }

  if ( programMode == 2 ) {
#ifdef kDebugMode
    Serial.println( prgBuf );
#endif
    if ( baseCode == 'c' ) {
#ifdef kDebugMode
      Serial.println( "Exiting program mode." );
#endif
      programMode = 0;
      return;
    }

    if ( baseCode == 'e' ) {
      programMode = 0;

      if ( useBank > 0 ) {
#ifdef kDebugMode
        Serial.println( "Accepting new character code" );
        Serial.println( prgBuf );
#endif

        int eepromOffset = (useBank * 20) + (programR * 7) + programC;
        EEPROM.write( eepromOffset, atoi( prgBuf ));
      }
      return;
    }

    if ( baseCode >= '0' && baseCode <= '9' ) {
      prgBuf[0] = prgBuf[1];
      prgBuf[1] = prgBuf[2];
      prgBuf[2] = baseCode;
#ifdef kDebugMode
      Serial.print( "Current value: " );
      Serial.println( prgBuf );
#endif
    }

    return; // don't process the key.
  }


  unsigned char keyCode = GetKeyCodeFor( r, c );

  keyDurations[r][c] = millis();  // start time

  // meta key
  if ( r == kMetaKeyR && c == kMetaKeyC ) {
    metaDown = true;
    metaMode = 0;
    metaModeSame = 0;

    LEDMode( 11 );
#ifdef kDebugMode
    Serial.println( "Meta press" );
#endif

  } else {
    if ( metaDown ) {
      if ( metaMode == keyCode ) {
        metaModeSame++;
      }
      metaMode = keyCode;
      MetaPressed( r, c );

    } else {
#ifdef kKeyboardOutput
      //kPress( keyCode ); // for multimedia keys
      Keyboard.press( keyCode );
#endif
#ifdef kDebugMode
      Serial.print( "Press " );
      Serial.println( keyCode );
#endif
    }
  }
}

void KeyReleased( int r, int c )
{
  unsigned char keyCode = GetKeyCodeFor( r, c );

  keyDurations[r][c] = millis() - keyDurations[r][c]; // duration it was pressed for

  // meta key
  if ( r == kMetaKeyR && c == kMetaKeyC && metaDown ) {
    if ( !metaMode ) {
      // if the meta key wasn't consumed by another, send the keycode
      Keyboard.press( keyCode );
      delay( 20 );
      Keyboard.release( keyCode );
    }
    metaDown = false;
    metaModeSame = 0;

    LEDMode( 0 );
  } else {
#ifdef kKeyboardOutput
    Keyboard.release( GetKeyCodeFor( r, c ) );
#endif
#ifdef kDebugMode
    Serial.print( "Release " );
    Serial.print( GetKeyCodeFor( r, c ) );
    Serial.print( " after " );
    Serial.print( keyDurations[r][c] );
    Serial.println( "ms" );
#endif
  }
}


void KeyIsBeingPressed( int r, int c )
{
}


void ReadKeys()
{
  for ( int c = 0 ; c < kNCols ; c++ )
  {
    for ( int c2 = 0 ; c2 < kNCols ; c2++ ) {
      digitalWrite( colPins[ c2 ], (c2 == c) ? LOW : HIGH );
    }

    for ( int r = 0 ; r < kNRows ; r++ )
    {
      unsigned char val = digitalRead( rowPins[ r ] );

      if ( !val ) {
        // is it being pressed?
        KeyIsBeingPressed( r, c );
      }

      if ( val != keyStates[r][c] ) {
        // changed!
        if ( val ) {
          // was just released
          KeyReleased( r, c );
        } else {
          // was just pressed
          KeyPressed( r, c );
          delay(50); // cheezo-debounce.
        }
        keyStates[r][c] = val;
      }
    }
  }
}


void LEDPoll() {
  static unsigned long timeout = 0;
  static char state = 0;
  static char lastMode = -1;

  switch ( ledMode ) {
    case ( 0 ):
      if ( lastMode != ledMode ) {
        RXLED0;
        TXLED0;
      }
      break;

    case ( 1 ):
      if ( lastMode != ledMode ) {
        RXLED1;
        TXLED1;
      }
      break;

    case ( 11 ):
      if ( millis() > timeout ) {
        if ( state ) {
          state = 0;
          RXLED0;
          TXLED1;
        } else {
          state = 1;
          RXLED1;
          TXLED0;
        }
        timeout = millis() + 100;
      }
      break;
  }
  lastMode = ledMode;
}

void LEDMode( int m ) {
  ledMode = m;
  LEDPoll();
}


void loop() {
  ReadKeys();
  LEDPoll();
}
