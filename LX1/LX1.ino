// Motorola Pager CMOS Editor
// V2.3 Release 


#include <SPI.h> // standart arduino lib
#include <EEPROM.h> // standart arduino lib for internal EEPROM

#include "CONFIG.h" // variables and settings
#include "EDITOR.h" // Editor library
#include "LX1_CMOS.h" // Library for LX1 read/write byte functions


void setup() {
Serial.begin (COM_Port_Baud_Rate, COM_Port_Config); // Opern port

pinMode (CPUR_PIN, OUTPUT); // set CPU reset pin
digitalWrite(CPUR_PIN, LOW); // CPU off
   

Serial.println("Paging Lab(c)2023");
Serial.println("Motorola Scriptor LX1 CMOS Dump Reader V2.3");
ShowHelp();

// Init SPI bus
SPI.setBitOrder(CMOS_BIT_ORDER); // Set bit order 
SPI.setDataMode(CMOS_MODE); //Set clock and data mode CPOL=1 CPHA=1
SPI.setClockDivider(SPI_CLOCK_DIV128); //set clock divider !!! вынести в настройки !!!
SPI.begin(); // Start SPI bus
}

//---------------------------------------------------------------------------------
void loop() {
int Wait = 1; // waiting for address also for indicator
byte Answer = 0x00; // answer from pager debug only
volatile byte Dump[256] = {}; // ram dump
// LX1 wiht 2222222222 password
// byte Dump[256] = {0x2b,0x04,0x20,0x14,0x00,0x00,0xe2,0x00,0xff,0x12,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x29,0x26,0x29,0x50,0x48,0x31,0x51,0x44,0x37,0x45,0x52,0x33,0x53,0x46,0x38,0x47,0x06,0x37,0x32,0x33,0x49,0x5a,0x47,0x32,0x42,0x50,0x4d,0x4d,0x4f,0x54,0x4f,0x52,0x4f,0x4c,0x41,0x20,0x20,0x16,0x66,0x66,0x60,0x10,0x12,0x0b,0x09,0x01,0x02,0x19,0xbc,0x90,0x20,0x50,0xe8,0x98,0xd7,0x1c,0xb9,0x72,0x4e,0x6f,0x6e,0x65,0x00,0x32,0x33,0x00,0x31,0xfa,0xff,0x00,0x05,0x08,0x07,0xec,0x08,0xf2,0x04,0x03,0x84,0x12,0x4d,0xbc,0x00,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x0d,0x05,0x53,0x7b,0x11,0xe1,0x02,0x0c,0x08,0xbc,0x00,0x40,0x90,0x3c,0x01,0x0b,0x07,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

if (Serial.available()) // waiting commands from Sprut
  {
  Command = Serial.readString(); // receive command 

// Parsing command
  switch (Command.toInt()) {  // Parsing command 

  case READ_ALL:  //Read dump from CMOS
   // fill array 
       for (int i = 0; i <= 255; i++) // fill array from CMOS
          {
            Dump[i] = Read_Byte(i); // read from pager
          }
       // Show Dump
      Serial.println("------------------------PAGER CMOS DUMP------------------------------");
      ShowDUMP (Dump);
      // Decode pass
      Command = "";
      Wait = 36; // offset for Lx1

      for (int i = 0; i < 10; i++) // decode pass 
          {
           Command = Command + Decrypt_Symbol (i+1, Dump[i+36]); // read from pager
           //Serial.println(Dump[i+36], HEX); // debug
        
                 
          }
      
      Serial.println("Password:"+ Command);  

  
  ShowHelp();
  break;

  case READ_BYTE:  //  1- Read byte from CMOS 
       Serial.print("Read.Address(HEX):");  // input adress 
       while (Wait == 1)
              {
                if (Serial.available()) // waiting commands
                    {
                      Command = Serial.readString(); // receive command
                      Command = "0x" + Command; // transfer string with hex to adress as integer
                      Address = strtol(Command.c_str(),NULL,0); // transfer string with hex to adress as integer
                      Wait = 0; // reset input flag
                    }
              }

       Data = Read_Byte (Address); // read byte 
       Serial.println(Command + ":0x" + Int2HEX (Data) + ":" + char(Data));
       ShowHelp();
  break;
  
  case WRITE_BYTE:  // 2 - Write byte to CMOS
      Serial.print("Write.Address(HEX):");  // input adress 
      while (Wait == 1)
            {
                if (Serial.available()) // waiting commands
                    {
                      Command = Serial.readString(); // receive command
                      Command = "0x" + Command; // transfer string with hex to adress as integer
                      Address = strtol(Command.c_str(),NULL,0); // transfer string with hex to adress as integer
                      Wait = 0; // reset input flag
                      Serial.print(Command); //print addres as HEX
                    }
            }   

      Serial.print(":Data(HEX):"); // input data
      Wait = 1; // waiting for data 
      while (Wait == 1)
            {
               if (Serial.available()) // waiting commands
                    {
                      Command = Serial.readString(); // receive command
                      Command = "0x" + Command; // transfer string with hex to adress as integer
                      Data = strtol(Command.c_str(),NULL,0);
                      Wait = 0; // reset input flag
                      Serial.print(Command); //print data as HEX
                    }
            }   
      
      Answer = Write_Byte (Address, Data);
      Serial.println(":Done");
      ShowHelp(); 
  break;
  
  case WRITE_FILL_BYTE:  //3 Write DUMP BY  BYTE -P---------------------------------------------------
     
        Serial.print("Fill CMOS(HEX):"); 
        Wait = 1; // waiting for address 
        while (Wait == 1)
              {
                if (Serial.available()) // waiting commands
                    {
                      Command = Serial.readString(); // receive command
                      Command = "0x" + Command; // transfer string with hex to adress as integer
                      Serial.print(Command + ":");
                      Data = strtol(Command.c_str(),NULL,0);
                      Wait = 0;
                    }
              }
    
        Answer = Write_Byte(0, Data); // for init write 
        delay (50);// for init write
         
        Wait = 0; // indicator   
        for (int i=0; i<256; i++)
            {
              Answer = Write_Byte(i, Data);
              // Answer = Write_Byte(i, Dump[i]); // debug
              delay (50);
              Wait = Wait+1 ;
                if (Wait == 8)
                    {
                      Serial.print("#"); 
                      Wait = 0;   
                    }              
            }
        
        Serial.println(":Done");
        ShowHelp();
  break;

  case EEPROM_SAVE:  //4 Save dump to arduino eeprom---------------------------------------------------
       Serial.print("Save CMOS to EEPROM: "); 
       Wait = 0;   
       for (int i=0; i<256; i++)
            {
              // EEPROM[i] = 0x55; // DEBUG
              EEPROM[i] = Read_Byte(i); // read byte from pager and save to EEPROM
              delay(10); //wait for write cycle end
              Wait = Wait+1 ;
                if (Wait == 8)
                    {
                      Serial.print("#"); 
                      Wait = 0;   
                    }
            }
    Serial.println(":Done");
    ShowHelp();
  break;
 
  case EEPROM_RESTORE:  //5 upLoad dump from arduino eeprom to pager CMOS---------------------------------------------------
       Serial.print("Upload EEPROM to CMOS:"); 
       Wait = 0; //Indicator 

       for (int i=0; i<256; i++)
            {
              delay(5);
              Data = EEPROM[i]; //Read byte from EEPROM 
              delay(5);
              Answer = Write_Byte(i, Data); //Write byte to CMOS
              delay (50);
              Wait = Wait+1;
               if (Wait == 8)
                    {
                      Serial.print("#"); 
                      Wait = 0;   
                    }
            }
        Serial.println(":Done");
        ShowHelp();
  break;
 
  case EEPROM_SHOW:  //6 Show dump from arduino eeprom---------------------------------------------------
       // fill array 
       for (int i = 0; i <= 255; i++) // fill array from eeprom
          {
            //Dump[i] = i; // debug
            Dump[i] = EEPROM[i];
          }
       // Show Dump
      Serial.println("--------------------------EEPROM DUMP--------------------------------");
      ShowDUMP (Dump);
      ShowHelp();
  break;
 



  default:  //  
    Serial.println("Unknown command");
    ShowHelp();
    break;  
    
} // End case parser 
} // End of command procedure
} // End loop
