/*
* to run on an arduino
*
* From Jason Bellenger
* The only tricky bit of implementation is transforming the wave
* functions such that their value at the current time isn't changed by
* the real-time changing of their period and duty cycle.  The part that
* seemed to make life a lot simpler with this overall algorithm was
* abandoning a schedule-driven method for opening and closing valves in
* favor of sampling square wave compositions.
*/

// Heart Machine valve control prototype.
// Child of pyropulse3 and mustang2...

// the macro sets or clears the appropriate bit in port D if the pin
is less than 8 or port B if between 8 and 13
#define fastWrite(_pin_, _state_) ( _pin_ < 8 ? (_state_ ?  PORTD |= 1
<< _pin_ : PORTD &= ~(1 << _pin_ )) : (_state_ ?  PORTB |= 1 << (_pin_
-8) : PORTB &= ~(1 << (_pin_ -8)  )))

//#define DEBUG_SERIAL 1

// slight expansion of the normal human range
// see setup for the other magic numbers
int minBpm = 40;
int maxBpm = 190;

// hardware
int leftPotPin = 0;
int rightPotPin = 1;
int v1Pin = 7;
int v2Pin = 12;
int ledPin = 13;
float potMax = 894.0f;

// the key primitive
struct SquareWave
{
 SquareWave()
 : phase( 0.0 )
 , period( 0 )
 , on( 0 )
 {}

 void pivot( long time, long newPeriod, long newOn )
 {
#ifdef DEBUG_SERIAL
   Serial.print( "old: phase=" );
   Serial.print( phase );
   Serial.print( " period=" );
   Serial.print( period );
   Serial.print( " on=" );
   Serial.print( on );
   Serial.print( "\n" );
#endif

   long t0 = time % period;
   float p0 = (float)t0 / (float)period;

   long t1 = time % newPeriod;
   float p1 = (float)t1 / (float)newPeriod;

   float offset = p1 - p0;

#ifdef DEBUG_SERIAL
   Serial.print( "offset=" );
   Serial.print( offset );
   Serial.print( "\n" );
#endif

   phase += offset;
   while( phase >= 1.0f )
     phase -= 1.0f;
   while( phase < 0.0f )
     phase += 1.0f;

   period = newPeriod;
   on = newOn;

#ifdef DEBUG_SERIAL
   Serial.print( "new: phase=" );
   Serial.print( phase );
   Serial.print( " period=" );
   Serial.print( period );
   Serial.print( " on=" );
   Serial.print( on );
   Serial.print( "\n\n" );
#endif
 }

 bool sample( long time )
 {
   time -= (phase * period);
   long t = time % period;
   return t < on;
 }

 float phase;
 long period;
 long on;
};

void readParams( long& time, long& period, long& on )
{
 float left = 1.0f - (analogRead(leftPotPin) / potMax);
 float right = 1.0f - (analogRead(rightPotPin) / potMax);

#if 0
 // POT test
 Serial.print( "L=" );
 Serial.print( left );
 Serial.print( " R=" );
 Serial.print( right );
 Serial.print( "\n " );
#endif

 int bpm = (minBpm + (maxBpm-minBpm) * left);

 period = 60000 / bpm;
 on = period * right;
 time = millis();
}


// valve 1 (systolic), valve 2 (diastolic) and bonus vibrato for 2
SquareWave v1;
SquareWave v2;
SquareWave vibe;

void setup()
{
 pinMode( ledPin, OUTPUT );
 pinMode( v1Pin, OUTPUT );
 pinMode( v2Pin, OUTPUT );

 long time, period, on;
 readParams( time, period, on );

 v1.period = period;
 v2.period = period;

 v1.on = on;
 v2.on = on;

 v1.phase = 0.0f;
 v2.phase = 0.25f;

 // these could vary given more pots but are constrained
 // at the low end by the open time and close time of
 // the valves we use
 vibe.period = period / 4;
 vibe.on = vibe.period / 2;
 vibe.phase = 0.0f;

#ifdef DEBUG_SERIAL
 Serial.begin( 9600 );
 Serial.flush();
#endif
}

void loop ()
{
 long time, period, on;
 readParams( time, period, on );

 v1.pivot( time, period, on );
 v2.pivot( time, period, on );
 vibe.pivot( time, period / 4, period / 8 );

 bool fire1 = v1.sample( time );
 bool fire2 = v2.sample( time ) && vibe.sample( time );

 if( fire1 )
   fastWrite( v1Pin, HIGH );
 else
   fastWrite( v1Pin, LOW );

 if( fire2 )
   fastWrite( v2Pin, HIGH );
 else
   fastWrite( v2Pin, LOW );

 if( fire2 )
   fastWrite( ledPin, HIGH );
 else
   fastWrite( ledPin, LOW );

#ifdef DEBUG_SERIAL
 delay( 500 );
#endif
}