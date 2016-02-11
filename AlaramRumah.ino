char karakter;
String cmd;

int sirine = 8;
int led = 13;
int lamp_teras = 9;
int keamanan = 0;
int valsensorpintu;
int setjam = 0;
int setlampu = 0;

long int petang, subuh, pagi, pukul, jam, menit, detik, jam2, menit2, detik2;

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
  delay(1000);
  
  while((setjam==0)||(setlampu==0))
  {
    if(Serial.available())
    {
      cekcmd();
    }
  }
  
  valsensorpintu = digitalRead(2);
  
  run_jam();
  cek_jam();
  cek_secure();
  
  while (Serial.available())
  {
    cekcmd();
  }
  
}

void cekcmd()
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
          if(valsensorpintu==LOW)
          {
            keamanan = 0;
            Serial.println("jendela terbuka!");
            delay(500);
            Serial.println("tutup dulu! ");
            delay(500);
            run_jam();
            //cek_jam();
            //cek_secure();
          }
          else
          {
            keamanan = 1;
            digitalWrite(led, HIGH);
            Serial.println("Sistem Keamanan ON");
            delay(1000);
            run_jam();
            //cek_jam();
            //cek_secure();
          }
        }
        else
        {
          keamanan = 0;
          digitalWrite(led, LOW);
          Serial.println("Sistem Keamanan OFF");
          delay(1000);
          run_jam();
          //cek_jam();
          //cek_secure();
        }
        cmd="";
      }
      else if(cmd.startsWith("jam"))
      {

        jam = cmd.substring(4,6).toInt();
        menit = cmd.substring(7).toInt();;
        detik = 0;
        pukul = second(jam,menit,detik);
        Serial.print("jam ");
        Serial.print(jam);
        Serial.print(":");
        Serial.print(menit);
        Serial.print(":");
        Serial.println(detik);
        delay(1000);
        setjam = 1;
        run_jam();
        //cek_jam();
        //cek_secure();
        cmd="";
        
      }
      else if(cmd.startsWith("set"))
      {

        jam = cmd.substring(4,6).toInt();
        menit = cmd.substring(7,9).toInt();;
        detik = 0;
        petang = second(jam,menit,detik);
        
        jam2 = cmd.substring(10,12).toInt();
        menit2 = cmd.substring(13).toInt();;
        detik2 = 0;
        pagi = second(jam2,menit2,detik2);
        subuh = pagi - second(1,15,0);
        
        Serial.print("set ");
        Serial.print(jam);
        Serial.print(":");
        Serial.print(menit);
        Serial.print(":");
        Serial.print(detik);
        Serial.print(" - ");
        Serial.print(jam2);
        Serial.print(":");
        Serial.print(menit2);
        Serial.print(":");
        Serial.println(detik2);
        delay(1000);
        setlampu = 1;
        run_jam();
        //cek_jam();
        //cek_secure();
        cmd="";
        
      }
      else if(cmd=="menu")
      {
        Serial.println("==|MENU PERINTAH|==");
        delay(200);
        Serial.println("? : Menampilkan Status");
        delay(200);
        Serial.println("a : Sistem Keamanan ON/OFF");
        delay(200);
        Serial.println("jam: cth: jam 23.10");
        delay(200);
        Serial.println("setlamp:cth: set 17.30-05.00");
        delay(200);
        run_jam();
        //cek_jam();
        //cek_secure();
        cmd="";
      }
      else if(cmd=="?")
      {
        Serial.println("==|STATUS|==");
        delay(250);
        
        Serial.print("Pintu/Jendela: ");
        if(valsensorpintu==HIGH)
        {
           Serial.println("Tertutup");
        }
        else
        {
          Serial.println("Terbuka");
        }
        delay(250);
        
        Serial.print("Sistem Keamanan: ");
        if(keamanan==1)
        {
           Serial.println("ON");
        }
        else
        {
          Serial.println("OFF");
        }
        delay(250);
        
        Serial.print("jam ");
        Serial.print(pukul/3600);
        Serial.print(":");
        Serial.print((pukul%3600)/60);
        Serial.print(":");
        Serial.println(((pukul%3600)%60));
        delay(250);
        
        run_jam();
        //cek_jam();
        //cek_secure();
        cmd="";
      }
      else
      {
        Serial.println("Maaf perintah salah");
        delay(500);
        Serial.println("ketik 'menu' untuk melihat perintah");
        delay(500);
        run_jam();
        //cek_jam();
        //cek_secure();
        cmd="";
      }
    }
    
}

void run_jam()
{
  if(pukul < second(23,59,59))
  {
    pukul++;
  }
  else
  {
    pukul = 0;
  }
}

void cek_jam()
{
  if( (pukul >= petang) || (pukul < pagi) )
  {
    digitalWrite(lamp_teras,LOW);
    //Serial.println("Lampu ON");
  }
  else
  {
    digitalWrite(lamp_teras,HIGH);
    //Serial.println("Lampu OFF");
  }
  
  if( (pukul == petang) || (pukul == pagi) )
  {
    digitalWrite(sirine, LOW);// sirine hidup
    delay(100);
    digitalWrite(sirine, HIGH);//sirine mati
    delay(900);
    run_jam();
    cek_jam();
    cek_secure();
  }
  
  if(pukul == subuh)
  {
    keamanan = 0;
    digitalWrite(led, LOW);
  }
  
  if((pukul == second(23,59,0))&&(valsensorpintu==HIGH))
  {
    keamanan = 1;
    digitalWrite(led, HIGH);
  }
  
}

void cek_secure()
{ 
  if((keamanan==1)&&(valsensorpintu==LOW))
  {
    digitalWrite(sirine, LOW);
    //Serial.println("Sirine ON");
    delay(250);
    digitalWrite(sirine, HIGH);
    //Serial.println("Sirine OFF");
    delay(750);
    run_jam();
    cek_jam();
  }
}

long int second( long int x, long int y, long int z)
{
  long int hasil;
  hasil = (x*3600)+(y*60)+z;
  return hasil;
}


