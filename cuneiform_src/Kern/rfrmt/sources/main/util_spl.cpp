/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

����������� ��������� ��������������� � ������������� ��� � ���� ��������� ����,
��� � � �������� �����, � ����������� ��� ���, ��� ���������� ��������� �������:

      * ��� ��������� ��������������� ��������� ���� ������ ���������� ���������
        ���� ����������� �� ��������� �����, ���� ������ ������� � ����������� 
        ����� �� ��������. 
      * ��� ��������� ��������������� ��������� ���� � ������������ �/��� � 
        ������ ����������, ������������ ��� ���������������, ������ �����������
        ��������� ���� ���������� �� ��������� �����, ���� ������ ������� �
        ����������� ����� �� ��������.  
      * �� �������� Cognitive Technologies, �� ����� �� ����������� �� ����� 
        ���� ������������ � �������� �������� ��������� �/��� ����������� 
        ���������, ���������� �� ���� ��, ��� ���������������� �����������
        ����������. 

��� ��������� ������������� ����������� ��������� ���� �/��� ������� ������ "���
��� ����" ��� ������-���� ���� ��������, ���������� ���� ��� ���������������, 
������� �������� ������������ �������� � ����������� ��� ���������� ����, �� ��
������������� ���. �� �������� ��������� ���� � �� ���� ������ ����, ������� 
����� �������� �/��� �������� �������������� ���������, �� � ���� ������ �� 
��Ѩ� ���������������, ������� ����� �����, ���������, ����������� ��� 
������������� ������, ��������� � �������������� ��� ���������� ���������� 
������������� ������������� ��������� (������� ������ ������, ��� ������, 
������� ���������, ��� ������ �/��� ������ �������, ���������� ��-�� �������� 
������� ��� �/��� ������ ��������� �������� ��������� � ������� �����������, 
�� �� ������������� ����� ��������), �� �� ������������� ���, ���� ���� ����� 
�������� ��� ������ ���� ���� �������� � ����������� ����� ������� � ������.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE 
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define TIGER_CORR       //alik

#include "lst3_win.h"
#ifdef SPELL_NEW
  #include "b_open.h"
  #include "spell.h"
  #include "lm.h"
#endif

#ifdef DLL_MOD
 #define LST_WWIN
   #define MAIN2
#endif
#ifndef WIN_MOD
  #define LST_WWIN
#endif
#include "ful_txt.h"
#include "globus.h"
#include "wind32.h"
#include "aldebug.h"
#include "decl.h"
	////////////// functions, which are moved from other modules //////////////
#ifdef alDebug
static int dets=1;
#endif
static HWND h_found=NULL;

/* These were in ful_txt.h. That caused missing symbols
 * so I moved them here. JussiP.
 */

//---�� ������ �������---
WORD NumCol;
int SizeSectionCol;
int *NumStr;//[nc] 
Word32 *UserNumber;//[nc]
Word32 *FragFlag;//[nc]
SRECT *BndCol;//[nc]
STAT_COL *StatCol;//[nc]
int len_col,k_frm;
//int k_colt,slope,space;
char name[80],name1[LEN_PATH];
int k_col[MAX_LEV+1],k_lev; LEV **knot; //������
int fl_EditStruct; //������� �������������� ������ ��������. ��������
//---�� ������ ���� ���---
int SizeSectionFntName; char *FntName;
//---�� ������ ���������---
ZN ****Zn;//[nc][ns][nw][nz]
TITLE_STR **TitleStr;//[nc][ns]
TITLE_WORD ***TitleWord;//[nc][ns][nw]
TITLE_FUL TitleFul;
COOR_COMP *CoorComp; long NumComp;
//--FullSpellFull
uchar word[MAX_SYM_WORD+1],word1[MAX_SYM_WORD+1];
float dist[MAX_SYM_WORD+1],dist1[MAX_SYM_WORD+1];
PAR_FUL par_ful;
char PunctuationMarks[256];
int ExtSettings;
//char AlphabetAllow[256];
char WordOne[256];
uchar Upper[256],Lower[256],UppLow[256],UppLowOther[256],EndSentence[256];
BYTE KoderRus[256],KoderLat[256];
int FlCarryStr;
char FileParSpel[MAXFILENAME],FileParSpelOut[MAXFILENAME];
int *Pen;
int MaxValue;
//--��������� �� ������������
FEAT_LET FeatLet[256];
BYTE **ClustOCR;int K_Clust;//�������� �������������
//---WORK---
BYTE buf[1024],buf1[1024];

/* End of things brought from ful_txt.h. */

BOOL __stdcall FindByPartOfTitle(
    HWND hwnd,	    // handle to parent window
    LPARAM lParam 	// application-defined value
   )
{
   char title[256];
   ::GetWindowText(hwnd, title, sizeof(title));
   if (strstr(title, (char*)lParam))
   {
		 h_found=hwnd;
      return FALSE;  // don't continue
   }
   return TRUE;
}

extern int ConsMess( const char* str, ... );
#ifdef SUB_ZN
  SUB_ALLOC SubZn;
#endif
/* // !!! Art - ��������

#ifdef SUB_FRM
  SUB_ALLOC SubFrm;
#endif
int TypeDoc;
*/ // !!! Art - ��������
Int16 MonoSpaceAllPage;
Int16 HeiStrAllPage;
/* // !!! Art - ��������
int getchW(void);

FILE1 *f03;

*/  // !!! Art - ��������


#ifdef TIGER_CORR
int CalcStatTiger(void);
//void GenArrAnsiOem(void);// !!! Art - ��������
//int PASC CorrTiger(char *FileNameFul,char *FileNameOut,char *FilePar,int fl_cor);

int SaveFullOutTiger(char *FileName);
//���������:
//FileNameFul - ��� ����� Full Out Tiger
//FileNameOut - ��� ��������� �����
//Return: 0 - OK
//
WORD NumZ,NumW,NumS;
Int16 SizeYGlobUpp;
int IsB1(BYTE a)
{ if((FeatLet[a].Chif && a != ',' && a != '.' && a != '-') ||
     (FeatLet[a].Let &&  a != '-' && a != '\'' &&
     (FeatLet[a].Shift==UPP || FeatLet[a].PosUpLine==UPP)))
     		return 1;
  return 0;
}
int IsB2(BYTE a)
{ if((FeatLet[a].Let && a != '-' && a != '\'' &&
      (FeatLet[a].Shift==LOW && FeatLet[a].PosUpLine==LOW)))
      	return 1;
  return 0;
}
//==������ ����������
int CalcStatTiger(void)
//==
{
	int numL,numU,maxL=1000,maxU=300,*Upp,*Low,med,mod,sig,ave,nc,ns,nw,nz;
  int numB1,*arrB1,maxB1=30;
	int ThrDif_b1b2;
  BYTE a;
	//SRECT rr;

	Upp  =(int*)malloc_m(maxU*sizeof(int));
	Low  =(int*)malloc_m(maxL*sizeof(int));
  arrB1=(int*)malloc_m(maxB1*sizeof(int));
  if(Upp==NULL || Low==NULL || arrB1==NULL)
		return NOT_ALLOC;
  // -- ������ ������.�������� � ������ OCR --
  numL=numU=0;
  for(nc=0; nc <= NumCol ; ++nc)
    for(ns=0; ns <= NumStr[nc]; ++ns)
      for(nw=0; nw < TitleStr[nc][ns].S_Gen.S_NumWord; ++nw)
        for(nz=0; nz < TitleWord[nc][ns][nw].W_Gen.W_NumSym; ++nz)
        { ZN *z=&Zn[nc][ns][nw][nz];
					     SRECT *r=&z->Title.Z_Rect;
          if(a=Get1Alt(z,0)) //�����������
          { if(IsB1(a))
            { if(numU >= maxU)
              { int MaxOld=maxU*sizeof(int); maxU*=2;
                if((Upp=(int*)realloc_m(Upp,MaxOld,maxU*sizeof(int)))==NULL)return NOT_ALLOC;
              }
              Upp[numU++]=r->bottom-r->top;
            }
            if(IsB2(a))
            { if(numL >= maxL)
              { int MaxOld=maxL*sizeof(int); maxL*=2;
                if((Low=(int*)realloc_m(Low,MaxOld,maxL*sizeof(int)))==NULL)return NOT_ALLOC;
              }
              Low[numL++]=r->bottom-r->top;
            }
        	}
				}
  sig=0; SizeYGlob=SizeYGlobUpp=INDEF;
  if(numU > 2)
		   Statist(Upp,numU,&ave,&sig,(int*)&SizeYGlobUpp,&mod,0);
  if(numL > 10)
		   Statist(Low,numL,&ave,&sig,&SizeYGlob   ,&mod,0);
  else //������ ������.�������� ��� ����� OCR
  { numL=numU=0;
    for(nc=0; nc <= NumCol; ++nc)
      for(ns=0; ns <= NumStr[nc]; ++ns)
        for(nw=0; nw < TitleStr[nc][ns].S_Gen.S_NumWord; ++nw)
          for(nz=0; nz < TitleWord[nc][ns][nw].W_Gen.W_NumSym; ++nz)
          { 
											 ZN *z=&Zn[nc][ns][nw][nz]; 
												SRECT *r=&z->Title.Z_Rect;
            if(numL >= maxL)
            { int MaxOld=maxL*sizeof(int); maxL*=2;
              if((Low=(int*)realloc_m(Low,MaxOld,maxL*sizeof(int)))==NULL)return NOT_ALLOC;
            }
            Low[numL++]=r->bottom-r->top;
          }
    if(numL > 0)           //ALIK 01.05.99
					  Statist(Low,numL,&ave,&sig,&SizeYGlob,&mod,0);
    else 
					  return -1;
  }
  // -- ������ �������� �������� --
  if(NumCol)
  { for(nc=0; nc <= NumCol; ++nc)
    { numL=numU=0;
      for(ns=0; ns <= NumStr[nc]; ++ns)
        for(nw=0; nw < TitleStr[nc][ns].S_Gen.S_NumWord; ++nw)
          for(nz=0; nz < TitleWord[nc][ns][nw].W_Gen.W_NumSym; ++nz)
          { ZN *z=&Zn[nc][ns][nw][nz]; SRECT *r=&z->Title.Z_Rect;
            if(a=Get1Alt(z,0))
            { if(IsB1(a))
              { if(numU >= maxU)
                { int MaxOld=maxU*sizeof(int); maxU*=2;
                  if((Upp=(int*)realloc_m(Upp,MaxOld,maxU*sizeof(int)))==NULL)return NOT_ALLOC;
                }
                Upp[numU++]=r->bottom-r->top;
              }
              if(IsB2(a))
              { if(numL >= maxL)
                { int MaxOld=maxL*sizeof(int); maxL*=2;
                  if((Low=(int*)realloc_m(Low,MaxOld,maxL*sizeof(int)))==NULL)return NOT_ALLOC;
                }
                Low[numL++]=r->bottom-r->top;
              }
          } }
      sig=0; StatCol[nc].dy_Upp=SizeYGlobUpp;
      if(numU > 2)
				Statist(Upp,numU,&ave,&sig,&StatCol[nc].dy_Upp,&mod,0);
      else
				StatCol[nc].dy_Upp=SizeYGlobUpp;
      if(numL > 10)
      { med=1;Statist(Low,numL,&ave,&sig,&med,&mod,0);
        StatCol[nc].dy_col=StatCol[nc].dy_Low=med;
      }
      else
				StatCol[nc].dy_col=StatCol[nc].dy_Low=SizeYGlob;
    }
  }
  else
  {
		StatCol[0].dy_col=StatCol[0].dy_Low=SizeYGlob;StatCol[0].dy_Upp=SizeYGlobUpp;
	}
  // -- ������ �������� �������� ����� ���� � ������� ������� b2 --
  for(nc=0; nc <= NumCol; ++nc)
  { int Dif_b1b2;//������� �������� ������ b1 � b2
    ThrDif_b1b2=NORM_SCAN(3);
    ThrDif_b1b2=MAX(ThrDif_b1b2,(StatCol[nc].dy_Upp - StatCol[nc].dy_Low)/2);
    if((StatCol[nc].stat_str=(STAT_STR*)malloc_m((NumStr[nc]+1)*sizeof(STAT_STR)))==NULL)
			return NOT_ALLOC;
    for(ns=0; ns <= NumStr[nc]; ++ns)
    { SRECT *rS=&TitleStr[nc][ns].S_Rect;
      numL=numU=numB1=0; Dif_b1b2=(rS->top - rS->left > ThrDif_b1b2);
      for(nw=0; nw < TitleStr[nc][ns].S_Gen.S_NumWord; ++nw)
        for(nz=0; nz < TitleWord[nc][ns][nw].W_Gen.W_NumSym; ++nz)
        { ZN *z=&Zn[nc][ns][nw][nz]; SRECT *r=&z->Title.Z_Rect;
          if(a=Get1Alt(z,0))
          { if(IsB2(a))
            { if(numL >= maxL)
              { int MaxOld=maxL*sizeof(int); maxL*=2;
                if((Low=(int*)realloc_m(Low,MaxOld,maxL*sizeof(int)))==NULL)return NOT_ALLOC;
              }
              Low[numL++]=r->bottom-r->top;
              //�������� �� �� b2, ���. ������� �� b1
              if(Dif_b1b2 && !FeatLet[a].ImUppLow)
              { TITLE_ZN *t=&z->Title; int na=t->Z_Num_Alt,flp,i;
                flp=TRUE;
                for(i=1; i < na; ++i)
                  if(!IsB2(a=Get1Alt(z,i)) || FeatLet[a].ImUppLow)
                    {flp=FALSE;break;}
                if(flp && abs(r->top-rS->left) < abs(r->top-rS->top))
                { if(numU >= maxU)
                  { int MaxOld=maxU*sizeof(int); maxU*=2;
                    if((Upp=(int*)realloc_m(Upp,MaxOld,maxU*sizeof(int)))==NULL)return NOT_ALLOC;
                  }
                  Upp[numU++]=r->top;
                }
              }
            }
            //==������� b1 ������ ���� �� ������ ������.,�.�. ������.���� ����==
            if(IsB1(a) && (!FeatLet[a].ImUppLow || FeatLet[a].Chif) &&
               abs(rS->left-r->top) > 1)
            { if(numB1 >= maxB1)
              { int MaxOld=maxB1*sizeof(int); maxB1*=2;
                if((arrB1=(int*)realloc_m(arrB1,MaxOld,maxB1*sizeof(int)))==NULL)return NOT_ALLOC;
              }
              arrB1[numB1++]=r->top;
            }
        } }
      sig=0;
      if(numL > 5)
      { med=1; Statist(Low,numL,&ave,&sig,&med,&mod,0);
        StatCol[nc].stat_str[ns].dy=med;
      }
      else 
				StatCol[nc].stat_str[ns].dy=StatCol[nc].dy_Low;
      #ifdef DRAWxxxx
      if(viz && (numU || numB1))
      { clear(); rr.left=32000;rr.top=32000;rr.right=-32000;rr.bottom=-32000;
        for(nw=0; nw < TitleStr[nc][ns].S_Gen.S_NumWord; ++nw)
          { for(nz=0; nz < TitleWord[nc][ns][nw].W_Gen.W_NumSym; ++nz)
            { rr.left  =MIN(rr.left  ,Zn[nc][ns][nw][nz].Title.Z_Rect.left  );
              rr.top   =MIN(rr.top   ,Zn[nc][ns][nw][nz].Title.Z_Rect.top   );
              rr.right =MAX(rr.right ,Zn[nc][ns][nw][nz].Title.Z_Rect.right );
              rr.bottom=MAX(rr.bottom,Zn[nc][ns][nw][nz].Title.Z_Rect.bottom);
            }
          }
        rr.top=MIN(rr.top,rS->top); rr.top-=30;
        rr.bottom=MAX(rr.bottom,rS->bottom); rr.bottom+=30;
        BoundsRect(0,(RECT*)&rr,0);
        for(nw=0; nw < TitleStr[nc][ns].S_Gen.S_NumWord; ++nw)
        { for(nz=0; nz < TitleWord[nc][ns][nw].W_Gen.W_NumSym; ++nz)
              image_rect((RECT*)&Zn[nc][ns][nw][nz].Title.Z_Rect,9,0xFFFF,_GBORDER);
        }
        { struct {int b1,b2,b3,b4;} bl; memcpy(&bl,rS,8);
          setcol(10);vector_w(rr.left,bl.b1,rr.right,bl.b1);pause();
          setcol(11);vector_w(rr.left,bl.b2,rr.right,bl.b2);pause();
          setcol(13);vector_w(rr.left,bl.b4,rr.right,bl.b4);pause();
          setcol(12);vector_w(rr.left,bl.b3,rr.right,bl.b3);pause();
        }
      }
      #endif
      if(numU) //������� ������� b2
      { med=1; Statist(Upp,numU,&ave,&sig,&med,&mod,0);
        #ifdef DRAWxxx
          if(viz){setcol(14);vector_w(rr.left,med,rr.right,med);pause();}
        #endif
        rS->top=med;
      }
      if(numB1) //������� ������� b2
      { med=1; Statist(arrB1,numB1,&ave,&sig,&med,&mod,0);
        #ifdef DRAWxxx
          if(viz){setcol(15);vector_w(rr.left,med,rr.right,med);pause();}
        #endif
        rS->left=med;//?��� ������ ���� med < old b1?
      }
    }
  }
  free_m(Upp); 
	free_m(Low); 
	free_m(arrB1);
	return 0;
}

