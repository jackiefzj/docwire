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

#ifndef DOCWIRE_INPUT_H
#define DOCWIRE_INPUT_H

#include <iostream>
#include "chain_element.h"
#include "parsing_chain.h"
#include <filesystem>

namespace docwire
{

class DllExport InputBase
{
public:
  explicit InputBase(std::istream* stream)
  : m_stream(stream)
  {}

  explicit InputBase(const std::string &path)
  : m_path(path),
    m_stream(nullptr)
  {}

  ParsingChain operator|(ChainElement &chainElement) const;

  ParsingChain operator|(ChainElement &&chainElement) const;

  ParsingChain operator|(ParsingChain &parsingChain);

  ParsingChain operator|(ParsingChain &&parsingChain);

  void process(ChainElement& chain_element) const;

private:
  std::istream* m_stream;
  std::string m_path;
};

template<class StreamClass> class StreamInput : public InputBase
{
  static_assert(std::is_base_of<std::istream, StreamClass>::value, "StreamClass must inherit from std::istream");
  public:
    explicit StreamInput(StreamClass&& stream)
      : m_s(std::move(stream)), InputBase(&m_s)
  {}

  private:
    StreamClass m_s;
};

template<class T> class OtherInput : public InputBase
{
  //static_assert(std::is_base_of<std::istream, StreamClass>::value, "StreamClass must inherit from std::istream");
  public:
    explicit OtherInput(const T& v)
      : InputBase(v)
  {}
};


template<class T>
using InputV=typename std::conditional<std::is_base_of_v<std::istream, T>,StreamInput<T>,/*InputBase*/OtherInput<T>>::type;

template<class T>
struct Input : InputV<T>
{
  using InputV<T>::InputV;
};

template<class T> Input(T) -> Input<T>;

}
#endif //DOCWIRE_INPUT_H
