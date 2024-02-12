#include <WiFi.h>
#include "time.h"
#include "sntp.h"

const char* ssid       = "PhayouneWIFI";
const char* password   = "PhayounE2014";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 25200; // UTC+7
const int   daylightOffset_sec = 0; // No daylight saving time in UTC+7

// Thai day names
const char* th_dayNames[] = {"อาทิตย์", "จันทร์", "อังคาร", "พุธ", "พฤหัสบดี", "ศุกร์", "เสาร์"};

// Thai month names
const char* th_monthNames[] = {"มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน", "พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม", "กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม"};

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return;
  }

  // Adjust to Thai timezone
  time_t thai_time;
  thai_time = mktime(&timeinfo) + gmtOffset_sec; // Adjust for timezone

  // Calculate Thai day of the week
  int thai_day = (timeinfo.tm_wday) % 7; // Adjust for Thai day index

  // Print Thai local time with year + 543
  Serial.printf("วัน%s, %02d %s %04d %02d:%02d:%02d\n", th_dayNames[thai_day], timeinfo.tm_mday, th_monthNames[timeinfo.tm_mon], timeinfo.tm_year + 1900 + 543, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

void setup()
{
  Serial.begin(115200);

  // Set notification call-back function
  sntp_set_time_sync_notification_cb(timeavailable);

  // Set NTP server mode to acquire via DHCP
  sntp_servermode_dhcp(1);

  // Configure NTP servers and time zone
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  // Connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
}

void loop()
{
  delay(5000);
  printLocalTime(); // It will take some time to sync time :)
}