//������ �����:
//Int16 ScanResolution
//Int16 NumCol,NumZ,NumW,NumS - ����� �������, ���������, ���� � ����� in all page
//Int16 MonoSpaceAllPage - distance between left margin letters-neighdoors from one word
//Int16 HeiStrAllPage - distance between top margin 
//Fragm[0], ..., Fragm[NumCol-1] ���������
//  Fragm[nf]: ���� ��������
//    Rect16 RectFragm
//    Int16  NumStr ����� �����
//    Str[0], ..., Str[NumStr-1] ������
//      Str[nf][ns]:
//        Rect16 S_Rect // 4 base lines
//        Int16  NumWordStr; ����� ���� ������� ������
//        Word[0], ..., Word[NumWordStr-1] �����
//          Word[nf][ns][nw]: ���� �����
//            Int16 NumSym ����� ��������� ����� (�������� ���)
//            Sym[0], ..., Sym[NumSym-1] ����������
//              Sym[nf][ns][nw][nz]:
//                Rect16 Z_Rect     ����� ����������
//                Int16  NumAlt     ����� �����������
//                Alt[0], ..., Alt[NumAlt-1] ������������
//                  Alt[nf][ns][nw][nz][na] ���� ������������
//                    BYTE Code ���
//                    BYTE Prob  �����������
//                    BYTE Spell check end string for - 
//                    BYTE Base  base number ����� �������� ������ ��� ������� ������� RTF (��������� + ���)

// -------  paragraphs mark --------
//if(BEG_PARAG1(Zn[nc][ns][0][0].Title.Z_Id) == 1 ||
//  (Zn[nc][ns][0][0].Title.Z_Num_Alt==1 &&
//   Zn[nc][ns][0][0].Alt[0].a_Code=='#') )
//     ++k_parag[nc];


extern Rect16  *RectFragm;
extern float    Twips;
extern Int16   K_TwipsInInch;

short __cdecl  OpenFullOutTiger(char *FileName)
{
	FILE1 *in;
	int nc,ns,nw,nz,k_word,k_z,i;
#ifdef alDebug
	if(dets)	{ ConsMess("OpenFullOutTiger Begin "); }
#endif
	if((in=fopen_m(FileName,OF_READ)) == NULL)
	{
#ifdef alDebug
		if(dets)	{ ConsMess("OpenFullOutTiger Internal file -- can not open "); }
#endif
		return FALSE;
	}

 fread_m(&ScanResolution,sizeof(WORD),1,in);
 fread_m(&NumCol,sizeof(WORD),1,in);
 fread_m(&NumZ,sizeof(WORD),1,in); 
 fread_m(&NumW,sizeof(WORD),1,in);
 fread_m(&NumS,sizeof(WORD),1,in);

#ifdef alDebug
		if(dets)	{ ConsMess("OpenFullOutTiger ScanResolution=%d ",ScanResolution); }
		if(dets)	{ ConsMess("OpenFullOutTiger NumCol=%d ",NumCol); }
		if(dets)	{ ConsMess("OpenFullOutTiger NumZ=%d ",NumZ); }
		if(dets)	{ ConsMess("OpenFullOutTiger NumW=%d ",NumW); }
		if(dets)	{ ConsMess("OpenFullOutTiger NumS=%d ",NumS); }
#endif

 fread_m(&MonoSpaceAllPage,2,1,in);
 fread_m(&HeiStrAllPage,2,1,in);

 Twips = ((float)K_TwipsInInch)/ScanResolution;
// Twips = (float)((int)(Twips+0.5));
	if(NumCol)
	{
		NumStr    = (int*)malloc_m(NumCol*sizeof(int));
		StatCol   = (STAT_COL*)malloc_m(NumCol*sizeof(STAT_COL)); 
		if(NumStr==NULL||StatCol==NULL)
		{

#ifdef alDebug
		if(dets)	{ ConsMess("OpenFullOutTiger Memory str2367 Numstr,StatCol"); }
#endif
		goto BadReturn;
		}
	}
	else
	{
		NumStr    = NULL;
		StatCol   = NULL;
//#ifdef alDebug
//		if(dets)	{ ConsMess("OpenFullOutTiger  str2375 NumCol=0"); }
//#endif
  goto BadReturn;		
	} 

 BndCol     = (SRECT*)malloc_m(NumCol * sizeof(SRECT));  
 UserNumber = (Word32*)malloc_m(NumCol*sizeof(Word32));
 FragFlag   = (Word32*)malloc_m(NumCol*sizeof(Word32));
 
 RectFragm = (Rect16*)malloc_m(NumCol*sizeof(Rect16)); 

 if(BndCol==NULL||UserNumber==NULL||RectFragm==NULL)
	{
#ifdef alDebug
		if(dets)	{ ConsMess("OpenFullOutTiger Memory str2385 BndCol,UserNumber,RectFragm"); }
#endif
  goto BadReturn;
	}
 // *********** ���������� slope,SizeXGlob,SizeYGlob + OneColStat *********
	
	{
		int fl;                //---Calculate Common Size Section Zn---
  long lenZn,lenWord,lenStr,NumT=NumCol;
  if(NumT+NumS+NumW)
		{
			lenZn=(NumT+NumS+NumW)*sizeof(PTR)+NumZ*sizeof(ZN);
			lenWord=(NumT+NumS)*sizeof(PTR)+NumW*sizeof(TITLE_WORD);
			lenStr=NumT*sizeof(PTR)+NumS*sizeof(TITLE_STR);
			if((fl=InitSubAlloc(2*(lenZn+lenWord+lenStr)+10000,&SubZn)))
			{
#ifdef alDebug
  		if(dets)	{ ConsMess("OpenFullOutTiger str2400 InitSubAlloc"); }
#endif
				goto BadReturn;
			}
		}
	}

  if(NumCol)
		{
			Zn=(ZN****)Submalloc(NumCol*sizeof(ZN***),&SubZn);
			TitleStr=(TITLE_STR**)Submalloc(NumCol*sizeof(TITLE_STR*),&SubZn);
			TitleWord=(TITLE_WORD***)Submalloc(NumCol*sizeof(TITLE_WORD**),&SubZn);
			if(Zn==NULL||TitleStr==NULL||TitleWord==NULL)
			{
#ifdef alDebug
  		if(dets)	{ ConsMess("OpenFullOutTiger Memory str2413 Zn,TitleStr,TitleWord"); }
#endif
				goto BadReturn;
			}
		}

 for(nc=0; nc < NumCol; ++nc)
	{
  fread_m(&RectFragm[nc],1,sizeof(Rect16),in);   
	 // *********** ������ �������� ���������� *************
  fread_m(&NumStr[nc],2,1,in); 
  fread_m(&UserNumber[nc],4,1,in); 
  fread_m(&FragFlag[nc],4,1,in); 

  Zn[nc]=(ZN***)Submalloc((NumStr[nc])*sizeof(ZN**),&SubZn);
  TitleStr[nc]=(TITLE_STR*)Submalloc((NumStr[nc])*sizeof(TITLE_STR),&SubZn);
  TitleWord[nc]=(TITLE_WORD**)Submalloc((NumStr[nc])*sizeof(TITLE_WORD*),&SubZn);
  if(Zn[nc]==NULL||TitleStr[nc]==NULL||TitleWord[nc]==NULL)
   goto BadReturn;
  --NumStr[nc];
  
		for(ns=0; ns <= NumStr[nc]; ++ns)
		{
			TITLE_STR *t = &TitleStr[nc][ns]; 
			int        tmp;

			t->Z_Code = 2;
			t->S_Attr = 0;

			fread_m(&t->S_Rect,sizeof(SRECT),1,in);             // 4 base lines,?
			//exchange : top,bottom - standard, left,right - extern
			tmp              = t->S_Rect.right; 
			t->S_Rect.right  = t->S_Rect.bottom; 
			t->S_Rect.bottom = tmp;  
			//�������� ����. ������!
			fread_m(&t->S_Real_Rect,sizeof(SRECT),1,in);
			fread_m(&tmp,2,1,in); 
			t->S_Gen.S_NumWord = tmp;               // NumWrd

			fread_m(&t->S_Flags,sizeof(t->S_Flags),1,in); //NEGA_STR

			t->S_Gen.HeadLine = 0; // zero HeadLine
			k_word            = TitleStr[nc][ns].S_Gen.S_NumWord-1;
			Zn[nc][ns]        = (ZN**)        Submalloc((k_word+1)*sizeof(ZN*),&SubZn);
			TitleWord[nc][ns] = (TITLE_WORD*) Submalloc((k_word+1)*sizeof(TITLE_WORD),&SubZn);
			if(Zn[nc][ns] == NULL || TitleWord[nc][ns] == NULL)  
				goto BadReturn;

 			for(nw=0; nw <= k_word; ++nw)
			{
				TITLE_WORD *tw=&TitleWord[nc][ns][nw];
				
				tw->Z_Code=1;
				fread_m(&tmp,2,1,in); 
				tw->W_Gen.W_NumSym=tmp;// NumZn
				k_z=tw->W_Gen.W_NumSym-1;
      
				fread_m(&tmp,2,1,in); 
				tw->W_Gen.FontNumber=(WORD)tmp;  

				fread_m(&tmp,2,1,in); 
				tw->W_Gen.FontSize=(WORD)tmp;  
    
				if((Zn[nc][ns][nw]=(ZN*)Submalloc((k_z+1)*sizeof(ZN),&SubZn))==NULL) 
					goto BadReturn;

				for(nz=0; nz <= k_z; ++nz)
				{
					ZN        *z  = &Zn[nc][ns][nw][nz]; 
					TITLE_ZN  *tz = &z->Title; 
					int        num;
#pragma pack(1)
					//   struct RECT_TIGER {int top,left,bottom,right;} rect;
					struct ALT_TIGER1  {unsigned char let, prob;} alt1;
					struct ALT_TIGER2  {unsigned char language, spellnocarrying, FlagCapDrop, spell, base;} alt2;
#pragma pack()

					fread_m(&tz->Z_Rect,sizeof(SRECT),1,in); // BOX
					fread_m(&tz->Z_RealRect,sizeof(SRECT),1,in); // Real BOX

					fread_m(&num,2,1,in);  tz->Z_Num_Alt=(BYTE)MIN(num,REC_MAX_VERS); //NumAlt
//					if(num > 1)
//						num = 1;

					tz->Z_Code     = 0;
					tz->Z_Id.col   = nc+1;
					tz->Z_Id.str   = ns+1;
					tz->Z_Id.comp  = nz+1;
					tz->Z_Id.word  = 0;
					for ( i = 0; i < num; i++ )
					{
						fread_m(&alt1,sizeof(struct ALT_TIGER1),1,in);
						if(i < REC_MAX_VERS)
						{
							ALT_ZN *Alt =&z->Alt[i];
							Alt->a_Code =alt1.let;  
							Alt->a_Prob =alt1.prob;
							Alt->a_Dist=(float)(alt1.prob/256.);
						}
					}
					fread_m(&alt2,sizeof(struct ALT_TIGER2),1,in);
					for ( i=0; i<tz->Z_Num_Alt;i++)
					{
						ALT_ZN *Alt =&z->Alt[i];
						Alt->a_Base =alt2.base;
						Alt->a_language =alt2.language;
						Alt->a_SpellNoCarrying=alt2.spellnocarrying;
						Alt->a_FlagCupDrop=alt2.FlagCapDrop;
						Alt->a_Spell=alt2.spell;
					}
				}
			}
		}
	}
	fclose_m(in);
	--NumCol;
#ifdef alDebug
	if(dets)	{ ConsMess("OpenFullOutTiger  End"); }
#endif
 return TRUE;
BadReturn:
	fclose_m(in);
#ifdef alDebug
	if(dets)	{ ConsMess("OpenFullOutTiger  End"); }
#endif
	return FALSE;

}

/*// !!! Art - ��������
BYTE *AnsiOem,*OemAnsi;
void GenArrAnsiOem(void)
{ uint i;
  for(i=0; i < 256; ++i) AnsiOem[i]=OemAnsi[i]=(BYTE)i;
  AnsiOem[256]=OemAnsi[256]=0;
  #ifdef WIN_MOD
    AnsiToOem((char*)&AnsiOem[1],(char*)&AnsiOem[1]);
    OemToAnsi((char*)&OemAnsi[1],(char*)&OemAnsi[1]);
  #else
  #endif
}*/// !!! Art - ��������
//===��������� Full Out
/*
int PASC CorrTiger(char *FileNameFul,char *FileNameOut,char *FilePar,int fl_cor)
//==
{ int fl;
  AnsiOem=(BYTE*)malloc_m(257); OemAnsi=(BYTE*)malloc_m(257);
  GenArrAnsiOem();
  #ifdef DRAW
    if(viz) viz=fl_cor;
  #endif
  if(fl_cor)
  { 
		//if((fl=InitSpell(FilePar,3,PLAIN))!=0)
		//	return fl-50;
    //if((fl=Init_FeatLet()) != 0)
		//	return fl-100;
  }
  if((fl=OpenFullOutTiger(FileNameFul))!=0) return fl-150;
  if(fl_cor && (fl=CalcStatTiger())!=0) return fl-250;

	//if(fl_cor && (fl=CorrFul()) != 0) //��������� �� �������������
	//	return fl-200;
  //��������� �����.����� Full-������� � ������
  SaveFullOutTiger(FileNameOut);
  free_m(AnsiOem); free_m(OemAnsi);
  return 0;
}
*/
#endif

//������������ ������, ����������� ��� ������ ��������� � ������� .ful-�����
//================
int FreeStructFull(void)
//================
{ int kw=0,nc,ns=0,nw=0,sp=0,num=0,i=0;
  char *err="FreeStructFull";
  #ifdef SUB_ZN
   do0(nc,0,NumCol)
   { 
		if(StatCol[nc].stat_str) free_m(StatCol[nc].stat_str);
     
   } 
	 free_m(StatCol);
	 DeleteSubAlloc(&SubZn);
  #else
   //---������ ���������---
   do0(nc,0,NumCol)
   { 
		 if(StatCol[nc].stat_str) free_m(StatCol[nc].stat_str);
		 do0(ns,0,NumStr[nc])
     { kw=TitleStr[nc][ns].S_Gen.S_NumWord-1;
       do0(nw,0,kw)
       { free_m(Zn[nc][ns][nw]);
         if((sp=TitleWord[nc][ns][nw].W_Gen.W_Spell) == SPELL_CORR)
         {
           #ifndef FUL
             ERR(1,err);
           #endif
           if((num=TitleWord[nc][ns][nw].NumAltSpell) <= 0) ERR(1,err);
           do0(i,0,num-1) free_m(TitleWord[nc][ns][nw].AltSpell[i].Alt);
           free_m(TitleWord[nc][ns][nw].AltSpell);
         }
       }
       free_m(Zn[nc][ns]);
			 free_m(TitleWord[nc][ns]);
     }
     free_m(Zn[nc]);
		 free_m(TitleWord[nc]);
		 free_m(TitleStr[nc]);
   }
   free_m(Zn);
	 free_m(TitleWord);
	 free_m(TitleStr);
  #endif
  
  #ifdef CMP
    free_m(CoorComp);
  #endif
  #ifndef PROF_COL
  	free_m(BndCol);
  #else
  	//---������ �������---
  	free_m(BndCol);
		free_m(NumStr);
		free_m(StatCol);
	  #ifndef TIGER_CORR
	  { int nlev; //������������ ������ ��� �������� �������
	    do0(nlev,0,k_lev)
	    { do0(nc,0,k_col[nlev])
	        if(knot[nlev][nc].kp >= 0)
						free_m(knot[nlev][nc].Addr);
	      free_m(knot[nlev]);
	    }
	    free_m(knot);
	  }
		#endif
  #endif //PROF_COL
  //---������ ���� ���---
  #ifndef TIGER_CORR
  	free_m(FntName);
  #endif
  return 0;
}

#ifndef WIN_MOD

	#if 0
