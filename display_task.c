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

#include "display_task.h"


uint8 imageBufferCache[CY_EINK_FRAME_SIZE] = {0};
#define VECTEUR_AFFICHAGE_LENGHT    260  // car il y a 264 pixels en y 

int16_t vecteurConverti[BUFFER_LENGTH] = {};  
int16_t vecteurAffichage[260] = {}; 
size_t longueurData = BUFFER_LENGTH;  //sizeof(vecteurData)/sizeof(uint32_t); // 160
size_t indexDebut = 200;
size_t espacement = 3;      // longeurData/nbrePixelY ; 1000/264 = 3.79


// 0 a 255
uint8_t courantLEDrouge = 175;
uint8_t courantLEDir = 175;

uint8_t flagAlarmeBPM = OFF;
uint8_t flagAlarmeAcclerometre = ON;

float32_t borneMaxBPM = 130;
float32_t borneMinBPM = 60;

uint8_t currentCourbe = RED;
//////////////////////////////////////////////////////////////////////

void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle)
{
    cy_eink_frame_t* pEmwinBuffer;

    /* Get the pointer to Emwin's display buffer */
    pEmwinBuffer = (cy_eink_frame_t*)LCD_GetDisplayBuffer();

    /* Update the EInk display */
    Cy_EINK_ShowFrame(imageBufferCache, pEmwinBuffer, updateMethod, powerCycle);

    /* Copy the EmWin display buffer to the imageBuffer cache*/
    memcpy(imageBufferCache, pEmwinBuffer, CY_EINK_FRAME_SIZE);
}


void ClearScreen(void)
{
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}


void DisplayInit(void){
    
    /* Initialize emWin Graphics */
    GUI_Init();

    /* Start the eInk display interface and turn on the display power */
    Cy_EINK_Start(20);
    Cy_EINK_Power(1);
    
    GUI_SetPenSize(1);
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
}


void convertirVecteurEnInt16(int16_t* vConverti, uint32_t* vData, size_t lData){
    for(int i =0; i < lData; i++){
        vConverti[i] = vData[i]/100;
    }
}

int16_t trouverMinimumVecteur(int16_t* vConverti, size_t lData){
    int16_t min = vConverti[indexDebut];    // Ne commence pas au début, à cause du problème mentionné plus haut
    for(int i = indexDebut; i < lData; i++){       
        if(vConverti[i] < min){
            min = vConverti[i];
        }
    }
    return min;
}

int16_t trouverMaximumVecteur(int16_t* vConverti, size_t lData){
    int16_t max = vConverti[indexDebut];    // Ne commence pas au début, à cause du problème mentionné plus haut
    for(int i = indexDebut; i < lData; i++){
        if(vConverti[i] > max){
            max = vConverti[i];
        }
    }
    return max;
}

void creerVecteurAffichage(int16_t* vConverti, int16_t* vAffichage, size_t lData, size_t espacement){
    
    int16_t min = trouverMinimumVecteur(vConverti, lData);
    int16_t max = trouverMaximumVecteur(vConverti, lData);
    
    int j = 0;
    for(int i = indexDebut; i < lData-21; i+=espacement){   
        // lData-21 : i va ainsi de 200 à 979. 
        // 979 - 200 = 779.
        // nbre de données dans vAffichage = 779/espacement = 779/3 = 260. Cela respecte donc la grandeur du vecteur d'affichage
        
        vAffichage[j] = (150*(vConverti[i]-min))/(max-min); // Courbe : 150 pixels en y
        
        // INVERSION car (0,0) est en haut a gauche (pas en bas a gauche)
        vAffichage[j] = (vAffichage[j]*-1)+150; 
        // Les + grandes valeurs sont maintenant affichees plus bas sur l'ecran
                                                            
        j++;                                                       
    }
}



