
#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h> 

void writeRegister(uint8_t slave_address, char register_address, char data) {
    // Constants for sensor.
    const uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_2500;

    if (!bcm2835_init()) {
        printf("bcm2835_init failed. Are you running as root??\n");
        return;
    }

    bcm2835_i2c_setSlaveAddress(slave_address);
    bcm2835_i2c_setClockDivider(clk_div);
    
    char buf[]={register_address, data};
    bcm2835_i2c_write(buf,2);

    bcm2835_close(); 
}

char ReadRegister(uint8_t slave_address, char register_address) {
    // Constants for sensor.
    const uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_2500;

    if (!bcm2835_init()) {
        printf("bcm2835_init failed. Are you running as root??\n");
        return 'n';
    }

    bcm2835_i2c_setSlaveAddress(slave_address);
    bcm2835_i2c_setClockDivider(clk_div);

    char buf[]={register_address};
    bcm2835_i2c_write(buf,1);
    bcm2835_i2c_read(buf,1);

    bcm2835_close();
    
    return (char)buf[0];
}

void initTime() {
    char data[7] = {0x10, 0x00, 0x52, 0x01, 0x01, 0x01, 0x01};
    for (int i = 0; i < 7; i++) {
        writeRegister(0x68, 0x00 + i, data[i]);
    }
    char data2[2] = {0x00, 0x00};
    writeRegister(0x4d, 0x00, data2[0]);
    writeRegister(0x4d, 0x01, data2[1]);
    
}

char* ReadTime() {
    char *ans = (char*)malloc(sizeof(char) * 8);
    for (int i = 0; i < 8; i++) {
        ans[i] = ReadRegister(0x68, 0x00 + i);
    }

    return ans;
}

char* toString(int num) {
    char *aux = (char*)malloc(sizeof(char) * 5);
    strcpy(aux, "");
    sprintf(aux, "%02d", num);
    return aux;
}

char mat[4][70];
void writeToFile(char *str, char temp) {
    static int index = 0;
    static int size = 1;
    if (temp){
        strcpy(mat[0], str);
    } else {
        strcpy(mat[1 + index], str);
        index = (index + 1) % 3;
        size = (size == 4 ? 4 : size + 1);
    }

    // Creating file pointer to work with files.
    FILE *fptr;

    // Opening file in writing mode
    fptr = fopen("Log.txt", "w");

    // exiting program.
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }

    for (int i = 0; i < size; i++){
        fprintf(fptr, "%s", mat[i]);
    }
    
    fclose(fptr);
}

void printTemp(int temp) {
    char printTemp[60] = "RECEIVER> Temperature: ";
    strcat(printTemp, toString(temp));
    strcat(printTemp, "°C\n");
    writeToFile(printTemp, 0x01);
}

int bcdToInt(int num) {
    int ans = (num >> 4) * 10 + (num & 15);
    return ans;
}

int readTemp() {
    return (int)ReadRegister(0x4d, 0x00);
}

char *getDay(int num){
    char *aux = (char*)malloc(sizeof(char) * 7);
    strcpy(aux, "");
    switch (num) {
        case 1:
            strcat(aux, "Mon");
            break;
        case 2:
            strcat(aux, "Tue");
            break;
        case 3:
            strcat(aux, "Wed");
            break;
        case 4:
            strcat(aux, "Thur");
            break;
        case 5:
            strcat(aux, "Fri");
            break;
        case 6:
            strcat(aux, "Sat");
            break;
        case 7:
            strcat(aux, "Sun");
            break;
        default:
            break;
    }

    return aux;
}

char* getRecord(char* buffer){

    char *aux = (char*)malloc(sizeof(char) * 30);
    strcpy(aux, "");

    // Date
    strcat(aux, toString(bcdToInt(buffer[4])));
    strcat(aux, "/");
    strcat(aux, toString(bcdToInt(buffer[5])));
    strcat(aux, "/");
    strcat(aux, toString(bcdToInt(buffer[6])));
    strcat(aux, " ");

    // DAY
    strcat(aux, getDay(bcdToInt(buffer[3])));
    strcat(aux, " ");
    
    // Print hours.
    strcat(aux, toString(bcdToInt(buffer[2] & 0x1F)));
    strcat(aux, ":");

    // Print minutes.
    strcat(aux, toString(bcdToInt(buffer[1])));
    strcat(aux, ":");

    // Print seconds.
    strcat(aux, toString(bcdToInt(buffer[0] & 0x7F)));

    // Print AM OR PM. Checking the bit 5.
    if (buffer[2] & 32){
        strcat(aux, " PM\n");
    } else {
        strcat(aux, " AM\n");
    }
    
    return aux;
}

void printRecord(int numRecord, char* buffer){
    char printAux[60] = "RECEIVER> Record ";
    strcat(printAux, toString(numRecord));
    strcat(printAux, " : ");
    strcat(printAux, getRecord(buffer));
    writeToFile(printAux, 0x00);
}

int main(int argc, char *argv[])
{
    initTime();
    int temperature, numRecord = 1;

    temperature = readTemp();
    printf("Temp = %d\n", temperature);
    printTemp(temperature);
    

    time_t base_time;
    base_time = time(NULL); 
    long long bSeconds = base_time - 10;

    while (1) {
        // Check if 10 seconds have passed.
        base_time = time(NULL); 
        long long bNow = base_time;
        if (abs(bNow - bSeconds) >= 10) {
            // TODO: Read with I2C.
            char* buffer = ReadTime();
            if (buffer == NULL){
                return 1;
            }
            
            // Print record.
            printRecord(numRecord++, buffer);

            // Update base time.
            bSeconds = bNow;
        }
        
        temperature = readTemp();
        printf("Temp = %d\n", temperature);
        if (temperature >= 30) {
            printTemp(temperature);
        }

        sleep(1); // Sleep for 1 second.
    }

    return 0;
}
