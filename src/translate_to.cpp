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

#include "translate_to.h"

#include "log.h"

namespace docwire
{
namespace openai
{

TranslateTo::TranslateTo(const std::string& language, const std::string& api_key, float temperature)
	: Chat("Your task is to translate every message to " + language + " language.", api_key, temperature)
{
	docwire_log_func_with_args(language);
}

TranslateTo::TranslateTo(const TranslateTo& other)
	: Chat(other)
{
	docwire_log_func();
}

TranslateTo::~TranslateTo()
{
}

TranslateTo* TranslateTo::clone() const
{
	return new TranslateTo(*this);
}

} // namespace openai
} // namespace docwire
