#include <Arduino.h>
#include "ESP8266.h"

ESP8266::ESP8266()
{
    Serial3.begin(115200);
    // Serial.begin(115200);
    while (!Serial3);
}

bool ESP8266::waitOK()
{
    unsigned long time = millis();
    while (millis() - time < _TIMEOUT)
    {
        if (Serial3.find("OK"))
        {
            while (Serial3.read() > 0);  //Clear buffer
            return true;
        }
    }
    while (Serial3.read() > 0);  //Clear buffer
    return false;

    // delay(500);
    // if (Serial3.find("OK"))
    //     return true;
    // else
    //     return false;
}

bool ESP8266::ATE(bool n)
{
    switch (n)
    {
    case 0:
        Serial3.println("ATE0");
        break;
    case 1:
        Serial3.println("ATE1");
        break;
    default:
        return false;
    }
    Serial3.flush();

    return waitOK();
}

bool ESP8266::Test()
{
    Serial3.println("AT");
    Serial3.flush();

    return waitOK();
}

bool ESP8266::SoftReset()
{
    Serial3.println("AT+RST");
    Serial3.flush();

    unsigned long time = millis();
    while (millis() - time < _TIMEOUT)
    {
        //if (Serial3.find("Ready"))    //For v0018000902 ECHO:[System Ready, Vendor:www.ai-thinker.com]
		if (Serial3.find("ready"))	  //For v0018000902-AI03 ECHO:[Vendor:www.ai-thinker.com Version:0.9.2.4] ready
        {
            // while (Serial3.read() > 0);  //Clear buffer
            return true;
        }
    }
    while (Serial3.read() > 0);  //Clear buffer
    return false;
}

String ESP8266::GetVersion()
{
    Serial3.println("AT+GMR");
    Serial3.flush();

    if (Serial3.find("\r\n"))
    {
        return Serial3.readStringUntil('\r');
    }
    else
        return "";
}

byte ESP8266::GetWIFIMode()
{
    Serial3.println("AT+CWMODE?");
    Serial3.flush();

    if (Serial3.find(":"))
    {
        return Serial3.parseInt();
    }
    return 0;
}

bool ESP8266::SetWIFIMode(byte mode)
{
    switch (mode)
    {
    case 1:
        Serial3.println("AT+CWMODE=1");
        break;
    case 2:
        Serial3.println("AT+CWMODE=2");
        break;
    case 3:
        Serial3.println("AT+CWMODE=3");
        break;
    default:
        return false;
    }
    Serial3.flush();

    return waitOK();
}

String ESP8266::GetAPList()
{
    String apList = "";
    Serial3.println("AT+CWLAP");
    Serial3.flush();

    while (1)
    {
        apList += Serial3.readString();
        if (apList.indexOf("OK") != -1 || apList.indexOf("ERROR") != -1)
            break;
    }

    return apList;
}

//TODO: Need Update
String ESP8266::GetIFList()
{
    String ifList = "";
    Serial3.println("AT+CWLIF");
    Serial3.flush();

    if (Serial3.find("\r\n"))
        return Serial3.readStringUntil('O');
    else
        return "";
}


bool ESP8266::SetJoinAP(String ssid, String pwd)       //For v0018000902
{
    String cmd = "\"" + ssid + "\"+\"" + pwd + "\"";
    Serial3.println("AT+CWJAP=" + cmd);
    Serial3.flush();

    return waitOK();
}

String ESP8266::GetCurrentAP()
{
    Serial3.println("AT+CWJAP?");
    Serial3.flush();

    if (Serial3.find(":\""))
        return Serial3.readStringUntil('\"');
    else
        return "";
}

bool ESP8266::SetQuitAP()
{
    Serial3.println("AT+CWQAP");
    Serial3.flush();

    return waitOK();
}

bool ESP8266::GetAPConfig(String *ssid, String *password, byte *channel, byte *encryption)
{
    Serial3.println("AT+CWSAP?");
    Serial3.flush();

    if (Serial3.find("+CWSAP:\""))
    {
        *ssid += Serial3.readStringUntil('\"');
        Serial3.find(",\"");
        *password += Serial3.readStringUntil('\"');
        *channel = Serial3.parseInt();
        *encryption = Serial3.parseInt();
        return true;
    }
    else
        return false;
}

bool ESP8266::SetAPConfig(String ssid, String password, byte channel, byte encryption)
{
    String cmd = "\"" + ssid + "\",\"" + password + "\"," + channel + "," + encryption;
    Serial3.println("AT+CWSAP=" + cmd);
    Serial3.flush();
    return waitOK();
}

String ESP8266::GetIPList()
{
    String ipList = "";
    Serial3.println("AT+CIFSR");
    Serial3.flush();

    while (1)
    {
        ipList += Serial3.readString();
        if (ipList.indexOf("OK") != -1 || ipList.indexOf("ERROR") != -1)
            break;
    }
    return ipList;
}

//TODO: 关闭时如果存在连接会"link is builded"
bool ESP8266::SetIPMux(bool on_off)
{
    Serial3.print("AT+CIPMUX=");
    Serial3.println(on_off);
    Serial3.flush();

    return waitOK();
}

