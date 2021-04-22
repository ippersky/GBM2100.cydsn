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

#define BLOCK_SIZE 200 // nombre de donnees par seconde
#define SECONDES 5 // nombre de secondes sur lesquelles on fait le traitement
#define FILTER_TAP_NUM 50

// Coefficients de calibration d'un filtre passe-bas 2 Hz avec matlab
float32_t filter_taps[FILTER_TAP_NUM] = {
0.002085,0.002268,0.002666,0.003295,0.004166,0.005282,0.006642,0.008237,0.010050,0.012062,0.014244,0.016564,0.018984,0.021463,0.023957,0.026419,0.028804,0.031063,0.033154,0.035033,0.036662,0.038008,0.039043,0.039746,0.040101,0.040101,0.039746,0.039043,0.038008,0.036662,0.035033,0.033154,0.031063,0.028804,0.026419,0.023957,0.021463,0.018984,0.016564,0.014244,0.012062,0.010050,0.008237,0.006642,0.005282,0.004166,0.003295,0.002666,0.002268,0.002085
};

float32_t firStateF32[BUFFER_LENGTH+FILTER_TAP_NUM-1];

/*************************************************************************/

/* Cette fonction prend en parametres le signal non-filtré et le signal filtré. Elle filtre le signal
 et place le signal filtré de le buffer Output.
*/
void filtre(uint32_t *Signal, uint32_t *Output){
    
       
    arm_fir_instance_f32 S;
    
    uint32_t *inputF32;
    uint32_t *outputF32;
    
    inputF32 = Signal;
    outputF32 = Output;
    
    uint32_t longueur = BUFFER_LENGTH;
    
    arm_fir_init_f32(&S, FILTER_TAP_NUM, (float32_t*)&filter_taps[0],&firStateF32[0],longueur);
    arm_fir_f32(&S,(float32_t*)inputF32,(float32_t*)outputF32,longueur);

}

/* Cette fonction calcule la frequence cardiaque. Elle prend en parametre
 le buffer rouge (ou infrarouge) filtré et l'intervalle de temps qui nous interesse et retourne le BPM
*/
float32_t HeartRate(uint32_t *Signal, uint16_t temps1, uint16_t temps2){
      
    
    uint16_t compteur = 0; // initialisation du nombre de maximums
    uint16_t i = 0;
    
    // On boucle sur toutes les valeurs du signal et on incremente le compteur
    // lorsqu'un maximum est trouvé
    for (i=temps1; i<temps2; i++)
    {
        if (Signal[i-1]<Signal[i] && Signal[i+1]<Signal[i]){
         compteur++;
        }
    }
    
    // On met toutes les variables en float pour eviter les problemes lors du calcul
    float32_t secondes = SECONDES, minute = 60;
    float32_t bpm = (float32_t)compteur/secondes*minute;
    
    return bpm;
    
}


/* Cette fonction calcule la composante AC du signal. Elle prend en 
 parametre le signal rouge ou infrarouge ainsi que le temps1 et temps2, soient
 soit le debut et la fin d'un echantillon de 1 seconde, et retourne la composante AC
*/
float32_t AC(uint32_t *Signal, uint32_t temps1, uint32_t temps2){

    uint32_t i, compteur = 0;
    uint32_t Input[BLOCK_SIZE]; // initialisation du vecteur Input
    
    for (i=temps1; i<temps2; i++){ // le buffer input doit contenir la partie du signal qui nous interesse
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
/* Cette fonction calcule la composante DC du signal. Elle prend en 
 parametre le signal rouge ou infrarouge ainsi que le temps1 et temps2, soient
 soit le debut et la fin d'un echantillon de 1 seconde, et retourne la composante DC
*/
float32_t DC(uint32_t *Signal, uint32_t temps1, uint32_t temps2){
    
    uint32_t i, compteur = 0;
    uint32_t Input[BLOCK_SIZE];
    
    for (i=temps1; i<temps2; i++){ // le buffer input doit contenir la partie du signal qui nous interesse
    Input[compteur]=Signal[i];   
    compteur++;
    }    
    
    // Calcul de la composante DC du signal avec une moyenne
    
    uint32_t block_size = BLOCK_SIZE;
    float32_t compoDC;
            
    arm_mean_f32((float32_t*)&Input[0],block_size,&compoDC);
    
    return compoDC;
    
}

/* Cette fonction calcule la saturation sanguine en oxygene.
 Elle prend en parametres les signaux rouge et infrarouge ainsi que le temps1 et le temps2,
 soit l'intervalle de 5 secondes. Elle retourne le SpO2 pour 5 secondes
*/
float32_t calculSpO2(uint32_t *InputRed, uint32_t *InputIR, uint16_t temps1, uint16_t temps2){
    
    // Coefficients de calibration
    float32_t a = 1.5958422;
    float32_t b = -34.6596622;
    float32_t c = 118.7;
    
    uint32_t i;
    // initialisation des buffers pour appeler les fonctions AC et DC
    float32_t AC660[SECONDES], DC660[SECONDES], AC880[SECONDES], DC880[SECONDES];
    
    // On appelle 5 fois les fonctions AC et DC pour les signaux rouges et infrarouges, ce qui correspond
    // a une periode de 5 secondes
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
    
    // Calcul de la moyenne des composantes sur la periode de 5 secondes
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
