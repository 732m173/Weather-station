
void do_send(osjob_t* j){
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        LectureCapteurs();
        unsigned char mydata[6];
        byte tempLow = lowByte(payloadTemp);
        byte tempHigh = highByte(payloadTemp);
        mydata[0] = lowByte(payloadTemp);
        mydata[1] = highByte(payloadTemp);
        for(byte i = 0; i < 4; i++){
        Serial.println(temp1.c[i],HEX);
          mydata[i] = temp1.c[i];
        }
        for(byte i = 0; i < 2; i++){
        Serial.println(temp1.c[i],HEX);
          mydata[i+4] = vitVent.c[i];
        }
        LMIC_setTxData2(1, mydata, sizeof(mydata), 0);
        Serial.println(F("Packet queued"));
    }
}