//TODO: no change
//!: Restart model to close server
bool ESP8266::SetIPServer(unsigned int port)
{
    Serial3.print("AT+CIPSERVER=1,");
    Serial3.println(port);
    Serial3.flush();

    return waitOK();
}

//Mux=0
bool ESP8266::SetIPConnection(bool protocol, String address, unsigned int port)
{
    Serial3.print("AT+CIPSTART=");
    Serial3.print(protocol == 0 ? "\"UDP\"" : "\"TCP\"");
    Serial3.print(",\"" + address + "\",");
    Serial3.println(port);
    Serial3.flush();

    return waitOK();
}
//Mux=1
bool ESP8266::SetIPConnection(byte id, bool protocol, String address, unsigned int port)
{
    Serial3.print("AT+CIPSTART=");
    Serial3.print(id);
    Serial3.print(protocol == 0 ? ",\"UDP\"" : ",\"TCP\"");
    Serial3.print(",\"" + address + "\",");
    Serial3.println(port);
    Serial3.flush();

    return waitOK();
}

//Mux=0
bool ESP8266::SendIPData(String data)
{
    if (transparency == 0)
    {
        Serial3.print("AT+CIPSEND=");
        Serial3.println(data.length());
        Serial3.flush();
    }
    else                //transparency = 1
    {
        Serial3.println("AT+CIPSEND");
        Serial3.flush();
    }

    unsigned long time = millis();
    while (millis() - time < _TIMEOUT)      //Wait system callback (">") to send data
    {
        if (Serial3.find(">"))
        {
            Serial3.println(data);
            if (transparency == 0)
                return waitOK();
            else                //transparency = 1
                return true;
        }
    }
    return false;
}

//None Transparency
//Mux=1
bool ESP8266::SendIPData(byte id, String data)
{
    //AT+CIPSEND=<id>,<length>\r\n
    Serial3.print("AT+CIPSEND=");
    Serial3.print(id);
    Serial3.print(",");
    Serial3.println(data.length());
    Serial3.flush();

    unsigned long time = millis();
    while (millis() - time < _TIMEOUT)      //Wait system callback (">") to send data
    {
        if (Serial3.find(">"))
        {
            Serial3.println(data);
            return waitOK();
        }
    }
    return false;
}

// Only valid in transparency open
void ESP8266::StopTransmission()
{
    if (transparency == 0)
        return;
    else
        Serial3.print("+++");
}

//Transparency
bool ESP8266::SetIPMode(bool on_off)
{
    transparency = on_off;
    Serial3.print("AT+CIPMODE=");
    Serial3.println(on_off);
    Serial3.flush();

    return waitOK();
}

byte ESP8266::GetIPStatus()
{
    byte connNum = 0;
    byte n = 0;
    Serial3.println("AT+CIPSTATUS");
    Serial3.flush();

    while (1)
    {
        if (Serial3.find("+CIPSTATUS:"))
        {
            connNum++;
            n = Serial3.parseInt();
            conn[n].id = n;
            Serial3.find(",\"");
            conn[n].protocol = Serial3.readStringUntil('\"');
            Serial3.find(",\"");
            conn[n].ip = Serial3.readStringUntil('\"');
            conn[n].port = Serial3.parseInt();
            conn[n].cs = Serial3.parseInt();

            // Serial.println(conn[n].id);
            // Serial.println(conn[n].protocol);
            // Serial.println(conn[n].ip);
            // Serial.println(conn[n].port);
            // Serial.println(conn[n].cs);
        }
        else
            break;

        // if (ipList.indexOf("OK") == 1 || ipList.indexOf("ERROR") == 1)
        //     break;
    }
    return connNum;
}

byte ESP8266::WaitConnect()     //Suggest used with GetIPData()
{
    unsigned long time = millis();
    while (millis() - time < _TIMEOUT)
    {
        if (Serial3.find("Link"))
        {
            return true;
        }
    }
    return false;
}

//Usage:    byte id, length;
//          String str = e.GetIPData(&id, &length);
String ESP8266::GetIPData(byte *id, byte *length)
{
    // String data = "";
    unsigned long time = millis();
    while (millis() - time < _RECV_TIMEOUT)
    {
        if (Serial3.find("+IPD,"))
        {
            *id = Serial3.parseInt();
            // Serial3.print(Serial3.parseInt());
            *length = Serial3.parseInt();
            Serial3.find(":");
            return Serial3.readStringUntil('\r');
        }
    }
    return "";
}

unsigned int ESP8266::GetIPTimeout()
{
    Serial3.println("AT+CIPSTO?");
    Serial3.flush();

    if (Serial3.find("+CIPSTO:"))
        return Serial3.parseInt();
    else
        return 0;
}

bool ESP8266::SetIPTimeout(unsigned int time)
{
    Serial3.print("AT+CIPSTO=");
    Serial3.println(time);
    Serial3.flush();

    return waitOK();
}

bool ESP8266::SetIPClose(byte id)
{
    Serial3.print("AT+CIPCLOSE=");
    Serial3.println(id);
    Serial3.flush();

    return waitOK();
}

String ESP8266::CustomCommand(String command)
{
	String result = "";
	Serial3.println(command);
	Serial3.flush();

	while (1)
	{
		result += Serial3.readString();
		if (result.indexOf("OK") != -1 || result.indexOf("ERROR") != -1)
			break;
	}
	return result;
}