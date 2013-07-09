/*
 * Copyright 2001,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
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
 * $Id: SchemaInfo.hpp 176114 2004-09-29 20:58:10Z knoaman $
 */

#if !defined(SCHEMAINFO_HPP)
#define SCHEMAINFO_HPP


/** When in a <redefine>, type definitions being used (and indeed
  * refs to <group>'s and <attributeGroup>'s) may refer to info
  * items either in the schema being redefined, in the <redefine>,
  * or else in the schema doing the redefining.  Because of this
  * latter we have to be prepared sometimes to look for our type
  * definitions outside the schema stored in fSchemaRootElement.
  * This simple class does this; it's just a linked list that
  * lets us look at the <schema>'s on the queue; note also that this
  * should provide us with a mechanism to handle nested <redefine>'s.
  * It's also a handy way of saving schema info when importing/including.
  */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/util/RefVectorOf.hpp>
#include <xercesc/util/ValueVectorOf.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Forward Declarations
// ---------------------------------------------------------------------------
class ValidationContext;


class VALIDATORS_EXPORT SchemaInfo : public XMemory
{
public:

    enum ListType {
        // Redefine is treated as an include
        IMPORT = 1,
        INCLUDE = 2
    };

    enum {
        C_ComplexType,
        C_SimpleType,
        C_Group,
        C_Attribute,
        C_AttributeGroup,
        C_Element,
        C_Notation,

        C_Count
    };

