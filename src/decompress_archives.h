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

#ifndef DOCWIRE_DECOMPRESS_ARCHIVES_H
#define DOCWIRE_DECOMPRESS_ARCHIVES_H

#include "chain_element.h"

namespace docwire
{

class DllExport DecompressArchives : public ChainElement
{
public:
	DecompressArchives();
	DecompressArchives(const DecompressArchives &other);
	virtual ~DecompressArchives();

	/**
	* @brief Executes transform operation for given node data.
	* @see docwire::Info
	* @param info
	*/
	void process(Info &info) const;

	bool is_leaf() const override
	{
		return false;
	}

	/**
	* @brief Creates clone of the DecompressArchives
	* @return new DecompressArchives
	*/
	DecompressArchives* clone() const override;
};

} // namespace docwire

#endif //DOCWIRE_DECOMPRESS_ARCHIVES_H
