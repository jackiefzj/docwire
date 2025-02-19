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

#ifndef DOCWIRE_METADATA_WRITER_H
#define DOCWIRE_METADATA_WRITER_H

#include <iostream>
#include <fstream>

#include "parser.h"
#include "writer.h"
#include "defines.h"

namespace docwire
{
/**
 * @brief The MetaDataWriter class writes the meta data of the document as plain text to an output stream.
 * @code
 * MetaDataWriter metaDataWriter;
 * parser.onNewNode([&metaDataWriter](Info &info){
 *      metaDataWriter.write_to(info, std::cout);
 *      }).parse();
 * @endcode
 */
class DllExport MetaDataWriter : public Writer
{
public:
  /**
   * @brief Writes meta data of the document to an output stream.
   * @param info data from callback
   * @param stream output stream
   */
  void write_to(const Info &info, std::ostream &stream) override;
  /**
   * @brief creates a new instance of MetaDataWriter
   */
  virtual Writer* clone() const override;
};
} // namespace docwire

#endif //DOCWIRE_METADATA_WRITER_H
