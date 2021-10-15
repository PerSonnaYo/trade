#pragma once
typedef struct or_buy{
     int id;
     int num;
     double price;
     struct or *next;
     struct or *prev;
 } order_buy;

typedef struct or_sell{
     int id;
     int num;
     double price;
     struct or *next;
     struct or *prev;
 } order_sell;