void afficherMenuPrincipal(uint32_t * vData){
    
    // en argument : int16_t * vConverti, int16_t * vAffichage
    //size_t longueurData = *ptrLongueurData;
    
    
    //if(xSemaphoreTake == TRUE)
    //int longueurAffichage = (longueurData-indexDebut)/espacement;
    GUI_Clear();
    
    convertirVecteurEnInt16(vecteurConverti, vData, longueurData);
    creerVecteurAffichage(vecteurConverti, vecteurAffichage, longueurData, espacement);
    GUI_DrawGraph(vecteurAffichage, VECTEUR_AFFICHAGE_LENGHT, 0, 0);
    
    updateParametres(SPO2, BPM, currentCourbe);
    
    //UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    
    
    //GiveSemaphore
    //}
   

}


//////////////////UPDATE PARAMETRES////////////////////////
void updateParametres(float32_t SPO2, float32_t BPM, uint8_t currentCourbe)  // mettre en argument les variables de bornes et flagAlarme ??
{
    char sSPO2[10];
    gcvtf(SPO2, 4, sSPO2);
    //itoa(SPO2, sSPO2, 4);
    
    char sBPM[10];
    gcvtf(BPM, 4, sBPM);
    //itoa(BPM, sBPM, 4);
    
    
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_DispStringAt("SPO2 : ", 5, 153);
    GUI_DispStringAt(sSPO2, 53, 153);
    GUI_DispStringAt("%", 90, 153);
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_DispStringAt("BPM : ", 125, 153);
    GUI_DispStringAt(sBPM, 168, 153);
    //GUI_DispStringAt("batt/min", 205, 153); // necessaire?
                                            // intensite lED a la place?
     
    if(currentCourbe == RED)
        GUI_DispStringAt("ROUGE", 205, 153);    
    else if (currentCourbe == IR)
        GUI_DispStringAt("IR", 205, 153);    
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    
    if(flagAlarmeBPM == ON){        // on peut mettre ca avant le update
        uint8_t font = 16;
        GUI_SetFont(GUI_FONT_16_1);
        
       if(BPM > borneMaxBPM){
            char sBorneMax[10];
            gcvtf(borneMaxBPM, 4, sBorneMax);
            
            GUI_Clear();
            GUI_DispStringAt("ATTENTION", 10, 10);
            GUI_DispStringAt("La frequence cardiaque actuelle", 10, 10+font);
            GUI_DispStringAt(sBPM, 10, 10+(2*font));
            GUI_DispStringAt("BPM", 60, 10+(2*font));
            GUI_DispStringAt("est au dessus de la borne maximale", 10, 10+(3*font));
            GUI_DispStringAt(sBorneMax, 10, 10+(4*font));
            GUI_DispStringAt("BPM", 60, 10+(4*font));
            
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
        }
        else if(BPM < borneMinBPM){
            char sBorneMin[10];
            gcvtf(borneMinBPM, 4, sBorneMin);
            
            GUI_Clear();
            GUI_DispStringAt("ATTENTION", 10, 10);
            GUI_DispStringAt("La frequence cardiaque actuelle", 10, 10+font);
            GUI_DispStringAt(sBPM, 10, 10+(2*font));
            GUI_DispStringAt("BPM", 60, 10+(2*font));
            GUI_DispStringAt("est en dessous de la borne minimale", 10, 10+(3*font));
            GUI_DispStringAt(sBorneMin, 10, 10+(4*font));
            GUI_DispStringAt("BPM", 60, 10+(4*font));
            
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
        
        }
               
    
    }
    
    if(flagAlarmeAcclerometre == ON){
        
        if(flagMOUVEMENT == ON){
            uint8_t font = 16;
            GUI_SetFont(GUI_FONT_16_1);
            
            GUI_Clear();
            GUI_DispStringAt("ATTENTION", 10, 10);
            GUI_DispStringAt("Mesure invalide!", 10, 10+font);
            GUI_DispStringAt("Veuillez garder votre doigt", 10, 10+(2*font));
            GUI_DispStringAt("immobile lors de l'acquisition", 10, 10+(3*font));
            
            
            flagMOUVEMENT = OFF;
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
        }
    }
    
    
}






