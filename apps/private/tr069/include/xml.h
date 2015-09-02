/*=======================================================================

       Copyright(c) 2009, Works Systems, Inc. All rights reserved.

       This software is supplied under the terms of a license agreement
       with Works Systems, Inc, and may not be copied nor disclosed except
       in accordance with the terms of that agreement.

  =======================================================================*/
/*!
 * \file xml.h
 *
 */
#ifndef __HEADER_XML__
#define __HEADER_XML__

enum {
    XML_INVALID,
    XML_OK,
    XML_END,
    SOAP_INVALID,
    SOAP_FAILED,
    SOAP_OK
};

enum {
    OPEN_TAG,
    CLOSE_TAG
};

struct xml {
    char *name;
    char *value;
    unsigned int attr_count: 5;
    unsigned int self_closed: 1;
    unsigned int type: 2;
    struct {
        char *attr_name;
        char *attr_value;
    } attributes[12];
};

char *xml_xmlstr2str( const char *src, char *dest );
char *xml_str2xmlstr( const char *str );
int xml_next_tag( char **, struct xml * );
#endif
