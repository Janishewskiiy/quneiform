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

#include "fon.h" 
#include "ctb.h"
#include "sfont.h" 
#include "fonrec.h" 

#include <memory.h>
#include <string.h>
#include <stdlib.h>
/*#include <io.h>*/
#include <fcntl.h>
/*#include <sys\stat.h>*/

Bool32 BadCluster(InfoCluster *infoC);

Int32 TestClustNames(welet *wel,int numWel,
					 int name1,int name2 )
{
	int i,j;
	RecRaster recRast;
	int n2,d2;
	int dist1=0; // distance from name2 to name1
	int dist2=0; // distance from name1 to name2
	int dist;
	FonTestInfo testInfo[4];

	for(i=0;i<numWel;i++)
	{
       if( wel[i].let == name1 )
	   { d2=dist2;  n2=name2; }
       else if( wel[i].let == name2 )
	   { d2=dist1;  n2=name1; }
	   else 
		   continue;

       j=FONGetClusterAsBW( NULL,i,0,&recRast);
	   if(j<0) continue;
       
	   dist= FONTestChar(&recRast,(Word8)n2,testInfo,0);
       if(dist <= 0 ||  testInfo[0].prob <= d2 )
		     continue;

	   if(n2==name2) dist2=dist;
	   else          dist1=dist;
	}

	return min(dist1,dist2);
}
////////////////
Int32 GetNearestClusters (int num, welet *wel,int numWel ,
	    RecVersions *collection )
{
 RecRaster recRast;
 RECRESULT recres[REC_MAX_VERS];
 int xbit;
 int bytesx;
 int ret,j;
 int let=wel[num].let;

    memset(collection,0, sizeof(RecVersions));

    j=FONGetClusterAsBW( NULL,num,0,&recRast);
    if(j<0) return 0;

	xbit=recRast.lnPixWidth;
	bytesx=((xbit+63)/64)*8;

    ret=RecogClu(recRast.Raster,(SINT)bytesx,(SINT)xbit,(SINT)recRast.lnPixHeight,
	  recres,(SINT)REC_MAX_VERS, wel,numWel,-1, 0,-1024,-1024, 1 );

    if(ret <= 0) return 0;

    for(xbit=j=0;xbit<ret;xbit++)
	{
     if( let == recres[xbit].name ) continue; 

     collection->Alt[j].Code=recres[xbit].name;
     // for every point - add straf
     collection->Alt[j].Prob=recres[xbit].prob;
	 collection->Alt[j].Info=recres[xbit].nClust+1;
	 j++;
	}

  collection->lnAltCnt=j;
  return j;
}
//////////////////////
//
//
extern int Num11[256];
//
static int DistMoveRasters(BYTE *buf,int fullByte,int w,int h,
						   BYTE *bufraz,int xbyte2,int xbit2,int yrow2,
					       int xmove,int ymove,int porog)
{
int   i,j;
BYTE  cbyte,ccbyte;
int   rbyte;
int   dist;
int   ylast=min(h,yrow2-ymove-1);

 if(xmove <-1 || xmove > 1) 
	 return porog+1;

 rbyte=(w+7)>>3;  // real bytes in row

 bufraz+=(ymove+1)*xbyte2;

 if( xmove == -1 )
 {
  for(i=0,dist=0; i < ylast;i++,buf+=fullByte,bufraz+=xbyte2)
  {
   for(j=0;j<rbyte;j++)
   {
    cbyte=buf[j];
    if(cbyte == 0) 
	   continue;

    if( j < xbyte2 ) 
     dist += Num11[ cbyte & (~bufraz[j])];
    else // points on buf1 only 
     dist += Num11[cbyte];

    if( dist > porog ) return dist;
   }  // end j
  }   // end i 
 }    // end if 
 else if( xmove == 0 )
 {
  for(i=0,dist=0; i < ylast;i++,buf+=fullByte,bufraz+=xbyte2)
  {
   for(j=0;j<rbyte;j++)
   {
    cbyte=buf[j];
    if(cbyte == 0) 
	   continue;

    if( j < xbyte2 ) 
	{
		ccbyte= (bufraz[j]<<1);
		if( j < xbyte2-1 )  
			ccbyte |= (bufraz[j+1]&128)>>7;
	}
	else
		ccbyte=0;

    dist += Num11[ cbyte & (~ccbyte) ];

    if( dist > porog ) return dist;
   }  // end j
  }   // end i 
 }    // end if 
 else // if( xmove == 1 )
 {
  for(i=0,dist=0; i < ylast;i++,buf+=fullByte,bufraz+=xbyte2)
  {
   for(j=0;j<rbyte;j++)
   {
    cbyte=buf[j];
    if(cbyte == 0) 
	   continue;

    if( j < xbyte2 ) 
	{
		ccbyte= (bufraz[j]<<2);
		if( j < xbyte2-1 )  
			ccbyte |= (bufraz[j+1]&192)>>6;
	}
	else
		ccbyte=0;

    dist += Num11[ cbyte & (~ccbyte) ];

    if( dist > porog ) return dist;
   }  // end j
  }   // end i 
 }    // end if 


 // last rows - on buf only
 for(i=ylast;i<h;i++,buf+=fullByte)
 {
	 for(j=0;j<rbyte;j++)
		 dist+=Num11[buf[j]];

	 if( dist > porog ) 
		 return dist;
 }

  return (dist);
}
/////////////////
//
// �������� ������� � ������������� � �������
//
extern welet *welBuf;
int TestMoveRaster(int start,Nraster_header *rh,int NumAll,
				   int NumClus, SINT *nClus,
				   SINT  *LasIn, SINT *NumIn,
				   int porog)
{
 BYTE *tmpbuf=(BYTE *)welBuf;
 BYTE *tmp2;
 int  maxSize=sizeof(welet);
 int i,j,k;
 int xbyte=(rh[start].w+9)>>3; // for razmaz2
 int rbyte=(rh[start].w+7)>>3; // real bytes 
 int numAdded=0;
 int xmove,ymove;

 if( (i=xbyte*(rh[start].h+2)) >= maxSize)
	 return 0;

 maxSize-=i;      // stay free

 Razmaz2xByte(rh[start].pHau,tmpbuf,rh[start].xbyte,rh[start].w,rh[start].h,
			  0,POROG_ANGLES);
 tmp2=tmpbuf+i;
 
 for(i=0;i<NumAll;i++)
 {
	 // itself ?
   if(i==start) continue;

   if(rh[i].let != rh[start].let) 
	   continue;

   // not single ?
   if( NumIn[nClus[i]] > 1 )
	   continue;

   // other sizes ?
   if( abs(rh[i].w-rh[start].w) > 2 || 
	   abs(rh[i].h-rh[start].h) > 2  
	   )
	   continue;

   j=(rh[i].w+9)>>3;      
   if( (j*(rh[start].h+2)) > maxSize)
	   continue;   // no free space

   Razmaz2xByte(rh[i].pHau,tmp2,rh[i].xbyte,rh[i].w,rh[i].h,
   			  0,POROG_ANGLES);

   for(xmove=-1;xmove<=1;xmove++)
   {
    for(ymove=-1;ymove<=1;ymove++)
	{
		// tested ... 
     if(xmove==0 && ymove==0) continue;

     if( DistMoveRasters(rh[start].pHau,rh[start].xbyte,rh[start].w,rh[start].h,
					   tmp2,j,rh[i].w+2,rh[i].h+2,
					   xmove, ymove, porog) <= porog &&
         DistMoveRasters(rh[i].pHau,rh[i].xbyte,rh[i].w,rh[i].h,
					   tmpbuf,xbyte,rh[start].w+2,rh[start].h+2,
					   -xmove, -ymove, porog) <= porog )
         break;
	}
	if(ymove <= 1) break;
   }

   if( xmove <= 1 ) // union cluster
   {
    j=nClus[i];  // removed cluster
    nClus[i]=nClus[start];
    NumIn[nClus[start]]++;
    for(k=0;k<NumAll;k++)
	  if(nClus[k]>j) nClus[k]--;
    NumClus--;
    memcpy(LasIn+j,LasIn+j+1,(NumClus-j)*sizeof(SINT));
    memcpy(NumIn+j,NumIn+j+1,(NumClus-j)*sizeof(SINT));
	if(LasIn[nClus[start]] < i)
		LasIn[nClus[start]] = i;
	numAdded++;
   }
 }

 return numAdded;
}
///////////////
int TestFromGoodRaster(int start,Nraster_header *rh,int NumAll,
				   int NumClus, SINT *nClus,
				   InfoCluster *infoC,
				   int porog)
{
 BYTE *tmpbuf=(BYTE *)welBuf;
 BYTE *tmp2;
 int  maxSize=sizeof(welet);
 int i,j;
 int xbyte=(rh[start].w+9)>>3; // for razmaz2
 int rbyte=(rh[start].w+7)>>3; // real bytes 
 int xmove,ymove;

 if( (i=xbyte*(rh[start].h+2)) >= maxSize)
	 return 0;

 maxSize-=i;      // stay free

 Razmaz2xByte(rh[start].pHau,tmpbuf,rh[start].xbyte,rh[start].w,rh[start].h,
			  0,POROG_ANGLES);
 tmp2=tmpbuf+i;
 
 for(i=0;i<NumAll;i++)
 {
	 // itself ?
   if(i==start) continue;

   if(rh[i].let != rh[start].let) 
	   continue;

   // not good ?
   if(nClus[i]<=0 || nClus[i]>=NumClus) 
	   continue;
   if( infoC[nClus[i]-1].good == 0 )
	   continue;

   // other sizes ?
   if( abs(rh[i].w-rh[start].w) > 3 || 
	   abs(rh[i].h-rh[start].h) > 3  
	   )
	   continue;

   j=(rh[i].w+9)>>3;      
   if( (j*(rh[start].h+2)) > maxSize)
	   continue;   // no free space

   Razmaz2xByte(rh[i].pHau,tmp2,rh[i].xbyte,rh[i].w,rh[i].h,
   			  0,POROG_ANGLES);

   for(xmove=-1;xmove<=1;xmove++)
   {
    for(ymove=-1;ymove<=1;ymove++)
	{
		// tested ... 
//     if(xmove==0 && ymove==0) continue;

     if( DistMoveRasters(rh[start].pHau,rh[start].xbyte,rh[start].w,rh[start].h,
					   tmp2,j,rh[i].w+2,rh[i].h+2,
					   xmove, ymove, porog) <= porog &&
         DistMoveRasters(rh[i].pHau,rh[i].xbyte,rh[i].w,rh[i].h,
					   tmpbuf,xbyte,rh[start].w+2,rh[start].h+2,
					   -xmove, -ymove, porog) <= porog )
         break;
	}
	if(ymove <= 1) break;
   }

   if( xmove <= 1 ) // find more-less same cluster
	   return nClus[i];
 }

 return 0;
}
///////////////
static const char twinHave[]="!136l\x87\xa1";
static const char twinLet[] ="1l\x87\xa1\x31\x33\x36";
static const char twinHave2[]="!1l\xAF";
static const char twinLet2[] ="l!!\xAB";
////////////////
static int TestSymbolGood(  Nraster_header *rh,
					 int start,int NumAll,
					 int porog,  SINT *nClus,
					 BYTE *metkaGood,BYTE *metkaValid,int nCompare
					 )
{
 BYTE *tmpbuf=(BYTE *)welBuf;
 BYTE *tmp2;
 int  maxSize=sizeof(welet);
 int i,j;
 int xbyte=(rh[start].w+9)>>3; // for razmaz2
 int rbyte=(rh[start].w+7)>>3; // real bytes 
 int xmove,ymove;
 BYTE twin1=0,twin2=0;
 char *qq;

  qq=strchr(twinHave,rh[start].let);
  if( qq ) twin1=twinLet[qq-twinHave];
  qq=strchr(twinHave2,rh[start].let);
  if( qq ) twin2=twinLet2[qq-twinHave2];

  if( (i=xbyte*(rh[start].h+2)) >= maxSize)
	 return 0;

  maxSize-=i;      // stay free

  Razmaz2xByte(rh[start].pHau,tmpbuf,rh[start].xbyte,rh[start].w,rh[start].h,
			  0,POROG_ANGLES);
  tmp2=tmpbuf+i;
 
  for(i=0;i<NumAll;i++)
  {
	 // itself ?
   if(i==start) continue;

   // tested ...
   if(rh[i].let == rh[start].let) 
	   continue;
   // not twin ?
   if( rh[i].let == twin1 || rh[i].let == twin2)
	   continue;

   // not good ?
   if(nClus[i]<=0 ) 
	   continue;
   if( metkaGood && (metkaGood[nClus[i]-1] == 0) )
	   continue;
   if( metkaValid && ((metkaValid[nClus[i]-1] & LEO_VALID_LINGVO)==0) )
	   continue;
   if( nCompare && (nClus[i]!=nCompare) )
	   continue;

   // other sizes ?
   if( abs(rh[i].w-rh[start].w) > 3 || 
	   abs(rh[i].h-rh[start].h) > 3  
	   )
	   continue;

   j=(rh[i].w+9)>>3;      
   if( (j*(rh[start].h+2)) > maxSize)
	   continue;   // no free space

   Razmaz2xByte(rh[i].pHau,tmp2,rh[i].xbyte,rh[i].w,rh[i].h,
   			  0,POROG_ANGLES);

   for(xmove=-1;xmove<=1;xmove++)
   {
    for(ymove=-1;ymove<=1;ymove++)
	{
		// tested ... 
//     if(xmove==0 && ymove==0) continue;

     if( DistMoveRasters(rh[start].pHau,rh[start].xbyte,rh[start].w,rh[start].h,
					   tmp2,j,rh[i].w+2,rh[i].h+2,
					   xmove, ymove, porog) <= porog &&
         DistMoveRasters(rh[i].pHau,rh[i].xbyte,rh[i].w,rh[i].h,
					   tmpbuf,xbyte,rh[start].w+2,rh[start].h+2,
					   -xmove, -ymove, porog) <= porog )
         break;
	}
	if(ymove <= 1) break;
   }

   if( xmove <= 1 ) // find more-less same cluster
	//   return nClus[i];
		return i+1;
  }

 return 0;
}
///////////////
int TestClusterGood(  Nraster_header *rh,
					 int testClus,int start,int inCluster,
					 int NumAll, int porog,  SINT *nClus,
					 BYTE *metkaGood,BYTE *metkaValid,
					 int nCompare
					 )
{
	int i,j;
	int count;

	for(i=start,count=0;i<NumAll;i++)
	{
		if( nClus[i] != testClus )
			continue;
		j = TestSymbolGood( rh,i, NumAll, porog,  nClus,
					  metkaGood, metkaValid,nCompare);
		if( j > 0)  // ���� ����� �������
		{
			// set start invalid 
			rh[start].num=j;
			return j;
		}

		count++;
		if(count >= inCluster)
			break;
	}

	return 0;
}
///////////
static int cou1[MAXFIELD];
static int cou2[MAXFIELD];
static void GetStatField(FONTFIELD *ff,InfoCluster *infoC,int *cou)
{
 Word32 fil;
 int i,j,k,best;

 memset(cou,0,sizeof(cou1));
 for(j=0;j<256;j++)
 {
   if((best=ff->inFont[j]) <= 0)  continue;
   for(k=0;k<NFIELDDWORD;k++)
   {
    for(i=0,fil=1;i<32;i++,fil<<=1)
	   if(infoC[best-1].fields[k] & fil)
		   cou[k*32+i]++;
   }
 }

}
/////////////
static int SummaCifr(Word32 *ww1,Word32 *ww2)
{
	Word32 odin,ww;
	int summa=0,k;

    for(k=0;k<NFIELDDWORD;k++)
	{
	 for(odin=1,ww=ww1[k]&ww2[k];odin;odin<<=1)
		if(ww&odin) summa++;
	}

    return summa;
}
///////////////
static void AddCountFields(Word32 *fifi,int *cou)
{
 int i,k;
 Word32 fil;

 for(k=0;k<NFIELDDWORD;k++)
 {
  for(i=0,fil=1;i<32;i++,fil<<=1)
	   if( fifi[k] & fil)
		   cou[k*32+i]++;
 }
}
///////////////
static void fillExclusiv(FONTFIELD *fil1,FONTFIELD *fil2,
						 InfoCluster *infoC,
						 Word32 *ff1,Word32 *ff2,
						 int *cou1,int *cou2)
{
Word32 f1[NFIELDDWORD],f2[NFIELDDWORD];
int    j;
Word32 onlyf1[NFIELDDWORD], onlyf2[NFIELDDWORD];

  SetFields(onlyf1,ff1);
  SetFields(onlyf2,ff2);

// ������� �������� ������ ����
  for(j=0;j<256;j++)
  {
       if( fil1->inFont[j] <= 0 || 
		   fil2->inFont[j] <= 0    )  
		   continue;
       SetFields(f1, infoC[fil1->inFont[j]-1].fields );
	   SetFields(f2, infoC[fil2->inFont[j]-1].fields );

	   // ���� ���� 
	   if( SummaCifr(f1,onlyf1)+SummaCifr(f2,onlyf2) >=
		   SummaCifr(f2,onlyf1)+SummaCifr(f1,onlyf2) )
	   {
		   AddCountFields(f1,cou1);
           AddCountFields(f2,cou2);
		   AddFields(onlyf1,f1);
		   AddFields(onlyf2,f2);
	   }
	   else // �������� 
		{
          Word16 tmpInFont=fil1->inFont[j];
		   fil1->inFont[j]=fil2->inFont[j];
		   fil2->inFont[j]=tmpInFont;
		   AddCountFields(f2,cou1);
           AddCountFields(f1,cou2);
		   AddFields(onlyf1,f2);
		   AddFields(onlyf2,f1);
	   }
  }

// ������ ��������
  for(j=0;j<256;j++)
  {
	   if( fil1->inFont[j] == 0 &&  fil2->inFont[j] == 0  )  
		   continue;
       if( fil1->inFont[j] > 0 &&  fil2->inFont[j] > 0  )  
		   continue;
        
	   if( fil1->inFont[j] )
	   {
        SetFields(f1,infoC[fil1->inFont[j]-1].fields) ;
	   //  ���� 
	    if( SummaCifr(f1,onlyf1) >=  SummaCifr(f1,onlyf2) )
		{
		   AddCountFields(f1,cou1);
		   AddFields(onlyf1,f1);
		}
	    else // �������� 
		{
		   fil2->inFont[j]=fil1->inFont[j];
		   fil2->inFont[j]=0;
           AddCountFields(f1,cou2);
		   AddFields(onlyf2,f1);
		}
		continue;
	   }

        SetFields(f2,infoC[fil2->inFont[j]-1].fields) ;
	   //  ���� 
	    if( SummaCifr(f2,onlyf2) >=  SummaCifr(f2,onlyf1) )
		{
		   AddCountFields(f2,cou2);
		   AddFields(onlyf2,f2);
		}
	    else // �������� 
		{
		   fil1->inFont[j]=fil2->inFont[j];
		   fil2->inFont[j]=0;
           AddCountFields(f2,cou1);
		   AddFields(onlyf1,f2);
		}
  }

  SetFields(ff1,onlyf1);
  SetFields(ff2,onlyf2);
}
/////////////////

