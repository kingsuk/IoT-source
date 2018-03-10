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
    void testPrivate();
    void Initialize();
    void phiLoop();
    void callback(char* topic, byte* payload, unsigned int length);
    void reconnect();
    void mqttInit();
    void CheckingConnectionStatusWithDelay();
    void setAccessPoint();
    void SwitchOffAccessPoint();
    void ServerRouters();
    void WifiConnectionSetup(String ssid, String password);
    void lightIndicatorConfirmation();
    void indicatorForAccessPoint();
  private:
    int _pin;
    String _token;
};

#endif

