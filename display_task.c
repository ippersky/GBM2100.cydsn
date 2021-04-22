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



 // vecteur de 160 elements, LED rouge 
uint32_t vecteurData[] = {205973,207305,206168,208001,206188,208050,206371,208225,206544,208577,206719,208501,206990,208473,207183,208417,207367,208379,207724,208151,207986,207993,208303,207802,208682,207622,208730,207587,209365,207455,209209,207541,209337,207549,209635,207806,209501,207957,209461,207996,209317,208103,209140,208491,209000,208672,208748,209006,208491,209357,208350,209326,208242,209947,208089,209826,208216,210028,208306,210395,208429,210203,208596,210292,208686,210169,208954,210055,209354,209987,209644,209821,210027,209660,210440,209483,210556,209554,211304,209484,211253,209696,211585,209817,211970,210011,211842,210178,211772,209972,211207,209606,210230,208954,209085,208043,207710,207263,206266,206439,205001,205392,203820,204970,202756,203955,202073,203512,201459,203377,201194,202759,200993,202713,201002,202459,201198,202302,201513,202188,201835,202108,202327,202081,202824,201965,203003,202176,203932,202267,203918,202500,204436,202777,204921,203032,204880,203312,205157,203520,205057,203810,205034,204221,204997,204527,204912,204994,204917,205416, //,204837,205671,204732,206338,204657,206251,204614,206460,204591,206662};
                            205973,207305,206168,208001,206188,208050,206371,208225,206544,208577,206719,208501,206990,208473,207183,208417,207367,208379,207724,208151,207986,207993,208303,207802,208682,207622,208730,207587,209365,207455,209209,207541,209337,207549,209635,207806,209501,207957,209461,207996,209317,208103,209140,208491,209000,208672,208748,209006,208491,209357,208350,209326,208242,209947,208089,209826,208216,210028,208306,210395,208429,210203,208596,210292,208686,210169,208954,210055,209354,209987,209644,209821,210027,209660,210440,209483,210556,209554,211304,209484,211253,209696,211585,209817,211970,210011,211842,210178,211772,209972,211207,209606,210230,208954,209085,208043,207710,207263,206266,206439,205001,205392,203820,204970,202756,203955,202073,203512,201459,203377,201194,202759,200993,202713,201002,202459,201198,202302,201513,202188,201835,202108,202327,202081,202824,201965,203003,202176,203932,202267,203918,202500,204436,202777,204921,203032,204880,203312,205157,203520,205057,203810,205034,204221,204997,204527,204912,204994,204917,205416,
                            205973,207305,206168,208001,206188,208050,206371,208225,206544,208577,206719,208501,206990,208473,207183,208417,207367,208379,207724,208151,207986,207993,208303,207802,208682,207622,208730,207587,209365,207455,209209,207541,209337,207549,209635,207806,209501,207957,209461,207996,209317,208103,209140,208491,209000,208672,208748,209006,208491,209357,208350,209326,208242,209947,208089,209826,208216,210028,208306,210395,208429,210203,208596,210292,208686,210169,208954,210055,209354,209987,209644,209821,210027,209660,210440,209483,210556,209554,211304,209484,211253,209696,211585,209817,211970,210011,211842,210178,211772,209972,211207,209606,210230,208954,209085,208043,207710,207263,206266,206439,205001,205392,203820,204970,202756,203955,202073,203512,201459,203377,201194,202759,200993,202713,201002,202459,201198,202302,201513,202188,201835,202108,202327,202081,202824,201965,203003,202176,203932,202267,203918,202500,204436,202777,204921,203032,204880,203312,205157,203520,205057,203810,205034,204221,204997,204527,204912,204994,204917,205416};
