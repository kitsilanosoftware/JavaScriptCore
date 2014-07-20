/*
 *  Copyright (C) 2006 George Staikos <staikos@kde.org>
 *  Copyright (C) 2006 Alexey Proskuryakov <ap@nypop.com>
 *  Copyright (C) 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *  Copyright (C) 2011, 2012, 2013 Electronic Arts, Inc. All rights reserved.
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

#include "config.h"
#include "UnicodeEA.h"

#include <WTF/assertions.h>
#include <EABase/eabase.h>
#if defined(EA_PLATFORM_MICROSOFT)
#include EAWEBKIT_PLATFORM_HEADER
#endif

COMPILE_ASSERT(sizeof(UChar)	== sizeof(uint16_t), UCharShouldBeTwoBytes);
COMPILE_ASSERT(sizeof(wchar_t)	== 2, wchar_tShouldBeTwoBytes); //We are relying on it in many places.


// Note by Arpit Baldeva:
// Our implementation here tries to use EAText wherever possible. If we know that the EAText implementation is unfinished,
// we try to take advantage of underlying OS if possible. After that, we fall back on stub code which may or 
// may not work.
// FWIW, it is non trivial to provide a perfectly Unicode compatible system due to memory cost it ensues.
// Also, some of the platforms don't even support the entire unicode themselves so it is not a big deal.

namespace WTF {
	namespace Unicode {

		UChar32 foldCase(UChar32 c)
		{
			// For most unicode characters, foldCase is same as toLower. 
			return toLower(c);
		}


		int foldCase(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error)
		{
			*error = false;
			if (resultLength < srcLength) {
				*error = true;
				return srcLength;
			}
			for (int i = 0; i < srcLength; ++i)
				result[i] = foldCase(src[i]);

			return srcLength;
		}

		UChar32 toLower(UChar32 c)
		{
            ASSERT_WITH_MESSAGE(c <= 0xffff || c==0xffffffff, "We don't support chars above 0xffff");

            const JSText::Char source = static_cast<const JSText::Char>(c);
            JSText::Char result; 
            JSTextInterface* pTI = JSGetTextInterface();
            pTI->ConvertCase(&source, 1, &result, 1, JSText::kCaseTypeLower);
            return static_cast<UChar32>(result); 
		}

		int toLower(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error)
		{
			*error = false;
			if (resultLength < srcLength) {
				*error = true;
				return srcLength;
			}

            JSTextInterface* pTI = JSGetTextInterface();
            return pTI->ConvertCase(static_cast<const JSText::Char*>(src), srcLength, static_cast<JSText::Char*>(result), resultLength, JSText::kCaseTypeLower);
		}

		UChar32 toUpper(UChar32 c)
		{
            ASSERT_WITH_MESSAGE(c <= 0xffff || c==0xffffffff, "We don't support chars above 0xffff");
            
            const JSText::Char source = static_cast<const JSText::Char>(c);
            JSText::Char result; 
            JSTextInterface* pTI = JSGetTextInterface();
            pTI->ConvertCase(&source, 1, &result, 1, JSText::kCaseTypeUpper);
            return static_cast<UChar32>(result); 
        }


		int toUpper(UChar* result, int resultLength, const UChar* src, int srcLength, bool* error)
		{
			*error = false;
			if (resultLength < srcLength) {
				*error = true;
				return srcLength;
			}
            
            JSTextInterface* pTI = JSGetTextInterface();
            return pTI->ConvertCase(static_cast<const JSText::Char*>(src), srcLength, static_cast<JSText::Char*>(result), resultLength, JSText::kCaseTypeUpper);
		}

		UChar32 toTitleCase(UChar32 c)
		{
			// It's not correct to use toUpper, but we'll have to do with this for now.
			// EATextCollation.h/cpp doesn't yet support title case, but can be made
			// to do so when needed. Ask Paul Pedriana.

			// Unicode defines three kinds of case mapping: lowercase, uppercase, and
			// titlecase. The difference between uppercasing and titlecasing a character
			// or character sequence can be seen in compound characters (that is,
			// a single character that represents a compound of two characters).
			// 
			// For example, in Unicode, character U+01F3 is LATIN SMALL LETTER DZ. 
			// Let us write this compound character using ASCII as "dz".
			// This character uppercases to character U+01F1, LATIN CAPITAL LETTER DZ, 
			// which is basically "DZ". But it titlecases to to character U+01F2, 
			// LATIN CAPITAL LETTER D WITH SMALL LETTER Z, which we can write "Dz".
			// 
			// character uppercase titlecase
			// --------- --------- ---------
			//    dz        DZ        Dz

			return toUpper(c);
		}

		
		bool isArabicChar(UChar32 c)
		{
			// This is a very simplistic test and doesn't handle alternative forms. But pretty much all Arabic you see will be in this range.
			return (((unsigned)c - 0x0600u) <= (0x06FFu - 0x0600u));
		}

		CharCategory category(UChar32 c)
		{
			JSTextInterface* pTI = JSGetTextInterface();
            const JSText::CharCategory cc =(JSText::CharCategory) pTI->GetCharCategory((JSText::Char)c);

			switch(cc)
			{
				case JSText::kCCGeneralOtherTypes:	    return Other_NotAssigned; 
				case JSText::kCCUppercaseLetter:	    return Letter_Uppercase; 
				case JSText::kCCLowercaseLetter:	    return Letter_Lowercase; 
				case JSText::kCCTitlecaseLetter:	    return Letter_Titlecase; 
				case JSText::kCCModifierLetter:		    return Letter_Modifier; 
				case JSText::kCCOtherLetter:		    return Letter_Other; 


				case JSText::kCCNonSpacingMark:		    return Mark_NonSpacing; 
				case JSText::kCCEnclosingMark:		    return Mark_Enclosing; 
				case JSText::kCCCombiningSpacingMark:	return Mark_SpacingCombining; 


				case JSText::kCCDecimalDigitNumber:	    return Number_DecimalDigit; 
				case JSText::kCCLetterNumber:			return Number_Letter; 
				case JSText::kCCOtherNumber:			return Number_Other; 


				case JSText::kCCSpaceSeparator:		    return Separator_Space; 
				case JSText::kCCLineSeparator:		    return Separator_Line; 
				case JSText::kCCParagraphSeparator:	    return Separator_Paragraph; 


				case JSText::kCCControlChar:			return Other_Control; 
				case JSText::kCCFormatChar:			    return Other_Format; 
				case JSText::kCCPrivateUseChar:		    return Other_PrivateUse; 
				case JSText::kCCSurrogate:			    return Other_Surrogate; 


				case JSText::kCCDashPunctuation:		return Punctuation_Dash; 
				case JSText::kCCStartPunctuation:		return Punctuation_Open; 
				case JSText::kCCEndPunctuation:		    return Punctuation_Close; 
				case JSText::kCCConnectorPunctuation:   return Punctuation_Connector; 
				case JSText::kCCOtherPunctuation:		return Punctuation_Other; 


				case JSText::kCCMathSuymbol:			return Symbol_Math; 
				case JSText::kCCCurrencySymbol:		    return Symbol_Currency; 
				case JSText::kCCModifierSymbol:		    return Symbol_Modifier; 
				case JSText::kCCOtherSymbol:			return Symbol_Other; 

				case JSText::kCCInitialPunctuation:	    return Punctuation_InitialQuote; 
				case JSText::kCCFinalPunctuation:		return Punctuation_FinalQuote; 
			}

			return NoCategory;
			/* Old code used in 1.x
			if (c == 0x0D)
				return WTF::Unicode::Separator_Line;
			else if (c < 0x20)
				return WTF::Unicode::NoCategory;
			else if (c == 0x20)
				return WTF::Unicode::Separator_Space;
			else if ((c >= 0x21 && c<=0x27)
				|| c == 0x2E
				|| (c >= 0x3A && c<=0x3B)
				|| c == 0x3F
				|| c == 0x40
				|| (c >= 0x5B && c<=0x60)
				|| (c >= 0x7B && c<=0x7E))
				return WTF::Unicode::Punctuation_Other;
			else if (c == 0x28)
				return WTF::Unicode::Punctuation_Open;
			else if (c == 0x29)
				return WTF::Unicode::Punctuation_Close;
			else if ((c >= 0x2A && c<=0x2F) || (c >= 0x3C && c<=0x3E))
				return WTF::Unicode::Symbol_Math;
			else if (c >= 0x30 && c<=0x39)
				return WTF::Unicode::Number_DecimalDigit;
			else if (c >= 0x41 && c<=0x5A)
				return WTF::Unicode::Letter_Uppercase;
			else if (c >= 0x61 && c<=0x7A)
				return WTF::Unicode::Letter_Lowercase;
			else
				return WTF::Unicode::NoCategory;
				*/
		}

		
		bool isFormatChar(UChar32 c)
		{
			////////////////////////////////////////////////////////////////////////////////////
			// http://code.google.com/p/google-caja/wiki/JsControlFormatChars
			// 
			// The Unicode format-control characters (i.e., the characters in category "Cf" in 
			// the Unicode Character Database such as LEFT-TO-RIGHT MARK or RIGHT-TO-LEFT MARK) 
			// are control codes used to control the formatting of a range of text in the 
			// absence of higher-level protocols for this (such as mark-up languages). 
			// It is useful to allow these in source text to facilitate editing and display. 
			// The format control characters can occur anywhere in the source text of an ECMAScript program. 
			// These characters are removed from the source text before applying the lexical grammar. 
			// Since these characters are removed before processing string and regular expression literals, 
			// one must use a. Unicode escape sequence (see 7.6) to include a Unicode format-control 
			// character inside a string or regular expression literal. 
			// 
			// Format chars: 0x00ad, 0x0600-0x0603, 0x070f, 0x17b4-0x17b5, 0x200c-0x200f, 0x202a-0x202e, 0x2060-0x206f, 
			//               0xfeff, 0xfff9-0xfffb, 0x0001d173-0x0001d17a, 0x000e0001, 0x000e0020-0x000e007f. 
			////////////////////////////////////////////////////////////////////////////////////

			// Test the most common case first. Most characters will be evaluated with two true 'if' evaluations.
			if(c < 0x00AD) // This will usually be true.
				return true;

			// To consider: Make sorted table for the test of the values and do a binary search, 
			//              as it will probably result in faster execution. It will have 36 values
			//              if we ignore values > 0xffff.

			// Test 0x00AD
			if(c < 0x0600) // This will usually be true.
			{
				if(c != 0x00AD) // This will usually be true.
					return false;

				return true;
			}

			// Test 0xfeff, 0xfff9-0xfffb, 0x0001d173-0x0001d17a, 0x000e0001, 0x000e0020-0x000e007f
			if(c > 0x206F) // This will usually be true. Only for uncommon languages will it be false.
			{
				if(c < 0xFEFF)  // This will usually be true.
					return false;

				if((c != 0xFEFF) && ((c - 0xFFF9u) > (0xFFFBu - 0xFFF9u))) // This will usually be true.
				{
					// We could test the values > 0xffff here, but we don't support them and they aren't really used by anybody in practice.
					return false;
				}

				return true;
			}

			// Test 0x0600-0x0603, 0x070f, 0x17b4-0x17b5, 0x200c-0x200f, 0x202a-0x202e, 0x2060-0x206f.
			return (c <= 0x0603) || 
				(c == 0x070f) || 
				((c - 0x17B5u) <= (0x17B5u - 0x17B4u)) || 
				((c - 0x200Fu) <= (0x200Fu - 0x200Cu)) || 
				((c - 0x202Au) <= (0x202Eu - 0x202Au)) || 
				((c - 0x2060u) <= (0x206Fu - 0x2060u));
		}

		
		// c==0xffffffff is used in the assert message below to avoid situations when the caller passes -1.
		bool isSeparatorSpace(UChar32 c)
		{
			ASSERT_WITH_MESSAGE(c <= 0xffff || c==0xffffffff, "We don't support chars above 0xffff");
    		JSTextInterface* pTI = JSGetTextInterface();
            const JSText::CharCategory cc = (JSText::CharCategory) pTI->GetCharCategory((JSText::Char)c);
			return (cc == JSText::kCCSpaceSeparator);
		}


		bool isPrintableChar(UChar32 c)
		{
			ASSERT_WITH_MESSAGE(c <= 0xffff || c==0xffffffff, "We don't support chars above 0xffff");
			JSTextInterface* pTI = JSGetTextInterface();
			const JSText::CharCategory cc = (JSText::CharCategory) pTI->GetCharCategory((JSText::Char)c);
			return (cc >= JSText::kCCSurrogate);  // Currently in EAText all the non-printables are enumerated first.
	}


		bool isDigit(UChar32 c)
		{
			ASSERT_WITH_MESSAGE(c <= 0xffff || c==0xffffffff, "We don't support chars above 0xffff");
    		JSTextInterface* pTI = JSGetTextInterface();
			const JSText::CharCategory cc = (JSText::CharCategory) pTI->GetCharCategory((JSText::Char)c);
			return (cc == JSText::kCCDecimalDigitNumber);
		}


		bool isPunct(UChar32 c)
		{
			ASSERT_WITH_MESSAGE(c <= 0xffff || c==0xffffffff, "We don't support chars above 0xffff");
    		JSTextInterface* pTI = JSGetTextInterface();
			const JSText::CharCategory cc = (JSText::CharCategory) pTI->GetCharCategory((JSText::Char)c);
			return (cc == JSText::kCCDashPunctuation
				|| cc == JSText::kCCStartPunctuation
				|| cc == JSText::kCCEndPunctuation
				|| cc == JSText::kCCConnectorPunctuation
				|| cc == JSText::kCCOtherPunctuation
				|| cc == JSText::kCCInitialPunctuation
				|| cc == JSText::kCCFinalPunctuation);
		}

		
		//It is not terribly clear what following two functions refer to. Every port except those using ICU have them unimplemented and simply return false.
		bool hasLineBreakingPropertyComplexContext(UChar32 c)
		{
			return false;

		}
		bool hasLineBreakingPropertyComplexContextOrIdeographic(UChar32 c)
		{
			return false;
		}


		
		UChar32 mirroredChar(UChar32 c)
		{
			ASSERT_WITH_MESSAGE(c <= 0xffff || c==0xffffffff, "We don't support chars above 0xffff");
     		JSTextInterface* pTI = JSGetTextInterface();
            return pTI->GetMirrorChar((JSText::Char)c);
		}


		Direction direction(UChar32 c)
		{
			using namespace JSText;
			using namespace WTF::Unicode;
            JSTextInterface* pTI = JSGetTextInterface();
            const JSText::BidiClass bidiClass = (JSText::BidiClass) pTI->GetBidiClass((Char)c);

			// The EAText BidiClass is the same thing as the WTF::Unicode::Direction enum, 
			// except that they have a different ordering. We may be able to change the 
			// WTF version to match the EA version, but let's be safe now and not mess
			// with it until later when we have some better confidence.

			switch (bidiClass)
			{
			case JSText::kBidiClassON:  return OtherNeutral;
			case JSText::kBidiClassL:   return LeftToRight;
			case JSText::kBidiClassR:   return RightToLeft;
			case JSText::kBidiClassAN:  return ArabicNumber;
			case JSText::kBidiClassEN:  return EuropeanNumber;
			case JSText::kBidiClassAL:  return RightToLeftArabic;
			case JSText::kBidiClassNSM: return NonSpacingMark;
			case JSText::kBidiClassCS:  return CommonNumberSeparator;
			case JSText::kBidiClassES:  return EuropeanNumberSeparator;
			case JSText::kBidiClassET:  return EuropeanNumberTerminator;
			case JSText::kBidiClassBN:  return BoundaryNeutral;
			case JSText::kBidiClassS:   return SegmentSeparator;
			case JSText::kBidiClassWS:  return WhiteSpaceNeutral;
			case JSText::kBidiClassB:   return BlockSeparator;
			case JSText::kBidiClassRLO: return RightToLeftOverride;
			case JSText::kBidiClassRLE: return RightToLeftEmbedding;
			case JSText::kBidiClassLRO: return LeftToRightOverride;
			case JSText::kBidiClassLRE: return LeftToRightEmbedding;
			case JSText::kBidiClassPDF: return PopDirectionalFormat;
			}

			return LeftToRight;
		}


		bool isLower(UChar32 c)
		{
			ASSERT_WITH_MESSAGE(c <= 0xffff || c==0xffffffff, "We don't support chars above 0xffff");
     		JSTextInterface* pTI = JSGetTextInterface();
            const JSText::CharCategory cc = (JSText::CharCategory) pTI->GetCharCategory((JSText::Char)c);
			return (cc == JSText::kCCLowercaseLetter);
		}


		int digitValue(UChar32 c)
		{
			// The proper solution to this is to find out what the zero value is for the 
			// given digit. Ideally all Unicode digits would begin on a 0x10 boundary, but 
			// they don't, and so we can't use 

			// Complicated but most correct solution:
			// Deal with each Unicode digit type one at a time. There are about 20 of them.

			// Less complicated but doesn't work for every language (but does work for Western, Arabic, Thai, Lao, Tibetan, Fullwidth):
			return (c % 0x10);

			// Simplest solution that works for ASCII digits only. On the other hand, 
			// 99% of the time that's what we'll be working with, even if the text 
			// is from another language.
			// return (c - 0x30);
		}

		unsigned char combiningClass(UChar32 c)
		{
			// To do: Make sure that the EAText combining class values are the same as ICU.
			//        They should be so, as EAText follows the Unicode Standard on this.
    		JSTextInterface* pTI = JSGetTextInterface();
			const int32_t cc = pTI->GetCombiningClass((JSText::Char)c);
			return (unsigned char)(uint32_t)cc;
		}

		DecompositionType decompositionType(UChar32 c)
		{
			// Currently WebKit only uses this to check of a char is DecompositionFont or DecompositionCompat.
			// But if you look at the chars that are Font or Compat, they are unusual chars that we're not 
			// likely to see (with the possible exception of:
			//      00A8          ; Compat # Sk       DIAERESIS
			//      00AF          ; Compat # Sk       MACRON
			//      00B4          ; Compat # Sk       ACUTE ACCENT
			//      00B5          ; Compat # L&       MICRO SIGN
			//      00B8          ; Compat # Sk       CEDILLA
			// So just always return DecompositionNone.

			return WTF::Unicode::DecompositionNone;
		}


		int umemcasecmp(const UChar* a, const UChar* b, int len)
		{
			for (int i = 0; i < len; ++i)
			{
				const UChar32 c1 = foldCase(a[i]);
				const UChar32 c2 = foldCase(b[i]);

				if (c1 != c2)
					return (c1 - c2);
			}

			return 0;
		}

	} // Unicode
} // WTF


