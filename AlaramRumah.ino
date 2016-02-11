char karakter;
String cmd = "";

int sirine = 8;
int led = 13;
int lamp_teras = 9;
int keamanan = 0;
int autolampteras = 0;
int valsensorpintu;
int dark = 25;
int valldr; // nilai sensor ldr
int vallamp = 5; // intensitas cahaya lampu
int minldr = dark + 1; // intensitas minimum
int maxldr = vallamp +1; //intensitas maximum

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(sirine, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(lamp_teras, OUTPUT);
  digitalWrite(sirine, HIGH); // sirine mati
  digitalWrite(lamp_teras, HIGH); //lampu mati
  digitalWrite(led, LOW);
}

void loop()
{ 
  valsensorpintu = digitalRead(2);
  if(valsensorpintu==HIGH)
  {
    valsensorpintu = 0;
  }
  else
  {
    valsensorpintu = 1;
  }
  
  readldr();
  
  cekcmd();
  ceksecure();
  cekldr();
  
}

void cekcmd()
{
  if(Serial.available())
  {
    karakter = (char)Serial.read();
    if(karakter != '\n')
    {
      cmd += karakter;
    }
    else
    {
      if(cmd=="a")
      {
        if(keamanan == 0)
        {
          if(valsensorpintu==1)
          {
            keamanan = 0;
            Serial.println("Maaf Pintu/jendela terbuka!");
            delay(500);
            Serial.print("periksa/tutup sebelum ");
            delay(500);
            Serial.println("mengaktifkan sistem keamanan");
            delay(500);
          }
          else
          {
            keamanan = 1;
            Serial.println("Sistem Keamanan ON");
            delay(500);
          }
        }
        else
        {
          keamanan = 0;
          konfirm();
          Serial.println("Sistem Keamanan OFF");
          delay(500);
        }
        cmd="";
      }
      else if(cmd=="b")
      {
        if(autolampteras==0)
        {
          autolampteras = 1;
          //kalibrasildr();
          Serial.println("Otomatis Lampu teras ON");
          delay(500);
        }
        else
        {
          autolampteras = 0;
          digitalWrite(lamp_teras, HIGH); //lampu mati
          Serial.println("Otomatis Lampu teras OFF");
          delay(500);
        }
        cmd="";
      }
      else if(cmd=="menu")
      {
        Serial.println("==|MENU PERINTAH|==");
        delay(500);
        Serial.println("? : Menampilkan Status");
        delay(500);
        Serial.println("a : Sistem Keamanan ON/OFF");
        delay(500);
        Serial.println("b : Otomatis Lampu teras ON/OFF");
        delay(500);
        cmd="";
      }
      else if(cmd=="?")
      {
        Serial.println("==|STATUS|==");
        delay(500);
        
        Serial.print("Pintu/Jendela: ");
        if(valsensorpintu==HIGH)
        {
           Serial.println("Terbuka");
           delay(500);
        }
        else
        {
          Serial.println("Tertutup");
          delay(500);
        }
        
        Serial.print("Sistem Keamanan: ");
        if(keamanan==1)
        {
           Serial.println("ON");
           delay(500);
        }
        else
        {
          Serial.println("OFF");
          delay(500);
        }
        
        Serial.print("Otomatis Lampu teras: ");
        if(autolampteras == 0)
        {
           Serial.println("OFF");
           delay(500);
        }
        else
        {
          Serial.println("ON");
          delay(500);
        }
        Serial.print("Intensitas Cahaya: ");
        Serial.println(valldr);
        delay(500);
        cmd="";
      }
      else
      {
        Serial.println("Maaf perintah salah");
        delay(500);
        Serial.println("ketik 'menu' untuk melihat perintah");
        delay(500);
        cmd="";
      }
    }
  }
}

void konfirm()
{
  digitalWrite(sirine, LOW);// sirine hidup
  delay(100);
  digitalWrite(sirine, HIGH);//sirine mati
}

void ceksecure()
{
  if(keamanan==1)
  {
    digitalWrite(led, HIGH);
  }
  else
  {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
  }
  
  
  if(valsensorpintu==1 && keamanan==1)//jika (pintu/jendela) terbuka dan sistem keamanan on
  {
    digitalWrite(sirine, LOW);// sirine hidup
    delay(250);
    digitalWrite(sirine, HIGH);// sirine mati
    delay(3000);
  }
}

void cekldr()
{
  if(autolampteras==1){
    if(valldr<=minldr) //jika intensitas cahaya <= intensitas minimum/batas bawah
    {
      digitalWrite(lamp_teras, LOW); //lampu hidup
    }
    else if(valldr>=maxldr)//jika intensitas cahaya >= intensitas maximum/batas atas
    {
      digitalWrite(lamp_teras, HIGH); //lampu mati
    }
  }
}

/*void kalibrasildr()
{
  Serial.println("Mengkalibrasi...");
  delay(500);
  Serial.println("Tutup sensor cahaya!");
  delay(500);
  digitalWrite(lamp_teras, HIGH); //lampu mati
  delay(9500);
  readldr();
  minldr = valldr + 10;
  Serial.println("Buka sensor cahaya!");
  delay(500);
  digitalWrite(lamp_teras, LOW); //lampu Hidup
  delay(9500);
  readldr();
  vallamp = valldr;
  maxldr = minldr + vallamp + 5;
  digitalWrite(lamp_teras, HIGH); //lampu mati
  Serial.println("Kalibrasi selesai");
  delay(500);
}*/

void readldr()
{
  valldr = map(analogRead(A0),0,1024,0,50);
  //valldr = 1023 - valldr;
}



