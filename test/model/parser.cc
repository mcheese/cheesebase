#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#endif
#include "catch.hpp"
#include "model/model.h"
#include "model/parser.h"
#include <gsl.h>
#include <iostream>
#include <memory>

using namespace cheesebase;

TEST_CASE("parse JSON") {
  std::string input = R"(
{
    "glossary": {
        "title": "example glossary",
        "GlossDiv": {
            "title": "S",
            "GlossList": {
                "GlossEntry": {
                    "ID": "SGML",
                    "SortAs": "SGML",
                    "GlossTerm": "Standard Generalized Markup Language",
                    "Acronym": "SGML",
                    "Abbrev": "ISO 8879:1986",
                    "GlossDef": {
                        "para": "A meta-markup language, used to crea"
                    },
                    "GlossSee": "markup","GlossSeeAlso": ["GML", "XML"]
                }
            }
        }
    },
     "test": 123,
    "list" : [{
      "id": 1,
      "first_name": "Beverly",
      "last_name": "Riley",
      "email": "briley0@phoca.cz",
      "country": "France",
      "ip_address": "254.153.188.156"
    }, {
      "id": 2,
      "first_name": "Irene",
      "last_name": "Rodriguez",
      "email": "irodriguez1@tripadvisor.com",
      "country": "Pakistan",
      "ip_address": "176.226.201.230"
    }, {
      "id": 3,
      "first_name": "Rebecca",
      "last_name": "Porter",
      "email": "rporter2@tinypic.com",
      "country": "Russia",
      "ip_address": "118.47.83.13"
    },12312,null, true, false, null ]
  }
)";

  model::Object doc;
  REQUIRE_NOTHROW(doc = parseJson(input.begin(), input.end()));
  REQUIRE(*doc.getChild("test") == model::Scalar(123.));
}
