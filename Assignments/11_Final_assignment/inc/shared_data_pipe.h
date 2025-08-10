
struct data_sensor {
    int id;
    int time;
    int temperature;
};

void shr_data_pipe(struct data_sensor *data);
