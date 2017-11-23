time_t rawtime;
struct tm *timeinfo;
int year, month , day, hour, minute, second;

void setup() {
  Serial.begin(9600);
  do {
    Serial.println("Enter Year (YYYY): ");
    year = readData ();
  } while ((year > 2000) && (year < 2050));

  do {
    Serial.println("Enter Month: ");
    month = readData ();
  } while ((month > 0) && (month < 13));

  do {
    Serial.println("Enter Day: ");
    day = readData ();
  } while ((day > 0) && (day < 32));

  do {
    Serial.println("Enter Hour: ");
    hour = readData ();
  } while ((hour >= 0) && (hour <= 60));

  do {
    Serial.println("Enter Minute: ");
    minute = readData ();
  } while ((minute >= 0) && (minute <= 60));

  do {
    Serial.println("Enter Second: ");
    second = readData ();
  } while ((second >= 0) && (second <= 60));

  timeinfo->tm_year = year;
  timeinfo->tm_mon = month;
  timeinfo->tm_mday = day;
  timeinfo->tm_hour = hour;
  timeinfo->tm_min = minute;
  timeinfo->tm_sec = second;

  rawtime = mktime(timeinfo);
  stime(&rawtime);  //Set system time
}

void loop() {
  Serial.println(time(&rawtime));
  delay(1000);
}

int readData () {
  String data = "";
  while (Serial.available() == 0) {
    //Wait enter data
  }

  do {
    char character;
    character = Serial.read();
    data += character;
    delay(5);
  }  while (Serial.available() > 0);

  return (data.toInt());
}