uint32_t vecteurData2[] = {99513,98160,99413,98419,99319,98729,99145,99050,98949,99419,98798,99851,98679,99846,98657,100433,98597,100453,98713,100550,98880,100921,98942,100758,99148,100637,99243,100496,99426,100316,99677,100090,99871,99833,100155,99657,100544,99416,100533,99350,101127,99266,100986,99359,101151,99370,101493,99586,101282,99597,101251,99713,101027,99896,100926,100109,100625,100345,100369,100625,100180,100986,99983,101152,99930,101751,99809,101581,99950,101877,100112,102210,100251,102006,100391,102068,100506,101956,100783,101872,101092,101692,101296,101532,101733,101311,102054,101081,101967,100723,102263,100171,101478,99530,101051,98881,100564,98080,99478,97452,98752,96709,97796,96168,97009,95842,96177,95534,95551,95512,95055,95623,94635,95492,94373,96057,94212,95856,94325,96206,94439,96594,94606,96428,94889,96724,95093,96645,95436,96622,95818,96520,96042,96387,96551,96312,96959,96200,97075,96116,97747,96027,97625,96166,98102,96258,98464,96444,98339,96751, //,98615,96864,98429,97113,98280,97281,98005,97386,97811,97731};
                            99513,98160,99413,98419,99319,98729,99145,99050,98949,99419,98798,99851,98679,99846,98657,100433,98597,100453,98713,100550,98880,100921,98942,100758,99148,100637,99243,100496,99426,100316,99677,100090,99871,99833,100155,99657,100544,99416,100533,99350,101127,99266,100986,99359,101151,99370,101493,99586,101282,99597,101251,99713,101027,99896,100926,100109,100625,100345,100369,100625,100180,100986,99983,101152,99930,101751,99809,101581,99950,101877,100112,102210,100251,102006,100391,102068,100506,101956,100783,101872,101092,101692,101296,101532,101733,101311,102054,101081,101967,100723,102263,100171,101478,99530,101051,98881,100564,98080,99478,97452,98752,96709,97796,96168,97009,95842,96177,95534,95551,95512,95055,95623,94635,95492,94373,96057,94212,95856,94325,96206,94439,96594,94606,96428,94889,96724,95093,96645,95436,96622,95818,96520,96042,96387,96551,96312,96959,96200,97075,96116,97747,96027,97625,96166,98102,96258,98464,96444,98339,96751,
                            99513,98160,99413,98419,99319,98729,99145,99050,98949,99419,98798,99851,98679,99846,98657,100433,98597,100453,98713,100550,98880,100921,98942,100758,99148,100637,99243,100496,99426,100316,99677,100090,99871,99833,100155,99657,100544,99416,100533,99350,101127,99266,100986,99359,101151,99370,101493,99586,101282,99597,101251,99713,101027,99896,100926,100109,100625,100345,100369,100625,100180,100986,99983,101152,99930,101751,99809,101581,99950,101877,100112,102210,100251,102006,100391,102068,100506,101956,100783,101872,101092,101692,101296,101532,101733,101311,102054,101081,101967,100723,102263,100171,101478,99530,101051,98881,100564,98080,99478,97452,98752,96709,97796,96168,97009,95842,96177,95534,95551,95512,95055,95623,94635,95492,94373,96057,94212,95856,94325,96206,94439,96594,94606,96428,94889,96724,95093,96645,95436,96622,95818,96520,96042,96387,96551,96312,96959,96200,97075,96116,97747,96027,97625,96166,98102,96258,98464,96444,98339,96751};
