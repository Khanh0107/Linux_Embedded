#include "storage_data.h"
#include <sqlite3.h>

void *storage_data() {
    sqlite3 *db;
    char sql[256];
    char *err_msg = NULL;

    // 1. Mở DB
    if (sqlite3_open("sensor.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    // Tạo bảng nếu chưa có
    const char *create_table_sql  =
        "CREATE TABLE IF NOT EXISTS SensorData ("
        "timestamp INTEGER,"
        "id INTEGER,"
        "temperature INTEGER);";

    if (sqlite3_exec(db, create_table_sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL error (create table): %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NULL;
    }
    printf("Table created or already exists.\n");

    // Vòng lặp lưu dữ liệu
    while (1) {
        pthread_mutex_lock(&mutex);
        while (condition == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        for (int i = 0; i < 10; i++) {
            snprintf(sql, sizeof(sql),
                "INSERT INTO SensorData (timestamp, id, temperature) "
                "VALUES (%d, %d, %d);",   data_buff[i].id, data_buff[i].time, data_buff[i].temperature); // timestamp là số nguyên
                

            if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
                fprintf(stderr, "SQL error (insert): %s\n", err_msg);
                sqlite3_free(err_msg);
            } else {
                printf("Inserted row %d\n", i);
            }
        }

        condition = 0;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    // Đóng DB (thực tế sẽ không tới đây nếu while(1))
    sqlite3_close(db);
    return NULL;
}