    // -----------------------------------------------------------------------
    //  Constructor/Destructor
    // -----------------------------------------------------------------------
    SchemaInfo(const unsigned short fElemAttrDefaultQualified,
               const int blockDefault,
               const int finalDefault,
               const int targetNSURI,
               const int scopeCount,
               const unsigned int namespaceScopeLevel,
               XMLCh* const schemaURL,
               const XMLCh* const targetNSURIString,
               const DOMElement* const root,
               MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    ~SchemaInfo();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    XMLCh*                            getCurrentSchemaURL() const;
    const XMLCh* const                getTargetNSURIString() const;
    const DOMElement*                 getRoot() const;
    bool                              getProcessed() const;
    int                               getBlockDefault() const;
    int                               getFinalDefault() const;
    int                               getTargetNSURI() const;
    int                               getScopeCount() const;
    unsigned int                      getNamespaceScopeLevel() const;
    unsigned short                    getElemAttrDefaultQualified() const;
    BaseRefVectorEnumerator<SchemaInfo>   getImportingListEnumerator() const;
    ValueVectorOf<const DOMElement*>* getRecursingAnonTypes() const;
    ValueVectorOf<const XMLCh*>*      getRecursingTypeNames() const;
    ValueVectorOf<DOMNode*>*          getNonXSAttList() const;
    ValidationContext*                getValidationContext() const;

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setProcessed(const bool aValue = true);
    void setScopeCount(const int aValue);
    void setBlockDefault(const int aValue);
    void setFinalDefault(const int aValue);
    void setElemAttrDefaultQualified(const unsigned short aValue);

    // -----------------------------------------------------------------------
    //  Access methods
    // -----------------------------------------------------------------------
    void addSchemaInfo(SchemaInfo* const toAdd, const ListType aListType);
    bool containsInfo(const SchemaInfo* const toCheck, const ListType aListType) const;
    SchemaInfo* getImportInfo(const unsigned int namespaceURI) const;
    DOMElement* getTopLevelComponent(const unsigned short compCategory,
                                     const XMLCh* const compName,
                                     const XMLCh* const name);
    DOMElement* getTopLevelComponent(const unsigned short compCategory,
                                     const XMLCh* const compName,
                                     const XMLCh* const name,
                                     SchemaInfo** enclosingSchema);
    void updateImportingInfo(SchemaInfo* const importingInfo);
    bool circularImportExist(const unsigned int nameSpaceURI);
    bool isFailedRedefine(const DOMElement* const anElem);
    void addFailedRedefine(const DOMElement* const anElem);
    bool isImportingNS(const int namespaceURI);
    void addImportedNS(const int namespaceURI);
    void addRecursingType(const DOMElement* const elem, const XMLCh* const name);

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    SchemaInfo(const SchemaInfo&);
    SchemaInfo& operator=(const SchemaInfo&);

    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void clearTopLevelComponents();

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    bool                              fAdoptInclude;
    bool                              fProcessed;
    unsigned short                    fElemAttrDefaultQualified;
    int                               fBlockDefault;
    int                               fFinalDefault;
    int                               fTargetNSURI;
    int                               fScopeCount;
    unsigned int                      fNamespaceScopeLevel;
    XMLCh*                            fCurrentSchemaURL;
    const XMLCh*                      fTargetNSURIString;
    const DOMElement*                 fSchemaRootElement;
    RefVectorOf<SchemaInfo>*          fIncludeInfoList;
    RefVectorOf<SchemaInfo>*          fImportedInfoList;
    RefVectorOf<SchemaInfo>*          fImportingInfoList;
    ValueVectorOf<const DOMElement*>* fFailedRedefineList;
    ValueVectorOf<int>*               fImportedNSList;
    ValueVectorOf<const DOMElement*>* fRecursingAnonTypes;
    ValueVectorOf<const XMLCh*>*      fRecursingTypeNames;
    ValueVectorOf<DOMElement*>*       fTopLevelComponents[C_Count];
    ValueVectorOf<DOMNode*>*          fNonXSAttList;
    ValidationContext*                fValidationContext;
    MemoryManager*                    fMemoryManager;
};

// ---------------------------------------------------------------------------
//  SchemaInfo: Getter methods
// ---------------------------------------------------------------------------
inline unsigned short SchemaInfo::getElemAttrDefaultQualified() const {

    return fElemAttrDefaultQualified;
}

inline bool SchemaInfo::getProcessed() const {

    return fProcessed;
}

inline int SchemaInfo::getBlockDefault() const {

    return fBlockDefault;
}

inline int SchemaInfo::getFinalDefault() const {

    return fFinalDefault;
}

inline unsigned int SchemaInfo::getNamespaceScopeLevel() const {
    return fNamespaceScopeLevel;
}

inline XMLCh* SchemaInfo::getCurrentSchemaURL() const {

    return fCurrentSchemaURL;
}

inline const XMLCh* const SchemaInfo::getTargetNSURIString() const {

    return fTargetNSURIString;
}

inline const DOMElement* SchemaInfo::getRoot() const {

    return fSchemaRootElement;
}

inline int SchemaInfo::getTargetNSURI() const {

    return fTargetNSURI;
}

inline int SchemaInfo::getScopeCount() const {

    return fScopeCount;
}

inline BaseRefVectorEnumerator<SchemaInfo>
SchemaInfo::getImportingListEnumerator() const {

    return BaseRefVectorEnumerator<SchemaInfo>(fImportingInfoList);
}

inline ValueVectorOf<const DOMElement*>*
SchemaInfo::getRecursingAnonTypes() const {

    return fRecursingAnonTypes;
}


inline ValueVectorOf<const XMLCh*>*
SchemaInfo::getRecursingTypeNames() const {

    return fRecursingTypeNames;
}

inline ValueVectorOf<DOMNode*>* SchemaInfo::getNonXSAttList() const
{
    return fNonXSAttList;
}

// ---------------------------------------------------------------------------
//  Setter methods
// ---------------------------------------------------------------------------
inline void SchemaInfo::setScopeCount(const int aValue) {

    fScopeCount = aValue;
}

inline void SchemaInfo::setBlockDefault(const int aValue) {

    fBlockDefault = aValue;
}

inline void SchemaInfo::setFinalDefault(const int aValue) {

    fFinalDefault = aValue;
}

inline void SchemaInfo::setElemAttrDefaultQualified(const unsigned short aValue) {

    fElemAttrDefaultQualified = aValue;
}

inline void SchemaInfo::setProcessed(const bool aValue) {

    fProcessed = aValue;

/*    if (fProcessed && fIncludeInfoList) {

        unsigned int includeListLen = fIncludeInfoList->size());
        for (unsigned int i = 0; i < includeListLen; i++) {
            fIncludeInfoList->elementAt(i)->clearTopLevelComponents();
        }
    }*/
}

// ---------------------------------------------------------------------------
//  SchemaInfo: Access methods
// ---------------------------------------------------------------------------
inline void SchemaInfo::addImportedNS(const int namespaceURI) {

    if (!fImportedNSList) {
        fImportedNSList = new (fMemoryManager) ValueVectorOf<int>(4, fMemoryManager);
    }

    if (!fImportedNSList->containsElement(namespaceURI))
        fImportedNSList->addElement(namespaceURI);
}

inline void SchemaInfo::addSchemaInfo(SchemaInfo* const toAdd,
                                      const ListType aListType) {

    if (aListType == IMPORT) {

        if (!fImportedInfoList)
            fImportedInfoList = new (fMemoryManager) RefVectorOf<SchemaInfo>(4, false, fMemoryManager);

        if (!fImportedInfoList->containsElement(toAdd)) {

            fImportedInfoList->addElement(toAdd);
            addImportedNS(toAdd->getTargetNSURI());
            toAdd->updateImportingInfo(this);
        }
    }
    else {

        if (!fIncludeInfoList) {

            fIncludeInfoList = new (fMemoryManager) RefVectorOf<SchemaInfo>(8, false, fMemoryManager);
            fAdoptInclude = true;
        }

        if (!fIncludeInfoList->containsElement(toAdd)) {

		    fIncludeInfoList->addElement(toAdd);
            toAdd->fIncludeInfoList = fIncludeInfoList;
        }
    }
}

inline SchemaInfo* SchemaInfo::getImportInfo(const unsigned int namespaceURI) const {

    unsigned int importSize = (fImportedInfoList) ? fImportedInfoList->size() : 0;
    SchemaInfo* currInfo = 0;

    for (unsigned int i=0; i < importSize; i++) {

        currInfo = fImportedInfoList->elementAt(i);

        if (currInfo->getTargetNSURI() == (int) namespaceURI)
            break;
    }

    return currInfo;
}

inline ValidationContext* SchemaInfo::getValidationContext() const {

    return fValidationContext;
}

inline bool SchemaInfo::containsInfo(const SchemaInfo* const toCheck,
                                     const ListType aListType) const {

    if ((aListType == INCLUDE) && fIncludeInfoList) {
        return fIncludeInfoList->containsElement(toCheck);
    }
    else if ((aListType == IMPORT) && fImportedInfoList) {
        return fImportedInfoList->containsElement(toCheck);
    }

    return false;
}

inline bool SchemaInfo::circularImportExist(const unsigned int namespaceURI) {

    unsigned int importSize = fImportingInfoList->size();

    for (unsigned int i=0; i < importSize; i++) {
        if (fImportingInfoList->elementAt(i)->getTargetNSURI() == (int) namespaceURI) {
            return true;
        }
    }

    return false;
}

inline bool SchemaInfo::isFailedRedefine(const DOMElement* const anElem) {

    if (fFailedRedefineList)
        return (fFailedRedefineList->containsElement(anElem));

    return false;
}

inline void SchemaInfo::addFailedRedefine(const DOMElement* const anElem) {

    if (!fFailedRedefineList) {
        fFailedRedefineList = new (fMemoryManager) ValueVectorOf<const DOMElement*>(4, fMemoryManager);
    }

    fFailedRedefineList->addElement(anElem);
}

inline bool SchemaInfo::isImportingNS(const int namespaceURI) {

    if (!fImportedNSList)
        return false;

    return (fImportedNSList->containsElement(namespaceURI));
}

inline void SchemaInfo::addRecursingType(const DOMElement* const elem,
                                         const XMLCh* const name) {

    if (!fRecursingAnonTypes) {
        fRecursingAnonTypes = new (fMemoryManager) ValueVectorOf<const DOMElement*>(8, fMemoryManager);
        fRecursingTypeNames = new (fMemoryManager) ValueVectorOf<const XMLCh*>(8, fMemoryManager);
    }

    fRecursingAnonTypes->addElement(elem);
    fRecursingTypeNames->addElement(name);
}

inline void SchemaInfo::clearTopLevelComponents() {

    for (unsigned int i = 0; i < C_Count; i++) {

        delete fTopLevelComponents[i];
        fTopLevelComponents[i] = 0;
    }
}

XERCES_CPP_NAMESPACE_END

#endif

/**
  * End of file SchemaInfo.hpp
  */

