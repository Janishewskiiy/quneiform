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

#include <string.h>
#include "cttypes.h"
#include "lang.h"
#include "ligas.h"
#include "evn32/evn.h"

#include "cfcompat.h"

static uchar alph_russian[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, // 5
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, // 7
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 8
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 9
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // a
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // d
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar alph_digital[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // 3
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 4
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 5
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 6
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 7
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // d
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar alph_ruseng[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, // 5
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, // 7
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 8
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 9
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // a
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // d
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar alph_english[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, // 5
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, // 7
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, // d
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar alph_ce[256] = {
//  0 1 2 3 4 5 6 7 8 9 A B C D E F
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
		0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, // 2
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 3
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, // 5
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, // 7
		1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 8
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 9
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a
		1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, // b
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // c
		0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, // d
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // e
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // f
		};
static uchar *alph_lang[LANG_TOTAL] = { alph_english, // LANGUAGE_ENGLISH    0
		alph_english, // LANGUAGE_GERMAN     1
		alph_english, // LANGUAGE_FRENCH     2
		alph_russian, // LANGUAGE_RUSSIAN    3
		alph_english, // LANGUAGE_SWEDISH    4
		alph_english, // LANGUAGE_SPANISH    5
		alph_english, // LANGUAGE_ITALIAN    6
		alph_ruseng, // LANGUAGE_RUS_ENG     7
		alph_russian, // LANGUAGE_UKRAINIAN  8
		alph_russian, // LANGUAGE_SERBIAN    9
		alph_ce, // LANGUAGE_CROATIAN   10
		alph_ce, // LANGUAGE_POLISH     11
		alph_english, // LANGUAGE_DANISH     12
		alph_english, // LANGUAGE_PORTUGUESE 13
		alph_english, // LANGUAGE_DUTCH      14
		alph_digital, // LANGUAGE_DIGITS        15
		alph_russian, // LANGUAGE_UZBEK		 16	// 01.09.2000 E.P.
		alph_russian, // LANGUAGE_KAZAKH	     17
		alph_ruseng, // LANGUAGE_KAZ_ENG    18
		alph_ce, // LANGUAGE_CZECH		 19
		alph_ce, // LANGUAGE_ROMANIAN		 20
		alph_ce, // LANGUAGE_HUNGARIAN	 21
		alph_russian, // LANGUAGE_BULGARIAN	 22
		alph_ce, // LANGUAGE_SLOVENIAN	 23
		alph_english, // LANGUAGE_LATVIAN	 24
		alph_english, // LANGUAGE_LITHUANIAN 25
		alph_english, // LANGUAGE_ESTONIAN	 26
		alph_english // LANGUAGE_TURKISH	 27
		};

Bool16 rec_set_alpha(uchar language, uchar *alphabet) {
	if (language >= LANG_TOTAL)
		return FALSE;
	if (alph_lang[language])
		memcpy(alphabet, alph_lang[language], 256);
	else
		return FALSE;
	if (language == LANGUAGE_RUSSIAN || language == LANGUAGE_RUS_ENG || language
			== LANGUAGE_BULGARIAN // 01.09.2000 E.P.
	) {
		alphabet[r_cu_d] = 1;
		alphabet[r_cu_g] = 1;
		alphabet[r_cu_m] = 1;
		alphabet[r_cu_u] = 1;
		alphabet[r_cu_z] = 1;
		alphabet[r_cu_a] = 1;

		alphabet[liga_exm] = 1;
		alphabet[liga_qm] = 1;
		alphabet[liga_uperc] = 1;
		alphabet[liga_lperc] = 1;
	}

	// � ���������� ��� ���� ������� ����. 01.09.2000 E.P.
	if (language == LANGUAGE_BULGARIAN) {
		alphabet[r_EE_2dot] = 0;
		alphabet[r_e_2dot] = 0;
		alphabet[(uchar) '�'] = 0;
		alphabet[(uchar) '�'] = 0;
		alphabet[(uchar) '�'] = 0;
		alphabet[(uchar) '�'] = 0;
	}

	if (language == LANGUAGE_UKRAINIAN) {
		alphabet[r_cu_d] = 1;
		alphabet[r_cu_g] = 1;
		alphabet[r_cu_m] = 1;
		alphabet[r_cu_u] = 1;
		alphabet[r_cu_z] = 1;
		alphabet[r_cu_a] = 1;
		alphabet['I'] = 1;
		alphabet['i'] = 1;
		alphabet[UKR_I] = 1;
		alphabet[UKR_i] = 1;
		alphabet[UKR_II] = 1;
		alphabet[UKR_ii] = 1;
		alphabet[UKR_E] = 1;
		alphabet[UKR_e] = 1;
		alphabet[UKR_G] = 1;
		alphabet[UKR_g] = 1;
		alphabet[liga_i] = 1;

		alphabet[liga_exm] = 1;
		alphabet[liga_qm] = 1;
		alphabet[liga_uperc] = 1;
		alphabet[liga_lperc] = 1;
	}
	if (language == LANGUAGE_SERBIAN) {
		alphabet[r_cu_d] = 1;
		alphabet[r_cu_g] = 1;
		alphabet[r_cu_m] = 1;
		alphabet[r_cu_u] = 1;
		alphabet[r_cu_z] = 1;
		alphabet[r_cu_a] = 1;
		alphabet[SERB_HH] = 1;
		alphabet[SERB_hh] = 1;
		alphabet[SERB_L] = 1;
		alphabet[SERB_l] = 1;
		alphabet[SERB_N] = 1;
		alphabet[SERB_n] = 1;
		alphabet[SERB_H] = 1;
		alphabet[SERB_h] = 1;
		alphabet[SERB_U] = 1;
		alphabet[SERB_u] = 1;
		alphabet[SERB_J] = 1;
		alphabet[SERB_j] = 1;
		alphabet['J'] = 1;
		alphabet['j'] = 1;
		alphabet[liga_j] = 1;

		alphabet[liga_exm] = 1;
		alphabet[liga_qm] = 1;
		alphabet[liga_uperc] = 1;
		alphabet[liga_lperc] = 1;
	}

	if (language == LANGUAGE_CROATIAN) {
		alphabet[CROAT_D] = 1;
		alphabet[CROAT_d] = 1;
		alphabet[CROAT_SR] = 1;
		alphabet[CROAT_sr] = 1;
		alphabet[CROAT_CA] = 1;
		alphabet[CROAT_ca] = 1;
		alphabet[CROAT_ZR] = 1;
		alphabet[CROAT_zr] = 1;
		alphabet[CROAT_CR] = 1;
		alphabet[CROAT_cr] = 1;
	}
	if (language == LANGUAGE_POLISH) {
		alphabet[POLISH_SS] = 1;
		alphabet[POLISH_s] = 1;
		alphabet[POLISH_ZZR] = 1;
		alphabet[POLISH_zr] = 1;
		alphabet[POLISH_ZZD] = 1;
		alphabet[POLISH_zd] = 1;
		alphabet[POLISH_CC] = 1;
		alphabet[POLISH_c] = 1;
		alphabet[POLISH_EE] = 1;
		alphabet[POLISH_e] = 1;
		alphabet[POLISH_NN] = 1;
		alphabet[POLISH_n] = 1;
		alphabet[POLISH_OO] = 1;
		alphabet[POLISH_o] = 1;
		alphabet[POLISH_LL] = 1;
		alphabet[POLISH_l] = 1;
	}

	if (language == LANGUAGE_GERMAN) {
		alphabet[AA_2dot_accent] = 1;
		alphabet[a_2dot_accent] = 1;
		alphabet[UU_2dot_accent] = 1;
		alphabet[u_2dot_accent] = 1;
		alphabet[OO_2dot_accent] = 1;
		alphabet[o_2dot_accent] = 1;
		alphabet[0xDF] = 1; // ??? small S-cet
	}

	if (language == LANGUAGE_FRENCH) {
		alphabet[AA_left_accent] = 1;
		alphabet[AA_roof_accent] = 1;
		alphabet[CC_bottom_accent] = 1;
		alphabet[EE_left_accent] = 1;
		alphabet[EE_right_accent] = 1;
		alphabet[EE_roof_accent] = 1;
		alphabet[EE_2dot_accent] = 1;
		alphabet[II_roof_accent] = 1;
		alphabet[II_2dot_accent] = 1;
		alphabet[OO_roof_accent] = 1;
		alphabet[OE_cap_deaf_sound] = 1;
		alphabet[AE_cap_deaf_sound] = 1;
		alphabet[UU_left_accent] = 1;
		alphabet[UU_roof_accent] = 1;
		alphabet[UU_2dot_accent] = 1;

		alphabet[a_left_accent] = 1;
		alphabet[a_roof_accent] = 1;
		alphabet[c_bottom_accent] = 1;
		alphabet[e_left_accent] = 1;
		alphabet[e_right_accent] = 1;
		alphabet[e_roof_accent] = 1;
		alphabet[e_2dot_accent] = 1;
		alphabet[i_roof_accent] = 1;
		alphabet[i_2dot_accent] = 1;
		alphabet[o_roof_accent] = 1;
		alphabet[oe_deaf_sound] = 1;
		alphabet[ae_deaf_sound] = 1;
		alphabet[u_left_accent] = 1;
		alphabet[u_roof_accent] = 1;
		alphabet[u_2dot_accent] = 1;
	}

	if (language == LANGUAGE_SWEDISH) {
		alphabet[AA_2dot_accent] = 1;
		alphabet[OO_2dot_accent] = 1;
		alphabet[AA_circle_accent] = 1;

		alphabet[a_2dot_accent] = 1;
		alphabet[o_2dot_accent] = 1;
		alphabet[a_circle_accent] = 1;
	}
	if (language == LANGUAGE_SPANISH) {
		alphabet[AA_right_accent] = 1;
		alphabet[EE_right_accent] = 1;
		alphabet[II_right_accent] = 1;
		alphabet[OO_right_accent] = 1;
		alphabet[UU_right_accent] = 1;
		alphabet[UU_2dot_accent] = 1;
		alphabet[NN_tild_accent] = 1;

		alphabet[a_right_accent] = 1;
		alphabet[e_right_accent] = 1;
		alphabet[i_right_accent] = 1;
		alphabet[o_right_accent] = 1;
		alphabet[u_right_accent] = 1;
		alphabet[u_2dot_accent] = 1;
		alphabet[n_tild_accent] = 1;
	}
	if (language == LANGUAGE_ITALIAN) {
		alphabet[AA_left_accent] = 1;
		alphabet[EE_left_accent] = 1;
		alphabet[EE_right_accent] = 1;
		alphabet[II_left_accent] = 1;
		alphabet[II_right_accent] = 1;
		alphabet[II_roof_accent] = 1;
		alphabet[OO_left_accent] = 1;
		alphabet[OO_right_accent] = 1;
		alphabet[UU_left_accent] = 1;
		alphabet[UU_right_accent] = 1;

		alphabet[a_left_accent] = 1;
		alphabet[e_left_accent] = 1;
		alphabet[e_right_accent] = 1;
		alphabet[i_left_accent] = 1;
		alphabet[i_right_accent] = 1;
		alphabet[i_roof_accent] = 1;
		alphabet[o_left_accent] = 1;
		alphabet[o_right_accent] = 1;
		alphabet[u_left_accent] = 1;
		alphabet[u_right_accent] = 1;
	}
	if (language == LANGUAGE_DANISH) {
		alphabet[AE_cap_deaf_sound] = 1;
		alphabet[AA_circle_accent] = 1;
		alphabet[OO_crossed] = 1;

		alphabet[ae_deaf_sound] = 1;
		alphabet[a_circle_accent] = 1;
		alphabet[o_crossed] = 1;
	}
	if (language == LANGUAGE_PORTUGUESE) {
		alphabet[AA_left_accent] = 1;
		alphabet[AA_right_accent] = 1;
		alphabet[AA_tild_accent] = 1;
		alphabet[AA_roof_accent] = 1;
		alphabet[CC_bottom_accent] = 1;
		alphabet[EE_left_accent] = 1;
		alphabet[EE_right_accent] = 1;
		alphabet[EE_2dot_accent] = 1;
		alphabet[EE_roof_accent] = 1;
		alphabet[OO_left_accent] = 1;
		alphabet[OO_right_accent] = 1;
		alphabet[OO_tild_accent] = 1;
		alphabet[OO_roof_accent] = 1;
		alphabet[II_right_accent] = 1;
		alphabet[II_2dot_accent] = 1;
		alphabet[UU_roof_accent] = 1;

		alphabet[a_left_accent] = 1;
		alphabet[a_right_accent] = 1;
		alphabet[a_tild_accent] = 1;
		alphabet[a_roof_accent] = 1;
		alphabet[c_bottom_accent] = 1;
		alphabet[e_left_accent] = 1;
		alphabet[e_right_accent] = 1;
		alphabet[e_2dot_accent] = 1;
		alphabet[e_roof_accent] = 1;
		alphabet[o_left_accent] = 1;
		alphabet[o_right_accent] = 1;
		alphabet[o_tild_accent] = 1;
		alphabet[o_roof_accent] = 1;
		alphabet[i_right_accent] = 1;
		alphabet[i_2dot_accent] = 1;
		alphabet[u_roof_accent] = 1;
	}
	if (language == LANGUAGE_DUTCH) {
		alphabet[AA_left_accent] = 1;
		alphabet[AA_2dot_accent] = 1;
		alphabet[AA_roof_accent] = 1;
		alphabet[CC_bottom_accent] = 1;
		alphabet[EE_left_accent] = 1;
		alphabet[EE_right_accent] = 1;
		alphabet[EE_2dot_accent] = 1;
		alphabet[EE_roof_accent] = 1;
		alphabet[OO_2dot_accent] = 1;
		alphabet[NN_tild_accent] = 1;
		alphabet[II_roof_accent] = 1;
		alphabet[II_2dot_accent] = 1;
		alphabet[UU_roof_accent] = 1;
		alphabet[UU_2dot_accent] = 1;

		alphabet[a_left_accent] = 1;
		alphabet[a_2dot_accent] = 1;
		alphabet[a_roof_accent] = 1;
		alphabet[c_bottom_accent] = 1;
		alphabet[e_left_accent] = 1;
		alphabet[e_right_accent] = 1;
		alphabet[e_2dot_accent] = 1;
		alphabet[e_roof_accent] = 1;
		alphabet[o_2dot_accent] = 1;
		alphabet[n_tild_accent] = 1;
		alphabet[i_roof_accent] = 1;
		alphabet[i_2dot_accent] = 1;
		alphabet[u_roof_accent] = 1;
		alphabet[u_2dot_accent] = 1;
	}

	// � ����� � ������ ������ ��� ��� cp1250 01.09.2000 E.P.
	if (is_cen_language(language)) {
		alphabet[liga_exm] = 1;
		alphabet[liga_inv_exm] = 1;
		alphabet[right_quocket] = 1;
		alphabet[liga_CC] = 1;
		alphabet[liga_CR] = 1;
	}

	if (language == LANGUAGE_CZECH) {
		alphabet[AA_right_accent] = alphabet[a_right_accent] = 1;
		alphabet[CC_inv_roof] = alphabet[c_inv_roof] = 1;
		alphabet[DD_inv_roof] = alphabet[d_inv_roof] = 1;
		alphabet[EE_right_accent] = alphabet[e_right_accent] = 1;
		alphabet[EE_inv_roof] = alphabet[e_inv_roof] = 1;
		alphabet[II_right_accent] = alphabet[i_right_accent] = 1;
		alphabet[NN_inv_roof] = alphabet[n_inv_roof] = 1;
		alphabet[OO_right_accent] = alphabet[o_right_accent] = 1;
		alphabet[RR_inv_roof] = alphabet[r_inv_roof] = 1;
		alphabet[SS_inv_roof_latin] = alphabet[s_inv_roof_latin] = 1;
		alphabet[TT_inv_roof] = alphabet[t_inv_roof] = 1;
		alphabet[UU_right_accent] = alphabet[u_right_accent] = 1;
		alphabet[UU_circle_accent] = alphabet[u_circle_accent] = 1;
		alphabet[YY_right_accent] = alphabet[y_right_accent] = 1;
		alphabet[ZZ_inv_roof_latin] = alphabet[z_inv_roof_latin] = 1;
	}

	if (language == LANGUAGE_ROMANIAN) {
		alphabet[AA_semicircle] = alphabet[a_semicircle] = 1;
		alphabet[AA_roof_accent] = alphabet[a_roof_accent] = 1;
		alphabet[II_roof_accent] = alphabet[i_roof_accent] = 1;
		alphabet[SS_bottom_accent] = alphabet[s_bottom_accent] = 1;
		alphabet[TT_bottom_accent] = alphabet[t_bottom_accent] = 1;
	}

	if (language == LANGUAGE_HUNGARIAN) {
		alphabet[AA_right_accent] = alphabet[a_right_accent] = 1;
		alphabet[EE_right_accent] = alphabet[e_right_accent] = 1;
		alphabet[II_right_accent] = alphabet[i_right_accent] = 1;
		alphabet[OO_right_accent] = alphabet[o_right_accent] = 1;
		alphabet[OO_2dot_accent] = alphabet[o_2dot_accent] = 1;
		alphabet[OO_double_right] = alphabet[o_double_right] = 1;
	}

	if (language == LANGUAGE_SLOVENIAN) {
		alphabet[CC_inv_roof] = alphabet[c_inv_roof] = 1;
		alphabet[SS_inv_roof_latin] = alphabet[s_inv_roof_latin] = 1;
		alphabet[ZZ_inv_roof_latin] = alphabet[z_inv_roof_latin] = 1;
	}

	// ������������� ���� cp1257 09.07.2001 E.P.
	if (is_baltic_language(language)) {
		alphabet[low_quotes] = 1;
		alphabet[liga_exm] = 1;
		alphabet[liga_i] = 1;
		alphabet[right_quocket] = 1;
		alphabet[liga_TM] = 1;
		alphabet[liga_CC] = 1;
		alphabet[liga_CR] = 1;
		alphabet[liga_bull_baltic] = 1;
	}

	if (language == LANGUAGE_LATVIAN) {
		// LATVIAN 09.07.2001 E.P.
		alphabet[AA_macron] = 1;
		alphabet[a_macron] = 1;
		alphabet[CC_inv_roof] = 1;
		alphabet[c_inv_roof] = 1;
		alphabet[EE_macron] = 1;
		alphabet[e_macron] = 1;
		alphabet[GG_bottom_accent] = 1;
		alphabet[LATVIAN_g] = 1;
		alphabet[II_macron] = 1;
		alphabet[i_macron] = 1;
		alphabet[KK_bottom_accent] = 1;
		alphabet[k_bottom_accent] = 1;
		alphabet[LL_bottom_accent] = 1;
		alphabet[l_bottom_accent] = 1;
		alphabet[NN_bottom_accent] = 1;
		alphabet[n_bottom_accent] = 1;
		alphabet[SS_inv_roof_baltic] = 1;
		alphabet[s_inv_roof_baltic] = 1;
		alphabet[UU_macron] = 1;
		alphabet[u_macron] = 1;
		alphabet[ZZ_inv_roof_baltic] = 1;
		alphabet[z_inv_roof_baltic] = 1;

		alphabet[(uchar) 'Q'] = alphabet[(uchar) 'q'] = 0;
		alphabet[(uchar) 'W'] = alphabet[(uchar) 'w'] = 0;
		alphabet[(uchar) 'X'] = alphabet[(uchar) 'x'] = 0;
		alphabet[(uchar) 'Y'] = alphabet[(uchar) 'y'] = 0;
	}

	if (language == LANGUAGE_LITHUANIAN) {
		// LITHUANIAN 09.07.2001 E.P.
		alphabet[AA_bottom_accent] = 1;
		alphabet[a_bottom_accent] = 1;
		alphabet[CC_inv_roof] = 1;
		alphabet[c_inv_roof] = 1;
		alphabet[EE_bottom_accent] = 1;
		alphabet[e_bottom_accent] = 1;
		alphabet[EE_dot_accent] = 1;
		alphabet[e_dot_accent] = 1;
		alphabet[II_bottom_accent] = 1;
		alphabet[i_bottom_accent] = 1;
		alphabet[SS_inv_roof_baltic] = 1;
		alphabet[s_inv_roof_baltic] = 1;
		alphabet[UU_bottom_accent] = 1;
		alphabet[u_bottom_accent] = 1;
		alphabet[UU_macron] = 1;
		alphabet[u_macron] = 1;
		alphabet[ZZ_inv_roof_baltic] = 1;
		alphabet[z_inv_roof_baltic] = 1;

		alphabet[(uchar) 'Q'] = alphabet[(uchar) 'q'] = 0;
		alphabet[(uchar) 'W'] = alphabet[(uchar) 'w'] = 0;
		alphabet[(uchar) 'X'] = alphabet[(uchar) 'x'] = 0;
	}

	if (language == LANGUAGE_ESTONIAN) {
		// ESTONIAN 09.07.2001 E.P.
		alphabet[AA_2dot_accent] = 1;
		alphabet[a_2dot_accent] = 1;
		alphabet[OO_tild_accent] = 1;
		alphabet[o_tild_accent] = 1;
		alphabet[OO_2dot_accent] = 1;
		alphabet[o_2dot_accent] = 1;
		alphabet[SS_inv_roof_baltic] = 1;
		alphabet[s_inv_roof_baltic] = 1;
		alphabet[UU_2dot_accent] = 1;
		alphabet[u_2dot_accent] = 1;
		alphabet[ZZ_inv_roof_baltic] = 1;
		alphabet[z_inv_roof_baltic] = 1;

		alphabet[(uchar) 'C'] = alphabet[(uchar) 'c'] = 0;
		alphabet[(uchar) 'Q'] = alphabet[(uchar) 'q'] = 0;
		alphabet[(uchar) 'W'] = alphabet[(uchar) 'w'] = 0;
		alphabet[(uchar) 'X'] = alphabet[(uchar) 'x'] = 0;
		alphabet[(uchar) 'Y'] = alphabet[(uchar) 'y'] = 0;

	}

	// �������� ����. 20.05.2002 E.P.
	if (language == LANGUAGE_TURKISH) {
		// �������� ����, �������� �� ������������������
		alphabet[liga_TM_turkish] = 1;
		alphabet[liga_CC_turkish] = 1;
		alphabet[liga_CR_turkish] = 1;
		alphabet[liga_bull_turkish] = 1;

		// �������� �����, �������� �� ������������������
		alphabet[GG_semicircle] = 1;
		alphabet[g_semicircle] = 1;
		alphabet[II_dot_accent] = 1;
		alphabet[i_sans_accent] = 1;
		alphabet[SS_bottom_accent_turkish] = 1;
		alphabet[s_bottom_accent_turkish] = 1;
		alphabet['Q'] = alphabet['q'] = 0;
		alphabet['W'] = alphabet['w'] = 0;
		alphabet['X'] = alphabet['x'] = 0;
	}

	return TRUE;
}

static const char *tabevn1[LANG_TOTAL] = { "rec1.dat", // LANGUAGE_ENGLISH		0
		"rec1.dat", // LANGUAGE_GERMAN		1
		"rec1.dat", // LANGUAGE_FRENCH		2
		"rec1rus.dat", // LANGUAGE_RUSSIAN		3
		"rec1.dat", // LANGUAGE_SWEDISH		4
		"rec1.dat", // LANGUAGE_SPANISH		5
		"rec1.dat", // LANGUAGE_ITALIAN		6
		"rec1r&e.dat", // LANGUAGE_RUS_ENG		7
		"rec1rus.dat", // LANGUAGE_UKRAINIAN	8
		"rec1rus.dat", // LANGUAGE_SERBIAN		9
		"rec1cen.dat", // LANGUAGE_CROATIAN	10
		"rec1cen.dat", // LANGUAGE_POLISH		11
		"rec1n.dat", // LANGUAGE_DANISH		12
		"rec1n.dat", // LANGUAGE_PORTUGUESE	13
		"rec1n.dat", // LANGUAGE_DUTCH		14
		"rec1.dat", // LANGUAGE_DIGITS			15
		"rec1uzb.dat", // LANGUAGE_UZBEK		16	// 01.09.2000 E.P.
		"rec1kaz.dat", // LANGUAGE_KAZAKH			17
		"rec1kaz.dat", // LANGUAGE_KAZ_ENG		18
		"rec1cen.dat", // LANGUAGE_CZECH		19
		"rec1cen.dat", // LANGUAGE_ROMANIAN		20
		"rec1cen.dat", // LANGUAGE_HUNGARIAN		21
		"rec1rus.dat", // LANGUAGE_BULGARIAN		22
		"rec1cen.dat", // LANGUAGE_SLOVENIAN	23
		"rec1blt.dat", // LANGUAGE_LATVIAN	    24
		"rec1blt.dat", // LANGUAGE_LITHUANIAN  25
		"rec1blt.dat", // LANGUAGE_ESTONIAN	26
		"rec1tur.dat" // LANGUAGE_TURKISH		27
		};

static const char *tabevn2[LANG_TOTAL] = { "rec2.dat", // LANGUAGE_ENGLISH		0
		"rec2.dat", // LANGUAGE_GERMAN		1
		"rec2.dat", // LANGUAGE_FRENCH		2
		"rec2rus.dat", // LANGUAGE_RUSSIAN		3
		"rec2.dat", // LANGUAGE_SWEDISH		4
		"rec2.dat", // LANGUAGE_SPANISH		5
		"rec2.dat", // LANGUAGE_ITALIAN		6
		"rec2r&e.dat", // LANGUAGE_RUS_ENG		7
		"rec2rus.dat", // LANGUAGE_UKRAINIAN	8
		"rec2rus.dat", // LANGUAGE_SERBIAN		9
		"rec2cen.dat", // LANGUAGE_CROATIAN	10
		"rec2cen.dat", // LANGUAGE_POLISH		11
		"rec2n.dat", // LANGUAGE_DANISH		12
		"rec2n.dat", // LANGUAGE_PORTUGUESE	13
		"rec2n.dat", // LANGUAGE_DUTCH		14
		"rec2.dat", // LANGUAGE_DIGITS			15
		"rec2uzb.dat", // LANGUAGE_UZBEK		16 // 01.09.2000 E.P.
		"rec2kaz.dat", // LANGUAGE_KAZAKH			17
		"rec2kaz.dat", // LANGUAGE_KAZ_ENG		18
		"rec2cen.dat", // LANGUAGE_CZECH		19
		"rec2cen.dat", // LANGUAGE_ROMANIAN		20
		"rec2cen.dat", // LANGUAGE_HUNGARIAN		21
		"rec2rus.dat", // LANGUAGE_BULGARIAN		22
		"rec2cen.dat", // LANGUAGE_SLOVENIAN	23
		"rec2blt.dat", // LANGUAGE_LATVIAN	    24
		"rec2blt.dat", // LANGUAGE_LITHUANIAN  25
		"rec2blt.dat", // LANGUAGE_ESTONIAN	26
		"rec2tur.dat" // LANGUAGE_TURKISH		27
		};

Bool16 rec_load_tables(uchar language) {
	if (language >= LANG_TOTAL)
		return FALSE;
	if (!EVNInitLanguage(tabevn1[language], tabevn2[language], language))
		return FALSE;
	return TRUE;
}

Bool16 rec_is_language(uchar language) {
	if (language < LANGUAGE_ENGLISH || language >= LANG_TOTAL)
		return FALSE;
	if (data_file_exists(tabevn1[language]) == -1)
		return FALSE;
	if (data_file_exists(tabevn2[language]) == -1)
		return FALSE;

	return TRUE;
}
