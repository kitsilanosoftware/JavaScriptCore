/*
Copyright (C) 2011, 2013 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef JSTextInterface_h
#define JSTextInterface_h

#include "JSBase.h"
#include <EABase/eabase.h>


namespace JSText 
{
      
    //+ This needs to stay in sync with the EAWebKitInterface.h enums  
    typedef char16_t Char;
    
    enum CharCategory
    {
        kCCUnassigned             =  0, /// Non-category for unassigned and non-character code points.
        kCCGeneralOtherTypes      =  0, /// Cn other, not assigned. Same as kCCUnassigned. Note that no characters in UnicodeData.txt have this property. 
        kCCControlChar            =  1, /// Cc
        kCCFormatChar             =  2, /// Cf
        kCCPrivateUseChar         =  3, /// Co
        kCCSurrogate              =  4, /// Cs
        kCCUppercaseLetter        =  5, /// Lu
        kCCLowercaseLetter        =  6, /// Ll
        kCCTitlecaseLetter        =  7, /// Lt
        kCCModifierLetter         =  8, /// Lm
        kCCOtherLetter            =  9, /// Lo
        kCCNonSpacingMark         = 10, /// Mn
        kCCEnclosingMark          = 11, /// Me
        kCCCombiningSpacingMark   = 12, /// Mc
        kCCDecimalDigitNumber     = 13, /// Nd
        kCCLetterNumber           = 14, /// Nl
        kCCOtherNumber            = 15, /// No
        kCCSpaceSeparator         = 16, /// Zs
        kCCLineSeparator          = 17, /// Zl
        kCCParagraphSeparator     = 18, /// Zp
        kCCDashPunctuation        = 19, /// Pd
        kCCStartPunctuation       = 20, /// Ps
        kCCEndPunctuation         = 21, /// Pe
        kCCConnectorPunctuation   = 22, /// Pc
        kCCOtherPunctuation       = 23, /// Po
        kCCInitialPunctuation     = 24, /// Pi
        kCCFinalPunctuation       = 25, /// Pf
        kCCMathSuymbol            = 26, /// Sm
        kCCCurrencySymbol         = 27, /// Sc
        kCCModifierSymbol         = 28, /// Sk
        kCCOtherSymbol            = 29  /// So
    };

    enum BidiClass
    {
        kBidiClassON = 0,   /// Other Neutrals. Includes all other characters, including OBJECT REPLACEMENT CHARACTER.
        kBidiClassL,        /// Left-to-Right. Includes LRM, Most alphabetic, syllabic, Han ideographic characters, digits that are neither European nor Arabic, ...
        kBidiClassR,        /// Right-to-Left. Includes RLM, Hebrew alphabet, most punctuation specific to that script, ...
        kBidiClassAN,       /// Arabic Number. Includes Arabic-Indic digits, Arabic decimal & thousands separators, ...
        kBidiClassEN,       /// European Number. Includes European digits, Eastern Arabic-Indic digits, ...
        kBidiClassAL,       /// Right-to-Left Arabic. Includes Arabic, Thaana, and Syriac alphabets, most punctuation specific to those scripts, ...
        kBidiClassNSM,      /// Non-Spacing Mark. Includes characters marked Mn (Non-Spacing Mark) and Me (Enclosing Mark) in the Unicode Character Database.
        kBidiClassCS,       /// Common Number Separator. Includes colon, comma, full stop (period), non-breaking space, ...
        kBidiClassES,       /// European Number Separator. Includes plus sign, minus sign.
        kBidiClassET,       /// European Number Terminator. Includes degree, currency symbols, ...
        kBidiClassBN,       /// Boundary Neutral. Includes most formatting and control characters, other than those explicitly given types above.
        kBidiClassS,        /// Segment Separator. Includes tab.
        kBidiClassWS,       /// Whitespace. Includes space, figure space, line separator, form feed, general punctuation spaces, ...
        kBidiClassB,        /// Paragraph Separator. Includes paragraph separator, appropriate newline functions, higher-protocol paragraph determination.
        kBidiClassRLO,      /// Right-to-Left Override.
        kBidiClassRLE,      /// Right-to-Left Embedding.
        kBidiClassLRO,      /// Left-to-Right Override.
        kBidiClassLRE,      /// Left-to-Right Embedding.
        kBidiClassPDF,      /// Pop Directional Format.
    };

    enum CaseType
    {
        kCaseTypeNone,      /// No case.
        kCaseTypeLower,     /// Lower case.
        kCaseTypeUpper,     /// Upper case.
        kCaseTypeTitle,     /// Title case, which refers to chars in some language which combine an upper and lower character, such as Dz.
        kCaseTypeReverse    /// The reverse of the current case. Useful for flipping string sort order between upper and lower case strings.
    };

    //- End of sync
} //namespace JSText 

class JSTextInterface
{
public:
    virtual ~JSTextInterface(void) {}
    
    virtual uint32_t GetCharCategory(char16_t c) =0;          
    virtual uint32_t GetMirrorChar(char16_t  c) =0; 
    virtual int32_t GetCombiningClass(char16_t c) =0;
    virtual uint32_t GetBidiClass(char16_t c) =0;
    virtual uint32_t ConvertCase(const JSText::Char* pTextInput, uint32_t nTextLength, JSText::Char* pTextOutput, uint32_t nTextOutputCapacity, JSText::CaseType caseType) = 0;
};


void JSSetTextInterface(JSTextInterface* p);
JSTextInterface* JSGetTextInterface(void); 




#endif // JSTextInterface_h


