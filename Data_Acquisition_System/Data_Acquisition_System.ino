#include <HX711.h> /* load sensor library */
#include <SD.h> /* the library required to communicate
                   card */
#include <SPI.h>



/* MOSI - pin 11
   MISO - pin 12
   Clock - pin 13
   Chip Select - pin 4
*/

File data_file; // creating a data file
char key = 0;
String file_title;

int analogPin_clk = 0; // Serial/Clock line for Load cell
int analogPin_data = 1;  // Data line from load cell

HX711 load_cell1(analogPin_clk, analogPin_data, 128);

unsigned long time_limit;

void setup() {
  Serial.begin(9600);

  Serial.println("Initializing the SD card....."); // output to show that SD card is initializing

  while (!SD.begin(4)) { // checks if SD work
    Serial.println("Initialization failed !!\n");
    return;
  }
  Serial.println("Initialization complete.\n");
  menu();

  load_cell1.power_up();
}

void loop() {

  while (!data_file)
    data_file = SD.open("test.txt", FILE_WRITE);
  key = 0;

  if (Serial.available() > 0) { // checks for key input from user
    key = Serial.read();
    Serial.print("Recieved input: ");
    Serial.println(key); // Reads in keyboard input
  }

  if (key == '1') { // Prints the data to the file
    load_cellprinter(load_cell1);
    data_file.println("Test case working");
    data_file.println();
    data_file.close();
  }

  else if (key == '2') {
    return;
  }

  else if (key == '3') {
    time_reciever(time_limit);

    while (millis() < time_limit) {
      delay(4000);
      load_cellprinter(load_cell1);
    }
    data_file.close();
    Serial.println("Recording complete");
    Serial.println();
    key = 0;
  }

}

void menu() {
  Serial.println(" 1 - Print to file");
  Serial.println(" 2 - Change file name");
  Serial.println(" 3 - Record till __ ms");
  Serial.println();
}

void time_reciever(unsigned long &time_limit)
{
  Serial.setTimeout(10000);
  while (Serial.available() > 0)
  {
    Serial.read();
    time_limit = Serial.parseInt();
  }
  Serial.println(time_limit);
  time_limit = 60000 + millis();
  Serial.print("Current time = ");
  Serial.println(millis());
  Serial.print("Recording data till - ");
  Serial.println(time_limit);
}

void load_cellprinter(HX711 load_cell)
{
    data_file.print(millis());
    data_file.print(" - Load cell 1 - ");
    data_file.println(load_cell1.read());
}

