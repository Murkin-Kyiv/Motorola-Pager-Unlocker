// ------------------- READ CMOS BYTE ----------------------------------- 
byte Read_Byte (byte Input_Address) // 
// input address as integer
// output data as integer
{
byte Address_Page = 0; // memory page
byte Address_Local = 0; // local adress on page
byte Buffer = 0; // buffer 

// calculate addres page and offset

Address_Page = Input_Address / 64; /// calculate memory page advisor has 4*64 = 256 bytes
Address_Local = 0xC0 + (Input_Address - Address_Page * 64); // calculate adress on the page C0-FF

// Send Read instructions
digitalWrite(CS_PIN, CMOS_CS_ON); // chip select
    
Buffer = SPI.transfer(0x1B); // 
Buffer = SPI.transfer(Address_Page + 0x40); // memory page 40,41,42, 43
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
        
Buffer = SPI.transfer(Address_Local); // adreess on page 
Buffer = SPI.transfer(0x00); // receive data from adresss 
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
// Buffer = 0xFF; // DEBUG    
return Buffer;
}

//------------------ WRITE CMOS BYTE ----------------------------------- 

byte Write_Byte (byte Input_Address, byte Input_Data) // 
// input address as integer
// output data as integer
{

byte Address_Page = 0; // memory page
byte Address_Local = 0; // local adress on page
byte Buffer = 0; // buffer 

// Calculate address page and offset   
Address_Page = Input_Address / 64 ; /// calculate memory page advisor has 4*64 = 256 bytes
Address_Local = 0x40 + (Input_Address - Address_Page * 64); // calculate adress on the page 0x40 - 0x7F

// Send WRITE instructions
// ----------- 1 packet 
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
    Buffer = SPI.transfer(0x1A); // 
    Buffer = SPI.transfer(0x00); //  

    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect

    Buffer = SPI.transfer(0x0B); //  
    Buffer = SPI.transfer(0x00); //  
    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect

// ----------- 2 packet
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
    Buffer = SPI.transfer(0x27); // 
    Buffer = SPI.transfer(0x40); //  

    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect

    Buffer = SPI.transfer(0x01); //  
    Buffer = SPI.transfer(0xB8); //  
    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect

// ----------- 3 packet
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
    Buffer = SPI.transfer(0x1B); //  
    Buffer = SPI.transfer(0x40 + Address_Page); // Page 
    
    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
    
    Buffer = SPI.transfer(Address_Local); // address offset
    Buffer = SPI.transfer(0x00); //  
    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
    
// ----------- 4 packet
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
    Buffer = SPI.transfer(0x1B); // 
    Buffer = SPI.transfer(0xC0 + Address_Page); //  
    
    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
    
    Buffer = SPI.transfer(Address_Local); //  
    Buffer = SPI.transfer(0x85); //  
    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect

delay (10); // 
// ----------- 5 packet
    digitalWrite(CS_PIN, CMOS_CS_ON); // chip select 1-st two bytes
    Buffer = SPI.transfer(0x1B); //
    Buffer = SPI.transfer(Address_Page + 0x40); // Page 
    
    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
    
    Buffer = SPI.transfer(Address_Local); // address offset
    Buffer = SPI.transfer(Input_Data); // data 
    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect

// ----------- 6 packet
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
    Buffer = SPI.transfer(0x1B); // 
    Buffer = SPI.transfer(Address_Page + 0xC0); //  

    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect

    Buffer = SPI.transfer(Address_Local); //  
    Buffer = SPI.transfer(0x85); //  
    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect

// ----------- 7 packet
delay (10);
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
    Buffer = SPI.transfer(0x1B); // 
    Buffer = SPI.transfer(Address_Page + 0x40); //  

    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
    digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect

    Buffer = SPI.transfer(0x01); //  
    Buffer = SPI.transfer(0x98); //  
    digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
       
  return Buffer;
}
 
//------- DECODE PASS FUNCTIONS ---------------
String Decrypt_Symbol(byte Position, byte Symbol)
// Input: position and crypted symbol as integer
// Output: decrypted symbol as string
{
String Decoded_Symbol = "*";

for (byte i=0; i < 37; i++)
{
if (Code_Table[i][Position] == Symbol)
  {
   Decoded_Symbol = char(Code_Table[i][0]);
  }
}
return Decoded_Symbol;
}