uint32_t vecteurData3[] = {205973,207305,206168,208001,206188,208050,206371,208225,206544,208577,206719,208501,206990,208473,207183,208417,207367,208379,207724,208151,207986,207993,208303,207802,208682,207622,208730,207587,209365,207455,209209,207541,209337,207549,209635,207806,209501,207957,209461,207996,209317,208103,209140,208491,209000,208672,208748,209006,208491,209357,208350,209326,208242,209947,208089,209826,208216,210028,208306,210395,208429,210203,208596,210292,208686,210169,208954,210055,209354,209987,209644,209821,210027,209660,210440,209483,210556,209554,211304,209484,211253,209696,211585,209817,211970,210011,211842,210178,211772,209972,211207,209606,210230,208954,209085,208043,207710,207263,206266,206439,205001,205392,203820,204970,202756,203955,202073,203512,201459,203377,201194,202759,200993,202713,201002,202459,201198,202302,201513,202188,201835,202108,202327,202081,202824,201965,203003,202176,203932,202267,203918,202500,204436,202777,204921,203032,204880,203312,205157,203520,205057,203810,205034,204221,204997,204527,204912,204994,204917,205416,204837,205671,204732,206338,204657,206251,204614,206460,204591,206662,204581,206367,204586,206402,204548,205967,204548,205683,204576,205312,204550,204995,204844,204647,205035,204328,205121,204143,205709,203943,205474,203995,205910,204074,206112,204207,205943,204379,206337,204543,206193,204879,206211,205289,206165,205581,206150,206104,206141,206566,206010,206910,206113,207618,206076,207602,206252,208240,206424,208563,206665,208406,206886,208919,207081,208835,207367,208762,207638,208729,207896,208526,208348,208387,208732,208203,209019,208070,209391,207916,209311,207900,209782,207913,209978,208038,209863,208165,210137,208232,209963,208477,209827,208599,209678,208829,209472,209224,209399,209521,209079,209731,208932,210161,208772,210177,208810,210728,208819,210815,208995,210858,209138,211288,209319,211079,209575,211060,209773,210901,209995,210738,210439,210645,210706,210529,211089,210404,211618,210365,211667,210429,212263,210423,212435,210609,212498,210811,212917,210990,212716,211242,212775,211454,212706,211741,212553,212006,212075,211822,211394,211512,210369,211017,209260,210003,208265,209523,207043,208474,206230,207583,205368,207020,204742,206069,204125,205441,203797,204749,203574,204319,203623,203937,203726,203711,204060,203522,204535,203455,204648,203514,205413,203600,205487,203925,205838,204139,206355,204422,206331,204765,206340,204905,206310,205237,206250,205671,206162,205966,205968,206378,205974,206914,205810,207094,205989,207848,206030,207872,206243,208126,206361,208540,206469,208161,206571,208104,206495,207865,206553,207469,206683,207157,206675,206745,206985,206432,207161,205979,207035,205823,207540,205630,207319,205637,207490,205725,207795,205872,207640,206063,207704,206191,207660,206451,207570,206852,207480,207082,207399,207523,207226,207973,207110,208152,207223,208946,207143,208810,207353,209287,207568,209686,207753,209513,208047,209837,208178,209768,208528,209740,208807,209638,209102,209523,209624,209366,210012,209201,210201,209221,210869,209090,210717,209231,211127,209364,211505,209471,211253,209641,211502,209749,211284,210005,211154,210212,210911,210296,210704,210713,210535,211061,210301,211173,210189,211774,210039,211600,210189,212038,210190,212362,210297,212096,210407,212316,210475,212075,210711,211948,210956,211803,211212,211653,211541,211458,211950,211297,212162,211147,212676,211065,212538,211107,213028,211128,213309,211291,213136,211550,213521,211655,213358,211962,213213,212192,213126,212324,212892,212745,212641,212897,212237,212817,211647,212736,210700,211734,209874,211225,208833,210424,207895,209190,207005,208490,206174,207482,205680,206645,205094,205800,204742,205171,204714,204613,204625,204101,204717,203793,205149,203572,205005,203696,205554,203726,205828,203942,205803,204307,206380,204622,206388,205060,206559,205421,206533,205667,206535,206180,206481,206490,206292,206883,206160,207464,206115,207521,206292,208305,206402,208474,206687,208511,206872,208939,206992,208721,207152,208503,207163,208264,207233,207947,207416,207560,207525,207132,207592,206828,207877,206444,207676,206334,208139,206136,207901,206112,207857,206140,208169,206238,207931,206358,207820,206447,207713,206638,207466,206977,207303,207277,207192,207706,207081,208103,206973,208188,207054,208950,207122,209005,207357,209176,207559,209677,207752,209623,208115,209711,208280,209649,208588,209549,208922,209296,209159,209208,209510,209029,209986,208906,210092,208903,210711,208890,210682,209036,210928,209164,211335,209311,211094,211720,211030,209525,210907,209785,210764,210028,210524,210284,210345,210516,210097,210907,209864,211006,209891,211597,209748,211450,209870,211707,209890,212048,209987,211765,210127,211854,210201,211623,210469,211470,210733,211316,210888,211118,211196,210930,211603,210667,211690,210606,212342,210484,212189,210579,212483,210694,212799,210842,212573,211019,212804,211122,212648,211327,212545,211738,212381,211884,212207,212313,211997,212555,211697,212417,211130,212474,210392,211663,209582,210986,208536,210186,207596,208845,206687,208006,205810,206859,205077,205887,204565,204967,204035,204126,203888,203512,203845,202960,203738,202628,204118,202439,203935,202488,204343,202575,204707,202795,204640,203013,205052,203336,204937,203668,204977,204040,204969,204330,204876,204830,204776,205237,204682,205516,204642,206110,204556,206120,204730,206553,204767,206984,205086,206843,205306,207283,205514,207187,205758,207073,206036,206817,205965,206536,206288,206252,206480,205757,206476,205521,206800,205189,206481,205014,206753,204868,206842,204892,206576,204847,206794,204864,206512,204991,206368,205193,206259,205479,206067,205917,206011,206185,205832,206505,205736,207049,205641,207054,205744,207677,205808,207841,205975,207873,206220,208307,206433,208201,206816,208156,206957,208166,207280,207951,207650,207851,207951,207751,208318,207524,208751,207360,208732,207447,209377,207486,209448,207647,209547,207845,209949,208064,209884,208361,209769,208443,209677,208653,209502,209006,209346,209267,209108,209543,208882,209991,208717,209936,208778,210564,208710,210582,208885,210665,208980,211015,209119,210898,209304,210890,209478,210768,209613,210563,209998,210388,210286,210144,210565,210008,210988,209826,210946,209720,211636,209803,211609,209905,211789,210059,212260,210340,212156,210567,212254,210845,212192,211165,212252,211804,212405,212285,212545,212971,212673,213594,212600,213848,212870,214737,212927,214762,213232,215114,213355,215520,213428,214996,213115,214297,212290,213075,211348,211815,210476,210447,209470,209150,208748,207828,208024,206645,207233,205750,207023,204975,206291,204508,206021,204099,205998,204007,205585,204018,205645,204029,205451,204382,205460,204681,205400,205052,205435,205558,205352,206132,205385,206300,205392,207043,205377,206977,205572,207429,205698,207808,205855,207670,206112,207954,206217,207802,206579,207710,206838,207691,207156,207571,207659,207520,208121,207396,208164,207266,208831,207087,208549,207031,208881,206937,209011,206953,208672,206911};

