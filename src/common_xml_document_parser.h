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

#ifndef DOCWIRE_COMMON_XML_PARSER_H
#define DOCWIRE_COMMON_XML_PARSER_H

#include "formatting_style.h"
#include "parser.h"
#include <string>
#include <vector>
#include <map>

namespace docwire
{
	class ZipReader;
	class Metadata;
	class XmlStream;

/**
	This class is inherited by ODFOOXMLParser and ODFXMLParser. It contains some common
	functions for both parsers.
	How inheritance works:
	Child classes (ODFOOXMLParser and ODFXMLParser for now) may want to add or change handlers for some xml tags
	(using registerODFOOXMLCommandHandler). Besides they have to implement two methods: plainText and metaData.
**/
class CommonXMLDocumentParser
{
	private:
		struct Implementation;
		Implementation* impl;
		class CommandHandlersSet;

	//public interface for derived classes (and its components)
	public:
		enum ODFOOXMLListStyle
		{
			number,
			bullet
		};

		struct Comment
		{
			std::string m_author;
			std::string m_time;
			std::string m_text;
			Comment() {}
			Comment(const std::string& author, const std::string& time, const std::string& text)
				: m_author(author), m_time(time), m_text(text) {}
		};

		struct SharedString
		{
			std::string m_text;
		};

		typedef std::vector<ODFOOXMLListStyle> ListStyleVector;

  typedef std::function<void(CommonXMLDocumentParser& parser, XmlStream& xml_stream, XmlParseMode mode,
                                 const FormattingStyle& options, const ZipReader* zipfile, std::string& text,
                                 bool& children_processed, std::string& level_suffix, bool first_on_level)> CommandHandler;

    void addCallback(const NewNodeCallback &callback);

		/**
			Each xml tag can have associated handler, which is a single function of CommandHandler type.
			CommonXMLDocumentParser has already a set of functions for some basic tags.
			ODFOOXMLParser and ODFXMLParser can add new/overwrite existing handlers in order to change/extend default behaviour of
			parseXmlData/extractText.
		**/
		void registerODFOOXMLCommandHandler(const std::string& xml_tag, CommandHandler handler);

		///it is executed for each undefined tag (xml tag without associated handler). Can be overwritten
		virtual void onUnregisteredCommand(XmlStream& xml_stream, XmlParseMode mode,
										   const FormattingStyle& options, const ZipReader* zipfile, std::string& text,
										   bool& children_processed, std::string& level_suffix, bool first_on_level);

		///parses xml data for given xml stream. It executes commands for each xml tag
		std::string parseXmlData(XmlStream& xml_stream, XmlParseMode mode, const FormattingStyle& options, const ZipReader* zipfile) const;

		///extracts text from xml data. It uses parseXmlData internally. Throws Exception on fail
		void extractText(const std::string& xml_contents, XmlParseMode mode, const FormattingStyle& options, const ZipReader* zipfile, std::string& text) const;

		///usefull since two parsers use this. Throws Exception on fail
		void parseODFMetadata(const std::string &xml_content, Metadata &metadata) const;

		///this is helpful function to format comment
		const std::string formatComment(const std::string& author, const std::string& time, const std::string& text);

		///Returns information "on how many list objects" we are. Returns 0 if we are not parsing any list actually. Should only be used inside command handlers
		size_t& getListDepth() const;

		///gets list styles for reading and writing
		std::map<std::string, ListStyleVector>& getListStyles() const;

		///gets comments for reading and writing
		std::map<int, Comment>& getComments() const;

		///gets vector of shared strings for reading and writing
		std::vector<SharedString>& getSharedStrings() const;

		///checks if writing to the text is disabled (only inside onUnregisteredCommand!)
		bool disabledText() const;

		///gets options which has been set for XmlStream object. (xmlParserOption from libxml2)
		int getXmlOptions() const;

		bool manageXmlParser() const;

		///disables modifying text data inside method onUnregisteredCommand
		void disableText(bool disable) const;

		///sets options for XmlStream objects. (xmlParserOption from libxml2)
		void setXmlOptions(int options) const;

		///Cleans up associated data. Call this method in constructor of derived class in case of bad_alloc.
		void cleanUp();

		void activeEmittingSignals(bool flag) const;

		void trySendTag(const std::string& tag_name, const std::string& text = "", const std::map<std::string, std::any> &attr = {}) const;

	//public interface
	public:
		CommonXMLDocumentParser();
		virtual ~CommonXMLDocumentParser();
		void setManageXmlParser(bool manage);
		virtual std::string plainText(XmlParseMode mode, FormattingStyle& options) const = 0;
		virtual Metadata metaData() const = 0;
};

} // namespace docwire

#endif
