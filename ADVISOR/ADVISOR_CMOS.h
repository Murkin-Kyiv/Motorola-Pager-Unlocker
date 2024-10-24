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

digitalWrite(CPUR_PIN, LOW); // off

// Send Read instructions
digitalWrite(CS_PIN, CMOS_CS_ON); // chip select
    
Buffer = SPI.transfer(0x00); // 
Buffer = SPI.transfer(0xE0); // 
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
        
Buffer = SPI.transfer(0x11); // 
Buffer = SPI.transfer(Address_Page); // memory page
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect

Buffer = SPI.transfer(0x10); // 
Buffer = SPI.transfer(0x00); // 
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect

Buffer = SPI.transfer(Address_Local); // adreess on page 
Buffer = SPI.transfer(0x00); // receive data from adresss 

digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
digitalWrite(CPUR_PIN, HIGH); // CPU on 
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

// Send first packet
// Init SPI bus
SPI.end(); // Start SPI bus
SPI.begin(); // Start SPI bus

digitalWrite(CPUR_PIN, LOW); // CPU off

// Send WRITE instructions
digitalWrite(CS_PIN, CMOS_CS_ON); // chip select
    
Buffer = SPI.transfer(0x11); // write command ??? 
Buffer = SPI.transfer(Address_Page + 0x80); // Page 
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
    
Buffer = SPI.transfer(0x10); // 
Buffer = SPI.transfer(0x06); // 
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect

Buffer = SPI.transfer(Address_Local); // address offset
Buffer = SPI.transfer(Input_Data); // data 
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect

Buffer = SPI.transfer(0x10); //  
Buffer = SPI.transfer(0x07); //  
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
  
delay (20);

// Send second packet
digitalWrite(CS_PIN, CMOS_CS_ON); // chip select
Buffer = SPI.transfer(0x10); // 
Buffer = SPI.transfer(0x06); // 
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
        
Buffer = SPI.transfer(0x10); // 
Buffer = SPI.transfer(0x02); // 
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
    
Buffer = SPI.transfer(Address_Local); // address ???
Buffer = SPI.transfer(Input_Data); // data ???
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
digitalWrite(CS_PIN,CMOS_CS_ON); // chip unselect
    
Buffer = SPI.transfer(0x10); //  
Buffer = SPI.transfer(0x03); //  
digitalWrite(CS_PIN,CMOS_CS_OFF); // chip unselect
digitalWrite(CPUR_PIN, HIGH); // CPU on

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
