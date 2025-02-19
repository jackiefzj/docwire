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

#include <boost/signals2.hpp>

#include "parser.h"

#include "log.h"

namespace docwire
{

struct Parser::Implementation
{
  Info
  sendTag(const std::string& tag_name, const std::string& text, const std::map<std::string, std::any> &attributes) const
  {
    Info info(tag_name, text, attributes);
    m_on_new_node_signal(info);
    return info;
  }

  void
  onNewNode(NewNodeCallback callback)
  {
    m_on_new_node_signal.connect(callback);
  }

private:
  boost::signals2::signal<void(Info &info)> m_on_new_node_signal;
};

void Parser::ImplementationDeleter::operator()(Parser::Implementation *impl)
{
  delete impl;
}

Parser::Parser(const std::shared_ptr<ParserManager> &inParserManager)
    : m_parser_manager(inParserManager)
{
  base_impl = std::unique_ptr<Implementation, ImplementationDeleter>{new Implementation{}, ImplementationDeleter{}};
}

Info Parser::sendTag(const std::string& tag_name, const std::string& text, const std::map<std::string, std::any> &attributes) const
{
  docwire_log(debug) << "Sending tag \"" << tag_name << "\" with text [" << text << "]";
  return base_impl->sendTag(tag_name, text, attributes);
}

Info Parser::sendTag(const Info &info) const
{
  return base_impl->sendTag(info.tag_name, info.plain_text, info.attributes);
}

Parser& Parser::addOnNewNodeCallback(NewNodeCallback callback)
{
  base_impl->onNewNode(callback);
  return *this;
}

Parser& Parser::withParameters(const ParserParameters &parameters)
{
    m_parameters += parameters;
    return *this;
}

FormattingStyle Parser::getFormattingStyle() const
{
  auto formatting_style = m_parameters.getParameterValue<FormattingStyle>("formatting_style");
  if (formatting_style)
  {
    return *formatting_style;
  }
  return FormattingStyle();
}

} // namespace docwire
