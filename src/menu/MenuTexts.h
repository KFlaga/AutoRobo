
#ifndef MENUTEXTS_H_
#define MENUTEXTS_H_

#define USE_MENU_TEXTS extern unsigned char* _menu_texts_[]
#define GET_MENU_TEXT(i) _menu_texts_[i]
#define COPY_MENU_TEXT(txt, line) { int tpos; for(tpos=0;tpos<16;tpos++)line[tpos]=_menu_texts_[txt][tpos]; }
#define COPY_TEXT(src,dst,size,off) while(size){dst[size-1+off]=src[size-1];size--;}

#define TXT_Prime_Top 0
#define TXT_Prime_ExeInst 1
#define TXT_Prime_Reset 2
#define TXT_ChooseInstr 3
#define TXT_ChooseParams 4
#define TXT_WaitForExec_Top 5
#define TXT_WaitForExec_Bot 6
#define TXT_Error_Top 7
#define TXT_Prog_Main 8
#define TXT_Prog_New 9
#define TXT_Prog_Edit 10
#define TXT_Prog_Exec 11
#define TXT_Prime_Program 12
#define TXT_Prog_Choose 13
#define TXT_Prog_NewName 14
#define TXT_Prog_NameExists 15
#define TXT_Prog_InstrEdit 16
#define TXT_Prog_InstrRemove 17
#define TXT_Prog_InstrChange 18


#endif /* MENUTEXTS_H_ */