//////////////////AFFICHER MENU SECONDAIRE-TERTIARE////////////////////////
void afficherMenuSecondaire(uint8_t * ptrOptionPresent){

    uint8_t font = 20;
    GUI_Clear();
    GUI_SetFont(GUI_FONT_20_1);
    GUI_DispStringAt("1. Modifier le courant des", 40, 10);
    GUI_DispStringAt("LEDs", 40+font, 10+font);
    
    GUI_DispStringAt("2. Modifier les bornes de", 40, 10+(2*font));
    GUI_DispStringAt("l'alarme BPM", 40+font, 10+(3*font));
    
    GUI_DispStringAt("3. Activer ou desactiver les", 40, 10+(4*font));
    GUI_DispStringAt("alarmes", 40+font, 10+(5*font));
    
    GUI_DispStringAt("4. Changer la courbe", 40, 10+(6*font));
    GUI_DispStringAt("affichee", 40+font, 10+(7*font));
    
    GUI_FillRect(10, 10, 30, 30);
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    
    *ptrOptionPresent = 1;
    
}

void updateMenuSecondaire(uint8_t * ptrOptionPresent){
    
    uint8_t font = 20;
    uint8_t optionPresent = *ptrOptionPresent;
    
    switch (optionPresent) {
        case 1: // curseur passe de option 1 a 2
            GUI_ClearRect(10, 10, 30, 30);
            GUI_FillRect(10, 10+(2*font), 30, 30+(2*font));
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            *ptrOptionPresent = 2;
            break;
        case 2: // curseur passe de option 2 a 3
            GUI_ClearRect(10, 10+(2*font), 30, 30+(2*font));
            GUI_FillRect(10, 10+(4*font), 30, 30+(4*font));
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            *ptrOptionPresent = 3;
            break;
        case 3: // curseur passe de option 3 a 4
            GUI_ClearRect(10, 10+(4*font), 30, 30+(4*font));
            GUI_FillRect(10, 10+(6*font), 30, 30+(6*font));
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            *ptrOptionPresent = 4;
            break;
        case 4: // curseur passe de option 4 a 1
            GUI_ClearRect(10, 10+(6*font), 30, 30+(6*font));
            GUI_FillRect(10, 10, 30, 30);
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            *ptrOptionPresent = 1;
            break;
    }
   

}


/////////// fonctions affichage des menus tertiaires /////////

void afficherMenuTertiaire1(uint8_t * ptrOptionPresent){

    uint8_t font = 20;
    GUI_Clear();
    GUI_SetFont(GUI_FONT_20_1);
    
    GUI_DispStringAt("Modifier le courant de :", 40, 10);    
    GUI_DispStringAt("1. LED rouge", 40, 10+(2*font));
    GUI_DispStringAt("2. LED infrarouge", 40, 10+(4*font));
    GUI_FillRect(10, 10+(2*font), 30, 30+(2*font));
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    *ptrOptionPresent = 1;
        
}

void afficherMenuTertiaire2(uint8_t * ptrOptionPresent){

    uint8_t font = 20;
    GUI_Clear();
    GUI_SetFont(GUI_FONT_20_1);
    
    GUI_DispStringAt("Bornes de l'alarme BPM :", 40, 10);    
    GUI_DispStringAt("1. Modifier borne maximale", 40, 10+(2*font));
    GUI_DispStringAt("2. Modifier borne minimale", 40, 10+(4*font));
    GUI_FillRect(10, 10+(2*font), 30, 30+(2*font));
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    *ptrOptionPresent = 1;
        
}


void afficherMenuTertiaire3(uint8_t * ptrOptionPresent){

    uint8_t font = 20;
    GUI_Clear();
    GUI_SetFont(GUI_FONT_20_1);
    
    GUI_DispStringAt("Activer ou desactiver :", 40, 10);    
    GUI_DispStringAt("1. Alarme Accelerometre", 40, 10+(2*font));
    GUI_DispStringAt("2. Alarme BPM", 40, 10+(4*font));
    GUI_FillRect(10, 10+(2*font), 30, 30+(2*font));
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    *ptrOptionPresent = 1;
        
}

