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

#include "boost/algorithm/string/predicate.hpp"
#include <memory>
#include "html_writer.h"
#include <numeric>
#include "parser.h"
namespace docwire
{

struct HtmlWriter::Implementation
{
  Implementation(HtmlWriter::RestoreOriginalAttributes restore_original_attributes)
    : m_restore_original_attributes(restore_original_attributes)
  {
  }

  HtmlWriter::RestoreOriginalAttributes m_restore_original_attributes;
  bool m_header_is_open { false };

  std::shared_ptr<TextElement>
  write_open_header()
  {
    std::string header = {"<!DOCTYPE html>\n"
           "<html>\n"
           "<head>\n"
           "<meta charset=\"utf-8\">\n"
           "<title>DocWire</title>\n"};
    m_header_is_open = true;
    return std::make_shared<TextElement>(header);
  }

  std::shared_ptr<TextElement> write_close_header_open_body()
  {
    m_header_is_open = false;
    return std::make_shared<TextElement>("</head>\n<body>\n");
  }

  std::shared_ptr<TextElement>
  write_footer()
  {
    std::string footer = {"</body>\n"
           "</html>\n"};
    return std::make_shared<TextElement>(footer);
  }

  std::string encoded(const std::string& value)
  {
    std::string encoded;
    encoded.reserve(value.size());
    for (auto& ch: value)
    {
      switch(ch)
      {
        case '&': encoded += "&amp;"; break;
        case '\"': encoded += "&quot;"; break;
        case '\'': encoded += "&apos;"; break;
        case '<': encoded += "&lt;"; break;
        case '>': encoded += "&gt;"; break;
        default: encoded += ch; break;
      }
    }
    return encoded;
  }

  using HtmlAttrs = std::map<std::string, std::string>;

  HtmlAttrs restored_original_attributes(const Info& info)
  {
    HtmlAttrs attrs;
    if (m_restore_original_attributes == RestoreOriginalAttributes{true})
      for (auto a: info.attributes)
      {
        if (boost::starts_with(a.first, "html:"))
          attrs.insert({a.first.substr(5), std::any_cast<std::string>(a.second)});
      }
    return attrs;
  }

  std::string to_string(const HtmlAttrs& attrs)
  {
    return std::accumulate(attrs.begin(), attrs.end(), std::string{},
      [this](const std::string &attrs_string, const auto &a){return attrs_string + " " + a.first + "=\"" + encoded(a.second) + "\""; });
  }

  std::shared_ptr<TextElement> tag_with_attributes(const std::string& tag_name, const HtmlAttrs& attributes)
  {
    return std::make_shared<TextElement>("<" + tag_name + to_string(attributes) + ">");
  }

  std::shared_ptr<TextElement> write_link(const Info& info)
  {
    HtmlAttrs attrs = restored_original_attributes(info);
    attrs.insert({"href", info.getAttributeValue<std::string>("url").value() });
    return tag_with_attributes("a", attrs);
  }

  std::shared_ptr<TextElement> write_image(const Info& info)
  {
    HtmlAttrs attrs = restored_original_attributes(info);
    attrs.insert(
    {
      { "src", info.getAttributeValue<std::string>("src").value() },
      { "alt", info.getAttributeValue<std::string>("alt").value_or("") }
    });
    return tag_with_attributes("img", attrs);
  }

  std::shared_ptr<TextElement> write_list(const Info& info)
  {
    HtmlAttrs attrs = restored_original_attributes(info);
    std::string orig_style = attrs.count("style") ? attrs["style"] + "; " : "";
    std::string list_type = info.getAttributeValue<std::string>("type").value_or("");
    if (list_type.empty())
    {
      if (info.getAttributeValue<bool>("is_ordered").value_or(false))
        list_type = "decimal";
      else
        list_type = info.getAttributeValue<std::string>("list_style_prefix").value_or("disc");
    }
    if (list_type != "decimal" && list_type != "disc" && list_type != "none")
      list_type = '"' + list_type + '"';
    attrs.insert_or_assign("style", orig_style + "list-style-type: " + list_type);
    return tag_with_attributes("ul", attrs);
  }

  std::shared_ptr<TextElement> write_style(const Info& info)
  {
    return std::make_shared<TextElement>(
      m_restore_original_attributes == RestoreOriginalAttributes{true} ?
        "<style type=\"text/css\">" + info.getAttributeValue<std::string>("css_text").value() + "</style>\n" : "");
  }

