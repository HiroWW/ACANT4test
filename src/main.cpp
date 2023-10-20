// LoopBackDemo for Teensy 4.x CAN1

// The FlexCAN module is configured in loop back mode:
//   it internally receives every CAN frame it sends.

// No external hardware required.

//-----------------------------------------------------------------

#ifndef __IMXRT1062__
    #error "This sketch should be compiled for Teensy 4.x"
#endif

//-----------------------------------------------------------------

#include <Arduino.h>
#include <ACAN_T4.h>

//-----------------------------------------------------------------

void setup () {
    pinMode (LED_BUILTIN, OUTPUT) ;
    Serial.begin (9600) ;
    while (!Serial) {
        delay (50) ;
        digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    }
    Serial.println ("CAN1 - CAN2 loopback test") ;
    ACAN_T4_Settings settings (125 * 1000) ; // 125 kbit/s
    // settings.mLoopBackMode = true ;
    // settings.mSelfReceptionMode = true ;
    // settings.mReceiveBufferSize = 4;
    // settings.mTransmitBufferSize = 64;
    const uint32_t errorCode = ACAN_T4::can1.begin (settings) ;
    const uint32_t errorCode2 = ACAN_T4::can2.begin(settings) ;
    Serial.print ("Bitrate prescaler: ") ;
    Serial.println (settings.mBitRatePrescaler) ;
    Serial.print ("Propagation Segment: ") ;
    Serial.println (settings.mPropagationSegment) ;
    Serial.print ("Phase segment 1: ") ;
    Serial.println (settings.mPhaseSegment1) ;
    Serial.print ("Phase segment 2: ") ;
    Serial.println (settings.mPhaseSegment2) ;
    Serial.print ("RJW: ") ;
    Serial.println (settings.mRJW) ;
    Serial.print ("Triple Sampling: ") ;
    Serial.println (settings.mTripleSampling ? "yes" : "no") ;
    Serial.print ("Actual bitrate: ") ;
    Serial.print (settings.actualBitRate ()) ;
    Serial.println (" bit/s") ;
    Serial.print ("Exact bitrate ? ") ;
    Serial.println (settings.exactBitRate () ? "yes" : "no") ;
    Serial.print ("Distance from wished bitrate: ") ;
    Serial.print (settings.ppmFromWishedBitRate ()) ;
    Serial.println (" ppm") ;
    Serial.print ("Sample point: ") ;
    Serial.print (settings.samplePointFromBitStart ()) ;
    Serial.println ("%") ;
    if (0 == errorCode) {
        Serial.println ("can1 ok") ;
    }else{
        Serial.print ("Error can1: 0x") ;
        Serial.println (errorCode, HEX) ;
        while (1) {
        delay (100) ;
        Serial.println ("Invalid setting") ;
        digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
        }
    }
}

//-----------------------------------------------------------------

uint64_t cnt = 0;
//-----------------------------------------------------------------
void handle_mymessage(const CANMessage & inMessage){
    Serial.println(inMessage.data64);
}
void loop () {
    // if (gBlinkDate <= millis ()) {
    //     gBlinkDate += 500 ;
    //     digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    // }
    CANMessage message ;

    // //* --------------SEND-----------------
    // // while(true){
    message.id = cnt ;
    message.len = 8;
    message.data[0] = cnt;
    message.data[1] = cnt + 1;
    message.data[3] = 123;
    const bool ok = ACAN_T4::can1.tryToSend (message) ;
    if (ok) {
        Serial.print ("Sent: ") ;
        Serial.println (message.data64) ;
    }
    if (cnt == 63){
        cnt =0;
        Serial.println("reset !!");
    }
    cnt++ ;
    delay(500);
    // //* -------RECEIVE-------
    if (ACAN_T4::can2.receive (message)) {
        // gReceivedCount += 1 ;
        Serial.print("ID :");
        Serial.println(message.id);
        Serial.print ("Received Data : ") ;
        // int tmp = message.data[];
        // Serial.println (message.data64) ;
        handle_mymessage(message);
        Serial.println(message.rtr);
        // Serial.print("count is ");
        // Serial.println(gReceivedCount);
    }
}