/* // !!! Art - ��������

	#define REWIND 0
  #define REW_SECT 2
  #define NO_REWIND 1
  static int Regim=REWIND,flSect=1;
  FILE1 *f03; //���������������� � InitParTabl
  char buf03[255];
  //Regim = NO_REWIND - ������� ���� ��� ���������������� � ������ �����
  //flSect � ���� ������ ���� ������������� � "1" ����� ������� �����
  //��������� ������
  //�������� �-���, ����������� ������ � Private-ini-������� ��� Windows
  char *fnexts_m(FILE1 *f, char *buf);

	static int findname(char *sect, char *key, char *res)
  { 
		if(Regim==REWIND) 
			    fseek_m(f03, 0l, SEEK_SET);

    while (fnexts_m(f03, buf03))
    { if (*buf03 == ';' || *buf03 == 0) ;
      else if (*buf03 == '[' && buf03[strlen(buf03)-1] == ']')
      { buf03[strlen(buf03)-1] = 0;
        if (strcmp(buf03+1, sect) == 0)
        { while (fnexts_m(f03, buf03))
          { if (*buf03 == ';' || *buf03 == 0) ;
            else if (*buf03 == '[') break;
            else if (buf03[strlen(key)] == '=')
            { buf03[strlen(key)] = 0;
              if (strcmp(buf03, key) == 0)
               {strcpy(res, buf03+strlen(key)+1); return 1;}
            }
          }
          return 0;
        }
      }
    }
    return 0;
  }
  static int findnameNoSect(char *sect, char *key, char *res)
  { 
		while (fnexts_m(f03, buf03))
    { if (*buf03 == ';' || *buf03 == 0) ;
      else if (*buf03 == '[') break;
      else if (buf03[strlen(key)] == '=')
      { buf03[strlen(key)] = 0;
        if (strcmp(buf03, key) == 0)
         {strcpy(res, buf03+strlen(key)+1); return 1;}
      }
    }
    return 0;
  }
  static int GetPrivateProfileInt(char *sect, char *key, int def, char *name)
  { int ret = def;
    char buf[88];
    if(flSect || Regim==REWIND)
         {if (findname(sect, key, buf)) ret = atoi(buf);}
    else
         {if (findnameNoSect(sect, key, buf)) ret = atoi(buf);}
    return ret;
  }
  static int GetPrivateProfileString(char *sect, char *key, char *def, char *res,
         int maxsize, char *name)
  { strcpy(res, def);
    if(flSect || Regim==REWIND)
         {if (!findname(sect, key, res)) strcpy(res, def); }
    else {if (!findnameNoSect(sect, key, res)) strcpy(res, def);}
    return 0;
  }
*/ // !!! Art - ��������
	#endif
#endif

#ifdef SPELL_NEW
/* // !!! Art - ��������
  int b_close(void)
  { SpellClose(); LmClose(); }
  svocab far* PASC b_char(unsigned kod);
  int PASC GetWordOcr(WORD inst, WORD instb,WORD del,WORD  Else,WORD unknow,float delta1,ZN *zz,int kz);
  void PASC SetRegimDelta(int in,int Porog);
  void PASC SetTimerOwn(DWORD NumMs);
*/ // !!! Art - ��������
#endif

