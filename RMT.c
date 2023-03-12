sbit LCD_RS at RB1_bit;
sbit LCD_EN at RB2_bit;
sbit LCD_D4 at RB3_bit;
sbit LCD_D5 at RB4_bit;
sbit LCD_D6 at RB5_bit;
sbit LCD_D7 at RB6_bit;

sbit LCD_RS_Direction at TRISB1_bit;
sbit LCD_EN_Direction at TRISB2_bit;
sbit LCD_D4_Direction at TRISB3_bit;
sbit LCD_D5_Direction at TRISB4_bit;
sbit LCD_D6_Direction at TRISB5_bit;
sbit LCD_D7_Direction at TRISB6_bit;

int voltage1 = 0, current1 = 0 , adc_val = 0;
char chrv[5];  //voltage
char chrt[4];  //temp
char chrc[5];  //current
unsigned char value[50],pos;
unsigned char sec;


void clearVAL(){
  char i;
  for(i=0;i<30;i++)
  value[i]=' ';
}

float findvoltage(int a){
  int i;
  unsigned int temp=0;
  float maxpoint = 0;
  for(i=0;i<500;i++){
      if(temp = ADC_Read(a),temp>maxpoint);
  {
maxpoint = temp;
}
}
maxpoint = ( maxpoint * 5 )/ (1023) ;
maxpoint = maxpoint * 4;
maxpoint = maxpoint + 1.4;
maxpoint = maxpoint * 18;
maxpoint = maxpoint * ( 1 / sqrt(2) );
delay_ms(20);
return maxpoint;
}
float findcurrent(int a){
int i;
unsigned int temp=0;
float maxpoint = 0;
    for(i=0;i<500;i++){
      if(temp = ADC_Read(a),temp>maxpoint);
    {
maxpoint = temp;
}
}
maxpoint = ( maxpoint * 5 )/ (1024) ;
maxpoint = maxpoint / 2 ;
maxpoint = maxpoint * ( 1 / sqrt(2) );
maxpoint = (maxpoint * 1000) / (66);
delay_ms(20);
return maxpoint;
}
void display()
{
intToStr(voltage1, chrv);
lcd_out(1,6, Ltrim(chrv));
intToStr(current1, chrc);
lcd_out(1,18, Ltrim(chrc));
intToStr(adc_val, chrt);
lcd_out(2,12, Ltrim(chrt));
lcd_out(1,1,"Vin: ");
lcd_out(1,9,"V");
lcd_out(1,13,"Iin: ");
lcd_out(1,20,"A");
lcd_out(2,6,"Temp: ");
lcd_out(2,14,"C");
}
void upload(){
     value[0]='\0';
        strcat(value,"AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80");
        UART2_WRITE_TEXT(&value) ;
        UART2_WRITE(0X0D);
        UART2_WRITE(0X0A);
        sec=0;
        delay_ms(2000);
        clearVAL();
        value[0]='\0';
        strcat(value,"AT+CIPSEND=4,70");
        UART2_WRITE_TEXT(&value) ;
        UART2_WRITE(0X0D);
        UART2_WRITE(0X0A);
        delay_ms(2000);
        clearVAL();
        value[0]='\0';
        strcat(value,"GET /update?key=0ZRPGDWWQU3SCA4J&field1="); //UPDATE CHANNEL using API
        UART2_WRITE_TEXT(&value) ;
        UART2_WRITE((voltage1/1000)+48);
        UART2_WRITE(((voltage1%1000)/100)+48);
        UART2_WRITE(((voltage1%100)/10)+48);
        UART2_WRITE((voltage1%10)+48);
        UART2_WRITE_TEXT("&field2=") ;
        UART2_WRITE((current1/1000)+48);
        UART2_WRITE(((current1%1000)/100)+48);
        UART2_WRITE(((current1%100)/10)+48);
        UART2_WRITE((current1%10)+48);
        UART2_WRITE_TEXT("&field3=") ;
        UART2_WRITE((adc_val/1000)+48);
        UART2_WRITE(((adc_val%1000)/100)+48);
        UART2_WRITE(((adc_val%100)/10)+48);
        UART2_WRITE((adc_val%10)+48);
        UART2_WRITE(0x0d);
        UART2_WRITE(0x0a);
        delay_ms(3000);
        value[0]='\0';
        UART2_WRITE_TEXT("Upload Successful\r\n") ;
}

void main()
{
    char i,x,flag;
    ANSELA=0XFF;
    ANSELB=0X00;
    ANSELC=0X00;
    TRISB=0X00;
    ADC_Init();
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    ANSELB=0X00;
    ANSELC=0X00;
    TRISB=0x00;
    delay_ms(1000);
    OSCCON.IRCF0=0;
    OSCCON.IRCF1=1;
    OSCCON.IRCF2=1;
    INTCON.GIE=1;
    INTCON.RCIE=1;
    INTCON.PEIE=1;
    UART2_Init(9600);
    adc_init();
    lcd_out(2,2, "Transformer Health");
    lcd_out(3,7, "Monitor");
    delay_ms(1000);
    clearVAL();
    UART2_WRITE_TEXT("AT\r\n");
    delay_ms(1000);
    UART2_WRITE_TEXT("ATE0\r\n");
    UART2_WRITE_TEXT("AT+CWMODE=3\r\n");
    UART2_WRITE_TEXT("AT+CWQAP\r\n");
    UART2_WRITE_TEXT("AT+CIPMUX=1\r\n") ;

    while(1)
    {
        delay_ms(1000);
        Lcd_Cmd(_LCD_CLEAR);
        clearVAL();
        lcd_out(3,6,"connecting");
        value[0]='\0';
        strcat(value,"AT+CWJAP=\"Methsoft Smart Life\",\"2554669\"");
        UART2_WRITE_TEXT(&value) ;
        UART2_WRITE(0X0D);
        UART2_WRITE(0X0A);
        delay_ms(1000);
        Lcd_Cmd(_LCD_CLEAR);
        sec=0;

    while(1)
    {
        voltage1 = findvoltage(0);
        current1 = findcurrent(1);
        adc_val=adc_read(2)*0.488;
        display();
        delay_ms(40);
        delay_ms(2000);
        lcd_out(4,5,"Synchronizing");
        upload();
}
}
}