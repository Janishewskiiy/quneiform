/*
 Copyright (c) 1993-2008, Cognitive Technologies
 All rights reserved.

 Разрешается повторное распространение и использование как в виде исходного кода,
 так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

 * При повторном распространении исходного кода должны оставаться указанное
 выше уведомление об авторском праве, этот список условий и последующий
 отказ от гарантий.
 * При повторном распространении двоичного кода в документации и/или в
 других материалах, поставляемых при распространении, должны сохраняться
 указанная выше информация об авторском праве, этот список условий и
 последующий отказ от гарантий.
 * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
 быть использованы в качестве средства поддержки и/или продвижения
 продуктов, основанных на этом ПО, без предварительного письменного
 разрешения.

 ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
 ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
 ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
 ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
 МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
 НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
 ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
 НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
 СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
 ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
 НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
 ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

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

/****************************************************************************
 *                                                                          *
 *                                                                          *
 *                   P A G E   L A Y O U T                                  *
 *                                                                          *
 *              S T R I N G S   E X T R A C T I O N                         *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *              Update  in 1994 by Pit  Khlebutin                           *
 *                                                                          *
 *              LTERRORS.C - common purposes data and routines              *
 *                                                                          *
 ***************************************************************************/
#define __LTERRORS__

# include <setjmp.h>
# include <stdio.h>
# include <stdlib.h>

# include "resource.h"
# include "newfunc.h"

# include "lterrors.h"
# include "layout.h"
# include "extract.h"

# include "my_mem.h"
# include "dpuma.h"

#include "compat_defs.h"

extern jmp_buf fatal_error_exit;

void FreeAllData(void) {
	SE_FreeAllData();
	LT_FreeAllData();

# ifdef MA_DEBUG
	AllocationsAccountingClose ();
# endif
}

#ifdef LT_STAND_ALONE
void ErrorNoEnoughMemory (const char * message)
{
	LDPUMA_Console ("No enough memory - press any key\nMESSAGE:%s",message);
	FreeAllData ();
	SetReturnCode_rselstr(IDS_ERR_NO_MEMORY);
	longjmp (fatal_error_exit, -1);
}

void ErrorInternal (const char * s)
{
	LDPUMA_Console ("Internal error: %s - press any key\n", s);
	FreeAllData ();
	SetReturnCode_rselstr(IDS_ERR_INTERNAL);
	longjmp (fatal_error_exit, -1);
}

void ErrorFile (void)
{
	LDPUMA_Console ( "File error - press any key\n");
	FreeAllData ();
	SetReturnCode_rselstr(IDS_ERR_INTERNAL);
	longjmp (fatal_error_exit, -1);
}

void ErrorEmptyPage (void)
{
	LDPUMA_Console ("Internal error: page is empty - press any key\n");
	//    LT_Getch ();
	FreeAllData ();
	SetReturnCode_rselstr(IDS_ERR_EMPTYPAGE);
	longjmp (fatal_error_exit, -1);
	//    exit (-1);
}
# else

# include "msgerr.h"

# define ERROR_NO_ENOUGH_MEMORY  6
# define ERROR_INCLINE           7
# define ERROR_EMPTY             8
# define ERROR_FILE              8
# define ERROR_INTERNAL          8

#define sizeof(a) ((long)sizeof(a))

#ifdef DebugFile
//  03-24-94 06:30pm,Pit при включении DebugFile:
//      1. Выводится отладочная информация в файл FileError
extern char StringError[]; // временная строка для сообщений
extern FILE * FileError; // файл для сообщений
char * TemporaleString[80]; // временная строка для сообщений
#endif

struct MemAllocate {
#define MEMFREE         0x00   // память свободна
#define MEMMALLOC       0x01   // память отведена при malloc
#define MEMEND          0x04   // код последнего блока
	char id; // описатель блока см.выше
	long size; // размер блока
};
typedef struct MemAllocate Memory;

char * lout_memory = NULL; // адрес начала буфера

#define SizeBuffer      (2048*1024L)// размер ОП под работу со строками
static long SizeAllocate = 0; // число размещенных байт
static long SizeFree = 0; // число освобожденных байт
static long SizeMemory = 0; // размер занятой кучи

static void JoinEmptyBlocks() {
	Memory * Block;
	Memory * NextBlock;

	if (lout_memory == NULL)
		ErrorInternal("Malloc:Кучи еще нет!");
	// Укрупнение пустых блоков
	for (Block = (Memory *) lout_memory; Block->id != MEMEND; Block
			= (Memory *) ((char *) Block + Block->size + sizeof(Memory))) {

		NextBlock = (Memory *) ((char *) Block + Block->size + sizeof(Memory));
		toNextBlock: if (Block->id == MEMFREE && (NextBlock->id == MEMFREE
				|| NextBlock->id == MEMEND)) {
			Block->id = NextBlock->id;
			Block->size += NextBlock->size + sizeof(Memory);
			if (Block->id == MEMEND) {
				SizeMemory = (long) ((char *) Block - lout_memory);
				break;
			}
			NextBlock = (Memory *) ((char *) NextBlock + NextBlock->size
					+sizeof(Memory));
			goto toNextBlock;
		}
	}

}

