/*
 *  Copyright (C) 2006 George Staikos <staikos@kde.org>
 *  Copyright (C) 2006 Alexey Proskuryakov <ap@nypop.com>
 *  Copyright (C) 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *  Copyright (C) 2011, 2012 Electronic Arts, Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef WTF_UNICODE_EA_H
#define WTF_UNICODE_EA_H

#include "UnicodeMacrosFromICU.h"
#include "ScriptCodesFromICU.h"
#include "JSTextInterface.h"

#ifndef U_SIZEOF_WCHAR_T // If the user hasn't specified that it is a given size...
	#if defined(__WCHAR_MAX__) // GCC defines this for most platforms.
		#if (__WCHAR_MAX__ == 2147483647) || (__WCHAR_MAX__ == 4294967295)
			#define U_SIZEOF_WCHAR_T 4
		#elif (__WCHAR_MAX__ == 32767) || (__WCHAR_MAX__ == 65535)
			#define U_SIZEOF_WCHAR_T 2
		#elif (__WCHAR_MAX__ == 127) || (__WCHAR_MAX__ == 255)
			#define U_SIZEOF_WCHAR_T 1
		#else
			#define U_SIZEOF_WCHAR_T 4
	#endif
	#elif defined(WCHAR_MAX) // The SN compiler defines this.
		#if (WCHAR_MAX == 2147483647) || (WCHAR_MAX == 4294967295)
			#define U_SIZEOF_WCHAR_T 4
		#elif (WCHAR_MAX == 32767) || (WCHAR_MAX == 65535)
			#define U_SIZEOF_WCHAR_T 2
		#elif (WCHAR_MAX == 127) || (WCHAR_MAX == 255)
			#define U_SIZEOF_WCHAR_T 1
		#else
			#define U_SIZEOF_WCHAR_T 4
		#endif
	#elif defined(_MSC_VER)
		#define U_SIZEOF_WCHAR_T 2
	#else
		#define U_SIZEOF_WCHAR_T 4
	#endif
#endif


#if U_SIZEOF_WCHAR_T == 4
	typedef unsigned short UChar;
#else
	typedef char16_t UChar;
#endif

typedef uint32_t UChar32;

namespace WTF {
	namespace Unicode {

		enum Direction {
			LeftToRight,
			RightToLeft,
			EuropeanNumber,
			EuropeanNumberSeparator,
			EuropeanNumberTerminator,
			ArabicNumber,
			CommonNumberSeparator,
			BlockSeparator,
			SegmentSeparator,
			WhiteSpaceNeutral,
			OtherNeutral,
			LeftToRightEmbedding,
			LeftToRightOverride,
			RightToLeftArabic,
			RightToLeftEmbedding,
			RightToLeftOverride,
			PopDirectionalFormat,
			NonSpacingMark,
			BoundaryNeutral
		};

		enum DecompositionType {
			DecompositionNone,
			DecompositionCanonical,
			DecompositionCompat,
			DecompositionCircle,
			DecompositionFinal,
			DecompositionFont,
			DecompositionFraction,
			DecompositionInitial,
			DecompositionIsolated,
			DecompositionMedial,
			DecompositionNarrow,
			DecompositionNoBreak,
			DecompositionSmall,
			DecompositionSquare,
			DecompositionSub,
			DecompositionSuper,
			DecompositionVertical,
			DecompositionWide,
		};

		enum CharCategory {
			NoCategory =  0,
			Other_NotAssigned			= U_MASK(JSText::kCCGeneralOtherTypes),
			Letter_Uppercase			= U_MASK(JSText::kCCUppercaseLetter),
			Letter_Lowercase			= U_MASK(JSText::kCCLowercaseLetter),
			Letter_Titlecase			= U_MASK(JSText::kCCTitlecaseLetter),
			Letter_Modifier				= U_MASK(JSText::kCCModifierLetter),
			Letter_Other				= U_MASK(JSText::kCCOtherLetter),

			Mark_NonSpacing				= U_MASK(JSText::kCCNonSpacingMark),
			Mark_Enclosing				= U_MASK(JSText::kCCEnclosingMark),
			Mark_SpacingCombining		= U_MASK(JSText::kCCCombiningSpacingMark),

			Number_DecimalDigit			= U_MASK(JSText::kCCDecimalDigitNumber),
			Number_Letter				= U_MASK(JSText::kCCLetterNumber),
			Number_Other				= U_MASK(JSText::kCCOtherNumber),

			Separator_Space				= U_MASK(JSText::kCCSpaceSeparator),
			Separator_Line				= U_MASK(JSText::kCCLineSeparator),
			Separator_Paragraph			= U_MASK(JSText::kCCParagraphSeparator),

			Other_Control				= U_MASK(JSText::kCCControlChar),
			Other_Format				= U_MASK(JSText::kCCFormatChar),
			Other_PrivateUse			= U_MASK(JSText::kCCPrivateUseChar),
			Other_Surrogate				= U_MASK(JSText::kCCSurrogate),

			Punctuation_Dash			= U_MASK(JSText::kCCDashPunctuation),
			Punctuation_Open			= U_MASK(JSText::kCCStartPunctuation),
			Punctuation_Close			= U_MASK(JSText::kCCEndPunctuation),
			Punctuation_Connector		= U_MASK(JSText::kCCConnectorPunctuation),
			Punctuation_Other			= U_MASK(JSText::kCCOtherPunctuation),

			Symbol_Math					= U_MASK(JSText::kCCMathSuymbol),
			Symbol_Currency				= U_MASK(JSText::kCCCurrencySymbol),
			Symbol_Modifier				= U_MASK(JSText::kCCModifierSymbol),
			Symbol_Other				= U_MASK(JSText::kCCOtherSymbol),

			Punctuation_InitialQuote	= U_MASK(JSText::kCCInitialPunctuation),
			Punctuation_FinalQuote		= U_MASK(JSText::kCCFinalPunctuation)
		};

		UChar32 foldCase(UChar32);
		int		foldCase(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error);
		
		UChar32 toLower(UChar32 c);
		int		toLower(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error);
		
		UChar32 toUpper(UChar32 c);
		int		toUpper(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error);
		
		UChar32 toTitleCase(UChar32 c);
		
		bool isArabicChar(UChar32 c); 
		bool isSeparatorSpace(UChar32 c);
		bool isPrintableChar(UChar32 c);
		bool isDigit(UChar32 c); 
		bool isPunct(UChar32 c);
		bool hasLineBreakingPropertyComplexContext(UChar32 c);
		bool hasLineBreakingPropertyComplexContextOrIdeographic(UChar32 c);
		UChar32 mirroredChar(UChar32 c);
		CharCategory category(UChar32 c);
		bool isLower(UChar32 c);
		int digitValue(UChar32 c);
		uint8_t combiningClass(UChar32 c);
		DecompositionType decompositionType(UChar32 c);
		int umemcasecmp(const UChar*, const UChar*, int len);
		Direction direction(UChar32 c);

	}
}
#endif // WTF_UNICODE_EA_H
