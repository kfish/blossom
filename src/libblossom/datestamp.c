
#include <time.h>

#include "private.h"

static char * weekdays[7] = {
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static char * mo[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static char * nums [100] = {
  "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
  "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
  "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
  "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
  "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
  "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
  "60", "61", "62", "63", "64", "65", "66", "67", "68", "69",
  "70", "71", "72", "73", "74", "75", "76", "77", "78", "79",
  "80", "81", "82", "83", "84", "85", "86", "87", "88", "89",
  "90", "91", "92", "93", "94", "95", "96", "97", "98", "99"
};

int
blossom_datestamp (Blossom * blossom, time_t clock)
{
  struct tm tmr, * ret;
  int century, cyr;
  char year[5];

  ret = gmtime_r (&clock, &tmr);
  if (!ret) return -1;

  /* Make year string (range 0000-9999) */
  century = (ret->tm_year + 1900) / 100;
  cyr = (ret->tm_year + 1900) % 100;
  year[0] = nums[century][0];
  year[1] = nums[century][1];
  year[2] = nums[cyr][0];
  year[3] = nums[cyr][1];
  year[4] = '\0';

  dictionary_insert (blossom->dictionary, "yr", year);
  dictionary_insert (blossom->dictionary, "mo", mo[ret->tm_mon]);
  dictionary_insert (blossom->dictionary, "mo_num", nums[ret->tm_mon+1]);
  dictionary_insert (blossom->dictionary, "da", nums[ret->tm_mday]);
  dictionary_insert (blossom->dictionary, "hr", nums[ret->tm_hour]);
  dictionary_insert (blossom->dictionary, "min", nums[ret->tm_min]);

  return 0;
} 
