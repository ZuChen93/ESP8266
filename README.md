#ESP8266 Arduino library
This Library is for Arduino Mega (Arduino DUE maybe ok). 
If you need to use it on other boards (only one serial port), replace all "Serail3" to "Serail" in the file "ESP8266.cpp".

2015/2/28

- Add Function "CustomCommand(String command)" which can make user input extended instruction.

2014/12/5
- Modified function:SoftReset()
- Add function:SetIPClose(byte id)
    
2014/11/8
- First Version

###########################################
## Methods and Functions ()
###########################################
     ATE 
     Test 
     SoftReset 
     GetVersion 
     GetWIFIMode 
     SetWIFIMode 
     SetJoinAP 
     SetQuitAP 
     GetAPConfig 
     SetAPConfig 
     GetCurrentAP 
     GetAPList 
     GetIFList 
     SetIPMode 
     SetIPMux 
     SetIPServer 
     SendIPData 
     SendIPData 
     SetIPConnection 
     SetIPConnection 
     SetIPTimeout 
     GetIPStatus 
     GetIPTimeout 
     GetIPList 
     StopTransmission 
     WaitConnect 
     GetIPData 
     SendIPClose 
     CustomCommand
