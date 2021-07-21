/*
 * Main_MC2.h
 *
 *  Created on: Dec 14, 2020
 *      Author: MRM
 */

#ifndef MAIN_MC2_H_
#define MAIN_MC2_H_

uint8 ready = 0x02;
uint16 address = 0x0002;
uint8 ch;

uint8 first_Time[32] = "Set Pass:#";
uint8 menu[32] = "1 change pass,2 open door:#";
uint8 option1[32] = "Enter Pass:#";
uint8 pass_Again[32] = "Again:#";
uint8 option2[32] = "Open Door#";
uint8 wrong_Pass[32]= "Wrong Pass#";
uint8 new_pass[32]= "New Pass:#";
uint8 success[32] = "Success#";
uint8 open_Door[32] = "Door Opening...#";
uint8 close_Door[32] = "Door closing...#";
uint8 menu_Option;

uint8 rec_Pass[6];
uint8 rec_Pass_2nd[6];
uint8 mem_Pass[6];
uint8 write_Mem_Pass[6];
uint8 flag1 = 0;
uint8 yes = '1';
uint8 no = '0';

unsigned int count = 0;

#endif /* MAIN_MC2_H_ */