#define VECTEUR_AFFICHAGE_LENGHT    260  // car il y a 264 pixels en y 

int16_t vecteurConverti[1000] = {};  
int16_t vecteurAffichage[260] = {}; // a redefinir : taille = longueurData/espacement
size_t longueurData = 1000; //sizeof(vecteurData3)/sizeof(uint32_t); // 160
size_t indexDebut = 200;


size_t espacement = 3;     
    

    // quand je mets en argument qqch pour le task, ca ne marche plus
    // l'ecran refresh, mais n'affiche rien

    // quand je mets les 2 vecteurs vides dans task, meme le refresh s'arrete

float32_t SPO2 = 95.51; 
float32_t BPM = 120.5;

// 0 a 255
uint8_t courantLEDrouge = 31;
uint8_t courantLEDir = 1;

uint8_t flagAlarmeBPM = ON;
uint8_t flagAlarmeAcclerometre = OFF;

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


/*******************************************************************************
Fonction : void DisplayInit(void)

Sommaire : Initialise GUI et écran EInk 

Parametres : None

Return: None  
*******************************************************************************/
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


/*******************************************************************************
Fonction : void convertirVecteurEnInt16(int16_t* vConverti, uint32_t* vData, size_t lData)

Sommaire : Converti les données d'acquisition de uint32_t à int16_t, car la fonction
d'affichage GUI_DrawGraph() utilise seulement des int16

Parametres : int16_t* vConverti (vecteur où sont rangées les données converties en int16)
uint32_t* vData (buffer de données d'acquisition)
size_t lData (longueur du buffer de données d'acquisition) 

Return: None  
*******************************************************************************/
void convertirVecteurEnInt16(int16_t* vConverti, uint32_t* vData, size_t lData){
    for(int i = 0; i < lData; i++){
        vConverti[i] = vData[i]/100;
    }
}

/*******************************************************************************
Fonction : int16_t trouverMinimumVecteur(int16_t* vConverti, size_t lData)

Sommaire : Trouve la valeur minimale du vecteur donné en paramètre

Parametres : int16_t* vConverti (vecteur où sont rangées les données converties en int16)
size_t lData (longueur du buffer de données d'acquisition) 

Return: int min (la valeur minimale)
*******************************************************************************/
int16_t trouverMinimumVecteur(int16_t* vConverti, size_t lData){
    int16_t min = vConverti[indexDebut];    // Ne commence pas au début, à cause du problème mentionné plus haut
    for(int i = indexDebut; i < lData; i++){       
        if(vConverti[i] < min){
            min = vConverti[i];
        }
    }
    return min;
}