void * DebugMalloc(size_t size) {
	void * memvoid;
	int idblock;
	long sizeblock;
	Memory * Block;
	Memory * NextBlock;

	if (lout_memory == NULL)
		ErrorInternal("Malloc:Кучи еще нет!");
	if (size == 0)
		return NULL;

	JoinEmptyBlocks();

	// поиск свободного места
	for (Block = (Memory *) lout_memory; (char *) Block < (lout_memory
			+SizeBuffer); Block = (Memory *) ((char *) Block + sizeof(Memory)
			+ Block->size)) {
		idblock = Block->id;
		sizeblock = Block->size;

		NextBlock = Block;

		if (idblock == MEMEND || idblock == MEMFREE) {
			if (sizeblock >= size + sizeof(Memory)) {
				// заполняем найденный свободный блок
				Block->id = MEMMALLOC;
				Block->size = size;
				memvoid = (void *) ((char *) Block + sizeof(Memory));
				// создаем новый свободный блок
				Block = (Memory *) ((char *) Block + size + sizeof(Memory));
				Block->id = idblock;
				Block->size = sizeblock - (size + sizeof(Memory));
				if (idblock == MEMEND)
					SizeMemory = (long) ((char *) Block - lout_memory);
				goto YES;
			} else {
				if (idblock == MEMEND)
					return NULL; //  ErrorNoEnoughMemory("Вся память исчерпана...");
			}
		} // if ...
	}// for ...

	ErrorNoEnoughMemory("Сбой при поиске свободного места...");
	return NULL;

	YES:
#ifdef DebugFile
	SizeAllocate+=size;
	if(FileError) {
		fprintf(FileError,"\nMalloc \t= %8li\tRunning \t= %li\tAll %li",
				(long)size,
				SizeAllocate-SizeFree,
				SizeMemory);
	}
#endif
	return memvoid;
}
// поиск уже размещенного блока
Memory * FindMem(void * blk) {
	Memory * Block;
	for (Block = (Memory *) lout_memory; Block->id != MEMEND; Block
			= (Memory *) ((char *) Block + Block->size + sizeof(Memory))) {
#ifdef DebugFile
		if((char *)Block>(lout_memory+SizeBuffer))
		ErrorInternal("\nСбой памяти при поиске блока...");
#endif
		if (blk == ((char *) Block + sizeof(Memory))) {
#ifdef DebugFile
			if(Block->id==MEMFREE)
			fprintf(FileError,"\nНайден освобожденный блок...");
#endif
			return Block;
		} // if(blk...
	}// for(...
	return NULL;
}

void DebugFree(void * blk) {
	Memory * Block;

	if (lout_memory == NULL)
		ErrorInternal("Free:Кучи еще нет!");
	if (blk == NULL)
		ErrorInternal("Попытка освободить память до ее размещения !");
	if ((Block = FindMem(blk)) == NULL) {
		//ErrorInternal("Освобождаемая память не найдена !");
		return;
	}
	//освобождение блока
	Block->id = MEMFREE;
	SizeFree += Block->size;
#ifdef DebugFile
	if(FileError) {
		fprintf(FileError,"\nFree \t= %8li\tRunning \t= %li\tAll %li",
				(long)Block->size,
				SizeAllocate-SizeFree,
				SizeMemory);
	}
#endif
}

void * DebugRealloc(void * old_blk, size_t size) {
	void * new_blk;
	Memory * Block;

	if (lout_memory == NULL)
		ErrorInternal("Realloc:Кучи еще нет!");
	if (size == 0)
		return NULL;
	if (old_blk == NULL) {
		new_blk = DebugMalloc(size);
		return new_blk;
	}

	if ((Block = FindMem(old_blk)) == NULL)
		ErrorInternal("\nИсходный буфер для REALLOC не найден!");

	if (Block->size <= size + sizeof(Memory)) {
		// требуется блок больших размеров
		new_blk = DebugMalloc(size);

		if (new_blk != NULL && old_blk != NULL)
			memcpy(new_blk, old_blk, (Block->size <= size ? Block->size : size));

		if (old_blk != NULL)
			DebugFree(old_blk);
	} else {
		// требуется блок меньших размеров
		Memory * NewBlock = (Memory *) ((char *) Block + size + sizeof(Memory));
		NewBlock->id = MEMFREE;
		NewBlock->size = Block->size - (size + sizeof(Memory));

		SizeFree += NewBlock->size;

		Block->size = size;
		new_blk = old_blk;
#ifdef DebugFile
		if(FileError) {
			fprintf(FileError,"\nRealloc \t= %8li\tRunning \t= %li\tAll %li",
					(long)Block->size,
					SizeAllocate-SizeFree,
					SizeMemory);
		}
#endif

	}

	return new_blk;
}

void ErrorNoEnoughMemory(const char * message) {
	message = message;
	FreeAllData();
#ifdef DebugFile

	fprintf(FileError,"\nMESSAGE:%s",message);
	fprintf(FileError,"\n%s\n%s",TemporaleString,StringError);

	fclose(FileError);
#endif
	error_exit(ERR_comp, ERROR_NO_ENOUGH_MEMORY);
}

void ErrorInternal(const char * msg) {
	FreeAllData();
#ifdef DebugFile
	fprintf(FileError,"\n%s", msg);
	fclose(FileError);
#endif
	error_exit(ERR_comp, ERROR_INTERNAL);
}

void ErrorFile(void) {
	FreeAllData();
	error_exit(ERR_comp, ERROR_FILE);
}

void ErrorEmptyPage(void) {
	FreeAllData();
#ifdef DebugFile

	fprintf(FileError,"\nPage is empty");

	fclose(FileError);
#endif
	error_exit(ERR_comp, ERROR_EMPTY);
}

#endif
