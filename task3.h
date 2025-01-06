#pragma once

#include <ctype.h>

#pragma region typedefs
typedef struct Address {
    String city;
    String street;
    unsigned int house;
    String building;
    unsigned int apartment;
    String receiver_index;
} Address, *p_address;

typedef struct Mail {
    Address receiver_address;
    float parcel_weight;
    String postal_id;
    String creation_time;
    String delivery_time;
} Mail, *p_mail;

typedef struct Post {
    p_address postal_address;
    p_mail parcels;
} Post, *p_post;
#pragma endregion

#pragma region constructors
int address_constructor(p_address result, char* city, char* street, unsigned int house,
                        char* building, unsigned int apartment, char* receiver_index) {
    Address entity;
    String str_city, str_street, str_building, str_receiver_index;
    int i, len;

    if (city == NULL || street == NULL || building == NULL || receiver_index == NULL) return INVALID_PARAMETER;

    if (strlen(city) == 0 || strlen(street) == 0
    || strlen(receiver_index) != 6 || is_every_char_digit(receiver_index)) return INVALID_INPUT;
    for (i = 0, len = strlen(receiver_index); i < len; i++) {
        if (!isdigit(receiver_index[i])) return INVALID_INPUT;
    }

    string_constructor(&str_city, city);
    string_constructor(&str_street, street);
    string_constructor(&str_building, building);
    string_constructor(&str_receiver_index, receiver_index);

    entity.city = str_city;
    entity.street = str_street;
    entity.house = house;
    entity.building = str_building;
    entity.apartment = apartment;
    entity.receiver_index = str_receiver_index;

    *result = entity;

    return OK;
}

int mail_constructor(p_address receiver_address, float parcel_weight,
                     char* postal_id, char* creation_time, char* delivery_time) {
    int i, len;

    if (receiver_address == NULL || postal_id == NULL || creation_time == NULL || delivery_time == NULL
        || parcel_weight < 0 || (len = strlen(postal_id) != 14) || !is_every_char_digit(postal_id)) return INVALID_PARAMETER;

}

int post_constructor() {

}
#pragma endregion

int task_3(int argc, char **argv) {

}