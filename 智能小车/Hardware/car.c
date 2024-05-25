    #include "car.h"

void Car_Init(void){
    PWM_Init();
}

//ÉèÖÃOCC
void Car_SetSpeed(uint8_t left1_speed,uint8_t left2_speed,uint8_t right1_speed,uint8_t right2_speed){
    TIM_SetCompare1(TIM4 ,left1_speed);
    TIM_SetCompare2(TIM4 ,left2_speed);
    TIM_SetCompare3(TIM4 ,right1_speed);
    TIM_SetCompare4(TIM4 ,right2_speed);
}

void Car_Run(int8_t speed , uint16_t time){
    if(speed > 100){
        speed = 100;
    }
    if(speed < 0){
        speed = 0;
    }
    Car_SetSpeed(speed , 0 , speed , 0);
    Delay_ms(time);
    //Car_SetSpeed(0,0,0,0);
    //Delay_ms(time);
}

void Car_Brake(uint16_t time){
    Car_SetSpeed(0,0,0,0);
    Delay_ms(time);
}

void Car_Left(int8_t speed , uint16_t time){
    if(speed > 100){
        speed = 100;
    }
    if(speed < 0){
        speed = 0;
    }
    Car_SetSpeed(0 , 0 , speed , 0);
    Delay_ms(time);
    //Car_SetSpeed(0,0,0,0);
}

void Car_Spin_Left(int8_t speed , uint16_t time){
    if(speed > 100){
        speed = 100;
    }
    if(speed < 0){
        speed = 0;
    }
    Car_SetSpeed(0 , speed , speed , 0);
    Delay_ms(time);
    //Car_SetSpeed(0,0,0,0);
}

void Car_Right(int8_t speed , uint16_t time){
    if(speed > 100){
        speed = 100;
    }
    if(speed < 0){
        speed = 0;
    }
    Car_SetSpeed(speed , 0 , 0 , 0);
    Delay_ms(time);
    //Car_SetSpeed(0,0,0,0);
}

void Car_Spin_Right(int8_t speed , uint16_t time){
    if(speed > 100){
        speed = 100;
    }
    if(speed < 0){
        speed = 0;
    }
    Car_SetSpeed(speed , 0 , 0 , speed);
    Delay_ms(time);
    //Car_SetSpeed(0,0,0,0);
}

void Car_Back(int8_t speed , uint16_t time){
    if(speed > 100){
        speed = 100;
    }
    if(speed < 0){
        speed = 0;
    }
    Car_SetSpeed(0 , speed , 0 , speed);
    Delay_ms(time);
    //Car_SetSpeed(0,0,0,0);
}

