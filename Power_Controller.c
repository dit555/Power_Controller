#include "RIMS.h"
// Written by: Dumitru Chiriac
// Email: dchir002@ucr.edu
// SID: 862126186
// Write up link: https://docs.google.com/document/d/1fw4FTZtxBqf0Z4JPjWuE2VEgnTkqyczkfK1PVaIq8e0/edit?usp=sharing
// Demo Link: https://youtu.be/3Ub69S2IddA


//Star destroyer power systems controller
//Allows commmands to be executed on the
//basis of sufficient power.
//assumes command is competent.
//No cannon reasorces on power usage
//so percentage used is asigned arbitrarily
//on what seems like a 'logical' amount.

//timer
unsigned char wait = 0;
void TimerISR(){
    wait = 1;
}

//state declarations (20 total):
enum LS_States {LS_Start, LS_On, LS_Off} LS_State; //life support power usage
enum AT_States {AT_Start, AT_Idle, AT_Detect, AT_Aim, AT_Fire} AT_State; //Attack power usage
enum EG_States {EG_Start, EG_Idle, EG_Move, EG_AfterBurn, EG_Jump, EG_Off} EG_State; //Engine power usage
enum DS_States {DS_Start, DS_Idle, DS_Combat, DS_Off} DS_State; //Deflector sheild power usage
enum PM_States {PM_Start, PM_Calc} PM_State; //Power Manager, uses negligeble power, calculates usage


//Functition stubs:
void TickFct_LS();
void TickFct_AT();
void TickFct_EG();
void TickFct_DS();
void TickFct_PM();

//variables:
static char TOT_POWER = 100; //total power %
char totCons = 0; //sum of power consumption
char powerLS = 0;
char powerAT = 0;
char powerEG = 0;
char powerDS = 0;

//---------------------------------------------------

void main(){
   //timer init
   const unsigned int period = 1000; 
   //1 s period for testing
   //In the final product the period will be much faster
   TimerSet(period);
   TimerOn();
   
   B = TOT_POWER;
   LS_State = LS_Start;
   AT_State = AT_Start;
   EG_State = EG_Start;
   DS_State = DS_Start;
   PM_State = PM_Start;

   while (1) { 
      TickFct_LS();
      TickFct_AT();
      TickFct_EG();
      TickFct_DS();
      TickFct_PM();
      
      while(!wait);
      wait = 0;
      
   }
}


//SM implementaition
void TickFct_LS (){
    switch(LS_State){//transitions
        case LS_Start: LS_State = LS_On; break;
        case LS_On: LS_State = (!A0) ? LS_On : LS_Off; break;
        case LS_Off: LS_State = (!A0) ? LS_On : LS_Off; break;
        default:
            LS_State = LS_Start;
    }
    
    switch(LS_State){
        case LS_On: powerLS = 5; break;
        case LS_Off: powerLS = 0; break;
        default:
            break;
    }
}

void TickFct_AT (){
    switch(AT_State){ //transitions
        case AT_Start: AT_State = AT_Idle; break;
        case AT_Idle: AT_State = (A1 && (B >= 5)) ? AT_Detect : AT_Idle; break;
        case AT_Detect: AT_State = (A1 && (B >= 10)) ? AT_Aim : AT_Idle; break;
        case AT_Aim: AT_State = (A1 && (B >= 20)) ? AT_Fire : AT_Idle; break;
        case AT_Fire: AT_State = AT_Idle; break;
        default:
            AT_State = AT_Start; break;
    }
    
    switch(AT_State){ //actions
        case AT_Idle: powerAT = 0; break;
        case AT_Detect: powerAT = 5; break;
        case AT_Aim: powerAT = 10; break;
        case AT_Fire: powerAT = 20; break;
        default:
            break;
    }
}

void TickFct_EG (){
    switch(EG_State){ //transition
         case EG_Start: EG_State = EG_Idle; break;
         case EG_Idle: 
         if (A2 && !A3 && !A4 && !A6 && (B >= 5)) EG_State = EG_Move;
         else if (A6) EG_State = EG_Off;
         else EG_State = EG_Idle;
         break;
         case EG_Move:
             if (A2 && !A3 &&!A4) EG_State = EG_Move;
             else if (A2 && A3 &&!A4 && !A6 && (B >= 20)) EG_State = EG_AfterBurn;
             else EG_State = EG_Idle;
             break;
             
         case EG_AfterBurn: 
            if (A2 && A3 && !A4 && !A6) EG_State = EG_AfterBurn;
            else if (A2 && A3 && A4 && !A6 && (B >= 50)) EG_State = EG_Jump;
            else EG_State = EG_Move;
            break;
            
         case EG_Jump: EG_State = (A2 && A3 && A4 && !A6) ? EG_Jump : EG_AfterBurn; break;
         
         case EG_Off: EG_State = (A6) ? EG_Off : EG_Idle; break;
         
         default:
            EG_State = EG_Start;
            break;
    }
    
    switch(EG_State){ //actions
        case EG_Idle: powerEG = 1; break;
        case EG_Move: powerEG = 5; break;
        case EG_AfterBurn: powerEG = 20; break;
        case EG_Jump: powerEG = 50; break;
        case EG_Off: powerEG = 0; break;
        default:
            break;
    }
}

void TickFct_DS (){
    switch(DS_State){ //transitions
        case DS_Start: DS_State = DS_Idle; break;
        case DS_Idle:
            if (!A5 && A7) DS_State = DS_Off;
            else if(A5 && !A7 && (B >= 40)) DS_State = DS_Combat;
            else DS_State = DS_Idle;
            break;
        
        case DS_Combat: DS_State = (A5 && !A7) ? DS_Combat : DS_Idle; break;
        case DS_Off: DS_State = (!A5 && A7) ? DS_Off : DS_Idle; break;
        default:
            DS_State = DS_Idle; 
            break;
     }
     
     switch(DS_State){
         case DS_Idle: powerDS = 10; break;
         case DS_Combat: powerDS = 40; break;
         case DS_Off: powerDS = 0; break;
         default: 
            break;
     }
}

void TickFct_PM (){
    totCons = 0; //reset just in case
    
    switch(PM_State){ //transitions
        case PM_Start: PM_State = PM_Calc; break;
        case PM_Calc: PM_State = PM_Calc; break;
        default:
            PM_State = PM_Start;
            break;
    }
    
    switch(PM_State){//actions
         case PM_Calc:
            totCons = powerLS + powerAT + powerEG + powerDS;
            B = TOT_POWER - totCons;
            break;
         default:
            break;
    }
    
}
