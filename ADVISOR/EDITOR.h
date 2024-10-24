// convert integer to HEX as string f.e. 0x15 -> "0F"
String Int2HEX (int Value) 
// input integer data or adress, output 2 symbols as hex
{
String Out = "";
  Out = String(Value,HEX); 
  if (Out.length() == 1) {Out = "0"+ Out;} // add first 0
return Out;
}


// ------------------------------------------------------------------- SHOW DUMP --------------------------------------
void ShowDUMP (byte Dump2Show[]) // show dump on the display
// input array[256]
{
String Line = ""; // line to output
int Code = 0x00; // code of current symbol

Serial.println("    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E OF");
//Serial.println("    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E OF  ----------------");
for (int i = 0; i < 16; i++) // output 16 lines
    {
      Line = String(i,HEX) + "0  "; // header of address
        for (int j = 0; j < 16; j++) // fill line by HEX
            {                          
               Line = Line + Int2HEX(Dump2Show[i*16+j]) + " ";
            }

      Line = Line + " "; 

         for (int j = 0; j < 16; j++) // fill line by ASCII
              {
                Code = Dump2Show[i*16+j]; // take a code of 
                if ((Code < 32) or (Code >126 )) // do not output unprintabe symbols
                    {Line = Line + ".";} 
                else
                    {Line = Line + char(Dump2Show[i*16+j]);}
            }
            
      Serial.println(Line); 
    }

Serial.println("----------------------------------------------------------------------");
}

void ShowHelp()
{
Serial.println("0-R-CMOS 4-CMOS2EEPROM");
Serial.println("1-R-Byte 5-EEPROM2CMOS");
Serial.println("2-W-Byte 6-R-EEPROM");
Serial.println("3-W-Fill");
}