/*******************************************************************************
Fonction : int16_t trouverMaximumVecteur(int16_t* vConverti, size_t lData)

Sommaire : Trouve la valeur maximale du vecteur donné en paramètre

Parametres : int16_t* vConverti (vecteur où sont rangées les données converties en int16)
size_t lData (longueur du buffer de données d'acquisition) 

Return: int max (la valeur maximale)
*******************************************************************************/
int16_t trouverMaximumVecteur(int16_t* vConverti, size_t lData){
    int16_t max = vConverti[indexDebut];    // Ne commence pas au début, à cause du problème mentionné plus haut
    for(int i = indexDebut; i < lData; i++){
        if(vConverti[i] > max){
            max = vConverti[i];
        }
    }
    return max;
}

/*******************************************************************************
Fonction : void creerVecteurAffichage(int16_t* vConverti, int16_t* vAffichage, size_t lData, size_t espacement)

Sommaire : Fait des calculs avec les données du vecteur converti en int16 pour obtenir 
des données finales qu'on peut afficher sur l'écran EInk

Parametres : int16_t* vConverti (vecteur où sont rangées les données converties en int16)
int16_t* vAffichage (vecteur où sont rangées les données prêtes à être affichées)
size_t lData (longueur du buffer de données d'acquisition) 
size_t espacement (bond à laquelle choisir des données à afficher, car il y a un nombre limité
de pixels en y. On ne peut pas afficher tous les données de vConverti) 

Return: None
*******************************************************************************/
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


/*******************************************************************************
Fonction : void afficherMenuPrincipal(uint32_t * vData)

Sommaire : Affiche le menu principal : appelle les fonctions pour convertir le 
buffer de données pris en paramètre en données affichables sur l'écran, affiche 
la courbe, appelle la fonction pour actualiser les parametres (BPM, SPO2, courbe 
affichee)

Parametres :  uint32_t * vData (Pointeur vers le buffer de donnée)

Return: None
*******************************************************************************/
void afficherMenuPrincipal(uint32_t * vData){
    

    GUI_Clear();
    
    convertirVecteurEnInt16(vecteurConverti, vData, longueurData);
    creerVecteurAffichage(vecteurConverti, vecteurAffichage, longueurData, espacement);
    GUI_DrawGraph(vecteurAffichage, VECTEUR_AFFICHAGE_LENGHT, 0, 0);
    
    updateParametres(SPO2, BPM, currentCourbe);
    

   

}


/*******************************************************************************
Fonction : void updateParametres(float32_t SPO2, float32_t BPM, uint8_t currentCourbe)

Sommaire : Update les parametres : BPM, SPO2 et la courbe présentement affichée
Si les flags pour les alarmes sont levés, affiche les messages d'alarme correspondant. 

Parametres :  float32_t SPO2 (valeur du SPO2)
float32_t BPM (valeur du BPM)
uint8_t currentCourbe (la courbe presentement affichée)

Return: None
*******************************************************************************/
void updateParametres(float32_t SPO2, float32_t BPM, uint8_t currentCourbe) 
{
    char sSPO2[10];
    gcvtf(SPO2, 4, sSPO2);
    
    char sBPM[10];
    gcvtf(BPM, 4, sBPM);
    
    
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_DispStringAt("SPO2 : ", 5, 153);
    GUI_DispStringAt(sSPO2, 53, 153);
    GUI_DispStringAt("%", 90, 153);
    GUI_SetFont(GUI_FONT_16B_1);
    GUI_DispStringAt("BPM : ", 115, 153);
    GUI_DispStringAt(sBPM, 155, 153);
    
    if(currentCourbe == RED)
        GUI_DispStringAt("ROUGE", 200, 153);    
    else if (currentCourbe == IR)
        GUI_DispStringAt("IR", 200, 153);    
    
                                            // intensite lEDs ?
     
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
        // and if Interrupt == TRUE
        uint8_t font = 16;
        GUI_SetFont(GUI_FONT_16_1);
        
        GUI_Clear();
        GUI_DispStringAt("ATTENTION", 10, 10);
        GUI_DispStringAt("Mesure invalide!", 10, 10+font);
        GUI_DispStringAt("Veuillez garder votre doigt immobile", 10, 10+(2*font));
        GUI_DispStringAt("lors de l'acquisition", 10, 10+(3*font));
        
        UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    }
    
    
}





