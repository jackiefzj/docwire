/*********************************************************************************************************************************************/
/*  DocWire SDK: Award-winning modern data processing in C++17/20. SourceForge Community Choice & Microsoft support. AI-driven processing.   */
/*  Supports nearly 100 data formats, including email boxes and OCR. Boost efficiency in text extraction, web data extraction, data mining,  */
/*  document analysis. Offline processing possible for security and confidentiality                                                          */
/*                                                                                                                                           */
/*  Copyright (c) SILVERCODERS Ltd, http://silvercoders.com                                                                                  */
/*  Project homepage: https://github.com/docwire/docwire                                                                                     */
/*                                                                                                                                           */
/*  SPDX-License-Identifier: GPL-2.0-only OR LicenseRef-DocWire-Commercial                                                                   */
/*********************************************************************************************************************************************/

#ifndef DOCWIRE_OPENAI_TRANSLATE_TO_H
#define DOCWIRE_OPENAI_TRANSLATE_TO_H

#include "chat.h"
#include "exception.h"

namespace docwire
{
namespace openai
{

class DllExport TranslateTo : public Chat
{
public:
	TranslateTo(const std::string& language, const std::string& api_key, float temperature = 0);
	TranslateTo(const TranslateTo& other);
	virtual ~TranslateTo();

	/**
	* @brief Creates clone of the TranslateTo
	* @return new TranslateTo
	*/
	TranslateTo* clone() const override;
};

} // namespace openai
} // namespace docwire

#endif //DOCWIRE_OPENAI_TRANSLATE_TO_H
