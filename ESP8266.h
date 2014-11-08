#ifndef _ESP8266_H_
#define _ESP8266_H_
#define _TIMEOUT 3000
#define _RECV_TIMEOUT 500


#define UDP 0
#define TCP 1

#define OFF 0
#define ON  1

class ESP8266
{
private:
    bool transparency;
    bool waitOK();

public:
    // struct apConfig
    // {
    //     String name;
    //     String password;
    //     byte channel;
    //     byte encryption;
    // };
    struct connList
    {
        byte id;
        String protocol;
        String ip;
        unsigned int port;
        bool cs;
    };
    struct connData
    {
        byte id;
        byte length;
        String data;
    };
    struct connList conn[4];        //Group of connected information
    byte connNum;    //Current connected number

    ESP8266();

    /*Standard Command*/
    bool ATE(bool n);
    bool Test();
    bool SoftReset();
    String GetVersion();
    /*=========================*/

    /*WiFi Command*/
    byte GetWIFIMode();
    bool SetWIFIMode(byte mode);
    bool SetJoinAP(String ssid, String pwd);
    bool SetQuitAP();
    bool GetAPConfig(String *ssid, String *password, byte *channel, byte *encryption);
    bool SetAPConfig(String ssid, String password, byte channel, byte encryption);
    String GetCurrentAP();
    String GetAPList();
    String GetIFList();
    /*=========================*/

    /*IP Command*/
    bool SetIPMode(bool on_off);
    bool SetIPMux(bool on_off);
    bool SetIPServer(unsigned int port);
    bool SendIPData(String data);
    bool SendIPData(byte id, String data);
    bool SetIPConnection(bool protocol, String address, unsigned int port);
    bool SetIPConnection(byte id, bool protocol, String address, unsigned int port);
    bool SetIPTimeout(unsigned int time);
    byte GetIPStatus();
    unsigned int GetIPTimeout();
    String GetIPList();
    /*-------------------------*/
    void StopTransmission();
    byte WaitConnect();
    String GetIPData(byte *id, byte *length);
    /*=========================*/

};

#endif
