#include "num2words-hu.h"
#include "string.h"

static const char* const ONES[] = {
  "nulla",
  "egy",
  "kettoo",
  "haarom",
  "neegy",
  "oot",
  "hat",
  "heet",
  "nyolc",
  "kilenc"
};

static const char* const TEENS[] ={
  "",
  "tizen",
  "tizenkettoo",
  "tizenhaarom",
  "tizenneegy",
  "tizenoot",
  "tizenhat",
  "tizenheet",
  "tizennyolc",
  "tizenkilenc"
};

static const char* const TENS[] = {
  "",
  "tiz",
  "huusz",
  "harminc",
  "negyven",
  "ootven",
  "hatvan",
  "hetven",
  "nyolcvan",
  "kilencven"
};

static const char* STR_OH_CLOCK = "kereken";
static const char* STR_NOON = "deel";
static const char* STR_MIDNIGHT = "eejfeel";
static const char* STR_QUARTER = "negyed";
static const char* STR_TO = "muulva";
static const char* STR_PAST = "muult";
static const char* STR_HALF = "feel";
static const char* STR_AFTER = "utaan";
static const char* STR_OH = "oh";

static size_t append_number(char* words, int num, int minutes) {
  int tens_val = num / 10 % 10;
  int ones_val = num % 10;

  size_t len = 0;

  if (tens_val > 0 || minutes ) {
	if ( tens_val == 0 && minutes ) {
		strcat(words,STR_OH);
		strcat(words," ");
	} else if (tens_val == 1 && num != 10) {
      strcat(words, TEENS[ones_val]);
      return strlen(TEENS[ones_val]);
    }
    strcat(words, TENS[tens_val]);
    len += strlen(TENS[tens_val]);
    if (ones_val > 0) {
      strcat(words, " ");
      len += 1;
    }
  }

  if (ones_val > 0 || num == 0) {
    strcat(words, ONES[ones_val]);
    len += strlen(ONES[ones_val]);
  }
  return len;
}

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}


void time_to_words(int hours, int minutes, char* words, size_t length) {

  size_t remaining = length;
  memset(words, 0, length);

  if (hours == 0) {
    remaining -= append_string(words, remaining, STR_MIDNIGHT);
  } else if (hours == 12) {
    remaining -= append_string(words, remaining, STR_NOON);
  } else {
    remaining -= append_number(words, hours % 12,0);
  }

  remaining -= append_string(words, remaining, " ");
  remaining -= append_number(words, minutes,1);

}
void time_to_2words(int hours, int minutes, char* words, size_t length, char* words2, size_t length2) {

  size_t remaining = length;
  memset(words, 0, length);

  if (hours == 0) {
    remaining -= append_string(words, remaining, STR_MIDNIGHT);
  } else if (hours == 12) {
    remaining -= append_string(words, remaining, STR_NOON);
  } else {
    remaining -= append_number(words, hours % 12,0);
  }

  size_t remaining2 = length2;
  memset(words2, 0, length2);

  remaining2 -= append_number(words2, minutes,1);

}

void fuzzy_time_to_words(int hours, int minutes, char* words, size_t length) {
  int fuzzy_hours = hours;
  int fuzzy_minutes = ((minutes + 2) / 5) * 5;

  // Handle hour & minute roll-over.
  if (fuzzy_minutes > 55) {
    fuzzy_minutes = 0;
    fuzzy_hours += 1;
    if (fuzzy_hours > 23) {
      fuzzy_hours = 0;
    }
  }

  size_t remaining = length;
  memset(words, 0, length);

  if (fuzzy_minutes != 0 && (fuzzy_minutes >= 10 || fuzzy_minutes == 5 || fuzzy_hours == 0 || fuzzy_hours == 12)) {
    if (fuzzy_minutes == 15) {
      remaining -= append_string(words, remaining, STR_QUARTER);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_AFTER);
      remaining -= append_string(words, remaining, " ");
    } else if (fuzzy_minutes == 45) {
      remaining -= append_string(words, remaining, STR_QUARTER);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_TO);
      remaining -= append_string(words, remaining, " ");

      fuzzy_hours = (fuzzy_hours + 1) % 24;
    } else if (fuzzy_minutes == 30) {
      remaining -= append_string(words, remaining, STR_HALF);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_PAST);
      remaining -= append_string(words, remaining, " ");
    } else if (fuzzy_minutes < 30) {
      remaining -= append_number(words, fuzzy_minutes,0);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_AFTER);
      remaining -= append_string(words, remaining, " ");
    } else {
      remaining -= append_number(words, 60 - fuzzy_minutes,1);
      remaining -= append_string(words, remaining, " ");
      remaining -= append_string(words, remaining, STR_TO);
      remaining -= append_string(words, remaining, " ");

      fuzzy_hours = (fuzzy_hours + 1) % 24;
    }
  }

  if (fuzzy_hours == 0) {
    remaining -= append_string(words, remaining, STR_MIDNIGHT);
  } else if (fuzzy_hours == 12) {
    remaining -= append_string(words, remaining, STR_NOON);
  } else {
    remaining -= append_number(words, fuzzy_hours % 12,0);
  }

  if (fuzzy_minutes == 5 && !(fuzzy_hours == 0 || fuzzy_hours == 12)) {
    remaining -= append_string(words, remaining, " ");
    remaining -= append_string(words, remaining, STR_OH_CLOCK);
  }
}