/*******************************************************************************
Fonction : void afficherMenuSecondaire(uint8_t * ptrOptionPresent)

Sommaire : Affiche le menu secondaire, en plaçant le curseur à l'option 1 et en écrivant dans 
le pointeur pris en parametre que l'option est 1. 

Parametres :  uint8_t * ptrOptionPresent (pointeur vers la variable contenant l'option actuelle)

Return: None
*******************************************************************************/
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

/*******************************************************************************
Fonction : void updateMenuSecondaire(uint8_t * ptrOptionPresent)

Sommaire : Update l'emplacement du curseur selon l'option actuelle 

Parametres :  uint8_t * ptrOptionPresent (pointeur vers la variable contenant l'option actuelle)

Return: None
*******************************************************************************/
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

/*******************************************************************************
Fonction : void afficherMenuTertiaire1(uint8_t * ptrOptionPresent)

Sommaire : Affiche le menu tertiaire 1, en plaçant le curseur à l'option 1 et en écrivant dans 
le pointeur pris en parametre que l'option est 1.  

Parametres :  uint8_t * ptrOptionPresent (pointeur vers la variable contenant l'option actuelle)

Return: None
*******************************************************************************/
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

/*******************************************************************************
Fonction : void afficherMenuTertiaire2(uint8_t * ptrOptionPresent)

Sommaire : Affiche le menu tertiaire 2, en plaçant le curseur à l'option 1 et en écrivant dans 
le pointeur pris en parametre que l'option est 1.  

Parametres :  uint8_t * ptrOptionPresent (pointeur vers la variable contenant l'option actuelle)

Return: None
*******************************************************************************/
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

/*******************************************************************************
Fonction : void afficherMenuTertiaire3(uint8_t * ptrOptionPresent)

Sommaire : Affiche le menu tertiaire 3, en plaçant le curseur à l'option 1 et en écrivant dans 
le pointeur pris en parametre que l'option est 1.  

Parametres :  uint8_t * ptrOptionPresent (pointeur vers la variable contenant l'option actuelle)

Return: None
*******************************************************************************/
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

void afficherMenuTertiaire4(uint8_t * ptrOptionPresent){

    uint8_t font = 20;
    GUI_Clear();
    GUI_SetFont(GUI_FONT_20_1);
    
    GUI_DispStringAt("Afficher la courbe de :", 40, 10);    
    GUI_DispStringAt("1. Absorption rouge", 40, 10+(2*font));
    GUI_DispStringAt("2. Absorption infrarouge", 40, 10+(4*font));
    GUI_FillRect(10, 10+(2*font), 30, 30+(2*font));
    
    GUI_DispStringAt("Courbe presentement affichee :", 10, 10+(6*font));
    if(currentCourbe == RED)
        GUI_DispStringAt("Rouge", 10, 10+(7*font));
    else if(currentCourbe == IR)
        GUI_DispStringAt("Infrarouge", 10, 10+(7*font));
    
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
    *ptrOptionPresent = 1;
        
}

/*******************************************************************************
Fonction : void afficherMenuTertiaire4()

Sommaire : Affiche le menu tertiaire 4. Indique la courbe présentement affichée (rouge ou infrarouge) 

Parametres :  None (Ne prend rien en parametre, car la variable indiquant la courbe présentement
affichée est une variable globale)

Return: None
*******************************************************************************/
void afficherMenuTertiaire4_v2(){  
    uint8_t font = 20;
    GUI_Clear();
    GUI_SetFont(GUI_FONT_20_1);


    GUI_DispStringAt("Presentement affichee :", 20, 10); 
    if(currentCourbe == RED)
        GUI_DispStringAt("ROUGE", 20, 10+font); 
    else if(currentCourbe == IR)
        GUI_DispStringAt("IR", 20, 10+font);
    
    GUI_DispStringAt("Afficher courbe d'absorption :", 20, 10+(2*font)); 
        
    GUI_SetFont(GUI_FONT_20B_1);
    GUI_DispStringAt("Rouge", 20, 10+(5*font)); 
    GUI_DispStringAt("Infrarouge", 145, 10+(5*font));
    
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}




/*******************************************************************************
Fonction : void updateMenuTertiaire(uint8_t * ptrOptionPresent

Sommaire : Update l'emplacement du curseur selon l'option actuelle 

Parametres :  uint8_t * ptrOptionPresent (pointeur vers la variable contenant l'option actuelle)

Return: None
*******************************************************************************/
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