void afficherMenuTertiaire4(){

    uint8_t font = 20;
    GUI_Clear();
    GUI_SetFont(GUI_FONT_20_1);


    GUI_DispStringAt("Presentement affichee :", 20, 10); 
    if(currentCourbe == RED){
        GUI_DispStringAt("ROUGE", 20, 10+font);
    }
    else if(currentCourbe == IR){
        GUI_DispStringAt("IR", 20, 10+font);
    }
    
    GUI_DispStringAt("Afficher courbe d'absorption :", 20, 10+(2*font)); 
        
    GUI_SetFont(GUI_FONT_20B_1);
    GUI_DispStringAt("Rouge", 20, 10+(5*font)); 
    GUI_DispStringAt("Infrarouge", 145, 10+(5*font));
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
        
}

///////// fonction de update menus tertiaires ///////

void updateMenuTertiaire(uint8_t * ptrOptionPresent){
    
    uint8_t font = 20;
    uint8_t optionPresent = *ptrOptionPresent;

    switch (optionPresent) {
        case 1: // curseur passe de option 1 a 2
            GUI_ClearRect(10, 10+(2*font), 30, 30+(2*font));
            GUI_FillRect(10, 10+(4*font), 30, 30+(4*font));
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            *ptrOptionPresent = 2;
            break;
        case 2: // curseur passe de option 2 a 1 
            GUI_ClearRect(10, 10+(4*font), 30, 30+(4*font));
            GUI_FillRect(10, 10+(2*font), 30, 30+(2*font));
            UpdateDisplay(CY_EINK_FULL_4STAGE, true);
            *ptrOptionPresent = 1;
            break;
    }
}

////////// fonctions affichage des menus quaternaires //////////

void afficherMenuQuat1(uint8_t * ptrCourantLED, uint8_t * ptrOptionTertiaire){

    uint8_t font = 20;
    GUI_Clear();
    GUI_SetFont(GUI_FONT_20_1);
    
    if(*ptrOptionTertiaire == 1){
        GUI_DispStringAt("Courant LED rouge :", 40, 10);  
        GUI_DispStringAt("Bonds de 5 mA", 40, 10+(6*font));
        GUI_DispStringAt("Intervalle : 0 a 51 mA", 40, 10+(7*font));
    }
    else if(*ptrOptionTertiaire == 2){
        GUI_DispStringAt("Courant LED infrarouge :", 40, 10); 
        GUI_DispStringAt("Bonds de 5 mA", 40, 10+(6*font));
        GUI_DispStringAt("Intervalle : 0 a 40 mA", 40, 10+(7*font));
    }

    
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_DispStringAt("-", 40, 3+(3*font));
    GUI_DispStringAt("+", 160, 5+(3*font));
    

    GUI_SetFont(GUI_FONT_20_1);
    uint8_t courantHEX = *ptrCourantLED;
    float courantDEC = ((float)courantHEX*51)/255;
    
    char sCourantDEC[10];
    gcvt(courantDEC, 4, sCourantDEC);  
    GUI_DispStringAt(sCourantDEC, 90, 10+(3*font));
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);

}

void afficherMenuQuat2(float32_t * ptrBorneBPM, uint8_t * ptrOptionTertiaire){
    
    uint8_t font = 20;
    GUI_Clear();
    GUI_SetFont(GUI_FONT_20_1);

    if(*ptrOptionTertiaire == 1){
        GUI_DispStringAt("Borne maximale BPM :", 40, 10);   
    }
    else if(*ptrOptionTertiaire == 2){
        GUI_DispStringAt("Borne minimale BPM :", 40, 10); 
    }
    
    GUI_DispStringAt("Bonds de 5 BPM", 40, 10+(6*font));
    GUI_DispStringAt("Intervalle : 25 a 250 BPM", 40, 10+(7*font));
    
    GUI_SetFont(GUI_FONT_32B_1);
    GUI_DispStringAt("-", 40, 3+(3*font));
    GUI_DispStringAt("+", 160, 5+(3*font));
    
    GUI_SetFont(GUI_FONT_20_1);
    float32_t borneBPM = *ptrBorneBPM;
    char sBorneBPM[10];
    gcvtf(borneBPM, 4, sBorneBPM);   
    GUI_DispStringAt(sBorneBPM, 90, 10+(3*font));
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    
}