#if !defined (LST_WWIN) && !defined (TIGER_CORR)
/*====*/
/* // !!! Art - ��������
int statis(TYPE *arr,int n,TYPE *ave1,TYPE *sig1,TYPE *med,TYPE *mod,int regim)
//====
//���� regim > 0, ������� ���� ������� ������ [0,n] - [n/regim,n-n/regim]
{ int i; long ave=0,sig=0,work;

  u4sort(arr,n+1,sizeof(TYPE),(COMP_FUN)comp1); //�������
  if(regim) //�������� ���� �������
  { int beg=n/regim; if((n-=2*beg)<0){n+=2*beg;goto m1;}
    if(beg) do0(i,0,n) arr[i]=arr[i+beg];
  }
  m1: *med=arr[n>>1];
  do0(i,0,n) ave+=arr[i]; ave/=(long)(n+1);
  do0(i,0,n)
  { work=arr[i]-ave;
    sig+=work*work;
  }
  sig=(long)sqrt(sig/(float)(n+1));
  *ave1=(TYPE)ave; *sig1=(TYPE)sig;
  return 0;
}
*/ // !!! Art - ��������
#endif
//���������� �������� � ����� FULL OUT OCR CRIPT:
// - ������ file.ful � ������� ���������
// - ��������� file.ful � file.txt � ������ ������� � ������������� �
//   �������� �� LeftMargin � �.�.
//��������� ������ �� ����� FileName ������� .ful �� ������� ������� ���������
//Return:
//0  - ���������� ����������,
//-1 - ������ �������� ����� FileName,
//-3 - �������� ������
//-5 - ������ ��������� ����� FileName,
//=============
/* // !!! Art - ��������
int OpenFullOut(char *FileName)
//=============
{ FILE1 *in; int nc,ns,nw,nz,na,k_word,k_z,nc1,ns1,nci,nsi,i,j;
  int num,sp,Len,NumZ;
  long K_MultiComp=1000,NumW,NumS;
  if((in=fopen_m(FileName,OF_READ)) == NULL) return -1;
  //���� ��������� ����� .ful
  fread_m(&TitleFul,sizeof(TITLE_FUL),1,in); 
  #ifdef CMP
    CoorComp=(COOR_COMP*)malloc_m((K_MultiComp+1)*sizeof(COOR_COMP));
    NumComp=-1L;
  #endif
  #ifdef PROF_COL
  //���� ������ �������
  //---��������. �������---
  fseek_m(in,TitleFul.ColOffset,SEEK_SET);
  fread_m(&SizeSectionCol,sizeof(int),1,in);
  fread_m(&NumCol,sizeof(int),1,in);
  ++NumCol;
  BndCol=(SRECT*)malloc_m(NumCol*sizeof(SRECT));
  NumStr=(int*)malloc_m(NumCol*sizeof(int));
  StatCol=(STAT_COL*)malloc_m(NumCol*sizeof(STAT_COL));
  if(BndCol==NULL||NumStr==NULL||StatCol==NULL)return -3;
  fread_m(BndCol,NumCol*sizeof(SRECT),1,in);
  fread_m(NumStr,NumCol*sizeof(int),1,in);
  --NumCol;
  do0(nc,0,NumCol)
  { fread_m(&StatCol[nc].dx_col,sizeof(int),1,in);
    fread_m(&StatCol[nc].dy_col,sizeof(int),1,in);
    fread_m(&StatCol[nc].dsym_col,sizeof(int),1,in);
    fread_m(&StatCol[nc].dy_Upp,sizeof(int),1,in);
    fread_m(&StatCol[nc].dy_Low,sizeof(int),1,in);
  }
  { int nlev; //---�������� �������---
    //������ ��������� ST-�����
    fread_m(&len_col,sizeof(uint),1,in);
    fread_m(name,20,1,in); fl_EditStruct = (name[17]=='Y');
    fread_m(name1,LEN_PATH,1,in);
    fread_m(&k_lev,sizeof(int),1,in);
    do0(j,0,MAX_LEV)fread_m(&k_col[j],sizeof(int),1,in);
    fread_m(&k_colt,sizeof(int),1,in);fread_m(&slope,sizeof(int),1,in);
    fread_m(&k_frm,sizeof(int),1,in); fread_m(&space,sizeof(int),1,in);
    fread_m(&SizeXGlob,sizeof(int),1,in); fread_m(&SizeYGlob,sizeof(int),1,in);
    //k_colt1=-1;//������� ��������. �������
    knot=(LEV**)malloc_m((k_lev+1)*sizeof(LEV*));
    do0(nlev,0,k_lev) //������ ������ �������
    { knot[nlev]=(LEV*)malloc_m((k_col[nlev]+1)*sizeof(LEV));
      do0(nc,0,k_col[nlev])
      { fread_m(&knot[nlev][nc].bnd,sizeof(SRECT),1,in);
        fread_m(&knot[nlev][nc].SpecInt,sizeof(WORD),1,in);
        fread_m(&knot[nlev][nc].kp,sizeof(int),1,in);
        if(knot[nlev][nc].kp >= 0)
        { knot[nlev][nc].Addr=(ADDR*)malloc_m((knot[nlev][nc].kp+1)*sizeof(ADDR));
          do0(i,0,knot[nlev][nc].kp)
          { fread_m(&knot[nlev][nc].Addr[i].ind,sizeof(int),1,in);
            fread_m(&knot[nlev][nc].Addr[i].lev,sizeof(int),1,in);
          }
        }
      }
    }
  }
  #endif
  //���� ������ ���� ���
  fseek_m(in,TitleFul.FntNameOffset,SEEK_SET);
  SizeSectionFntName=(int)(TitleFul.ColOffset-TitleFul.FntNameOffset);//������� � 1
  if((FntName=malloc_m(SizeSectionFntName))==NULL) return NOT_ALLOC;
  fread_m(FntName,SizeSectionFntName,1,in);
  //���� ������ ���������
  fseek_m(in,TitleFul.ZOffset,SEEK_SET);
  #ifdef SUB_ZN
   { int fl;//---Calculate Common Size Section Zn---
     long lenZn,lenWord,lenStr,NumT=NumCol+1,NumS=TitleFul.nStr,NumW=TitleFul.nWord;
     lenZn=(NumT+NumS+NumW)*sizeof(PTR)+TitleFul.dNumZ*sizeof(ZN);
     lenWord=(NumT+NumS)*sizeof(PTR)+NumW*sizeof(TITLE_WORD);
     lenStr=NumT*sizeof(PTR)+NumS*sizeof(TITLE_STR);
     if((fl=InitSubAlloc(lenZn+lenWord+lenStr,&SubZn))) return fl-100;
   }
   Zn=(ZN****)Submalloc((NumCol+1)*sizeof(ZN***),&SubZn);
   TitleStr=(TITLE_STR**)Submalloc((NumCol+1)*sizeof(TITLE_STR*),&SubZn);
   TitleWord=(TITLE_WORD***)Submalloc((NumCol+1)*sizeof(TITLE_WORD**),&SubZn);
  #else
   Zn=(ZN****)malloc_m((NumCol+1)*sizeof(ZN***));
   TitleStr=(TITLE_STR**)malloc_m((NumCol+1)*sizeof(TITLE_STR*));
   TitleWord=(TITLE_WORD***)malloc_m((NumCol+1)*sizeof(TITLE_WORD**));
  #endif
  if(Zn==NULL||TitleStr==NULL||TitleWord==NULL)return -3;
  NumW=NumS=NumZ=0;
  do0(nc,0,NumCol)
  {
    #ifdef SUB_ZN
     Zn[nc]=(ZN***)Submalloc((NumStr[nc]+1)*sizeof(ZN**),&SubZn);
     TitleStr[nc]=(TITLE_STR*)Submalloc((NumStr[nc]+1)*sizeof(TITLE_STR),&SubZn);
     TitleWord[nc]=(TITLE_WORD**)Submalloc((NumStr[nc]+1)*sizeof(TITLE_WORD*),&SubZn);
    #else
     Zn[nc]=(ZN***)malloc_m((NumStr[nc]+1)*sizeof(ZN**));
     TitleStr[nc]=(TITLE_STR*)malloc_m((NumStr[nc]+1)*sizeof(TITLE_STR));
     TitleWord[nc]=(TITLE_WORD**)malloc_m((NumStr[nc]+1)*sizeof(TITLE_WORD*));
    #endif
    if(Zn[nc]==NULL||TitleStr[nc]==NULL||TitleWord[nc]==NULL)return -3;
    do0(ns,0,NumStr[nc])
    { if(!fread_m(&TitleStr[nc][ns],sizeof(TITLE_STR),1,in))
      { int NumC;
        if((NumStr[nc]=ns-1) < 0) NumC=nc-1; else NumC=nc;
        if(NumCol < 0)
        {
          #ifndef WIN_MOD
           printf("err -40");
          #endif
          return -40;
        }

        do0(nci,NumC+1,NumCol) //��������� ������. ��������� �������
        { TITLE_STR *ts=&TitleStr[nci][0];TITLE_WORD *tw=&TitleWord[nci][0][0];
          TITLE_ZN *tz=&Zn[nci][0][0][0].Title;ALT_ZN *Alt=Zn[nci][0][0][0].Alt;
          NumStr[nci]=0;
          #ifdef SUB_ZN
           Zn[nci]=(ZN***)Submalloc((NumStr[nci]+1)*sizeof(ZN**),&SubZn);
           TitleStr[nci]=(TITLE_STR*)Submalloc((NumStr[nci]+1)*sizeof(TITLE_STR),&SubZn);
           TitleWord[nci]=(TITLE_WORD**)Submalloc((NumStr[nci]+1)*sizeof(TITLE_WORD*),&SubZn);
          #else
           Zn[nci]=(ZN***)malloc_m((NumStr[nci]+1)*sizeof(ZN**));
           TitleStr[nci]=(TITLE_STR*)malloc_m((NumStr[nci]+1)*sizeof(TITLE_STR));
           TitleWord[nci]=(TITLE_WORD**)malloc_m((NumStr[nci]+1)*sizeof(TITLE_WORD*));
          #endif
          if(Zn[nci]==NULL||TitleStr[nci]==NULL||TitleWord[nci]==NULL)return -3;
          ts->Z_Code=2;ts->S_Gen.S_NumWord=1;ts->S_Rect=BndCol[nci];
          //ts->S_Rect.left=32000;
          k_word=0;
          #ifdef SUB_ZN
           Zn[nci][0]=(ZN**)Submalloc((k_word+1)*sizeof(ZN*),&SubZn);
           TitleWord[nci][0]=(TITLE_WORD*)Submalloc((k_word+1)*sizeof(TITLE_WORD),&SubZn);
          #else
           Zn[nci][0]=(ZN**)malloc_m((k_word+1)*sizeof(ZN*));
           TitleWord[nci][0]=(TITLE_WORD*)malloc_m((k_word+1)*sizeof(TITLE_WORD));
          #endif
          if(Zn[nci][0]==NULL||TitleWord[nci][0]==NULL)return -3;
          tw->Z_Code=1;tw->W_Gen.W_NumSym=1;tw->W_Gen.W_Spell=SPELL_NOT;
          tw->NumAltSpell=0;//tw->W_Rect=BndCol[nci];
          #ifdef SUB_ZN
           if((Zn[nci][0][0]=(ZN*)Submalloc(sizeof(ZN),&SubZn))==NULL)return -3;
          #else
           if((Zn[nci][0][0]=(ZN*)malloc_m(sizeof(ZN)))==NULL)return -3;
          #endif
          tz->Z_Code=0;tz->Z_Id.col=(CONV)(nci+1);tz->Z_Id.str=(CONV)1;
          tz->Z_Id.comp=(CONV)1;tz->Z_Id.word=(CONV)1;tz->Z_Rect=BndCol[nci];
          tz->Z_Num_Alt=1;tz->Z_NumComp=0;//tz->Z_Rect.left=32000;
          Alt->a_Code='$';
					Alt->a_Base=0;
					Alt->a_Dist=(float)0;
        }
        #ifndef WIN_MOD
          printf("\nEOF1 nc=%d ns=%d",nc,ns);getchW();
        #endif
        goto end;
      }
      if(TitleStr[nc][ns].Z_Code != 2)
      { NumCol=nc;NumStr[nc]=ns-1;
        //fread_m(&TitleStr[nc][ns+1],sizeof(TITLE_STR),1,in);
        #ifndef WIN_MOD
          printf("\nERROR39 STR nc=%d ns=%d",nc,ns);
          //printf("\nZZ=%d ZZ+1=%d",TitleStr[nc][ns].Z_Code,TitleStr[nc][ns+1].Z_Code);
        #endif
        if(!nc && !ns) return -39;
        goto end;
      }
      k_word=TitleStr[nc][ns].S_Gen.S_NumWord-1;
      if(k_word>100)
      {
        #ifndef WIN_MOD
         printf("err -41");
        #endif
        return -41;
      }
      if(k_word < 0) //������ ������
      {
        #ifndef WIN_MOD
         printf("\nEMPTY STRING");
        #else
         //MessageBox(NULL,"EMPTY STRING", "", MB_OK);
        #endif
        #ifndef SUB_ZN
          Zn[nc][ns]=TitleWord[nc][ns]=NULL;
        #endif
        continue;
      }
      ++NumS;
      #ifdef SUB_ZN
       Zn[nc][ns]=(ZN**)Submalloc((k_word+1)*sizeof(ZN*),&SubZn);
       TitleWord[nc][ns]=(TITLE_WORD*)Submalloc((k_word+1)*sizeof(TITLE_WORD),&SubZn);
      #else
       Zn[nc][ns]=(ZN**)malloc_m((k_word+1)*sizeof(ZN*));
       TitleWord[nc][ns]=(TITLE_WORD*)malloc_m((k_word+1)*sizeof(TITLE_WORD));
      #endif
      if(Zn[nc][ns]==NULL||TitleWord[nc][ns]==NULL)return -3;
      do0(nw,0,k_word)
      { TITLE_WORD *tw=&TitleWord[nc][ns][nw];
        if(!fread_m(tw,sizeof(TITLE_WORD),1,in))
        { if((NumStr[nc]=ns-1) < 0) NumCol=nc-1; else NumCol=nc;
          if(NumCol < 0)
          {
            #ifndef WIN_MOD
             printf("err -43");
            #endif
            return -43;
          }
          #ifndef WIN_MOD
            printf("\nEOF2");getchW();
          #endif
          goto end;
        }
        if(tw->Z_Code != 1)
        { //if(!fl_str || tw->Z_Code != 2)
          { NumCol=nc;NumStr[nc]=ns-1;
            #ifndef WIN_MOD
              printf("\nERROR WRD");getchW();
            #endif
            if(!nc && !ns) return -49;
            goto end;
          }
        }
        ++NumW;
        if((sp=tw->W_Gen.W_Spell) == SPELL_CORR)
        { if((num=tw->NumAltSpell) <= 0) return -11;
          #ifndef SUB_ZN
           if((tw->AltSpell=(ALT_SPELL*)malloc_m(num*sizeof(ALT_SPELL)))==NULL)return -3;
          #else
           if((tw->AltSpell=(ALT_SPELL*)Submalloc(num*sizeof(ALT_SPELL),&SubZn))==NULL)return -3;
          #endif
          do0(i,0,num-1)
          { BYTE w; ALT_SPELL *AltS=&tw->AltSpell[i];
            fread_m(&AltS->Len,sizeof(BYTE),1,in);
            fread_m(&w,sizeof(BYTE),1,in);fread_m(&AltS->Penalty,sizeof(WORD),1,in);
            Len=(int)AltS->Len;
            #ifndef SUB_ZN
             if((AltS->Alt=(char*)malloc_m(Len))==NULL)return NOT_ALLOC;
            #else
             if((AltS->Alt=(char*)Submalloc(Len,&SubZn))==NULL)return NOT_ALLOC;
            #endif
            fread_m(AltS->Alt,Len,1,in);
          }
        }
        k_z=tw->W_Gen.W_NumSym-1;
        if(k_z<0 ||  k_z>100)
        { if((NumStr[nc]=ns-1) < 0) NumCol=nc-1; else NumCol=nc;
          if(NumCol < 0)
          {
            #ifndef WIN_MOD
             printf("err -45");
            #endif
            return -45;
          }
          #ifndef WIN_MOD
           printf("\nerr -42 k_z=%d",k_z);
          #endif
          goto end;
        }
        #ifdef SUB_ZN
         if((Zn[nc][ns][nw]=(ZN*)Submalloc((k_z+1)*sizeof(ZN),&SubZn))==NULL)return -3;
        #else
         if((Zn[nc][ns][nw]=(ZN*)malloc_m((k_z+1)*sizeof(ZN)))==NULL)return -3;
        #endif
        do0(nz,0,k_z)
        { ZN *z=&Zn[nc][ns][nw][nz]; TITLE_ZN *tz=&z->Title;
          if(!fread_m(tz,TitleFul.wZHSize,1,in))
          { if((NumStr[nc]=ns-1) < 0) NumCol=nc-1; else NumCol=nc;
            if(NumCol < 0)
            {
              #ifndef WIN_MOD
               printf("err -44");
              #endif
              return -44;
            }
            #ifndef WIN_MOD
              printf("\nEOF3");getchW();
            #endif
            goto end;
          }
          ++NumZ;
          if(tz->Z_Code != 0)
          { NumCol=nc;NumStr[nc]=ns-1;
            #ifndef WIN_MOD
              printf("\nERROR SYM");
            #endif
            goto end;
          }
          nc1=tz->Z_Id.col-1; ns1=tz->Z_Id.str-1;
          if((nc1 != nc || (ns1 != ns && nc==0)) &&
           TitleFul.ProcSpell != PROC_SPELL && TitleFul.ProcSpell != PROC_OPEN)
          {
            #ifndef WIN_MOD
              printf("\nERROR SYM1 Id.col=%d nc=%d",Zn[nc][ns][nw][nz].Title.Z_Id.col-1,nc);
              printf("\nns=%d ns1=%d",ns,ns1);
            #endif
            if(nc==0&&ns==0) //�� � ������ ������� ��� � ������ ������� �� � ������ str
            {
              #ifndef WIN_MOD
              printf("\nBegin Middle");
              #endif
              do0(j,0,ns)
              { do0(i,0,k_word) free_m(Zn[nc][j][i]);
                free_m(Zn[nc][j]); free_m(TitleWord[nc][j]);
              }
              free_m(Zn[nc]); free_m(TitleWord[nc]); free_m(TitleStr[nc]);
              do0(nci,0,nc1-1)//��������� ������.�������
              { TITLE_STR *ts=&TitleStr[nci][0];TITLE_WORD *tw=&TitleWord[nci][0][0];
                TITLE_ZN *tz=&Zn[nci][0][0][0].Title;ALT_ZN *Alt=&Zn[nci][0][0][0].Alt[0];
                NumStr[nci]=0;
                #ifdef SUB_ZN
                 Zn[nci]=(ZN***)Submalloc((NumStr[nci]+1)*sizeof(ZN**),&SubZn);
                 TitleStr[nci]=(TITLE_STR*)Submalloc((NumStr[nci]+1)*sizeof(TITLE_STR),&SubZn);
                 TitleWord[nci]=(TITLE_WORD**)Submalloc((NumStr[nci]+1)*sizeof(TITLE_WORD*),&SubZn);
                #else
                 Zn[nci]=(ZN***)malloc_m((NumStr[nci]+1)*sizeof(ZN**));
                 TitleStr[nci]=(TITLE_STR*)malloc_m((NumStr[nci]+1)*sizeof(TITLE_STR));
                 TitleWord[nci]=(TITLE_WORD**)malloc_m((NumStr[nci]+1)*sizeof(TITLE_WORD*));
                #endif
                if(Zn[nci]==NULL||TitleStr[nci]==NULL||TitleWord[nci]==NULL)return -3;
                ts->Z_Code=2;ts->S_Gen.S_NumWord=1;ts->S_Rect=BndCol[nci];
                k_word=0; //ts->S_Rect.left=32000;
                #ifdef SUB_ZN
                 Zn[nci][0]=(ZN**)Submalloc((k_word+1)*sizeof(ZN*),&SubZn);
                 TitleWord[nci][0]=(TITLE_WORD*)Submalloc((k_word+1)*sizeof(TITLE_WORD),&SubZn);
                #else
                 Zn[nci][0]=(ZN**)malloc_m((k_word+1)*sizeof(ZN*));
                 TitleWord[nci][0]=(TITLE_WORD*)malloc_m((k_word+1)*sizeof(TITLE_WORD));
                #endif
                if(Zn[nci][0]==NULL||TitleWord[nci][0]==NULL)return -3;
                tw->Z_Code=1;tw->W_Gen.W_NumSym=1;tw->W_Gen.W_Spell=SPELL_NOT;
                tw->NumAltSpell=0; //tw->W_Rect=BndCol[nci];
                #ifdef SUB_ZN
                 if((Zn[nci][0][0]=(ZN*)Submalloc(sizeof(ZN),&SubZn))==NULL)return -3;
                #else
                 if((Zn[nci][0][0]=(ZN*)malloc_m(sizeof(ZN)))==NULL)return -3;
                #endif
                tz->Z_Code=0;tz->Z_Id.col=(CONV)(nci+1);tz->Z_Id.str=(CONV)1;
                tz->Z_Id.comp=(CONV)1;tz->Z_Id.word=(CONV)1;tz->Z_Rect=BndCol[nci];
                tz->Z_NumComp=0;tz->Z_Num_Alt=1;//tz->Z_Rect.left=32000;
                Alt->a_Code='$';Alt->a_Base=0;Alt->a_Dist=(float)0;
              }
              #ifdef SUB_ZN
               Zn[nc1]=(ZN***)Submalloc((NumStr[nc1]+1)*sizeof(ZN**),&SubZn);
               TitleStr[nc1]=(TITLE_STR*)Submalloc((NumStr[nc1]+1)*sizeof(TITLE_STR),&SubZn);
               TitleWord[nc1]=(TITLE_WORD**)Submalloc((NumStr[nc1]+1)*sizeof(TITLE_WORD*),&SubZn);
              #else
               Zn[nc1]=(ZN***)malloc_m((NumStr[nc1]+1)*sizeof(ZN**));
               TitleStr[nc1]=(TITLE_STR*)malloc_m((NumStr[nc1]+1)*sizeof(TITLE_STR));
               TitleWord[nc1]=(TITLE_WORD**)malloc_m((NumStr[nc1]+1)*sizeof(TITLE_WORD*));
              #endif
              if(Zn[nc1]==NULL||TitleStr[nc1]==NULL||TitleWord[nc1]==NULL)return -3;
              do0(nsi,0,ns1-1)//��������� ������. ����� �����. �������
              { TITLE_STR *ts=&TitleStr[nc1][nsi];TITLE_WORD *tw=&TitleWord[nc1][nsi][0];
                TITLE_ZN *tz=&Zn[nc1][nsi][0][0].Title;ALT_ZN *Alt=&Zn[nc1][nsi][0][0].Alt[0];
                ts->Z_Code=2;ts->S_Gen.S_NumWord=1;ts->S_Rect=BndCol[nc1];
                k_word=0; //ts->S_Rect.left=32000;
                #ifdef SUB_ZN
                 Zn[nc1][nsi]=(ZN**)Submalloc((k_word+1)*sizeof(ZN*),&SubZn);
                 TitleWord[nc1][nsi]=(TITLE_WORD*)Submalloc((k_word+1)*sizeof(TITLE_WORD),&SubZn);
                #else
                 Zn[nc1][nsi]=(ZN**)malloc_m((k_word+1)*sizeof(ZN*));
                 TitleWord[nc1][nsi]=(TITLE_WORD*)malloc_m((k_word+1)*sizeof(TITLE_WORD));
                #endif
                if(Zn[nc1][nsi]==NULL||TitleWord[nc1][nsi]==NULL)return -3;
                tw->Z_Code=1;tw->W_Gen.W_NumSym=1;tw->W_Gen.W_Spell=SPELL_NOT;
                tw->NumAltSpell=0; //tw->W_Rect=BndCol[nc1];
                #ifdef SUB_ZN
                 if((Zn[nc1][nsi][0]=(ZN*)Submalloc(sizeof(ZN),&SubZn))==NULL)return -3;
                #else
                 if((Zn[nc1][nsi][0]=(ZN*)malloc_m(sizeof(ZN)))==NULL)return -3;
                #endif
                tz->Z_Code=0;tz->Z_Id.col=(CONV)(nc1+1);tz->Z_Id.str=(CONV)(nsi+1);
                tz->Z_Id.comp=(CONV)1;tz->Z_Id.word=(CONV)1;tz->Z_Rect=BndCol[nc1];
                tz->Z_Num_Alt=1;tz->Z_NumComp=0; //tz->Z_Rect.left=32000;
                Alt->a_Code='#';Alt->a_Base=0;Alt->a_Dist=(float)0;
              }
              nc=nc1;ns=ns1-1;
              fseek_m(in,-3L*(long)(TitleFul.wZHSize),SEEK_CUR);
              goto end_str;
            }
            { if(nc1 < nc || nc1-nc != 1)
              { if(ns1 <= ns) return -12;
                else //��������� ������. �����
                { //if(nw != 0 || nz != 0)
                  //{
                  //  #ifndef WIN_MOD
                  //   printf("\nnw=%d nz=%d",nw,nz);
                  //  #endif
                  //  return -13;
                  //}
                  if(ns1 > NumStr[nc]) return -17;
                  TitleStr[nc][ns1]=TitleStr[nc][ns];
                  TitleWord[nc][ns1]=TitleWord[nc][ns];
                  TitleWord[nc][ns1][0]=TitleWord[nc][ns][0];
                  Zn[nc][ns1]=Zn[nc][ns];
                  Zn[nc][ns1][0]=Zn[nc][ns][0];
                  Zn[nc][ns1][0][0].Title=Zn[nc][ns][0][0].Title;
                  do0(nsi,ns,ns1-1)
                  { TITLE_STR *ts=&TitleStr[nc][nsi];TITLE_WORD *tw=&TitleWord[nc][nsi][0];
                    TITLE_ZN *tz=&Zn[nc][nsi][0][0].Title;ALT_ZN *Alt=&Zn[nc][nsi][0][0].Alt[0];
                    ts->Z_Code=2;ts->S_Gen.S_NumWord=1;ts->S_Rect=BndCol[nc];
                    //ts->S_Rect.left=32000;
                    #ifdef SUB_ZN
                     Zn[nc][nsi]=(ZN**)Submalloc(sizeof(ZN*),&SubZn);
                     TitleWord[nc][nsi]=(TITLE_WORD*)Submalloc(sizeof(TITLE_WORD),&SubZn);
                    #else
                     Zn[nc][nsi]=(ZN**)malloc_m(sizeof(ZN*));
                     TitleWord[nc][nsi]=(TITLE_WORD*)malloc_m(sizeof(TITLE_WORD));
                    #endif
                    if(Zn[nc][nsi]==NULL||TitleWord[nc][nsi]==NULL)return -3;
                    tw->Z_Code=1;tw->W_Gen.W_NumSym=1;tw->W_Gen.W_Spell=SPELL_NOT;
                    tw->NumAltSpell=0;//tw->W_Rect=BndCol[nc];
                    #ifdef SUB_ZN
                     if((Zn[nc][nsi][0]=(ZN*)Submalloc(sizeof(ZN),&SubZn))==NULL)return -3;
                    #else
                     if((Zn[nc][nsi][0]=(ZN*)malloc_m(sizeof(ZN)))==NULL)return -3;
                    #endif
                    tz->Z_Code=0;tz->Z_Id.col=(CONV)(nc+1);tz->Z_Id.str=(CONV)(nsi+1);
                    tz->Z_Id.comp=(CONV)1;tz->Z_Id.word=(CONV)1;tz->Z_Rect=BndCol[nc];
                    tz->Z_Num_Alt=1;tz->Z_NumComp=0; //tz->Z_Rect.left=32000;
                    Alt->a_Code='*';Alt->a_Base=0;Alt->a_Dist=(float)0;
                  }
                  ns=ns1; goto alt;
                }
              }
              if(nw != 0 || nz != 0) return -13;
              --NumStr[nc];
              fseek_m(in,-3L*(long)(TitleFul.wZHSize),SEEK_CUR);
              #ifndef SUB_ZN
               free_m(Zn[nc][ns][nw]);free_m(Zn[nc][ns]);free_m(TitleWord[nc][ns]);
              #endif
              goto end_col;
            }
          }
          alt:
          #ifdef CMP
          { int NumC=tz->Z_NumComp-1;
            if(!NumC) //����������� ���� �����
            { if(!fread_m(tz->CoorComp.start_pos,LEN_START,1,in)||
                 !fread_m(&tz->CoorComp.lenght,sizeof(int),1,in))
                   return -42;
               //{ long ww=0;
               //  memmove_m(&ww,tz->CoorComp.start_pos,3);
               //  ww=ww;
               //}
            }
            else if(NumC > 0)//������������
            { long w=NumComp+1;
              memmove_m(tz->CoorComp.start_pos,&w,3);
              do0(i,0,NumC)
              { if(++NumComp > K_MultiComp) return -43;
                if(!fread_m(CoorComp[NumComp].start_pos,LEN_START,1,in)||
                   !fread_m(&CoorComp[NumComp].lenght,sizeof(int),1,in))
                     return -44;
              }
            }
            else return -41;
          }
          #endif
          { int ka=tz->Z_Num_Alt-1,ii;
            float Dmin;
            ALT_ZN Alt,*AltZ=Zn[nc][ns][nw][nz].Alt;
            na=-1;
            do0(i,0,ka)
            { if(!fread_m(&Alt,sizeof(ALT_ZN),1,in))
              {
                #ifndef WIN_MOD
                  printf("\nEOF4");
                #endif
                goto end;
              }
              if(na < REC_MAX_VERS-1)
              { do0(ii,0,na) //���������� ����������� �����������
                  if(AltZ[ii].a_Code == Alt.a_Code) goto end_alt;
                AltZ[++na]=Alt;
                end_alt: continue;
              }
            }
            tz->Z_Num_Alt=(BYTE)(na+1);
            if(na > 0) //����������� Alt-�� � min Dist �����
            { Dmin=AltZ[0].a_Dist; ii=0;
              do0(i,1,na)
               { if(AltZ[i].a_Dist < Dmin) {Dmin=AltZ[i].a_Dist; ii=i;} }
              if(ii) { Alt=AltZ[0]; AltZ[0]=AltZ[ii]; AltZ[ii]=Alt; }
            }
          }
        }
      }
      end_str: continue;
    }
    end_col: continue;
  }
  //if(TitleFul.nWord < NumW || TitleFul.nStr < NumS || TitleFul.dNumZ < NumZ)
  //  return -33;
  end:
  fclose_m(in);
  return 0;
}*/// !!! Art - ��������
//==========��������� FullOutCript
/*int SaveFull(char *FileName)
//==========
{ FILE1 *in; int nc,ns,nw,nz,na,k_word,k_z,sp,num,i;
  char *err="SaveFull";
  if((in=fopen_m(FileName,OF_WRITE)) == NULL) return -1;
  //����� ��������� ����� .ful
  fwrite_m(&TitleFul,sizeof(TITLE_FUL),1,in);
  #ifdef PROF_COL
    //����� ������ �������
    fseek_m(in,TitleFul.ColOffset,SEEK_SET);
    fwrite_m(&SizeSectionCol,sizeof(int),1,in);
    fwrite_m(&NumCol,sizeof(int),1,in);
    fwrite_m(BndCol,(NumCol+1)*sizeof(SRECT),1,in);
    fwrite_m(NumStr,(NumCol+1)*sizeof(int),1,in);
    do0(nc,0,NumCol)
    { fwrite_m(&StatCol[nc].dx_col,sizeof(int),1,in);
      fwrite_m(&StatCol[nc].dy_col,sizeof(int),1,in);
      fwrite_m(&StatCol[nc].dsym_col,sizeof(int),1,in);
      fwrite_m(&StatCol[nc].dy_Upp,sizeof(int),1,in);
      fwrite_m(&StatCol[nc].dy_Low,sizeof(int),1,in);
    }
    { int j,nlev; //---�������� �������---
      fwrite_m(&len_col,sizeof(uint),1,in);
      fwrite_m(name,20,1,in); fl_EditStruct = (name[17]=='Y');
      fwrite_m(name1,LEN_PATH,1,in);
      fwrite_m(&k_lev,sizeof(int),1,in);
      do0(j,0,MAX_LEV)fwrite_m(&k_col[j],sizeof(int),1,in);
      fwrite_m(&k_colt,sizeof(int),1,in);fwrite_m(&slope,sizeof(int),1,in);
      fwrite_m(&k_frm,sizeof(int),1,in); fwrite_m(&space,sizeof(int),1,in);
      fwrite_m(&SizeXGlob,sizeof(int),1,in); fwrite_m(&SizeYGlob,sizeof(int),1,in);
      do0(nlev,0,k_lev)
      { do0(nc,0,k_col[nlev])
        { fwrite_m(&knot[nlev][nc].bnd,sizeof(SRECT),1,in);
          fwrite_m(&knot[nlev][nc].SpecInt,sizeof(WORD),1,in);
          fwrite_m(&knot[nlev][nc].kp,sizeof(int),1,in);
          if(knot[nlev][nc].kp >= 0)
          { do0(i,0,knot[nlev][nc].kp)
            { fwrite_m(&knot[nlev][nc].Addr[i].ind,sizeof(int),1,in);
              fwrite_m(&knot[nlev][nc].Addr[i].lev,sizeof(int),1,in);
            }
          }
        }
      }
    }
  #endif
  //����� ������ ���� ���
  fseek_m(in,TitleFul.FntNameOffset,SEEK_SET);
  fwrite_m(FntName,SizeSectionFntName,1,in);
  //����� ������ ���������
  fseek_m(in,TitleFul.ZOffset,SEEK_SET);
  do0(nc,0,NumCol)
  { do0(ns,0,NumStr[nc])
    { fwrite_m(&TitleStr[nc][ns],sizeof(TITLE_STR),1,in);
      k_word=TitleStr[nc][ns].S_Gen.S_NumWord-1;
      if( k_word>100) return -41;
      do0(nw,0,k_word)
      { fwrite_m(&TitleWord[nc][ns][nw],sizeof(TITLE_WORD),1,in);
        if((sp=TitleWord[nc][ns][nw].W_Gen.W_Spell) == SPELL_CORR)
        { if((num=TitleWord[nc][ns][nw].NumAltSpell) <= 0)
            ERR(1,err);
          do0(i,0,num-1)
          { BYTE w;
            fwrite_m(&TitleWord[nc][ns][nw].AltSpell[i].Len ,sizeof(BYTE),1,in);
            fwrite_m(&w ,sizeof(BYTE),1,in);
            fwrite_m(&TitleWord[nc][ns][nw].AltSpell[i].Penalty,sizeof(WORD),1,in);
            fwrite_m(TitleWord[nc][ns][nw].AltSpell[i].Alt,(int)TitleWord[nc][ns][nw].AltSpell[i].Len,1,in);
          }
        }
        k_z=TitleWord[nc][ns][nw].W_Gen.W_NumSym-1;
        if(k_z<0 ||  k_z>100) return -45;
        do0(nz,0,k_z)
        {
          #ifndef CMP
            Zn[nc][ns][nw][nz].Title.Z_NumComp=0;
          #endif
          fwrite_m(&Zn[nc][ns][nw][nz].Title,TitleFul.wZHSize,1,in);
          #ifdef CMP
          { int NumC=Zn[nc][ns][nw][nz].Title.Z_NumComp;
            if(NumC == 1) //����������� ���� �����
            { fwrite_m(Zn[nc][ns][nw][nz].Title.CoorComp.start_pos,LEN_START,1,in);
              fwrite_m(&Zn[nc][ns][nw][nz].Title.CoorComp.lenght,sizeof(int),1,in);
            }
            else if(NumC > 1)//������������
            { long w=0L;
              memmove_m(&w,Zn[nc][ns][nw][nz].Title.CoorComp.start_pos,3);
              do0(i,0,NumC)
              { fwrite_m(CoorComp[w].start_pos,LEN_START,1,in);
                fwrite_m(&CoorComp[w].lenght,sizeof(int),1,in);
                ++w;
              }
            }
          }
          #endif
          do0(na,0,Zn[nc][ns][nw][nz].Title.Z_Num_Alt-1)
            fwrite_m(&Zn[nc][ns][nw][nz].Alt[na],sizeof(ALT_ZN),1,in);
        }
      }
    }
  } 
  fclose_m(in); return 0;
}*/// !!! Art - ��������

//=========��� �����������.���������� Return=0
/* // !!! Art - ��������
uchar GetAlt(int nc,int ns,int nw,int nz,int na)
//=========
{ int ka=Zn[nc][ns][nw][nz].Title.Z_Num_Alt;
  if(ka <= 0 || na >= ka) return 0;
  else return Zn[nc][ns][nw][nz].Alt[na].a_Code;
}
*/ // !!! Art - ��������

//���������� ��������. ���������� �� �������-������� � ����� (������)
/* // !!! Art - ��������
float DistMaxWord(ZN *Zn,int kz)
{ float Dmax,d; int i,j;
  Dmax=(float)0.;
  do0(i,0,kz) do0(j,0,((int)Zn[i].Title.Z_Num_Alt-1))
     if( (d=Zn[i].Alt[j].a_Dist) > Dmax) Dmax=d;
  return Dmax;
}
*/ // !!! Art - ��������
//===����� ��������� SPELL-������������ � ������������ � �������������� OCR
/* // !!! Art - ��������
int ChooseBestWord(ZN *Zn,TITLE_WORD t,char *word,float k1,float k2,
    float gam_Ins,float gam_Del)
{ int kz=t.W_Gen.W_NumSym-1,fl,i,num;
  fl=0;//Test Full NoRecog Word
  do0(i,0,kz) if(Zn[i].Title.Z_Num_Alt > 0) {fl=1; break;}
  do0(i,0,kz)
  { if(Zn[i].Title.Z_Num_Alt > 0) word[i]=Zn[i].Alt[0].a_Code;
    else word[i]=par_ful.KodNoRecogOut;
  }
  word[kz+1]=0;
  if(t.W_Gen.W_Spell != SPELL_CORR || !fl || !t.NumAltSpell)return 0;//Return OCR-Word
  if((num=ChooseRelationMin(t)) >= 0)
    { strcpy_m(word,t.AltSpell[num].Alt); return 1; }
  else return 0;
}
*/ // !!! Art - ��������
//Return - >= 0 - ������ ��������� ���������, < 0 - ��������� ���
//-------------����� ��������� ��������� �� ����������� �� ������ �� ��
/* // !!! Art - ��������
int ChooseRelationMin(TITLE_WORD t)
{ int num=t.NumAltSpell; float Rel;
  if(num <= 0) return -1;
  else if(num == 1) return 0;
  else
  { Rel=((float)t.AltSpell[1].Penalty)/((float)t.AltSpell[0].Penalty);
    if(Rel > par_ful.RelKrit) return 0;
    else //������ �� ������������ 0-�� � 1-�� ��������� � ��������� �� ��������
    { int k0=strlen_m(t.AltSpell[0].Alt),k1=strlen_m(t.AltSpell[1].Alt),i;
      BYTE cod0,cod1;
      if(k0 == k1)
      { --k0;
        do0(i,0,k0)
        { cod0=t.AltSpell[0].Alt[i]; cod1=t.AltSpell[1].Alt[i];
          if(cod0 != cod1 && cod0 != UppLow[cod1]) return -1;
        }
        return 0;
      }
    }
  }
  return -1;
}
*/ // !!! Art - ��������
#ifndef WIN_MOD
/* // !!! Art - ��������
void AnsiToOem(const char *word1,char *word2)
{;}

void OemToAnsi(const char *word1,char *word2)
{;}
*/ // !!! Art - ��������
#endif
//============�������� � Full-��������� ����� ����� ������� <nc,ns,nw>
/*
int InsertWord(int nc,int ns,int nw,ZN *zi,int kz)
//============
{ int k_word=++TitleStr[nc][ns].S_Gen.S_NumWord,i;
  TITLE_WORD *t=TitleWord[nc][ns]; ZN **z=Zn[nc][ns];
  #ifndef SUB_ZN
   if((t=(TITLE_WORD*)realloc_m(t,(k_word-1)*sizeof(TITLE_WORD),k_word*sizeof(TITLE_WORD)))==NULL)return -3;
   if((z=(ZN**)realloc_m(z,(k_word-1)*sizeof(ZN*),k_word*sizeof(ZN*)))==NULL)return -3;
  #else
   t=(TITLE_WORD*)Submalloc(k_word*sizeof(TITLE_WORD),&SubZn);
   z=(ZN**)Submalloc(k_word*sizeof(ZN*),&SubZn);
   if(t==NULL||z==NULL)return NOT_ALLOC;
   memcpy(t,TitleWord[nc][ns],(k_word-1)*sizeof(TITLE_WORD));
   memcpy(z,Zn[nc][ns],(k_word-1)*sizeof(ZN*));
  #endif
  TitleWord[nc][ns]=t; Zn[nc][ns]=z;
  doi(i,k_word-2,nw+1) { t[i+1]=t[i]; z[i+1]=z[i]; }
  t[nw+1].Z_Code=1;t[nw+1].W_Gen.W_NumSym=kz+1;
  t[nw+1].W_Gen.W_Spell=SPELL_NOT;t[nw+1].NumAltSpell=0;
  #ifndef SUB_ZN
   if((z[nw+1]=(ZN*)malloc_m((kz+1)*sizeof(ZN)))==NULL)return -3;
  #else
   if((z[nw+1]=(ZN*)Submalloc((kz+1)*sizeof(ZN),&SubZn))==NULL)return -3;
  #endif
  do0(i,0,kz) z[nw+1][i]=zi[i];
  return 0;
}
*/// !!! Art - ��������
//s - ������� �����(����� ������ ������������), frm - ����� ���� �����
//Return= 0 - OK, -1 - ������ ������� ����������
//�������� � Full-��������� ����� ����� ������� <nc,ns,nw>
//====================!!���� ���� �������� ����� ����� ������ ������,�� nw=-1
/*int InsertWordFirstAlt(int nc,int ns,int nw,char *s,SRECT *frm)
//====================
{ int kz=strlen_m(s),i,num,nw1,fl;
  CONV comp,ns_curr;
  ZN *zi;
  //---�������� �����---
  if(nc < 0 || nc > NumCol || ns < 0 || ns > NumStr[nc] ||
     nw < -1 || nw > TitleStr[nc][ns].S_Gen.S_NumWord-1) return -1;
  zi=(ZN*)malloc_m(kz*sizeof(ZN));
  --kz; comp=0; ns_curr=Zn[nc][ns][0][0].Title.Z_Id.str;
  if(nw >= 0)//��������� �� � ������ ������
  { num=TitleWord[nc][ns][nw].W_Gen.W_NumSym-1;
    comp=Zn[nc][ns][nw][num].Title.Z_Id.comp;
  }
  do0(i,0,kz)
  { zi[i].Title.Z_Code=0;
    zi[i].Title.Z_Id.col=(CONV)(nc+1);
    zi[i].Title.Z_Id.str=ns_curr;
    zi[i].Title.Z_Id.comp=(CONV)(comp+i+1);
    zi[i].Title.Z_Id.word=0;
    zi[i].Title.Z_Rect=frm[i];
    zi[i].Title.Z_Num_Alt=1;
    zi[i].Alt[0].a_Code=s[i];
    zi[i].Alt[0].a_Dist=(float)0.;
    zi[i].Alt[0].a_Base=0;
  }
  do0(nw1,nw+1,TitleStr[nc][ns].S_Gen.S_NumWord-1)//������������� ���� Z_Id.comp
    do0(i,0,TitleWord[nc][ns][nw1].W_Gen.W_NumSym-1)
      Zn[nc][ns][nw1][i].Title.Z_Id.comp+=(CONV)(kz+1);
  fl=InsertWord(nc,ns,nw,zi,kz);
  free_m(zi); return fl;
}*/// !!! Art - ��������
//������� ����� �� ful-��������
//============Return= 0 - ����� �������
/* // !!! Art - ��������
int DelWordFul(int nc,int ns,int nw)
{ int k_word=TitleStr[nc][ns].S_Gen.S_NumWord-1,i;
  TITLE_WORD *t=TitleWord[nc][ns],t1=t[nw];
  ZN **z=Zn[nc][ns];
  //---�������� �����---
  if(nc < 0 || nc > NumCol || ns < 0 || ns > NumStr[nc] ||
     nw < 0 || nw > TitleStr[nc][ns].S_Gen.S_NumWord-1) return -1;
  if(t1.NumAltSpell > 0 && t1.W_Gen.W_Spell == SPELL_CORR) //---������������ ��������� Spell---
  { do0(i,0,t1.NumAltSpell-1) free_m(t1.AltSpell[i].Alt);
    free_m(t1.AltSpell);
  }
  free_m(z[nw]);
  do0(i,nw,k_word-1) { t[i]=t[i+1]; z[i]=z[i+1]; }
  if(k_word  > 0) TitleStr[nc][ns].S_Gen.S_NumWord=k_word;
  else  //Del String
  { TITLE_STR  *s=TitleStr[nc];
    TITLE_WORD **t=TitleWord[nc];
    ZN         ***z=Zn[nc];
    free_m(z[ns]); free_m(t[ns]);
    do0(i,ns,NumStr[nc]-1) { s[i]=s[i+1]; t[i]=t[i+1]; z[i]=z[i+1]; }
    if((--NumStr[nc]) < 0 && NumCol > 0) //Del Column
    { TITLE_STR  **s=TitleStr;
      TITLE_WORD ***t=TitleWord;
      ZN         ****z=Zn;
      free_m(z[nc]); free_m(t[nc]); free_m(s[nc]);
      do0(i,nc,NumCol-1)
      { s[i]=s[i+1]; t[i]=t[i+1]; z[i]=z[i+1];
        NumStr[i]=NumStr[i+1]; BndCol[i]=BndCol[i+1]; StatCol[i]=StatCol[i+1];
      }
      --NumCol;
    }
  }
  return 0;
}
*/ // !!! Art - ��������
#ifdef FUL
//LastCod - ��������� ��� ��������. �����
//FirstCod - ������ ��� ������� �����
//Return: 1 - ����� � ������� ����� � ������ �����������
//       -1 - ����� � ������� ����� � �������� �����������
//       0 - ����� � ����� �����
//============
/* // !!! Art - ��������
int TstNameOwr(uchar LastCod,uchar FirstCod)
//============
{
  if(Upper[(int)FirstCod]==0) return 0;
  else if(EndSentence[(int)LastCod]==1) return 1;
  else return -1;
} 
*/ // !!! Art - ��������
/*// !!! Art - ��������
#define MAX_LEN 50 //max ����� ������ � ���������
#define MAX_STR 249 //max ����� ������ � ���������

WORD Penalty1LenWord(int n)
{ static WORD Pen[16]={ 0,2,3,3,3,6,6,8,8,8,8,8,8,8,8,8 },ii=1,i;
  if(n < 0)
  { char *str=malloc_m(250),param[MAX_LEN+1],*s,*err="Penalty1LenWord";
    ii=0; s=str;
    GetPrivateProfileString("orfo","REMAX","",s,100,FileParSpel);
    do0(i,0,15) {s=get_param(s,param,MAX_LEN-1); Pen[i]=(WORD)atoi(param);}
    free_m(str); return 0;
  }
  return Pen[MIN(n,15)];
}
*/ // !!! Art - ��������
/* // !!! Art - ��������
#define MAX_ZN_WORD 100
int MaxZn=MAX_ZN_WORD; ZN *zBuf;

FUNC_GEN_FULWORD FuncGenFulWord;
 */ // !!! Art - ��������
