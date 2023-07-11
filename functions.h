#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <bitset>
#include <iomanip>
#include <iostream>
#include <vector>

#include <QRegularExpression>
#include <QDateTime>
#include <QVector>

#include "Config.h"

#include "Logger.h"
#include "templates_cout.h"
#include "TestRunner.h"

std::string to_string_exact(double x, bool fixed = true, int precision = 3);
bool string_replace(std::string& str, const std::string& from, const std::string& to);
int round_up_to_value(int value, int value_to_round);
bool startsWith(std::string mainStr, std::string toMatch);
std::vector<std::string> split(std::string strToSplit, char delimeter);
double extract_double(QString str);
double extract_double_scientific(QString str);
unsigned int extract_id(const QString str);
unsigned int extract_outcome_id(const QString str);
QString extract_name(const QString str);
QString extract_signal_type(const QString str);
QString extract_testType(const QString str);
QString extract_standardUnit(const QString str);
QString extract_dimension(const QString str);
unsigned int extract_connectorPin(const QString str);
double extract_tolerance(const QString str);

double invert_double_order(double value);

std::string TRANSLATION_EN_RU(std::string EN);

#define TRANSLATION_RU_EN(x) (translations_RU_EN.find(((x))) != translations_RU_EN.end()) ? \
    translations_RU_EN[((x))] : ""

static std::map<std::string, std::string> translations_RU_EN {
    {"кА", "kA"},
    {"кВ", "kV"},
    {"А", "A"},
    {"В", "V"},
    {"мА", "mA"},
    {"мВ", "mV"},
    {"мкА", "uA"},
    {"мкВ", "uV"},
    {"нА", "nA"},
    {"нВ", "nV"},
    {"нс", "ns"},
    {"мкс", "us"},
    {"мс", "ms"},
    {"с", "s"},

    {"ГОм", "GOhm"},
    {"МОм", "MOhm"},
    {"кОм", "kOhm"},
    {"Ом", "Ohm"},
    {"мАОм", "mOhm"},
    {"мкОм", "uOhm"},
    {"нОм", "nOhm"},

    {"кСм", "kS"},
    {"См", "S"},
    {"мСм", "mS"},
    {"мкСм", "uS"},
    {"нСм", "nS"},

    {"В норме", "Passed"},
    {"Не в норме", "Failed"},
    {"Годен", "Passed"},
    {"Брак", "Failed"}
};

void FunctionsTest();

#endif // FUNCTIONS_H
