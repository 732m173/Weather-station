
void LectureCapteurs() {// fonction qui envoie les valeurs de l'éclairement et du temps  {"E_Lux":E_Lux,"temps":leTemps})

  
  //sensors.requestTemperatures(); //Demande la température aux capteurs DS18B20
  //int temperatureInt = sensors.getTempCByIndex(0)*100;// enregistre la valeur de la température
  //temp1.f = (float)temperatureInt/100.0;// pour avoir au maximum 2 chifres après la virgule
  dirVent.i = analogRead(GirouettePin);
  Serial.println("Direction du vent : ");
  Serial.println(dirVent.i);
  dirVent.i = getGirouetteAngle(dirVent.i);
  Serial.println(dirVent.i);

  int uvLevel = averageAnalogRead(UVOUT);
  //int refLevel = averageAnalogRead(REF_3V3);
  
  float outputVoltage = 3.3 / 3.3 * uvLevel;
  
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0);

  Serial.println("Temp: ");
  Serial.println(bmp280.getTemperature());
  Serial.println("C");
  
  Serial.println("Pressure: ");
  Serial.println(pressure = bmp280.getPressure());
  Serial.println("Pa");

  Serial.println("Altitude: ");
  Serial.println(bmp280.calcAltitude(pressure));
  Serial.println("m");

  Serial.println("UV: ");
  Serial.println(light.getUV());
 // Serial.println(uvLevel);

  /*Serial.println("Voltage: ");
  Serial.println(outputVoltage);

  Serial.println(" UV Intensity (mW/cm^2): ");
  Serial.println(uvIntensity);*/
  
  //Si7021
  //int temp3int = sensorTmpHum.readTemperature()*100;
  //temp3.f = (float)temp3int/100.0;

  //int humidityint = sensorTmpHum.readHumidity()*100;
  //humidity.f = (float)humidityint/100.0;
  temp1.f = 19.23; //°C
  payloadTemp = LMIC_f2sflt16(18.23/100);
  
  // envoi des données port série console arduino
  String chaine = "{\"temp1\":";   // il faut toujours initialiser la chaine avant de concaténer à la ligne suivante
  Serial.println(chaine + temp1.f + ",\"vitVent\":"+ vitVent.i/10 +"}");
  
}
