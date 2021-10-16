#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct or_buy{
     int id;
     int num;
     double price;
    //  struct or *next;
    //  struct or *prev;
 } order_buy;

typedef struct or_sell{
     int id;
     char buy;
     int id_early;
     int id_present;
     int num;
     double price;
 } order_sell;