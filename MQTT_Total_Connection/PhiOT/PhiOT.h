/*
  PhiOT.h - Library for flashing PhiOT code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#ifndef PhiOT_h
#define PhiOT_h



class PhiOT
{
  public:
    PhiOT(String token);
    void lightIndicator();
    void testPrivate();
    void setup_wifi();
    void phiLoop();
    void callback(char* topic, byte* payload, unsigned int length);
    void reconnect();
  private:
    int _pin;
    String _token;
};

#endif

