/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "algorithme.h"
#include "variables.h"

#define LONGUEUR_ECH 1000 // longueur totale des donnees
#define BLOCK_SIZE 200 // nombre de donnees par seconde
#define SECONDES 5 // nombre de secondes sur lesquelles on fait le traitement

#define FILTER_TAP_NUM 50

// 2 Hz avec matlab
float32_t filter_taps[FILTER_TAP_NUM] = {
0.002333,0.002496,0.002881,0.003498,0.004348,0.005432,0.006741,0.008263,0.009982,0.011876,0.013918,0.016077,0.018321,0.020614,0.022917,0.025190,0.027395,0.029493,0.031445,0.033217,0.034776,0.036093,0.037144,0.037909,0.038374,0.038530,0.038374,0.037909,0.037144,0.036093,0.034776,0.033217,0.031445,0.029493,0.027395,0.025190,0.022917,0.020614,0.018321,0.016077,0.013918,0.011876,0.009982,0.008263,0.006741,0.005432,0.004348,0.003498,0.002881,0.002496

};


float32_t firStateF32[LONGUEUR_ECH+FILTER_TAP_NUM-1];

/*************************************************************************/

void filtre(uint32_t *Signal, uint32_t *Output, uint16_t temps1, uint16_t temps2){  // 
    
    //Filtrage le signal avec un filtre passe-bas
    
    uint32_t Input[LONGUEUR_ECH]; // initialiser le vecteur input
    //uint32_t Output[LONGUEUR_ECH]; //initialiser le buffer output
    uint32_t i, compteur=0;
    for (i=temps1; i<temps2; i++){ // On veut que le buffer input contienne soit la premiere moitié ou la deuxième moitié du buffer signal
        Input[compteur]=Signal[i];   
        compteur++;
        }
    
       
    arm_fir_instance_f32 S;
    
    uint32_t *inputF32;
    uint32_t *outputF32;
    
    inputF32 = &Signal[0];
    outputF32 = &Output[0];
    
    uint32_t longueur = LONGUEUR_ECH;
    
    arm_fir_init_f32(&S, FILTER_TAP_NUM, (float32_t*)&filter_taps[0],&firStateF32[0],longueur);
    arm_fir_f32(&S,(float32_t*)inputF32,(float32_t*)outputF32,longueur);
    
    uint16_t j = 0;
    
    
    UART_1_PutString("//////////////////////////////////////////////////////////////////");
    for(j=0; j<LONGUEUR_ECH; j++){
        //printf("%s \n\r", Output[j]);
        char cOutput[6];
        itoa(Output[j], cOutput, 10);
        UART_1_PutString(cOutput);
        UART_1_PutString("\n\r");
        
    }
    
    
    if(j == 1000){
            j=0;
        }
    
}

// Cette fonction calcule la frequence cardiaque. Elle prend en parametre
// le buffer rouge (ou infrarouge) et l'intervalle de temps qui nous interesse et retourne le BPM
float32_t HeartRate(uint32_t *Signal, uint16_t temps1, uint16_t temps2){
    
    
    /*
    //Filtrage le signal avec un filtre passe-bas
    uint32_t Input[LONGUEUR_ECH]; // initialiser le vecteur input
    uint32_t Output[LONGUEUR_ECH]; //initialiser le buffer output
    uint32_t i, compteur=0;
    for (i=temps1; i<temps2; i++){ // On veut que le buffer input contienne soit la premiere moitié ou la deuxième moitié du buffer signal
    Input[compteur]=Signal[i];   
    compteur++;
    }
    
       
    arm_fir_instance_f32 S;
    
    uint32_t *inputF32;
    uint32_t *outputF32;
    inputF32 = &Input[0];
    outputF32 = &Output[0];
    
    uint32_t longueur = LONGUEUR_ECH;
    
    arm_fir_init_f32(&S, FILTER_TAP_NUM, (float32_t*)&filter_taps[0],&firStateF32[0],longueur);
    arm_fir_f32(&S,(float32_t*)inputF32,(float32_t*)outputF32,longueur);
    */

    
    // Calcul de la frequence cardiaque
    
    uint16_t compteur = 0; // initialisation du nombre de maximums
    uint16_t i = 0;
    for (i=0; i<LONGUEUR_ECH; i++)
    {
        if (Signal[i-1]<Signal[i] && Signal[i+1]<Signal[i]){
         compteur++;
        }
    }
    
    float32_t secondes = SECONDES, minute = 60;
    float32_t bpm = (float32_t)compteur/secondes*minute;
    
    return bpm;
    
    
    
}


