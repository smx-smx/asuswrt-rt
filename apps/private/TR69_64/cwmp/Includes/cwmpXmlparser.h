
 
#ifndef _IXMLPARSER_H
#define _IXMLPARSER_H

#include "cwmpXml.h"
#include "cwmpXmlmembuf.h"
#include "stdio.h"

// Parser definitions
#define QUOT        "&quot;"
#define LT          "&lt;"
#define GT          "&gt;"
#define APOS        "&apos;"
#define AMP         "&amp;"
#define ESC_HEX     "&#x"
#define ESC_DEC     "&#"

typedef struct _IXML_NamespaceURI 
{
    char                        *nsURI;
    char                        *prefix;
    struct _IXML_NamespaceURI   *nextNsURI;
} IXML_NamespaceURI;


typedef struct _IXML_ElementStack
{
    char                    *element;
    char                    *prefix;
    char                    *namespaceUri;
    IXML_NamespaceURI            *pNsURI;
    struct _IXML_ElementStack    *nextElement;
} IXML_ElementStack;


typedef enum
{
    eELEMENT,
    eATTRIBUTE,
    eCONTENT
} PARSER_STATE;

typedef struct _Parser
{
    char            *dataBuffer;	//data buffer
    char            *curPtr;		//ptr to the token parsed 
    char            *savePtr;		//Saves for backup
    ixml_membuf     lastElem;
    ixml_membuf     tokenBuf;    

    IXML_Node           *pNeedPrefixNode;
    IXML_ElementStack   *pCurElement;
    IXML_Node           *currentNodePtr;
    PARSER_STATE        state;

    BOOL                bHasTopLevel;

} Parser;

typedef struct IXMLNodeStack{
	IXML_Node* xmlNode;
	struct IXMLNodeStack* next;
} IXMLNodeStack, *IXMLNodeStackPtr;

int     Parser_LoadDocument( IXML_Document **retDoc, char * xmlFile, BOOL file);
#if CWMP_REDUCE_CODE
BOOL    Parser_isValidXmlName( DOMString name);
#endif

int     Parser_setNodePrefixAndLocalName(IXML_Node *newIXML_NodeIXML_Attr);
void    Parser_freeNodeContent( IXML_Node *IXML_Nodeptr);

void    ixmlAttr_free(IXML_Attr *attrNode);
void    ixmlAttr_init(IXML_Attr *attrNode);

int     ixmlElement_setTagName(IXML_Element *element, char *tagName);

void    ixmlNamedNodeMap_init(IXML_NamedNodeMap *nnMap);
#if CWMP_REDUCE_CODE
int     ixmlNamedNodeMap_addToNamedNodeMap(IXML_NamedNodeMap **nnMap, IXML_Node *add);
#endif

void    ixmlNode_init(IXML_Node *IXML_Nodeptr);
BOOL    ixmlNode_compare(IXML_Node *srcIXML_Node, IXML_Node *destIXML_Node);

#if CWMP_REDUCE_CODE
void    ixmlNode_getElementsByTagName( IXML_Node *n, char *tagname, IXML_NodeList **list);
#endif

#if CWMP_REDUCE_CODE
void    ixmlNode_getElementsByTagNameNS( IXML_Node *IXML_Node, char *namespaceURI,
                char *localName, IXML_NodeList **list);
#endif

int     ixmlNode_setNodeProperties(IXML_Node* node, IXML_Node *src);
#if CWMP_REDUCE_CODE
int     ixmlNode_setNodeName( IXML_Node* node, DOMString qualifiedName);
#endif

void    ixmlNodeList_init(IXML_NodeList *nList);
int     ixmlNodeList_addToNodeList(IXML_NodeList **nList, IXML_Node *add);


extern char*	strdup(const char *);//added by hqfeng, 2006.02.27
int		ixmlNode_setPrefix( IN IXML_Node * nodeptr,IN char *prefix );
int		ixmlNode_setLocalName( IN IXML_Node * nodeptr,IN char *localName );
int		ixmlNode_setNamespaceURI( IN IXML_Node * nodeptr,IN char *namespaceURI );

#endif  // _IXMLPARSER_H

