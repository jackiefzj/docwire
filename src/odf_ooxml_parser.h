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

#ifndef DOCWIRE_ODFOOXML_PARSER_H
#define DOCWIRE_ODFOOXML_PARSER_H

#include "common_xml_document_parser.h"
#include "parser.h"
#include "parser_builder.h"

namespace docwire
{

class ODFOOXMLParser : public Parser,
                       public CommonXMLDocumentParser
{
  private:
    struct ExtendedImplementation;
    ExtendedImplementation* extended_impl;
    class CommandHandlersSet;
    int lastOOXMLRowNum();
    void setLastOOXMLRowNum(int r);
    int lastOOXMLColNum();
    void setLastOOXMLColNum(int c);
  void onOOXMLBreak(CommonXMLDocumentParser& parser, XmlStream& xml_stream, XmlParseMode mode,
                     const FormattingStyle& options, const ZipReader* zipfile, std::string& text,
                     bool& children_processed, std::string& level_suffix, bool first_on_level) const;

	public:

    void parse() const override;
    Parser& addOnNewNodeCallback(NewNodeCallback callback) override;
    static std::vector <std::string> getExtensions() {return {"odt", "ods", "odp", "odg", "docx", "xlsx", "pptx", "ppsx"};}
    Parser& withParameters(const ParserParameters &parameters) override;

    ODFOOXMLParser(const std::string &file_name, const std::shared_ptr<ParserManager> &inParserManager = nullptr);
    ODFOOXMLParser(const char* buffer, size_t size, const std::shared_ptr<ParserManager> &inParserManager = nullptr);
    ~ODFOOXMLParser();
    bool isODFOOXML();
    std::string plainText(XmlParseMode mode, FormattingStyle& options) const;
    Metadata metaData() const;
};

} // namespace docwire

#endif