static void TestCommonFields(FONTFIELD *f1,FONTFIELD *f2,
					 InfoCluster *infoC)
{
  int i;
  Word32 ff1[NFIELDDWORD],ff2[NFIELDDWORD];
  Word32 tField[NFIELDDWORD];
  Word32 odin=1;
  Word32 onlyf1[NFIELDDWORD]={0,0},onlyf2[NFIELDDWORD]={0,0};

  GetStatField(f1,infoC,cou1);
  GetStatField(f2,infoC,cou2);

  SetFields(ff1,f1->testField);
  SetFields(ff2,f2->testField);

  for(i=0;i<MAXFIELD;i++)
  {
	  MakeDWORDField(i,tField);
	  if( cou1[i] == 0 )
	  {
		  if( cou2[i] != 0)   
			  AddFields(onlyf2,tField);
	  }
	  else if( cou2[i] == 0)
		  AddFields( onlyf1, tField);
  }

  AddFields( ff1,onlyf1);
  AddFields( ff2,onlyf2);

  memset(cou1,0,sizeof(cou1));
  memset(cou2,0,sizeof(cou2));
  fillExclusiv(f1,f2,infoC, ff1,ff2,cou1,cou2);

  ClearFields(ff1);
  ClearFields(ff2);
  for(i=0;i<MAXFIELD;i++)
  {
	  if( cou1[i] == 0 && cou2[i]==0)
		  continue;
	  MakeDWORDField(i,tField);

	  if( cou1[i] >= cou2[i] )
		  AddFields(ff1,tField)
	  else if ( cou2[i] > cou1[i] )
  	      AddFields(ff2,tField)
  }


  SetFields(f1->field,ff1);
  SetFields(f2->field,ff2);
}
//////////////////
int TestIntersectFields(int count,FONTFIELD *f1,InfoCluster *infoC)
{
	int i,j;

	for(i=0;i<count-1;i++)
	{
      for(j=i+1;j<count;j++)  
	  {
		  // ������ ������ ?
/*       if(f1[i].testField==0 && f1[j].testField==0) 
          continue;
       if(f1[i].sBig > 0 && f1[j].sBig > 0 &&
		  abs( f1[i].sBig - f1[j].sBig ) > 2 )
          continue;
       if(f1[i].sLit > 0 && f1[j].sLit > 0 &&
		  abs( f1[i].sLit - f1[j].sLit ) > 2 )
          continue;
*/
       TestCommonFields(f1+i,f1+j,infoC);
	  }
	}

	return count;
}
////////////
// ��������� - ����� �������� �������� � �������
// test - multi-fonts in strings 
#include "clu_lang.h"