  std::map<std::string, std::function<std::shared_ptr<TextElement>(const Info &info)>> writers = {
    {StandardTag::TAG_P, [this](const Info &info) { return tag_with_attributes("p", restored_original_attributes(info)); }},
    {StandardTag::TAG_CLOSE_P, [](const Info &info) { return std::make_shared<TextElement>("</p>"); }},
    {StandardTag::TAG_SECTION, [this](const Info &info) { return tag_with_attributes("div", restored_original_attributes(info)); }},
    {StandardTag::TAG_CLOSE_SECTION, [](const Info &info) { return std::make_shared<TextElement>("</div>"); }},
    {StandardTag::TAG_SPAN, [this](const Info &info) { return tag_with_attributes("span", restored_original_attributes(info)); }},
    {StandardTag::TAG_CLOSE_SPAN, [](const Info &info) { return std::make_shared<TextElement>("</span>"); }},
    {StandardTag::TAG_B, [this](const Info &info) { return tag_with_attributes("b", restored_original_attributes(info)); }},
    {StandardTag::TAG_CLOSE_B, [](const Info &info) { return std::make_shared<TextElement>("</b>"); }},
    {StandardTag::TAG_I, [this](const Info &info) { return tag_with_attributes("i", restored_original_attributes(info)); }},
    {StandardTag::TAG_CLOSE_I, [](const Info &info) { return std::make_shared<TextElement>("</i>"); }},
    {StandardTag::TAG_U, [this](const Info &info) { return tag_with_attributes("u", restored_original_attributes(info)); }},
    {StandardTag::TAG_CLOSE_U, [](const Info &info) { return std::make_shared<TextElement>("</u>"); }},
    {StandardTag::TAG_TABLE, [this](const Info &info) { return tag_with_attributes("table", restored_original_attributes(info)); }},
    {StandardTag::TAG_CLOSE_TABLE, [](const Info &info) { return std::make_shared<TextElement>("</table>"); }},
    {StandardTag::TAG_TR, [this](const Info &info) { return tag_with_attributes("tr", restored_original_attributes(info)); }},
    {StandardTag::TAG_CLOSE_TR, [](const Info &info) { return std::make_shared<TextElement>("</tr>"); }},
    {StandardTag::TAG_TD, [this](const Info &info) { return tag_with_attributes("td", restored_original_attributes(info)); }},
    {StandardTag::TAG_CLOSE_TD, [](const Info &info) { return std::make_shared<TextElement>("</td>"); }},
    {StandardTag::TAG_BR, [](const Info &info) { return std::make_shared<TextElement>("<br />"); }},
    {StandardTag::TAG_TEXT, [](const Info &info) { return std::make_shared<TextElement>(info.plain_text); }},
    {StandardTag::TAG_LINK, [this](const Info &info) { return write_link(info); }},
    {StandardTag::TAG_CLOSE_LINK, [](const Info &info) { return std::make_shared<TextElement>("</a>"); }},
    {StandardTag::TAG_IMAGE, [this](const Info &info) { return write_image(info); }},
    {StandardTag::TAG_LIST, [this](const Info &info) { return write_list(info); }},
    {StandardTag::TAG_CLOSE_LIST, [](const Info &info) { return std::make_shared<TextElement>("</ul>"); }},
    {StandardTag::TAG_LIST_ITEM, [](const Info &info) { return std::make_shared<TextElement>("<li>"); }},
    {StandardTag::TAG_CLOSE_LIST_ITEM, [](const Info &info) { return std::make_shared<TextElement>("</li>"); }},
    {StandardTag::TAG_DOCUMENT, [this](const Info &info) { return write_open_header(); }},
    {StandardTag::TAG_CLOSE_DOCUMENT, [this](const Info &info) { return write_footer(); }},
    {StandardTag::TAG_STYLE, [this](const Info &info) { return write_style(info); }}};

  void write_to(const Info &info, std::ostream &stream)
  {
    if (info.tag_name != StandardTag::TAG_STYLE && m_header_is_open)
      write_close_header_open_body()->write_to(stream);
    auto writer_iterator = writers.find(info.tag_name);
    if (writer_iterator != writers.end())
    {
      writer_iterator->second(info)->write_to(stream);
    }
  }
};

void HtmlWriter::ImplementationDeleter::operator()(Implementation *impl)
{
  delete impl;
}

HtmlWriter::HtmlWriter(RestoreOriginalAttributes restore_original_attributes)
  : impl(new Implementation(restore_original_attributes))
{
}

HtmlWriter::HtmlWriter(const HtmlWriter& html_writer)
  : HtmlWriter(html_writer.impl->m_restore_original_attributes)
{
}

void HtmlWriter::write_to(const Info &info, std::ostream &stream)
{
	impl->write_to(info, stream);
}

Writer*
HtmlWriter::clone() const
{
return new HtmlWriter(*this);
}

} // namespace docwire