// Cette fonction calcule la composante AC du signal. Elle prend en 
// parametre le signal rouge ou infrarouge ainsi que le temps1 et temps2, soient
// le debut et la fin d'un echantillon de 1 seconde, et retourne la composante AC
float32_t AC(uint32_t *Signal, uint32_t temps1, uint32_t temps2){

    uint32_t i, compteur = 0;
    uint32_t Input[BLOCK_SIZE]; // initialisation du vecteur Input
    
    for (i=temps1; i<temps2; i++){ // On veut que le buffer input contienne soit la premiere moitié ou la deuxième moitié du buffer signal
    Input[compteur]=Signal[i];   
    compteur++;
    }
    
    uint32_t block_size = BLOCK_SIZE;

    // Calcul de la composante AC
    
    float32_t maximum, minimum, compoAC; // initialisation des variables
    uint32_t indexMax, indexMin;
    compteur = 0;
    
    // Calcul du maximum et du minimum
    arm_max_f32((float32_t*)&Input[0],block_size,&maximum,&indexMax);
    arm_min_f32((float32_t*)&Input[0],block_size,&minimum,&indexMin);
    
    compoAC=maximum-minimum;
    
    return compoAC;
    
}
// Cette fonction calcule la composante DC du signal. Elle prend en 
// parametre le signal rouge ou infrarouge ainsi que le temps1 et temps2, soient
// le debut et la fin d'un echantillon de 1 seconde, et retourne la composante DC
float32_t DC(uint32_t *Signal, uint32_t temps1, uint32_t temps2){
    
    uint32_t i, compteur = 0;
    uint32_t Input[BLOCK_SIZE];
    
    for (i=temps1; i<temps2; i++){ // On veut que le buffer input contienne soit la premiere moitié ou la deuxième moitié du buffer signal
    Input[compteur]=Signal[i];   
    compteur++;
    }    
    
    // Calcul de la composante DC du signal
    
    uint32_t block_size = BLOCK_SIZE;
    float32_t compoDC;
            
    arm_mean_f32((float32_t*)&Input[0],block_size,&compoDC);
    
    return compoDC;
    
}

// Cette fonction calcule la saturation sanguine en oxygene.
// Elle prend en parametres les signaux rouge et infrarouge ainsi que le temps1 et le temps2,\
// soient 0 ou 1000 pour traiter la premiere moitié du buffer ou 1000 et 2000 pour traiter
// la deuxieme moitié du buffer. Elle retourne le SpO2 pour 5 secondes
float32_t calculSpO2(uint32_t *InputRed, uint32_t *InputIR, uint16_t temps1, uint16_t temps2){
    
    // Coefficients de calibration
    float32_t a = 1.5958422;
    float32_t b = -34.6596622;
    float32_t c = 118.7;
    
    uint32_t i;
    // initialisation des buffers pour appeler les fonctions AC et DC
    float32_t AC660[SECONDES], DC660[SECONDES], AC880[SECONDES], DC880[SECONDES];
    
    // On appelle les fonctions AC et DC pour les signaux rouges et infrarouges pour une periode
    // de 5 secondes
    uint32_t fech = BLOCK_SIZE;
    
    for(i=temps1/BLOCK_SIZE; i<temps2/BLOCK_SIZE; i++){
        AC660[i]=AC(&InputRed[0],fech*i, fech*i + fech);
        DC660[i]=DC(&InputRed[0],fech*i, fech*i + fech);
        AC880[i]=AC(&InputIR[0],fech*i, fech*i + fech);
        DC880[i]=DC(&InputIR[0],fech*i, fech*i + fech);
    }
    
    // initialisation des variables utilisees pour calculer la saturation
    float32_t R;
    float32_t mAC660, mDC660, mAC880, mDC880;
    
    // Calcul de la moyenne des composantes sur une periode de 5 secondes
    arm_mean_f32(&AC660[0],SECONDES,&mAC660);
    arm_mean_f32(&DC660[0],SECONDES,&mDC660);
    arm_mean_f32(&AC880[0],SECONDES,&mAC880);
    arm_mean_f32(&DC880[0],SECONDES,&mDC880);
    
    // Calcul du ratio R
    R=(mAC660/mDC660)/(mAC880/mDC880);
    
    // Calcul de la saturation sur une periode de 5 secondes
    float32_t SpO2=a*pow(R,2)+b*R+c;
    
    return SpO2;
}



/* [] END OF FILE */
