/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id$
 *
 */

/*
 * This test verifies the fix of the CVE-2018-1311 issue (see
 * https://issues.apache.org/jira/projects/XERCESC/issues/XERCESC-2188 for all
 * the gory details).
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include    <xercesc/util/XMLUni.hpp>
#include    <xercesc/util/Janitor.hpp>
#include    <xercesc/util/XMLString.hpp>
#include    <xercesc/util/PlatformUtils.hpp>
#include    <xercesc/sax/InputSource.hpp>
#include    <xercesc/framework/XMLEntityDecl.hpp>
#include    <xercesc/framework/MemoryManager.hpp>
#include    <xercesc/validators/DTD/DTDValidator.hpp>
#include    <xercesc/internal/ReaderMgr.hpp>

#if defined(XERCES_NEW_IOSTREAMS)
#include	<iostream>
#else
#include	<iostream.h>
#endif

#include <set>
#include <memory>  // unique_ptr
#include <string>
#include <cstddef> // size_t

#undef NDEBUG
#include <cassert>

XERCES_CPP_NAMESPACE_USE

using namespace std;

inline XERCES_STD_QUALIFIER ostream& operator<<(XERCES_STD_QUALIFIER ostream& os, const XMLCh* xmlStr)
{
	char* transcoded = XMLString::transcode(xmlStr);
  os << transcoded;
  XMLString::release(&transcoded);
  return os;
}

struct XMLStringDeleter
{
  void operator() (XMLCh* p) const {if (p != nullptr) XMLString::release(&p);}
};

using XMLChPtr = unique_ptr<XMLCh, XMLStringDeleter>;

// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int
main(int argc, char** argv)
{
  // Init the XML platform
  try
  {
    XMLPlatformUtils::Initialize();
  }
  catch(const XMLException& toCatch)
  {
    XERCES_STD_QUALIFIER cout << "Error during platform init! Message:\n"
                              << toCatch.getMessage() << XERCES_STD_QUALIFIER endl;
    return 1;
  }

  MemoryManager* const fMemoryManager = XMLPlatformUtils::fgMemoryManager;

  {
    set<string> entity_names;
    ReaderMgr fReaderMgr;

    for (int i (1); i != argc; ++i)
    {
      string s (argv[i]);
      size_t n (s.find ('='));

      assert (n != string::npos);

      string name (s, 0, n);

      XMLChPtr sysId (XMLString::transcode(string (s, n + 1).c_str ()));

      {
        XMLChPtr gDTDStr (XMLString::transcode(name.c_str ()));
        bool new_entity (entity_names.emplace (move (name)).second);

        InputSource* srcUsed=0;
        Janitor<InputSource> janSrc(srcUsed);

        XMLReader* reader =
          fReaderMgr.createReader
          (
            sysId.get ()
            , XMLUni::fgEmptyString /* pubId */
            , false /* xmlDecl */
            , XMLReader::RefFrom_NonLiteral
            , XMLReader::Type_General
            , XMLReader::Source_External
            , srcUsed
          );

        assert (reader != nullptr);

        janSrc.reset(srcUsed);

        DTDEntityDecl* declDTD =
          new (fMemoryManager) DTDEntityDecl(gDTDStr.get (), false, fMemoryManager);

        assert (fReaderMgr.pushReaderAdoptEntity(reader, declDTD) == new_entity);
      }

      {
        InputSource* srcUsed=0;
        Janitor<InputSource> janSrc(srcUsed);

        XMLReader* reader =
          fReaderMgr.createReader
          (
            sysId.get ()
            , XMLUni::fgEmptyString /* pubId */
            , false /* xmlDecl */
            , XMLReader::RefFrom_NonLiteral
            , XMLReader::Type_General
            , XMLReader::Source_External
            , srcUsed
          );

        assert (reader != nullptr);

        janSrc.reset(srcUsed);

        assert (fReaderMgr.pushReader(reader, nullptr));
      }
    }
  }

  // And call the termination method
  XMLPlatformUtils::Terminate();

  return 0;
}