#define POROG_STAY  10 
#define POROG_COUNT 20
#define POROG_ADD   240

static int allStay[256];
int TestStayGood(int numCluster, int numSymbol,InfoCluster *infoC,
				 FONTFIELD *fontField)
{
	int i,j,let;
	int numStay;
	int sBig,sLit,numBig,numLit;
	int countStay;
	Bool32 BadCluster(InfoCluster *infoC);
	Word32 ff[NFIELDDWORD]={0,0};
	int porogSize;

	memset(allStay,0,256*sizeof(int));
	if( fontField ) memset(fontField,0,sizeof(FONTFIELD));

	for(i=0;i<numCluster;i++)
	{
		// ��� ����� ?
		if( infoC[i].good )
			continue;
		// bad name
		if( (let=infoC[i].let) <= 0 || let > 255 )
			continue;
		
        if( allStay[let] <= 0 ||
			infoC[allStay[let]-1].count < infoC[i].count ||
			infoC[allStay[let]-1].count == infoC[i].count &&
			infoC[allStay[let]-1].prob < infoC[i].prob 
			)
			allStay[let]=i+1;
	}

	countStay=0;
	for(i=1,numStay=sLit=sBig=numLit=numBig=0;i<256;i++)
	{
		if( allStay[i]<=0 ) continue;

		// ������������ ?
		if( BadCluster(infoC+allStay[i]-1) )
		 	continue;

		numStay++; 
		countStay+=infoC[allStay[i]-1].count;

		let=infoC[allStay[i]-1].let;

		if( NotStandardSymbol(let))
			continue;
		if( SmallSymbol(let))
		{
           numLit++;
		   sLit+=infoC[allStay[i]-1].mh;
		}
		else if( BigSymbol(let))
		{
           numBig++;
		   sBig+=infoC[allStay[i]-1].mh;
		}
	}

	if( numStay < POROG_STAY )
		return 0;
	if( countStay*100 < POROG_COUNT*numSymbol )
		return 0;

	if( numLit <= 0 && numBig <= 0)
		return 0;

    if( numBig > 0 ) sBig=(sBig+(numBig>>1))/numBig;
	if( numLit > 0 ) sLit=(sLit+(numLit>>1))/numLit;

    if( numBig <= 0)  sBig=(sLit<<2)/3;
	if( numLit <= 0)  sLit=(sBig*3)>>2;

	if( sBig <= sLit +1  ) return 0;
	porogSize=(sBig-sLit)>>1;


    for(i=1,numStay=countStay=0;i<256;i++)
	{
		if( allStay[i]<=0 ) continue;
        
		j=allStay[i]-1;
        let=infoC[j].let;

		if( infoC[j].invalid  || 
			(infoC[j].valid & LEO_VALID_LINGVO) == 0 &&
			infoC[j].prob < POROG_ADD 
		  )
		  continue;

		if( NotStandardSymbol(let))
		{
            if( infoC[j].mh < sBig - porogSize )    
			 continue;
		}
		else if( BigSymbol(let))
		{
			if( infoC[j].mh < sBig - porogSize || 
				infoC[j].mh > sBig + porogSize )    
			 continue;
		}
		else if( SmallSymbol(let))
		{
			if( infoC[j].mh < sLit - porogSize || 
				infoC[j].mh > sLit + porogSize )    
			 continue;
		}
		else if( BigSmallSymbol(let))
		{
			if( infoC[j].mh < sLit - porogSize || 
				infoC[j].mh > sBig + porogSize )    
			 continue;
		}
		else if( LetDigSymbol(let) )
		{
			if( infoC[j].mh < sLit - porogSize )
			 continue;
		}

		infoC[j].good=1;

		AddFields(ff, infoC[j].fields);
		numStay++; 
		countStay+=infoC[j].count;
		if( fontField)
			fontField->inFont[i]=j+1;
	}


	if( numStay < POROG_STAY ||
	    countStay*100 < POROG_COUNT*numSymbol )
	{ // restore old
		for(i=0;i<256;i++)
		{
			if((j=allStay[i]) >0)
				infoC[j-1].good=0;
		}
		return 0;
	}

    if(fontField)
	{
		fontField->sBig=sBig;
		fontField->sLit=sLit;
        SetFields(fontField->field,ff);  
		ClearFields(fontField->testField);
	}

	return 1;
}
////////////////
int TestAddFontGood(int numCluster, InfoCluster *infoC,
				 int sBig,int sLit,int *maxC,int fromAll,Word32 *fifi)
{
	int i,let;
	int numStay;
	int porogSize;

	if( sBig <= sLit +1  ) return 0;
	porogSize=(sBig-sLit)>>1;

	memset(allStay,0,256*sizeof(int));
	for(i=0;i<numCluster;i++)
	{
		// ��� ����� ?
		if( infoC[i].good )
			continue;
		// bad name
		if( (let=infoC[i].let) <= 0 || let > 255 )
			continue;

		if( BadCluster(infoC+i))
			continue;

		if( !IntersectFields(infoC[i].fields,fifi) && infoC[i].prob < 235 )
			continue;

		if( NotStandardSymbol(let))
		{
            if( infoC[i].mh < sBig - porogSize )    
			 continue;
		}
		else if( BigSymbol(let))
		{
			if( infoC[i].mh < sBig - porogSize || 
				infoC[i].mh > sBig + porogSize )    
			 continue;
		}
		else if( SmallSymbol(let))
		{
			if( infoC[i].mh < sLit - porogSize || 
				infoC[i].mh > sLit + porogSize )    
			 continue;
		}
		else if( BigSmallSymbol(let))
		{
			if( infoC[i].mh < sLit - porogSize || 
				infoC[i].mh > sBig + porogSize )    
			 continue;
		}
		else if( LetDigSymbol(let) )
		{
			if( infoC[i].mh < sLit - porogSize )
			 continue;
		}


		if( allStay[let] <= 0 ||
			infoC[allStay[let]-1].count < infoC[i].count ||
			infoC[allStay[let]-1].count == infoC[i].count &&
			infoC[allStay[let]-1].prob < infoC[i].prob 
			)
			allStay[let]=i+1;
	}


    for(i=1,numStay=0;i<256;i++)
	{
		if( allStay[i] <= 0 ) continue;

		if( fromAll == 0 && maxC[i] >= 0 )  	
			continue;

		maxC[i]=allStay[i]-1;
		numStay++;
	}
        
	return numStay;
}
////////////////
static Word8 isInColumn[256];
int AnalyzeTablColumn(welet *wel,int numWelet,int column)
{
	int i;
	int numDig,numOther;

    if(column <=0 || column > 32) 
		return 0;
    column=1<<(column-1);

	memset(isInColumn,0,256);
	for(i=0;i<numWelet;wel++,i++)
	{
      if( wel->tablColumn & column )
       isInColumn[wel->let]=1;
	}

	for(i=1,numDig=numOther=0;i<256;i++)
	{
		if(isInColumn[i]==0)
			continue;
		if( i >= '0' && i <= '9')
			numDig++;
		else
			numOther++;
	}

    if( numOther == 0) 
	{
		if(numDig==0) 
			return 0;
		return FONDigitColumn;     // digit column
	}

	if( numDig > numOther*3)
		return FONDigitFuzzyColumn;   // fuzzy digit

	return FONSmesColumn;   
}
////////////
int TectTablColumn(InfoCluster *infoCluster,int numCluster,int i,
				   Nraster_header *rh,int numAll,
				   SINT *nClus)
{
	int j,k;

        if( infoCluster[i].good ) 
			return infoCluster[i].good;
			
		if( BadCluster(infoCluster+i) )
			return 0;

		if( infoCluster[i].count < 20 )
			return 0;

        if( infoCluster[i].prob < 220 )
			return 0;

		if( !infoCluster[i].tablColumn )
			return 0;
		
        for(j=0;j< numCluster;j++)
		{
			if(!infoCluster[j].good)
				continue;
			if( infoCluster[j].let != infoCluster[i].let)
				continue;
			if( infoCluster[j].tablColumn & infoCluster[i].tablColumn)
				return 0;
		}

		// test invalid 
		for(j=0;j< numCluster;j++)
		{
			if(!infoCluster[j].good)
				continue;
			if( infoCluster[j].let == infoCluster[i].let)
				continue;
			if( !(infoCluster[j].tablColumn & infoCluster[i].tablColumn) )
   				continue;

            k = TestClusterGood(  rh,
					 i+1,infoCluster[i].first,infoCluster[i].count,
					 numAll, 0,  nClus,
					 NULL,NULL,j+1 ) ;

			if( k <= 0 )
				continue;

			// find the same

			if( infoCluster[i].prob > infoCluster[j].prob &&
				infoCluster[i].count > infoCluster[j].count 
				)  // our is better !
            {
			 // set start j invalid 
			  rh[infoCluster[j].first].num = infoCluster[i].first+1;
			  infoCluster[j].invalid = TRUE;
			  infoCluster[j].good = 0;

			  // our set nice
			  rh[infoCluster[i].first].num = 0;
			  return 1;
			}
			else if( infoCluster[j].prob > infoCluster[i].prob &&
				     infoCluster[j].count > infoCluster[i].count 
				)
			{
				infoCluster[i].invalid = TRUE;
				return 0;
			}

			// same alternatives
			rh[infoCluster[i].first].num = 0;
			return 0;
		}
		
		return 1;
}
