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
    //int is_delivered;
} Mail, *p_mail;

typedef struct Post {
    p_address postal_address;
    p_mail *parcels;
    size_t parcels_count;
} Post, *p_post;
#pragma endregion

void print_options();
int print_mail_info(p_mail mail);
int print_address(p_address address, char first_char);
int search_mail(p_mail *result, p_mail parcels, char *key, size_t parcels_len, int *is_found);
int delivery_mail(p_post *post, p_mail mail);
int remove_mail(p_post *post, p_mail mail, int *is_successful);

#pragma region inits
int address_init(p_address result, char* city, char* street, unsigned int house,
                        char* building, unsigned int apartment, char* receiver_index) {
    Address entity;
    String str_city, str_street, str_building, str_receiver_index;

    if (city == NULL || street == NULL || building == NULL || receiver_index == NULL) return INVALID_PARAMETER;

    if (strlen(city) == 0 || strlen(street) == 0
    || strlen(receiver_index) != 6 || !is_every_char_digit(receiver_index)) return INVALID_INPUT;

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

int mail_init(p_mail result, p_address receiver_address, float parcel_weight,
                     char* postal_id, char* creation_time, char* delivery_time) {
    Mail entity;
    String str_postal_id, str_creation_time, str_delivery_time;

    if (receiver_address == NULL || postal_id == NULL || creation_time == NULL || delivery_time == NULL
        || parcel_weight < 0 || strlen(postal_id) != 14 || !is_every_char_digit(postal_id)
        || !is_valid_datetime_str(creation_time) || !is_valid_datetime_str(delivery_time)) return INVALID_PARAMETER;

    string_constructor(&str_postal_id, postal_id);
    string_constructor(&str_creation_time, creation_time);
    string_constructor(&str_delivery_time, delivery_time);

    entity.receiver_address = *receiver_address;
    entity.parcel_weight = parcel_weight;
    entity.postal_id = str_postal_id;
    entity.creation_time = str_creation_time;
    entity.delivery_time = str_delivery_time;

    *result = entity;

    return OK;
}

int post_init(p_post result, p_address postal_address, p_mail parcels, size_t parcels_len) {
    if (result == NULL || postal_address == NULL || parcels == NULL) return INVALID_PARAMETER;

    result->postal_address = postal_address;
    result->parcels = (p_mail*) malloc(sizeof(p_mail) * parcels_len);
    if (result->parcels == NULL) return MEMORY_ALLOCATION_ERROR;
    for (int i = 0; i < parcels_len; i++) {
        result->parcels[i] = NULL;
    }
    //memcpy(*result->parcels, parcels, parcels_len * sizeof(Mail));
    result->parcels_count = parcels_len;

    return OK;
}

#pragma endregion

int task_3(int argc, char **argv) {
    Post post;
    Address postal_address, mail1_address, mail2_address, mail3_address;
    Mail mail_1, mail_2, mail_3;
    p_mail parcels, p_parcels, post_parcels, found_mail;
    p_post p_post;
    int user_choice, is_found, is_successful, i;
    int const PARCELS_LEN = 3;
    char *search_key;

    if ((search_key = (char*) malloc(BUFSIZ * sizeof(char))) == NULL) return MEMORY_ALLOCATION_ERROR;
    // mocks
    address_init(&postal_address, "Moscow", "Donskaya", 39, "K1", 718, "123456");
    address_init(&mail1_address, "Moscow", "Shaboldovka", 52, "K3", 128, "337337");
    address_init(&mail2_address, "Moscow", "Kosygina", 1, "", 1708, "228228");
    address_init(&mail3_address, "Moscow", "A????? Biom", 5, "1", 228, "525252");

    mail_init(&mail_1, &mail1_address, 140.12f,
              "12345678901234", "17:12:2005 04:21:37", "25:12:2005 12:30:00");
    mail_init(&mail_2, &mail2_address, 15.3333f,
              "43210987654321", "13:05:2021 10:35:21", "19:05:2021 14:45:00");
    mail_init(&mail_3, &mail3_address, 20.0f,
              "77777777777777", "21:11:2024 15:00:21", "01:05:2021 14:45:00");

    if ((parcels = (p_mail) malloc(sizeof(Mail) * PARCELS_LEN)) == NULL) {
        govnochist('m', postal_address, 'm', mail1_address, 'm', mail2_address, 'm', mail3_address,
                   'm', mail_1, 'm', mail_2, 'm', mail_3, 'm', search_key, 'q', NULL);

        return MEMORY_ALLOCATION_ERROR;
    }

    if ((post_parcels = (p_mail) malloc(sizeof(Mail) * PARCELS_LEN)) == NULL) {
        govnochist('m', postal_address, 'm', mail1_address, 'm', mail2_address, 'm', mail3_address,
                   'm', mail_1, 'm', mail_2, 'm', mail_3, 'm', parcels, 'm', search_key, 'q', NULL);

        return MEMORY_ALLOCATION_ERROR;
    }

    post.parcels = (p_mail*) malloc(sizeof(p_mail) * PARCELS_LEN);
    if (post.parcels == NULL) {
        govnochist('m', postal_address, 'm', mail1_address, 'm', mail2_address, 'm', mail3_address, 'm', post_parcels,
                   'm', mail_1, 'm', mail_2, 'm', mail_3, 'm', parcels, 'm', search_key, 'q', NULL);

        return MEMORY_ALLOCATION_ERROR;
    }

    post_init(&post, &postal_address, post_parcels, PARCELS_LEN);
    printf("post alright");

    p_parcels = parcels;
    *p_parcels++ = mail_1;
    *p_parcels++ = mail_2;
    *p_parcels = mail_3;

    print_options();
    p_post = &post;
    do {
        printf("\nChoose option: ");
        scanf("%d", &user_choice);

        switch (user_choice) {
            case 0:
                break;
            case 1:
                if (found_mail == NULL) {
                    printf("Sorry, you have to search for a record first.\n");
                    break;
                }

                delivery_mail(&p_post, found_mail);
                printf("Delivered successfully!");

                printf("\n\n");
                break;
            case 2:
                if (found_mail == NULL) {
                    printf("Sorry, you have to search for a record first.\n");
                    break;
                }

                remove_mail(&p_post, found_mail, &is_successful);
                if (!is_successful) {
                    printf("Sorry, this parcel is not in the post yet.\n");
                    break;
                }

                printf("Removed successfully!");
                break;
            case 3:
                found_mail = NULL;
                printf("Enter postal ID: ");
                scanf("%s", search_key);
                search_mail(&found_mail, parcels, search_key, PARCELS_LEN, &is_found);

                if (!is_found) {
                    printf("Sorry, no record was found.\n");
                    break;
                }

                printf("Found a record!\n");
                print_mail_info(found_mail);
                break;
        }
    } while (user_choice != 0);

    printf("Success!");

    return OK;
}

void print_options() {
    printf("1. Delivery previously found mail to post;\n2. Remove previously found mail from post;\n3. Search a mail by ID;");
    printf("\n4. Show every sent mail;\n5. Show every delivered mail;\n6. Search for every overdue mail;\n0. Exit\n");
}

int search_mail(p_mail *result, p_mail parcels, char *key, size_t parcels_len, int *is_found) {
    size_t i;
    int cmp_result;
    String key_str;

    if (result == NULL || parcels == NULL || key == NULL) return INVALID_PARAMETER;

    string_constructor(&key_str, key);

    for (i = 0; i < parcels_len; i++) {
        if (string_cmp(&cmp_result, &parcels[i].postal_id, &key_str) == 0) {
            *result = &parcels[i];
            *is_found = 1;
            return OK;
        }
    }

    *is_found = 0;

    return OK;
}

int delivery_mail(p_post *post, p_mail mail) {
    size_t i;

    if (post == NULL || mail == NULL) return INVALID_PARAMETER;

    for (i = 0; i < (*post)->parcels_count; i++) {
        if ((*post)->parcels[i] != NULL) {
            printf("retard 1\n");
            continue;
        }

        (*post)->parcels[i] = mail;
        break;
    }

    return OK;
}

int remove_mail(p_post *post, p_mail mail, int *is_successful) {
    size_t i;
    int cmp_result;

    if (post == NULL || mail == NULL) return INVALID_PARAMETER;

    for (i = 0; i < (*post)->parcels_count; i++) {
        if ((*post)->parcels[i] == NULL) continue;
        string_cmp(&cmp_result, &(*post)->parcels[i]->postal_id, &mail->postal_id);
        if (cmp_result != 0) continue;

        (*post)->parcels[i] = NULL;
        *is_successful = 1;

        return OK;
    }

    *is_successful = 0;

    return OK;
}

int print_mail_info(p_mail mail) {
    if (mail == NULL) return INVALID_PARAMETER;

    printf("Postal ID: ");
    print_string(&mail->postal_id, '\n');
    printf("Receiver address:\n");
    print_address(&mail->receiver_address, '\t');
    printf("Parcel weight: %f\n", mail->parcel_weight);
    printf("Creation time: ");
    print_string(&mail->creation_time, '\n');
    printf("Delivery time: ");
    print_string(&mail->delivery_time, '\n');

    return OK;
}

int print_address(p_address address, char first_char) {
    if (address == NULL) return INVALID_PARAMETER;

    printf("%cReceiver index: ", first_char);
    print_string(&address->receiver_index, '\n');
    printf("%cCity: ", first_char);
    print_string(&address->city, '\n');
    printf("%cStreet: ", first_char);
    print_string(&address->street, '\n');
    printf("%cHouse: %u\n", first_char, address->house);
    printf("%cBuilding: ", first_char);
    print_string(&address->building, '\n');
    printf("%cApartment: %u\n", first_char, address->apartment);

    return OK;
}