//����-��� ���������� Spell ��� ������ ������ ����� - ����� �� �������� �����
//�-��� ��������� ������ ����� �� ��� ID_SYM
//int AddrFuncGenFulWord(ID_SYM id,ZN *z,int MaxZn)
//Return: ����� �������� �����
//===
/* // !!! Art - ��������
int PASC InitOneSpell(FUNC_GEN_FULWORD AddrFuncGenFulWord)
//===
{ if((zBuf=(ZN*)malloc_m(MaxZn*sizeof(ZN)))==NULL) return NOT_ALLOC;
  ExtSettings=1;//����� ������� �������� ������� for FindWord
  FuncGenFulWord=AddrFuncGenFulWord;  return 0;
}
*/ // !!! Art - ��������
/* // !!! Art - ��������
void PASC CloseOneSpell(void)
  { free_m(zBuf); }
*/ // !!! Art - ��������
#else
 //int InitSpell(char *PAthName,int cod,int TypeDoc1) {return 0;}
/* // !!! Art - ��������
 int TstNameOwr(uchar LastCod,uchar FirstCod) {return 0;}
*/ // !!! Art - ��������
#endif
//======================���� �� ���� ����������
/*int TestPunctuationMarks(char sym)
//======================
  { return (int)PunctuationMarks[(int)sym]; }
*/ // !!! Art - ��������
//===========����-��� ���������� Spell
/* // !!! Art - ��������
int PASC InitSpell(char *PathName,int cod,int TypeDoc)
//===========
{
 #if defined (WIN_MOD) && !defined (TIGER_CORR) 
	FILE1 *f; 
  int OpenFlag;
 #endif
  if(PathName == NULL)
  { char WinPath[100];
		WinPath[0]=0;
		_splitpath(WinPath,drive,dir,NULL,NULL);

    #ifndef SPELL_NEW
      if(TypeDoc!=NORV) _makepath(FileParSpel,drive,dir,"orfo","ini");
      else _makepath(FileParSpel,drive,dir,"norveg","ini");
    #endif
  }
  else strcpy_m(FileParSpel,PathName);
  #if defined (WIN_MOD) && !defined (TIGER_CORR)
    OpenFlag=chk_open();
    { if(OpenFlag >= 0) b_close();
      f=b_open(FileParSpel,GetAddrFunKill(), NULL, cod);
      if(f->hFile != HFILE_ERROR)
      { //int nc = b_erflag();
        fclose_m(f);
      }
      else return -5;
    }
  #else
    if( (f03=fopen_m("c:\\alik\\orfo.ini",OF_READ)) == NULL)
    	return -1;
  #endif
  init_ful();
  #ifdef WIN_MOD
  //  SetRegimDelta(par_ful.IndexDelta);
  #endif
//  free_m(f03);    //mycorr
  return 0;
}
*/// !!! Art - ��������
/* // !!! Art - ��������
void close_f03(void)
{
 //fclose_m(f03);
}
*/ // !!! Art - ��������
//==============
// !!! Art - ��������
//
//int Init_FeatLet(void)
////==============
//{ STATIC char *p,buf[8]; int i,k;
//  STATIC BYTE UpLine[14];//��������� �� �������
//  STATIC BYTE DownLine[25];//
//  STATIC BYTE Chif[25];
//  STATIC BYTE ImUppLow[80];
//  STATIC BYTE DelimSubWord[16];
//  WORD w;
//  if(TypeDoc!=NORV)
//  #ifdef WIN_MOD
//  { strcpy_m((char*)UpLine,(char*)"���bdfhijklt");
//    strcpy_m((char*)DownLine,(char*)"���������Qfgjpqy,;");
//    strcpy_m((char*)Chif,(char*)"0123456789()[]{}+-*/=.,");
//    strcpy_m((char*)ImUppLow,(char*)"���������������������������������������������������������CcKkOoPpSsUuVvWwXxZz");
//    strcpy_m((char*)DelimSubWord,"/-()[]{}\"\\'");
//  }
//  #else
//  { strcpy_m((char*)UpLine,(char*)"���bdfhijklt");
//    strcpy_m((char*)DownLine,(char*)"���������Qfgjpqy,;");
//    strcpy_m((char*)Chif,(char*)"0123456789()[]{}+-*/=.,");
//    strcpy_m((char*)ImUppLow,(char*)"���������������������������������������������������������CcKkOoPpSsUuVvWwXxZz");
//    strcpy_m((char*)DelimSubWord,"/-()[]{}\"\\'");
//  }
//  #endif
//  else
//  { strcpy_m((char*)UpLine,(char*)"bdfhijklt");
//    strcpy_m((char*)Chif,(char*)"0123456789.");
//    strcpy_m((char*)ImUppLow,(char*)"CcKkOoPpSsUuVvWwXxZz��");
//    strcpy_m((char*)DelimSubWord,(char*)"/-()[]\"\\'");
//  }
//  do0(i,0,255)
//   FeatLet[i].Let=FeatLet[i].Chif=FeatLet[i].DelimSubWord=FeatLet[i].ImUppLow=0;
//  #if defined (WIN_MOD) && !defined (TIGER_CORR)
//    #ifndef SPELL_NEW
//      b_getlang(buf);//���������� ������� ����� ������ ("RE")
//    #else
//      SpellGetLang(buf);
//    #endif
//    p=buf;
//    while(*p!=0)
//    { svocab *pl=b_char(*p);
//      #ifndef SPELL_NEW
//       char *Lo=pl->tablet0,*Up=pl->tabletl0;//������� ������� � ����� �����.��.
//      #else
//       char *LoIn,*UpIn,*Lo,*Up;
//       if(pl==NULL) return -5;
//       LoIn=pl->Ltablet; UpIn=pl->Ltabletl;
//       Lo=malloc_m(strlen_m(LoIn)+2); strcpy_m(Lo,LoIn); LmUnWord(Lo);
//       Up=malloc_m(strlen_m(UpIn)+2); strcpy_m(Up,UpIn); LmUnWord(Up);
//      #endif
//      while(*Lo != 0)
//      { if(*Up != '-' && *Up != '\'')
//        { FeatLet[(BYTE)(*Up)].Let=FeatLet[(BYTE)(*Lo)].Let=1;
//          FeatLet[(BYTE)(*Up)].Lang=(*p == 'R') ? RUS : LAT;
//          FeatLet[(BYTE)(*Lo)].Lang=(*p == 'R') ? RUS : LAT;
//          FeatLet[(BYTE)(*Up)].Shift=FeatLet[(BYTE)(*Up)].PosUpLine=UPP;
//          FeatLet[(BYTE)(*Lo)].Shift=FeatLet[(BYTE)(*Lo)].PosUpLine=LOW;
//          FeatLet[(BYTE)(*Up)].PosDownLine=FeatLet[(BYTE)(*Lo)].PosDownLine=LOW;
//        }
//        ++Lo;++Up;
//      }
//      ++p;
//      #ifdef SPELL_NEW
//        free_m(Lo); free_m(Up);
//      #endif
//    }
//  #else
//    { BYTE *loR=(BYTE*)malloc_m(40),*upR=(BYTE*)malloc_m(40),*Lo,*Up;
//      BYTE *loE=(BYTE*)malloc_m(40),*upE=(BYTE*)malloc_m(40),*bLo[2],*bUp[2];
//      k=-1; bLo[0]=loR; bUp[0]=upR; bLo[1]=loE; bUp[1]=upE;
//      strcpy_m((char*)loR,"��������������������������������-");
//      strcpy_m((char*)upR,"��������������������������������-");
//      strcpy_m((char*)loE,"abcdefghijklmnopqrstuvwxyz-\'");
//      strcpy_m((char*)upE,"ABCDEFGHIJKLMNOPQRSTUVWXYZ-\'");
//      for(i=0; i < 2; ++i)
//      { Lo=bLo[i]; Up=bUp[i];
//        while(*Lo != 0)
//        { if(*Up != '-' && *Up != '\'')
//          { FeatLet[(BYTE)(*Up)].Let=FeatLet[(BYTE)(*Lo)].Let=1;
//            FeatLet[(BYTE)(*Up)].Lang= !i ? RUS : LAT;
//            FeatLet[(BYTE)(*Lo)].Lang= !i ? RUS : LAT;
//            FeatLet[(BYTE)(*Up)].Shift=FeatLet[(BYTE)(*Up)].PosUpLine=UPP;
//            FeatLet[(BYTE)(*Lo)].Shift=FeatLet[(BYTE)(*Lo)].PosUpLine=LOW;
//            FeatLet[(BYTE)(*Up)].PosDownLine=FeatLet[(BYTE)(*Lo)].PosDownLine=LOW;
//          }
//          ++Lo;++Up;
//        }
//      }
//      free_m(loR); free_m(upR); free_m(loE); free_m(upE);
//    }
//  #endif
//  k=strlen_m((char*)UpLine)-1;   do0(i,0,k) FeatLet[UpLine[i]].PosUpLine=UPP;
//  k=strlen_m((char*)DownLine)-1; do0(i,0,k) FeatLet[DownLine[i]].PosDownLine=UPP;
//  k=strlen_m((char*)Chif)-1;     do0(i,0,k) FeatLet[Chif[i]].Chif=1;
//  k=strlen_m((char*)ImUppLow)-1; do0(i,0,k) FeatLet[ImUppLow[i]].ImUppLow=1;
//  k=strlen_m((char*)DelimSubWord)-1;
//  do0(i,0,k) FeatLet[DelimSubWord[i]].DelimSubWord=1;
//  //---������ ��������� �������������---
//  { BYTE *str=(BYTE*)malloc_m(50);
//    BYTE *NameCl=(BYTE*)malloc_m(20);
//    BYTE *NameClOne=(BYTE*)malloc_m(20);
//    int in,k,*act;
//    STATIC BYTE *s;
//    strcpy_m((char*)NameCl,"Clust0");
//    strcpy_m((char*)NameClOne,"ClustOneSym0");
//    ClustOCR=(BYTE**)malloc_m(MAX_CLUST*sizeof(BYTE*));
//    act=(int*)malloc_m(MAX_CLUST*sizeof(int));
//    K_Clust=-1;
//    for(;;) //Common Cluster
//    { GetPrivateProfileString("corr_word",(char *)NameCl,"",(char *)str,49,(char *)FileParSpel);
//      if(str[0] == 0) break;//������ ��������� ��������
//      ClustOCR[++K_Clust]=(BYTE*)malloc_m(((k=strlen_m((char*)str))+1));
//      --k; strcpy_m((char *)ClustOCR[K_Clust],(char *)str);
//      do0(i,0,k)
//        FeatLet[(BYTE)str[i]].IndCl=K_Clust+1;
//      //NameCl[5]=++cod;
//      ++NameCl[strlen_m((char*)NameCl)-1];
//    }
//    for(;;) //Cluster One Sym
//    { GetPrivateProfileString("corr_word",(char *)NameClOne,"",(char *)str,49,FileParSpel);
//      if(str[0] == 0) break;//������ ��������� ��������
//      do0(i,0,K_Clust) act[i]=0;
//      k=0;while(str[++k]!=':') if(k>48) goto EndOne;
//      s=&str[k+1]; --k;
//      do0(i,0,k)
//      { in=FeatLet[(BYTE)str[i]].IndCl-1;//������ old-cluster
//        if(in < 0) //Letter not found Cluster
//        { ClustOCR[++K_Clust]=(BYTE*)malloc_m(strlen_m((char*)s)+1);
//          strcpy_m((char *)ClustOCR[K_Clust],(char *)s);
//          in=K_Clust;
//        }
//        else if(act[in] == 0) //Create new Cluster
//        { 
//					ClustOCR[++K_Clust]=(BYTE*)malloc_m(strlen_m((char*)s)+strlen_m((char*)ClustOCR[in])+2);
//          strcpy_m((char*)ClustOCR[K_Clust],(char*)ClustOCR[in]); 
//					strcat((char*)ClustOCR[K_Clust],(char*)s);
//          act[in]=K_Clust; in=K_Clust;
//        }
//        else in=act[in];
//        FeatLet[(BYTE)str[i]].IndCl=in+1;
//      }
//      ++NameClOne[strlen_m((char*)NameClOne)-1];
//    }
//    EndOne:
//    GetPrivateProfileString("corr_word","PairDifShift","",(char*)str,49,FileParSpel);
//    do0(i,0,255) UppLowOther[i]=0;
//    i=0;
//    while(str[i]!=0)
//    { if(str[i+1]==0) break;
//      UppLowOther[str[i]]=str[i+1]; UppLowOther[str[i+1]]=str[i];
//      if(str[i+2]==0) break;
//      i+=3;
//    }
//
//		if(K_Clust>=0)
//		{
//		  do0(i,0,K_Clust) free_m(ClustOCR[i]);
//    } 
//    free_m(ClustOCR);
//    free_m(str);
//		free_m(act);
//		free_m(NameCl);
//		free_m(NameClOne);
//  }
//  //---������ ������� ������������� ����. Alt---
//  par_ful.AllowTestChifCl=GetPrivateProfileInt("corr_word","AllowTestChifCl",0,FileParSpel);
//  par_ful.AllowTestLangCl=GetPrivateProfileInt("corr_word","AllowTestLangCl",0,FileParSpel);
//  par_ful.AllowCorrChifCl=GetPrivateProfileInt("corr_word","AllowCorrChifCl",0,FileParSpel);
//  par_ful.AllowCorrLangCl=GetPrivateProfileInt("corr_word","AllowCorrLangCl",0,FileParSpel);
//  par_ful.AllowCorr=GetPrivateProfileInt("corr_word","AllowCorr",0,FileParSpel);
//  par_ful.AllowCorrShift=GetPrivateProfileInt("corr_word","AllowCorrShift",0,FileParSpel);
//  par_ful.AllowCorrChifLet=GetPrivateProfileInt("corr_word","AllowCorrChifLet",0,FileParSpel);
//  par_ful.AllowCorrRusLat=GetPrivateProfileInt("corr_word","AllowCorrRusLat",0,FileParSpel);
//  par_ful.AllowCorrFnt=GetPrivateProfileInt("corr_word","AllowCorrFnt",0,FileParSpel);
//  par_ful.DelStick=(float)(GetPrivateProfileInt("corr_word","DelStick",0,FileParSpel)/100.);
//  w=(WORD)GetPrivateProfileInt("ful_txt","RelDistAltOCR",(int)10,FileParSpel);
//  par_ful.RelDistAltOCR=(float)(w/10.);
//  return 0;
//}
// !!! Art - ��������
//============
/* // !!! Art - ��������
int init_ful()
//============
{ uchar Punct[22]=",.<>?/:;\"'\\[]-{}()!";int NumPunct=18,i;
  #ifdef WIN_MOD
    uchar WordOneSymb[]="��������������������";
  #else
    uchar WordOneSymb[]="��������������������";
  #endif
  int NumOne;
  BYTE s[3];WORD w;
  static uchar Upp[159]="��������������������������������ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static uchar Low[159]="��������������������������������abcdefghijklmnopqrstuvwxyz";
  static uchar Punct1[5]=".!?";
  static int k=57,k1=2;
  char *p=0,buf[8];
	buf[0]=0;
  if(TypeDoc==NORV) strcpy_m((char*)WordOneSymb,"&");
  do0(i,0,255) {Upper[i]=Lower[i]=EndSentence[i]=0;UppLow[i]=(uchar)i;}
  #if defined (WIN_MOD) && !defined (TIGER_CORR)
    #ifndef SPELL_NEW
      b_getlang(buf);//���������� ������� ����� ������ ("RE")
    #else
      SpellGetLang(buf);
    #endif
    p=buf; k=-1;
    while(*p!=0)
    { svocab *pl=b_char(*p);
      #ifndef SPELL_NEW
       char *Lo=pl->tablet0,*Up=pl->tabletl0;//������� ������� � ����� �����.��.
      #else
       char *LoIn,*UpIn,*Lo,*Up;
       if(pl==NULL) return -5;
       LoIn=pl->Ltablet; UpIn=pl->Ltabletl;
       Lo=malloc_m(strlen_m(LoIn)+2); strcpy_m(Lo,LoIn); LmUnWord(Lo);
       Up=malloc_m(strlen_m(UpIn)+2); strcpy_m(Up,UpIn); LmUnWord(Up);
      #endif
      while(*Lo != 0)
      { Upper[(BYTE)(*Up)]=1; Lower[(BYTE)(*Lo)]=1;
        Upp[++k]=*Up;Low[k]=*Lo;
        ++Lo;++Up;
      }
      ++p;
      #ifdef SPELL_NEW
        free_m(Lo); free_m(Up);
      #endif
    }
  #else
    { BYTE *loR=(BYTE*)malloc_m(40),*upR=(BYTE*)malloc_m(40),*Lo,*Up;
      BYTE *loE=(BYTE*)malloc_m(40),*upE=(BYTE*)malloc_m(40),*bLo[2],*bUp[2];
      k=-1; bLo[0]=loR; bUp[0]=upR; bLo[1]=loE; bUp[1]=upE;
      strcpy_m((char*)loR,"��������������������������������-");
      strcpy_m((char*)upR,"��������������������������������-");
      strcpy_m((char*)loE,"abcdefghijklmnopqrstuvwxyz-\'");
      strcpy_m((char*)upE,"ABCDEFGHIJKLMNOPQRSTUVWXYZ-\'");
      for(i=0; i < 2; ++i)
      { Lo=bLo[i]; Up=bUp[i];
        while(*Lo != 0)
        { Upper[(BYTE)(*Up)]=1; Lower[(BYTE)(*Lo)]=1;
          Upp[++k]=*Up;Low[k]=*Lo;
          ++Lo;++Up;
        }
      }
      free_m(loR); free_m(upR); free_m(loE); free_m(upE);
    }
  #endif
  do0(i,0,k)  {UppLow[(int)Upp[i]]=Low[i]; UppLow[(int)Low[i]]=Upp[i];}
  do0(i,0,k1)  EndSentence[Punct1[i]]=1;
  do0(i,0,255) PunctuationMarks[i]=WordOne[i]=0;
  do0(i,0,NumPunct) PunctuationMarks[(int)Punct[i]]=1;
  NumOne=strlen_m((char*)WordOneSymb)-1;
  do0(i,0,NumOne) WordOne[(int)WordOneSymb[i]]=1;
  #if defined (WIN_MOD) && !defined (TIGER_CORR)
    #ifndef SPELL_NEW
     ParSpell=b_charg();
     par_ful.KodNoRecog=ParSpell->unknow;
    #else
     ParSpell=(svocabg*)malloc_m(sizeof(svocabg));
     GetPrivateProfileString("b_dll","UNKNOW","@",s,20,FileParSpel);
     par_ful.KodNoRecog=s[0];
     { char *p=&s[2],*pp=p;
       while(*pp++ != ')'); *(--pp)=0;
       ParSpell->reunknow=atoi(p);
     }
     ParSpell->reinst=GetPrivateProfileInt("b_dll","REINST",0,FileParSpel);
     ParSpell->reinstb= GetPrivateProfileInt("b_dll","REINSTB",0,FileParSpel);
     ParSpell->redel=   GetPrivateProfileInt("b_dll","REDEL",0,FileParSpel);
     ParSpell->reelse=  GetPrivateProfileInt("b_dll","REELSE",0,FileParSpel);
    #endif
  #else
    GetPrivateProfileString("b_dll","UNKNOW","@",(char*)s,20,FileParSpel);
    par_ful.KodNoRecog=s[0];
  #endif
  GetPrivateProfileString("ful_txt","KodNoRecogOut","@",(char*)s,2,FileParSpel);
  par_ful.KodNoRecogOut=s[0];
  par_ful.NumAlt=GetPrivateProfileInt("ful_txt","NumAlt",2,FileParSpel);
  w=(WORD)GetPrivateProfileInt("ful_txt","RelKrit",(int)10,FileParSpel);
  par_ful.RelKrit=(float)(w/10.);
  par_ful.IndexDelta=GetPrivateProfileInt("ful_txt","IndexDelta",1,FileParSpel);
  if(par_ful.IndexDelta!=2) MaxValue=MAX_VALUE;
  else MaxValue=MAX_VALUE*MAX_VALUE;
  par_ful.IntervalTimer=(DWORD)GetPrivateProfileInt("ful_txt","IntervalTimer",32000,FileParSpel);
  { //Temp filling KoderTabl R<->L coincidal tracing
    #ifdef WIN_MOD
      BYTE *Rus=(BYTE*)"��������������������",*Lat=(BYTE*)"AaBEeKkMHOoPpCcTYyXx";
    #else
      BYTE *Rus=(BYTE*)"��������������������",*Lat=(BYTE*)"AaBEeKkMHOoPpCcTYyXx";
    #endif
    do0(i,0,255) KoderRus[i]=KoderLat[i]=(BYTE)i;
    if(TypeDoc!=NORV)
     do0(i,0,19) { KoderLat[Rus[i]]=Lat[i]; KoderRus[Lat[i]]=Rus[i]; }
  }
  return 0;
}
*/ // !!! Art - ��������
#ifndef BLANK
 #define SPACE_RED 4
