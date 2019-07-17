#include <Keyboard.h>
#include <EEPROM.h>

// I built the matrix wrong, so we gotta reverse the rows and columns. oops.

//  

#define kNRows (3)
unsigned int rowPins[kNRows] = { 7, 8, 9 };

#define kNCols (7)
unsigned int colPins[kNCols] = { A3, A2, A1, A0, 15, 14, 16 };

unsigned int keyCodes[kNRows][kNCols] = {
  { 'v',  '+',  '7',  '8',  '9',  'c',  'e' },
  { 'n',  '-',  '4',  '5',  '6',  'p',  'r' },
  { ' ',  '0',  '1',  '2',  '3',  'k',  ' ' },
};

unsigned char keyStates[kNRows][kNCols];

void setup() {
  int i;

  ///Serial.begin( 115200 );
  //while ( !Serial );

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
  Keyboard.releaseAll();
  
  for( int r=0 ; r<kNRows ; r++ ) {
    for( int c=0 ; c<kNCols ; c++ ) { 
      keyStates[r][c] = 0;
    }
  }
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

      if( val != keyStates[r][c] ) {
        // changed!
        if( val ) {
          // was just released
          Keyboard.release( keyCodes[r][c] );
        } else { 
          // was just pressed
          Keyboard.press( keyCodes[r][c] );
        }
        keyStates[r][c] = val;
      }
    }
  }
}

void loop() {
  ReadKeys();
}