void afficherMenuQuat3(uint8_t * ptrFlagAlarme, uint8_t * ptrOptionTertiaire){
    
    uint8_t font = 20;
    GUI_Clear();
    GUI_SetFont(GUI_FONT_20_1);

    if(*ptrOptionTertiaire == 1){
        GUI_DispStringAt("Alarme Accelerometre :", 20, 10);   
    }
    else if(*ptrOptionTertiaire == 2){
        GUI_DispStringAt("Alarme BPM :", 20, 10); 
    }
    
    if(*ptrFlagAlarme == ON){
        GUI_DispStringAt("Presentement ACTIVEE", 20, 10+font);
    }
    else if(*ptrFlagAlarme == OFF){
        GUI_DispStringAt("Presentement DESACTIVEE", 20, 10+font);
    }
    
    GUI_SetFont(GUI_FONT_20B_1);
    GUI_DispStringAt("Activer", 20, 10+(4*font)); 
    GUI_DispStringAt("Desactiver", 125, 10+(4*font));
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);

}






////////////////////////// fonction du Task ////////////////////////////

void Task_AffichageGraphique(void *data){
   
    //int32_t * vecteurData = (int32_t *)data;   
    // si Task prend en argument le vecteur data original 
    
    DisplayInit();


    
    uint8_t optionMenuSecondaire = 0;
    uint8_t optionMenuTertiaire = 0;
    //uint8_t optionMenuTertiare2 = 0;
    //uint8_t optionMenuTertiare3 = 0;
    //uint8_t optionMenuTertiare4 = 0;
    
    page_data_t currentPage = MENU_PRINCIPAL;
    
    touch_data_t touchData;
    
    BaseType_t rtosApiResult; 
    
    uint32_t * vData = filteredIR;
    afficherMenuPrincipal(vData);
   
    
    for(;;){
    
        rtosApiResult = xQueueReceive(touchDataQ, &touchData, portMAX_DELAY);
                
        if (rtosApiResult == pdTRUE)
        {
            vTaskSuspend(xSample);
            
            
    
            if(touchData == BUTTON0_TOUCHED){   //aller a la prochaine page
                switch(currentPage){
                    case MENU_PRINCIPAL:
                        afficherMenuSecondaire(&optionMenuSecondaire);
                        currentPage = MENU_SECONDAIRE;
                        break;
                    case MENU_SECONDAIRE:
                        if(optionMenuSecondaire == 1){
                            afficherMenuTertiaire1(&optionMenuTertiaire);
                            currentPage = MENU_TERTIAIRE_1;
                            break;
                        }
                        else if(optionMenuSecondaire == 2){
                            afficherMenuTertiaire2(&optionMenuTertiaire);
                            currentPage = MENU_TERTIAIRE_2;
                            break;
                        }
                        else if(optionMenuSecondaire == 3){
                            afficherMenuTertiaire3(&optionMenuTertiaire);
                            currentPage = MENU_TERTIAIRE_3;
                            break;
                        }
                        else if(optionMenuSecondaire == 4){
                            afficherMenuTertiaire4(&optionMenuTertiaire);
                            currentPage = MENU_TERTIAIRE_4;
                            break;
                        }                     
                    ////// menus tertiaires /////
                    case MENU_TERTIAIRE_1:
                        if (optionMenuTertiaire == 1){
                            afficherMenuQuat1(&courantLEDrouge, &optionMenuTertiaire);
                            currentPage = MENU_QUAT_1_1;    
                            break;
                        }
                        else if (optionMenuTertiaire == 2){
                            afficherMenuQuat1(&courantLEDir, &optionMenuTertiaire);
                            currentPage = MENU_QUAT_1_2;
                            break;
                        }
                
                    case MENU_TERTIAIRE_2:
                        if (optionMenuTertiaire == 1){
                            afficherMenuQuat2(&borneMaxBPM, &optionMenuTertiaire);
                            currentPage = MENU_QUAT_2_1;    
                            break;
                        }
                        else if (optionMenuTertiaire == 2){
                            afficherMenuQuat2(&borneMinBPM, &optionMenuTertiaire);
                            currentPage = MENU_QUAT_2_2;
                            break;
                        }                    
                                              
                    case MENU_TERTIAIRE_3:
                        if (optionMenuTertiaire == 1){
                            afficherMenuQuat3(&flagAlarmeAcclerometre, &optionMenuTertiaire);
                            currentPage = MENU_QUAT_3_1;    
                            break;
                        }
                        else if (optionMenuTertiaire == 2){
                            afficherMenuQuat3(&flagAlarmeBPM, &optionMenuTertiaire);
                            currentPage = MENU_QUAT_3_2;
                            break;
                        }
                                    
                    case MENU_TERTIAIRE_4:
                        /*if (optionMenuTertiaire == 1){
                            vData = filteredIR;
                            afficherMenuPrincipal(vData);
                            vTaskResume(xSample);
                            currentPage = MENU_PRINCIPAL;
                            break;
                        }
                        else if (optionMenuTertiaire == 2){
                            vData = filteredIR;
                            
                            afficherMenuPrincipal(vData);
                            vTaskResume(xSample);
                            currentPage = MENU_PRINCIPAL;
                            break;
                        }*/
                        currentCourbe = RED;
                        vData = filteredRED;
                        afficherMenuTertiaire4();
                        break;
                    /////// menu quat 1 ///////
                        
                    case MENU_QUAT_1_1:
                        if(courantLEDrouge >= 25)
                            courantLEDrouge -= 25;
                        else
                            courantLEDrouge = 0;
                        afficherMenuQuat1(&courantLEDrouge, &optionMenuTertiaire);                        
                        break;
                    case MENU_QUAT_1_2:
                        if(courantLEDir >= 25)
                            courantLEDir -= 25;
                        else 
                            courantLEDir = 0;
                        afficherMenuQuat1(&courantLEDir, &optionMenuTertiaire);
                        break;
                    /////// Quat 2 //////
                    case MENU_QUAT_2_1:
                        borneMaxBPM -= 5;
                        afficherMenuQuat2(&borneMaxBPM, &optionMenuTertiaire);
                        break;
                    case MENU_QUAT_2_2:
                        borneMinBPM -= 5;
                        afficherMenuQuat2(&borneMinBPM, &optionMenuTertiaire);
                        break;
                    /////// Quat 3 ///////
                    case MENU_QUAT_3_1:
                        flagAlarmeAcclerometre = ON;
                        afficherMenuQuat3(&flagAlarmeAcclerometre, &optionMenuTertiaire);
                        break;
                    case MENU_QUAT_3_2:
                        flagAlarmeBPM = ON;
                        afficherMenuQuat3(&flagAlarmeBPM, &optionMenuTertiaire);
                        break;
                    default :
                        break;
                
                }
               
            }
            
            
            else if(touchData == BUTTON1_TOUCHED){   //deplacer le curseur
                
                if (currentPage == MENU_SECONDAIRE)
                    updateMenuSecondaire(&optionMenuSecondaire);
                else if(currentPage == MENU_TERTIAIRE_1 || currentPage == MENU_TERTIAIRE_2 || currentPage == MENU_TERTIAIRE_3){
                    updateMenuTertiaire(&optionMenuTertiaire);
                }
                ////// menu tertiaire 4 (affichage courbe) //////
                else if(currentPage == MENU_TERTIAIRE_4){
                    currentCourbe = IR;
                    vData = filteredIR;
                    afficherMenuTertiaire4();                
                }
                
                ////// menus quat 1 //////
                
                else if(currentPage == MENU_QUAT_1_1){
                    if(courantLEDrouge <= 255-25)
                        courantLEDrouge += 25;      // variable globale, directement accessible          
                    else
                        courantLEDrouge = 255;
                    afficherMenuQuat1(&courantLEDrouge, &optionMenuTertiaire);
                }
                else if(currentPage == MENU_QUAT_1_2){
                    if(courantLEDir <= 200-25)      //Max permis car sinon saturation
                        courantLEDir += 25;
                    else
                        courantLEDir = 200;
                    afficherMenuQuat1(&courantLEDir, &optionMenuTertiaire);                
                }
                /////// Menu Quat 2 ///////
                else if(currentPage == MENU_QUAT_2_1){
                    borneMaxBPM += 5;
                    afficherMenuQuat2(&borneMaxBPM, &optionMenuTertiaire);
                }
                else if(currentPage == MENU_QUAT_2_2){
                    borneMinBPM += 5;
                    afficherMenuQuat2(&borneMinBPM, &optionMenuTertiaire);
                }
                /////// Menu Quat 3 ///////
                else if(currentPage == MENU_QUAT_3_1){
                    flagAlarmeAcclerometre = OFF;
                    afficherMenuQuat3(&flagAlarmeAcclerometre, &optionMenuTertiaire);
                }
                else if(currentPage == MENU_QUAT_3_2){
                    flagAlarmeBPM = OFF;
                    afficherMenuQuat3(&flagAlarmeBPM, &optionMenuTertiaire);
                }
                
                
            }
            
            
            
            
            else if(touchData == BUTTON2_TOUCHED){    // retourner en arriere
                
                if(currentPage == MENU_PRINCIPAL){
                    afficherMenuPrincipal(vData);
                    vTaskResume(xSample);
                }
                if (currentPage == MENU_SECONDAIRE){
                    
                    afficherMenuPrincipal(vData);
                    vTaskResume(xSample);
                    currentPage = MENU_PRINCIPAL;
                }
                else if(currentPage == MENU_TERTIAIRE_1 || currentPage == MENU_TERTIAIRE_2 || currentPage == MENU_TERTIAIRE_3){
                    afficherMenuSecondaire(&optionMenuSecondaire);
                    currentPage = MENU_SECONDAIRE;
                }
                                ////// menu tertiaire 4 (affichage courbe) //////
                else if(currentPage == MENU_TERTIAIRE_4){
                    afficherMenuPrincipal(vData);
                    currentPage = MENU_PRINCIPAL;
                }
                ////// menus quat 1 //////
                else if(currentPage == MENU_QUAT_1_1){        
                    // appel fonction qui ecrit dans le registre LED rouge 
                    // OU lever flag
                    changeLED_red(courantLEDrouge);
                    afficherMenuPrincipal(vData);
                    vTaskResume(xSample);
                    currentPage = MENU_PRINCIPAL;   // ou retour vers menu tertiaire/ secondaire??
                }
                else if(currentPage == MENU_QUAT_1_2){
                    // appel fonction qui ecrit dans le registre LED rouge 
                    // OU lever flag
                    changeLED_IR(courantLEDir);
                    afficherMenuPrincipal(vData);
                    vTaskResume(xSample);
                    currentPage = MENU_PRINCIPAL;   // ou retour vers menu tertiaire/ secondaire??
                }
                
                ///// Menu Quat 2 /////
                else if(currentPage == MENU_QUAT_2_1 || currentPage == MENU_QUAT_2_2){
                    afficherMenuPrincipal(vData);
                    vTaskResume(xSample);
                    currentPage = MENU_PRINCIPAL;                  
                }
                ///// Menu Quat 3 /////
                else if(currentPage == MENU_QUAT_3_1 || currentPage == MENU_QUAT_3_2){
                    afficherMenuPrincipal(vData);
                    vTaskResume(xSample);
                    currentPage = MENU_PRINCIPAL;                  
                }
                
                
            }
            

        }
    

    //GUI_Clear();
    
    }

}
  

  
    




/* [] END OF FILE */