/*******************************************************************************
Fonction : void afficherMenuQuat1(uint8_t * ptrCourantLED, uint8_t * ptrOptionTertiaire)

Sommaire : Affiche le menu quaternaire 1 : modifier le courant d'un LED. Dépendemment de l'option choisi au menu tertiaire, 
affiche le courant pour la LED rouge ou le courant pour la LED IR. 

Parametres : uint8_t * ptrCourantLED (pointeur vers le courant du LED)
uint8_t * ptrOptionTertiaire (pointeur vers l'option choisi au menu tertiaire)

Return: None
*******************************************************************************/
void afficherMenuQuat1(uint8_t * ptrCourantLED, uint8_t * ptrOptionTertiaire){

    uint8_t font = 20;
    GUI_Clear();
    GUI_SetFont(GUI_FONT_20_1);
    
    if(*ptrOptionTertiaire == 1){
        GUI_DispStringAt("Courant LED rouge :", 40, 10);   
    }
    else if(*ptrOptionTertiaire == 2){
        GUI_DispStringAt("Courant LED infrarouge :", 40, 10); 
    }

    GUI_DispStringAt("Bonds de 5 mA", 40, 10+(6*font));
    GUI_DispStringAt("Intervalle : 0 a 51 mA", 40, 10+(7*font));
    
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

/*******************************************************************************
Fonction : void afficherMenuQuat2(float32_t * ptrBorneBPM, uint8_t * ptrOptionTertiaire)

Sommaire : Affiche le menu quaternaire 2 : modifier une borne de l'alarme BPM. 
Dépendemment de l'option choisi au menu tertiaire, affiche la borne maximale ou la borne minimale actuelle

Parametres : float32_t * ptrBorneBPM (pointeur vers la valeur de la borne)
uint8_t * ptrOptionTertiaire (pointeur vers l'option choisi au menu tertiaire)

Return: None
*******************************************************************************/
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

/*******************************************************************************
Fonction : void afficherMenuQuat3(uint8_t * ptrFlagAlarme, uint8_t * ptrOptionTertiaire)

Sommaire : Affiche le menu quaternaire 3 : activer ou désactiver une alarme.
Dépendemment de l'option choisi au menu tertiaire, affiche l'état actuel de l'alarme BPM ou 
de l'alarme accéléromètre.

Parametres : uint8_t * ptrFlagAlarme (pointeur vers l'état de l'alarme)
uint8_t * ptrOptionTertiaire (pointeur vers l'option choisi au menu tertiaire)

Return: None
*******************************************************************************/
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





/*******************************************************************************
Fonction : Task_Display(void *data)

Sommaire : Fonction de la tâche d'affichage. Dépendamment de l'information 
obtenue de la queue (quel bouton a été touché) et de la page de menu actuelle,
effectue quelque chose. 

Parametres : void *data  (pas utilisé dans la tâche)

Return: None
*******************************************************************************/
void Task_Display(void *data){
   
   
    DisplayInit(); 

    
    uint8_t optionMenuSecondaire = 0;
    uint8_t optionMenuTertiaire = 0;
    
    page_data_t currentPage = MENU_PRINCIPAL;
    
    touch_data_t touchData;
    
    BaseType_t rtosApiResult; 
    
    uint32_t * vData = vecteurData3;
    afficherMenuPrincipal(vData);
   
    
    
    for(;;){
    
        rtosApiResult = xQueueReceive(touchDataQ, &touchData, portMAX_DELAY);
        if (rtosApiResult == pdTRUE) // si on reçoit quelque chose de la queue
        {
    
            if(touchData == BUTTON0_TOUCHED){   
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
                            //afficherMenuTertiaire4(&optionMenuTertiaire);
                            afficherMenuTertiaire4_v2();
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
                        /*
                        if (optionMenuTertiaire == 1){
                            currentCourbe = RED;
                            vData = vecteurData;
                            afficherMenuPrincipal(vData);
                            currentPage = MENU_PRINCIPAL;
                            break;
                        }
                        else if (optionMenuTertiaire == 2){
                            currentCourbe = IR;
                            vData = vecteurData2;
                            afficherMenuPrincipal(vData);
                            currentPage = MENU_PRINCIPAL;
                            break;
                        }
                        */
                        currentCourbe = RED;
                        vData = vecteurData;
                        afficherMenuTertiaire4_v2();
                        
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
                        if(borneMaxBPM >= 30)
                            borneMaxBPM -= 5;
                        else
                            borneMaxBPM = 25;
                        afficherMenuQuat2(&borneMaxBPM, &optionMenuTertiaire);
                        break;
                    case MENU_QUAT_2_2:
                        if(borneMinBPM >= 30)
                            borneMinBPM -= 5;
                        else
                            borneMinBPM = 25;
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
                else if(currentPage == MENU_TERTIAIRE_1 || currentPage == MENU_TERTIAIRE_2 || currentPage == MENU_TERTIAIRE_3 ){  //|| currentPage == MENU_TERTIAIRE_4
                    updateMenuTertiaire(&optionMenuTertiaire);
                }
                
                ////// menu tertiaire 4 (affichage courbe) //////
                else if(currentPage == MENU_TERTIAIRE_4){
                    currentCourbe = IR;
                    vData = vecteurData2;
                    afficherMenuTertiaire4_v2();                
                }
                                
                ////// menu quat 1 //////
                
                else if(currentPage == MENU_QUAT_1_1){
                    if(courantLEDrouge <= 255-25)
                        courantLEDrouge += 25;      // variable globale, directement accessible          
                    else
                        courantLEDrouge = 255;
                    afficherMenuQuat1(&courantLEDrouge, &optionMenuTertiaire);
                }
                else if(currentPage == MENU_QUAT_1_2){
                    if(courantLEDir <= 255-25)
                        courantLEDir += 25;
                    else
                        courantLEDir = 255;
                    afficherMenuQuat1(&courantLEDir, &optionMenuTertiaire);                
                }
                /////// Menu Quat 2 ///////
                else if(currentPage == MENU_QUAT_2_1){
                    if(borneMaxBPM <= 250-5)
                        borneMaxBPM += 5;
                    else 
                        borneMaxBPM = 250;
                    afficherMenuQuat2(&borneMaxBPM, &optionMenuTertiaire);
                }
                else if(currentPage == MENU_QUAT_2_2){
                    if(borneMinBPM <= 250-5)
                        borneMinBPM += 5;
                    else
                        borneMinBPM = 250;
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
                
                if (currentPage == MENU_SECONDAIRE){
                    afficherMenuPrincipal(vData);
                    currentPage = MENU_PRINCIPAL;
                }
                else if(currentPage == MENU_TERTIAIRE_1 || currentPage == MENU_TERTIAIRE_2 || currentPage == MENU_TERTIAIRE_3){ //|| currentPage == MENU_TERTIAIRE_4
                    afficherMenuSecondaire(&optionMenuSecondaire);
                    currentPage = MENU_SECONDAIRE;
                }
                
                ////// menu tertiaire 4 (affichage courbe) //////
                else if(currentPage == MENU_TERTIAIRE_4){
                    afficherMenuPrincipal(vData);
                    currentPage = MENU_PRINCIPAL;
                }
                
                
                ////// menu quat 1 //////
                else if(currentPage == MENU_QUAT_1_1){        
                    // appel fonction qui ecrit dans le registre LED rouge 
                    // OU lever flag
                    afficherMenuPrincipal(vData);                    
                    currentPage = MENU_PRINCIPAL;   // ou retour vers menu tertiaire/ secondaire??
                }
                else if(currentPage == MENU_QUAT_1_2){
                    // appel fonction qui ecrit dans le registre LED rouge 
                    // OU lever flag
                    afficherMenuPrincipal(vData);                    
                    currentPage = MENU_PRINCIPAL;   // ou retour vers menu tertiaire/ secondaire??
                }
                
                ///// Menu Quat 2 /////
                else if(currentPage == MENU_QUAT_2_1 || currentPage == MENU_QUAT_2_2){
                    afficherMenuPrincipal(vData);
                    currentPage = MENU_PRINCIPAL;                  
                }
                ///// Menu Quat 3 /////
                else if(currentPage == MENU_QUAT_3_1 || currentPage == MENU_QUAT_3_2){
                    afficherMenuPrincipal(vData);
                    currentPage = MENU_PRINCIPAL;                  
                }
                
                
            }
            

        }
    
  
    }

}
  

  
    




/* [] END OF FILE */