#else
 #define SPACE_RED 0
#endif
//nc - ����� ������������� �������
//Bnd - �� ��������� ����� (����� � ��������� �����������)
//Return:
// 0 - ���������� ����������,
//-1 - ������ ����������,
//===========�������������� �������� � ������� ����� �������
/*// !!! Art - ��������
int FormatStr(int nc,SRECT *Bnd,char ***Str,int **Ksym,int ncO)
//===========
{
	int DelStr=0,ks=NumStr[nc],ns,LeftMargin=0,kw,kz,nw,nz,ksy,i,k=0,del=0,fl,len,
      Fract,Znam=0,Chisl=0,inWord;
  uchar LastCod,sy=0;
  #ifdef PROF_COL
    //---������ �������� ��������. ��������� DelStr---
    if(!ks)
			DelStr=2*StatCol[nc].dy_col;
    else
    {
			DelStr=0; k=0; fl=0;
      do0(ns,1,ks) //���������� �/��
      {
				if(Zn[nc][ns][0][0].Title.Z_Num_Alt != 0 &&
           ((sy=Zn[nc][ns-1][0][0].Alt[0].a_Code) == '$' || sy == '#'))
					 			continue;
        del=(TitleStr[nc][ns].S_Rect.bottom+TitleStr[nc][ns].S_Rect.top)/2 -
            (TitleStr[nc][ns-1].S_Rect.bottom+TitleStr[nc][ns-1].S_Rect.top)/2;
        if(!del)
					{fl=1;break;}
        DelStr+=del; ++k;
      }
      if(!fl && !k)
				DelStr/=k;
			else
				DelStr=2*StatCol[nc].dy_col;
      if(!DelStr)
				DelStr=2*StatCol[nc].dy_col;
    }
    Bnd->top=Zn[nc][0][0][0].Title.Z_Rect.top/DelStr;
    LeftMargin=Zn[nc][0][0][0].Title.Z_Rect.left;
    do0(ns,0,ks)
			LeftMargin=MIN(LeftMargin,(Zn[nc][ns][0][0].Title.Z_Rect.left));
    Bnd->left=LeftMargin/(StatCol[nc].dx_col+StatCol[nc].dsym_col);
  #else
    Bnd->left=Bnd->top=0;
  #endif
  if(nc < 0 || ncO < 0)
		return -10;
  Bnd->bottom=Bnd->top+ks; Bnd->right=-32000;
  #ifdef SUB_FRM
   Str[ncO]=(char**)Submalloc((ks+1)*sizeof(char*),&SubFrm);
   Ksym[ncO]=(int*)Submalloc((ks+1)*sizeof(int),&SubFrm);
  #else
   Str[ncO]=(char**)malloc_m((ks+1)*sizeof(char*));
   Ksym[ncO]=(int*)malloc_m((ks+1)*sizeof(int));
  #endif
  if(Str[ncO]==NULL||Ksym[ncO]==NULL)
		return NOT_ALLOC;
  LastCod=0;
  do0(ns,0,ks)
  {
		Fract=0;
//		Chisl=0;
    ksy=-1;//CurrentPosition in Str
    if((kw=TitleStr[nc][ns].S_Gen.S_NumWord-1) >= 0)
    { if(BEG_PARAG1(Zn[nc][ns][0][0].Title.Z_Id)==1)//������� ������
        do0(i,0,SPACE_RED) buf[++ksy]=' ';
      do0(nw,0,kw)
      { //if(nw > 0 && TitleWord[nc][ns][nw].W_Gen.W_NumSym == 1 &&
        //   Zn[nc][ns][nw][0].Alt[0].a_Code == '"') --ksy;
        if(par_ful.AllowSpell!=1)
        {
					if(TitleFul.ProcSpell == PROC_SPELL &&
             TitleWord[nc][ns][nw].W_Gen.W_Spell == SPELL_PART)
						 		continue;
          kz=TitleWord[nc][ns][nw].W_Gen.W_NumSym-1;
          do0(nz,0,kz)
          { if(INDICE1(Zn[nc][ns][nw][nz].Title.Z_Id))
              buf[++ksy]='^'; //������
            if(MULTI_POINT1(Zn[nc][ns][nw][nz].Title.Z_Id))
              {buf[++ksy]=' ';buf[++ksy]='.';buf[++ksy]='.';goto END;}//����������
            if(FRACT1(Zn[nc][ns][nw][nz].Title.Z_Id))
            { if(!Fract) //��������� �����
                {Fract=1;buf[++ksy]='<';buf[++ksy]='<'; inWord=nw;}
              else if(nw != inWord) {inWord=nw;buf[++ksy]='/';} //�������� �����������
            }
            else if(Fract)
							{ Fract=0;buf[++ksy]='>'; buf[++ksy]='>'; }
            if(Zn[nc][ns][nw][nz].Title.Z_Num_Alt) //���������� �����������
              buf[++ksy]=Zn[nc][ns][nw][nz].Alt[0].a_Code;
            else
              #ifdef FUL
               buf[++ksy]=par_ful.KodNoRecogOut;
              #else
               buf[++ksy]='?';
              #endif
          }
        }
        else if(TitleWord[nc][ns][nw].W_Gen.W_Spell!=SPELL_PART)
        { fl=ChooseBestWord(Zn[nc][ns][nw],TitleWord[nc][ns][nw],(char*)buf1,
          par_ful.k1,par_ful.k2,par_ful.gam_Ins,par_ful.gam_Del);
          len=strlen_m((char*)buf1)-1;
          if((fl=TstNameOwr(LastCod,GetAlt(nc,ns,nw,0,0)))==1)
          { //����� � ������� ����� ������� �����-�
            if(Upper[(int)buf1[0]]==0)
              buf1[0]=UppLow[(int)buf1[0]]; //������ ������� �� ���������
          }
          else if(fl == 0) //����� � ����� �����
          { if(Lower[(int)buf1[0]]==0)
              buf1[0]=UppLow[(int)buf1[0]]; //������ ������� �� ���������
          }
          LastCod=buf1[len];
          do0(i,0,len) buf[++ksy]=buf1[i];
        }
        if(nw != kw)
        { if(!COMMA1(Zn[nc][ns][nw+1][0].Title.Z_Id) && !Fract)
            buf[++ksy]=' '; //InterWord Space,���� ������ �� ���� ����������
        }
      }
    END:
			if(Fract)
				{buf[++ksy]='>'; buf[++ksy]='>';}
    }
    else
			buf[ksy=0]=' ';//Empty Str
    Bnd->right=MAX(Bnd->right,ksy);
    if(ksy < 0)
			return -11;
    #ifdef SUB_FRM
     if((Str[ncO][ns]=(char*)Submalloc(ksy+1,&SubFrm))==NULL)return NOT_ALLOC;
    #else
     if((Str[ncO][ns]=(char*)malloc_m(ksy+1))==NULL)return NOT_ALLOC;
    #endif
    Ksym[ncO][ns]=ksy; memcpy(Str[ncO][ns],buf,ksy+1);
  }
  Bnd->right+=Bnd->left;
  return 0;
}
*/ // !!! Art - ��������
#ifdef PROF_COL
/* int PASC GenFullTxtfromTree(char *FileNameFul,char *FileNameOut,INF_TREE *Inf)
 {;}*/ // !!! Art - ��������
