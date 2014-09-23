#include <inttypes.h> // пригодится, мы будем использовать числовые типы
#include <Arduino.h> // ЕМНИП pow() отсюда
#include <circleArea.h> // тут наши дефайны и объявления

//Реализуем нашу библиотечную функцию, ради которой все и затевалось

float circleArea(float radius)
{
    return PI*pow(radius, 2);
}

