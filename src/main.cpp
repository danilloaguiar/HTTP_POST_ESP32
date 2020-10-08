#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define LOCATION_MOTO (0)
#define LOCATION_BAT (1)
#define INFORMATION_MOTO (2)
#define INFORMATION_BAT (3)


const char* ssid = "kikulindo"; // trocar aqui
const char* password =  "kikumaravilhoso"; //trocar aqui

const char* loc_moto = "https://ecov.heroku.com/api/v1/geolocm";
const char* loc_bat = "https://ecov.heroku.com/api/v1/geolocb";
const char* info_moto = "https://ecov.heroku.com/api/v1/paramm/";
const char* info_bat = "https://ecov.heroku.com/api/v1//api/v1/paramb";

static uint8_t cont = 0;
static float lat[12] = {-23.5565750, 23.5558840 , -23.5565680, -23.5572540, -23.5579190, -23.5587600, -23.5597660, -23.5606090, -23.5668970, -23.5735450, -23.5814590, -23.5860590};
static float lon[12] = {-46.6641230, -46.6633810, -46.6620300, -46.6609920, -46.6602770, -46.6592620, -46.6581830, -46.6572770, -46.6500340, -46.6531680, -46.6623880, -46.6605720};

void request(const char* URL, int type_i){

  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

    HTTPClient http;   

    http.begin(URL);  //Specify destination for HTTP request

    if (type_i == LOCATION_MOTO || type_i == LOCATION_BAT){
      
      http.addHeader("Content-Type", "text/plain");             //Specify content-type header //provavelmente é outro parâmetro aqui

      int httpResponseCode = http.POST("MSG aqui");   //Send the actual POST request //colocar valores de lat e lon aqui, já tem um contador definido (cont) pode usar ele pra incrementar a posiçao da lista

      if(httpResponseCode>0){

        String response = http.getString();                       //Get the response to the request

        Serial.println(httpResponseCode);   //Print return code
        Serial.println(response);           //Print request answer

      }else{

        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);

      }
    }

    http.end();  //Free resources

  }else{

      Serial.println("Error in WiFi connection");   

  }

  
}


void setup() {

  Serial.begin(115200);
  delay(4000);   //Delay needed before calling the WiFi.begin

  WiFi.begin(ssid, password); 

  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

}

void loop() {

  for(int i = 0; i < 4; i++){
    switch (i)
    {
      case LOCATION_MOTO:
        request(loc_moto, i);
      break;

      case LOCATION_BAT:
        request(loc_bat, i);
      break;

      case INFORMATION_BAT:
        request(info_bat, i);
      break;

      case INFORMATION_MOTO:
        request(info_moto, i);
      break;

      default:
        delay(100);
      break;
    }
  }
  if (cont < 12) cont++;
  else cont = 0;
  delay(800);  //Send a request every 0.8 seconds
}