#else
//������� ���� Curr �� ������ ��� ���.�������� �� ������(��� ��������� � ListFree)
//back � next - ������ ���� (����� ��� ������� ��� ������� � ���� �����)
//==
/*// !!! Art - ��������
void ExtrKnot(KNOTT *Curr,KNOTT **back,KNOTT **next)
//==
{ if(Curr==NULL)ERR(1,"ExtrKnot");
  *back=Curr->back; *next=Curr->next;
  if(*back)(*back)->next=*next;if(*next)(*next)->back=*back;
}
*/// !!! Art - ��������
//��������� ������� ���� Curr ����� ������ back � next
//==
/* // !!! Art - ��������
void ReverseInsertKnot(KNOTT *Curr,KNOTT *back,KNOTT *next)
//==
{ if(back) back->next=Curr; Curr->back=back;
  if(next) next->back=Curr; Curr->next=next;
}
*/// !!! Art - ��������
//���������:
//FileNameFul - ��� ����� Full Out Cript
//FileNameOut - ��� ��������� �����
//Inf - ������ ��������� � ������
//Return:
// 0 - ���������� ����������
//
//==��������� ���������� ����� �� ����� Full Out
/* // !!! Art - ��������
int PASC GenFullTxtfromTree(char *FileNameFul,char *FileNameOut,INF_TREE *Inf)
//==
{
	SRECT *BndColTxt;
	char ***Str;
	int **Ksym;
  char **Txt,*err="GenFullTxtfromTree"; int K_Col,K_Rows;
  int fl,nc,ns,nz,dx,dy,tmp,nT,in,beg,end,i,y0,y1,x0,x1,DelLine,j;
  BYTE *MultiPoint;
  SRECT BndPage;
  STAT_CELL *StatCell=Inf->StatCell;
  LINE_KNOT *LineH=Inf->LineHK,*LineV=Inf->LineVK;
  KNOTT *Root=Inf->Tree.Root,**AllT,*Curr;
  int NumTV=Inf->NumTV,nH=Inf->nH,nV=Inf->nV,HeiStr=StatCell[0].HeiStr,
      Space=abs(Inf->MonoSpaceTrue);
  STACK St;
  BYTE ImageLineV[MAX_TYPE_LINE],ImageLineH[MAX_TYPE_LINE],charLine;
  ImageLineV[SOLID_LINE ]='|'; ImageLineH[SOLID_LINE ]='-';
  ImageLineV[DOUBLE_LINE]='#'; ImageLineH[DOUBLE_LINE]='=';
  ImageLineV[THICK_LINE ]='*'; ImageLineH[THICK_LINE ]='*';
  //~ImageLineV[HIDDEN_LINE]=' '; ImageLineH[HIDDEN_LINE]=' ';
  ImageLineV[HIDDEN_LINE]='|'; ImageLineH[HIDDEN_LINE]='-';
  ImageLineV[DOT_LINE   ]='.'; ImageLineH[DOT_LINE   ]='.';
  #ifndef TIGER_CORR
  	if(Inf->TypeDoc==NORV)
			par_ful.AllowSpell=0;
		else
			par_ful.AllowSpell=1;
	#endif
  //--�������� ������ down ���� HierH ��� ������������ ������ TERM-Cell Tree
  if(Inf->TypeDoc == NORV)
		Inf->HierH->down=NULL;
  //--Init Txt-Coor H- and V-Line
  for(i=0; i < nH; ++i)
  {
		if(i)
			tmp=(LineH[i].beg-LineH[i-1].beg)/HeiStr + 1;
    LineH[i].BegTxt= !i ? 2 : LineH[i-1].BegTxt + MAX(2,tmp);
  }
  for(i=0; i < nV; ++i)
  {
		if(i)
			tmp=Space ? (LineV[i].beg-LineV[i-1].beg)/Space + 1 : 0;
    LineV[i].BegTxt= !i ? 2 : LineV[i-1].BegTxt + MAX(2,tmp);
  }
  //--������������ ����� ����� TERM-Cell
  if(NewStack(20,&St))
		return NOT_ALLOC;
  Curr=Root; nT=0;
  while(Curr != NULL)
  {
		if(Curr->down == NULL && Curr->Type != HIER_H)
			++nT;
    Curr=NextKnot(Curr,&St);
  }
  ClearStack(&St);
  //����� ������������� ����. ����� KNOTT
  //InBegFrm - ����� ��������.������ � �������� Zn,...
  //NumFrm - ������ � ������� AllT
  AllT=(KNOTT**)malloc_m(nT*sizeof(PTR));
  Curr=Root; nT=-1;
  while(Curr != NULL)
  { if(Curr->down == NULL && Curr->Type != HIER_H)
    { AllT[++nT]=Curr;
			if(Curr->NumFrm < 1)
				Curr->AllowOCR=0;
      AllT[nT]->NumFrm=nT;
    }
    Curr=NextKnot(Curr,&St);
  }
  ++nT;
  DelStack(&St);
  NumCol=Inf->NumT; //!!!��� ������ ��������� ������!!!
  //for(i=0; i < nT; ++i)
  //{ if(AllT[i]->AllowOCR)
  //   {if(AllT[i]->InBegFrm > NumCol || AllT[i]->InBegFrm < 0) ERR(5,err);}
  //  if(AllT[i]->NumFrm > nT || AllT[i]->NumFrm < 0) ERR(6,err);
  //}
  NumStr=Inf->k_str; StatCol=Inf->StatCol;
  BndCol=(SRECT*)malloc_m((NumCol+1)*sizeof(SRECT));
  for(i=0;i<=NumCol;++i)
  { BndCol[i].left=Inf->bnd_col[i].left;BndCol[i].right=Inf->bnd_col[i].right;
    BndCol[i].top=Inf->bnd_col[i].up;   BndCol[i].bottom=Inf->bnd_col[i].down;
  }
  #ifdef FUL
   #ifndef DLL_MOD
    if((fl=InitSpell(NULL,1,0))!=0) return fl-50;
   #endif
  #endif

	#ifndef TIGER_CORR
		if((fl=OpenFullOut(FileNameFul))!=0)
			return fl-100;
	#else

	  //~	if((fl=OpenFullOutTiger(FileNameFul))!=0)
		//~	return fl-100;
	#endif
  //!!!��� ������� nT - ����� ����� TERM-�����, NumCol - ���� AllowOcr
  BndColTxt=(SRECT*)malloc_m(nT*sizeof(SRECT));
  #ifdef SUB_FRM
   { long Num=nT*2*sizeof(PTR)+(TitleFul.nStr+(nT-NumCol)*2)*
          (sizeof(PTR) + sizeof(int)) + (TitleFul.dNumZ+500);
     if((fl=InitSubAlloc(Num,&SubFrm))) return fl-100;
     Str=(char***)Submalloc(nT*sizeof(char**),&SubFrm);
     Ksym=(int**)Submalloc(nT*sizeof(int*),&SubFrm);
   }
  #else
    Str=(char***)malloc_m(nT*sizeof(char**));
    Ksym=(int**)malloc_m(nT*sizeof(int*));
  #endif
  if(BndColTxt==NULL||Str==NULL||Ksym==NULL)
		return NOT_ALLOC;
  #if defined (FUL) && !defined (TIGER_CORR)
  if(TitleFul.ProcSpell != PROC_SPELL &&
     ((Inf->TypeDoc==NORV  && (fl=CorrFulCell(AllT,nT,Inf)) != 0) ||
      (Inf->TypeDoc==PLAIN && (fl=CorrFul()) != 0))
    ) //��������� �� �������������
			return fl-3000;
  #endif
  //--�������������� �������� � ��������--
  for(i=0; i < nT; ++i)
    if(fl=FormatKnot(AllT[i],&BndColTxt[i],Str,Ksym)) return fl-200;
  //--�������������� ������������ � ����������--
  DelLine= Inf->TypeDoc==PLAIN ? 0 : 2;
  u4sort(AllT,nT,sizeof(PTR),(COMP_FUN)compKNOTT_RectBottom);
  for(i=0; i < nT; ++i)
  { //in - ������ ������ ������ � �������� BndColTxt,Str,Sym
    //beg, end - ������� ��������� ����� ������
    beg=AllT[i]->Rect.top;end=AllT[i]->Rect.bottom;
    if( (tmp=BndColTxt[AllT[i]->NumFrm].bottom + 2 - (LineH[end].BegTxt -
        LineH[beg].BegTxt)) > 0)
    { for(j=end; j < nH; ++j) //���������� ���������,������ ���� ������������� �����
      { LineH[j].BegTxt+=tmp;
        if(j < nH-1 && LineH[j].BegTxt+DelLine <= LineH[j+1].BegTxt)
					break;
      }
    }
  }
  for(i=1; i < nH; ++i)
		if(LineH[i].BegTxt - LineH[i-1].BegTxt < DelLine)
			ERR(8,err);
  u4sort(AllT,nT,sizeof(PTR),(COMP_FUN)compKNOTT_RectRight);
  for(i=0; i < nT; ++i)
  { beg=AllT[i]->Rect.left;end=AllT[i]->Rect.right;
    if( (tmp=BndColTxt[AllT[i]->NumFrm].right + 2 - (LineV[end].BegTxt -
        LineV[beg].BegTxt)) > 0)
    { for(j=end; j < nV; ++j) //���������� ���������
      { LineV[j].BegTxt+=tmp;
        if(j < nV-1 && LineV[j].BegTxt+DelLine <= LineV[j+1].BegTxt)
					break;
      }
    }
  }
  for(i=1; i < nV; ++i)
		if(LineV[i].BegTxt - LineV[i-1].BegTxt < DelLine)
			ERR(9,err);
  //--���� ������������� ��������� ��� �����--
  for(i=0; i < nT; ++i)
  { in=AllT[i]->NumFrm;
    beg=AllT[i]->Rect.top;end=AllT[i]->Rect.bottom;
    if((tmp=BndColTxt[in].bottom+2-(LineH[end].BegTxt - LineH[beg].BegTxt)) > 0)
			ERR(1,err);
    beg=AllT[i]->Rect.left;end=AllT[i]->Rect.right;
    if((tmp=BndColTxt[in].right+2-(LineV[end].BegTxt - LineV[beg].BegTxt)) > 0)
			ERR(11,err);
  }
  //--������������ ��������� ������ � ������--
  BndPage.left=LineV[0].BegTxt; BndPage.right=LineV[nV-1].BegTxt;
  BndPage.top =LineH[0].BegTxt; BndPage.bottom=LineH[nH-1].BegTxt;
  K_Col=BndPage.right-BndPage.left; K_Rows=BndPage.bottom-BndPage.top;
  if((Txt=(char**)malloc_m((K_Rows+1)*sizeof(char*)))==NULL)return NOT_ALLOC;
  do0(ns,0,K_Rows)
  {
		if((Txt[ns]=(char*)malloc_m((K_Col+1)))==NULL)
			return NOT_ALLOC;
    memset(Txt[ns],' ',K_Col+1);
  }
  if(Inf->TypeDoc==NORV)
		MultiPoint=(BYTE*)malloc_m(100);
  for(i=0; i < nT; ++i)
  { KNOTT *Kn=AllT[i];
    int l,nci,nw,NumAdd,dx1;
    int left=LineV[Kn->Rect.left].BegTxt,right=LineV[Kn->Rect.right].BegTxt,
        top=LineH[Kn->Rect.top].BegTxt,  bottom=LineH[Kn->Rect.bottom].BegTxt;
    nc=Kn->NumFrm; dx=left-BndPage.left+1; dy=top-BndPage.top+1;
    //--������������--
    if(Kn->JustH == ALG_END && (tmp=right-left-1-BndColTxt[nc].right-1) > 0)
      dx+=tmp;
    if(Kn->JustH == ALG_CENTER && (tmp=(right-left-1-BndColTxt[nc].right)/2-1) > 0)
      dx+=tmp;
    if(Kn->JustV == ALG_END && (tmp=bottom-top-1-BndColTxt[nc].bottom-1) > 0)
      dy+=tmp;
    if(Kn->JustV == ALG_CENTER && (tmp=(bottom-top-1-BndColTxt[nc].bottom)/2-1) > 0)
      dy+=tmp;
    in=BndColTxt[nc].bottom;
    if(Inf->TypeDoc==NORV)
			memset(MultiPoint,0,in+1);
    if(Kn->AllowOCR)
    { nci=Kn->InBegFrm;
      for(l=0; l <= in; ++l)
      { nw=TitleStr[nci][l].S_Gen.S_NumWord;
        if(nw > 0)
        { if(Inf->TypeDoc==NORV && MULTI_POINT1(Zn[nci][l][nw-1][0].Title.Z_Id))
          { if(MULTI_POINT1(Zn[nci][l][0][0].Title.Z_Id))
							MultiPoint[l]=1;//AllStr ...
            else
							MultiPoint[l]=2;//EndStr ...
          }
        }
      }
    }
    //--������������� V-�������--
    beg=left-BndPage.left; charLine=ImageLineV[LineV[Kn->Rect.left].TypeLine];
    y0=top-BndPage.top; y1=bottom-BndPage.top;
    do0(ns,y0,y1) Txt[ns][beg]=charLine;
    beg=right-BndPage.left; charLine=ImageLineV[LineV[Kn->Rect.right].TypeLine];
    do0(ns,y0,y1) Txt[ns][beg]=charLine;
    //--������������� H-�������--
    beg=top-BndPage.top; charLine=ImageLineH[LineH[Kn->Rect.top].TypeLine];
    x0=left-BndPage.left; x1=right-BndPage.left;
    do0(nz,x0,x1) Txt[beg][nz]=charLine;
    beg=bottom-BndPage.top; charLine=ImageLineH[LineH[Kn->Rect.bottom].TypeLine];
    do0(nz,x0,x1) Txt[beg][nz]=charLine;
    //--����� ������ ������--
    do0(ns,0,in)
    { //if(ns+dy == top-BndPage.top || ns+dy == bottom-BndPage.top ||
      //   nz+dx == left-BndPage.left|| nz+dx == right-BndPage.left)
      //      ERR(13,err); //������� ���������
      if(Inf->TypeDoc!=NORV || MultiPoint[ns] != 1)
      { do0(nz,0,Ksym[nc][ns]) Txt[ns+dy][nz+dx]=Str[nc][ns][nz];
        if(Inf->TypeDoc==NORV && MultiPoint[ns])
        { dx1=Ksym[nc][ns]+1+dx; NumAdd=right-left-2-Ksym[nc][ns];
          for(nz=0; nz < NumAdd; ++nz) Txt[ns+dy][nz+dx1]='.';
        }
      }
      else //������� ���������� ������ ����������
        for(nz=right-3-BndPage.left; nz <= right-1-BndPage.left; ++nz)
          Txt[ns+dy][nz]='.';
    }
  }
  if(Inf->TypeDoc==NORV)free_m(MultiPoint);
  //--����� �����. ������ � ����--
  { FILE1 *out=fopen_m(FileNameOut,OF_WRITE);
    char Lf[2];
    Lf[0]=0x0D; Lf[1]=0x0A;
    do0(ns,0,K_Rows)
    { fwrite_m(Txt[ns],K_Col+1,1,out); fwrite_m(Lf,2,1,out); }
    fclose_m(out);
  }
  //--��� ����������� ���������.�������������� ��������� ������� ������������
  //--���� ���� �� ��� ��������
  for(i=0; i <= Inf->NumT; ++i)
    if(Inf->ColT[i]->NumFrm == 0) Inf->ColT[i]->NumFrm=1;
  //--������������ ������--
  do0(ns,0,K_Rows) free_m(Txt[ns]); free_m(Txt);
  #ifdef SUB_FRM
   DeleteSubAlloc(&SubFrm);
  #else
   for(i=0; i < nT; ++i)
   { nc=AllT[i]->NumFrm; in=BndColTxt[nc].bottom;
     free_m(Ksym[nc]); do0(ns,0,in) free_m(Str[nc][ns]); free_m(Str[nc]);
   }
   free_m(Ksym); free_m(Str);
  #endif
  free_m(BndColTxt);
//END:
  FreeStructFull();
  free_m(AllT);
  #ifdef FUL
   #ifndef DLL_MOD
    b_close();
   #endif
  #endif
  return 0;
}*/// !!! Art - ��������

// !!! Art - ��������
//int FormatKnot(KNOTT *Knot,SRECT *Bnd,char ***Str,int **Ksym)
//{
//	int fl;
//
//	if(TRUE/*Knot->AllowOCR && Knot->InBegFrm <= NumCol*/)//���� ��������� OCR
//  {
//		if(fl=FormatStr(Knot->InBegFrm,Bnd,Str,Ksym,Knot->NumFrm))
//			return fl-10;
//	}
//  else //��������� ������.������, ���� ��� �� ����� NULL
//  { char **str,*s;
//		int *ks,kstr,len,ksy,Num;
//
//    Knot->AllowOCR=0;
//    if(Knot->Name)
//    { //--������������ ����� ����� ��� ��������. (kstr) � ������. (Num) �����
//      s=Knot->Name; kstr=1; Num=0;
//      while(*s) { if(*s == '\\' && *(++s) == 'n') ++kstr; ++s; ++Num; }
//      s=Knot->Name; if(*s == '\\' && *(s+1) == 'v') kstr=Num-1;//~
//      #ifdef SUB_FRM
//       str=(char**)Submalloc(kstr*sizeof(PTR),&SubFrm);
//       ks=(int*)Submalloc(kstr*sizeof(int),&SubFrm);
//      #else
//       str=(char**)malloc_m(kstr*sizeof(PTR));
//       ks=(int*)malloc_m(kstr*sizeof(int));
//      #endif
//      kstr=-1;  len=-32000; ksy=-1;
//      if(*s == '\\' && *(s+1) == 'v') //��������.�������
//      { s+=2;
//        while(*s)
//        { len=MAX(len,1);
//          #ifdef SUB_FRM
//           str[++kstr]=(char*)Submalloc(1,&SubFrm);
//          #else
//           str[++kstr]=(char*)malloc_m(1);
//          #endif
//          str[kstr][0]=*s;
//          ks[kstr]=0; ++s;
//        }
//      }
//      else
//      { while(*s)
//        { if(*s == '\\')
//          { if( *(++s) == 'n')
//            { len=MAX(len,ksy);
//              #ifdef SUB_FRM
//               str[++kstr]=(char*)Submalloc(ksy+1,&SubFrm);
//              #else
//               str[++kstr]=(char*)malloc_m(ksy+1);
//              #endif
//              memcpy(str[kstr],buf,ksy+1); ks[kstr]=ksy;
//              ksy=-1;
//            }
//          }
//          else buf[++ksy]=*s;
//          ++s;
//        }
//        if(ksy >= 0)
//        { len=MAX(len,ksy);
//          #ifdef SUB_FRM
//           str[++kstr]=(char*)Submalloc(ksy+1,&SubFrm);
//          #else
//           str[++kstr]=(char*)malloc_m(ksy+1);
//          #endif
//          memcpy(str[kstr],buf,ksy+1); ks[kstr]=ksy;
//        }
//      }
//    }
//    else //������ ������ - ������� ����������.���������.������
//    {
//      #ifdef SUB_FRM
//       str=(char**)Submalloc(sizeof(PTR),&SubFrm);
//       str[0]=(char*)Submalloc(1,&SubFrm);
//       ks=(int*)Submalloc(sizeof(int),&SubFrm);
//      #else
//       str=(char**)malloc_m(sizeof(PTR));
//       str[0]=(char*)malloc_m(1);
//       ks=(int*)malloc_m(sizeof(int));
//      #endif
//      str[0][0]=' '; kstr=0; ks[0]=0; len=0;
//    }
//    Bnd->left=0;Bnd->right=len;Bnd->top=0;Bnd->bottom=kstr;
//    Str[Knot->NumFrm]=str; Ksym[Knot->NumFrm]=ks; return 0;
//  }
//  return 0;
//}// !!! Art - ��������

// !!! Art - ��������
/*int compKNOTT_RectBottom(KNOTT **a,KNOTT **b)
  {return (*a)->Rect.bottom >= (*b)->Rect.bottom ? 1:-1;}
int compKNOTT_RectRight(KNOTT **a,KNOTT **b)
  {return (*a)->Rect.right >= (*b)->Rect.right ? 1:-1;}
*/// !!! Art - ��������
#endif

//InitSpell()
//{
//    if (LmOpen("d:\windows\system\",25)) ;
//    else
//      ERROR
//    if (SpellOpen("RE"))
//      SpellDeltaScript(Porog,Fr,Fd,Fi,Fi,Fo);
//    else
//      ERROR;
//    int LmSetFnt("windows.kod");//�����.Windows-����.���������
//     //����� ����� ��� �������� �������������� �����
//     int SpellFindW(char *Word, char *buf, int n);
//     //Word - ����� � ���� xxx\0
//     //buf  - ����� ����������.
//     //n    - ������������ ���������� ���������
//     //������ b_char ---->
//      unsigned SpellCharC(unsigned L, unsigned Letter);
//     //�������� �������������� ������� Letter ������������ ����� L.
//     //������� ����������: 0 - ������ �� ������ � ������� &0x1 - ����� �����
//     //    &0x2 - ������� �����
//    SpellClose();
//    LmClose();
//}

