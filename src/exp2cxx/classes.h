#ifndef CLASSES_H
#define CLASSES_H
/*
** Fed-x parser output module for generating C++  class definitions
** December  5, 1989
** release 2 17-Feb-1992
** release 3 March 1993
** release 4 December 1993
** K. C. Morris
**
** Development of Fed-x was funded by the United States Government,
** and is not subject to copyright.

*******************************************************************
The conventions used in this binding follow the proposed specification
for the STEP Standard Data Access Interface as defined in document
N350 ( August 31, 1993 ) of ISO 10303 TC184/SC4/WG7.
*******************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include "express/express.h"
#include "exppp/exppp.h"
#include "express/dict.h"

#define DEBUG                if (0) printf

/* Values for multiple schema support: */
#define NOTKNOWN             1
#define UNPROCESSED          2
#define CANTPROCESS          3
#define CANPROCESS           4
#define PROCESSED            5

#define TD_PREFIX            "t_"
#define ATTR_PREFIX          "a_"
#define ENT_PREFIX           "e_"
#define SCHEMA_PREFIX        "s_"

#define TYPEprefix(t)        (TYPEis_entity (t) ? ENT_PREFIX : TD_PREFIX)

#define SCHEMA_FILE_PREFIX   "Sdai"
#define ENUM_PREFIX          ""

#define move(b)              (b = (b + strlen(b)))
#define TYPEtd_name(t)       TypeDescriptorName (t)

Variable VARis_type_shifter( Variable a );
int isAggregateType( const Type t );
int isAggregate( Variable a );

typedef  struct file_holder  {
    FILE * inc;                /**<  include file */
    FILE * lib;                /**<  library file */
    FILE * incall;             /**<  include file for collecting all include files */
    FILE * initall;            /**<  for registering all entities from all schemas */
    FILE * init;               /**<  contains function to initialize program to use schema's entities */
    FILE * create;             /**<  DAR - added - to create all schema & ent descriptors.  In multiple
                                *    interrelated schemas, must be done before attribute descriptors and
                                *    sub-super links created.
                                */
    FILE * classes;            /**<  DAR - added - a new .h file to contain declarations of all the
                                *    classes, so that all the .h files can refer any of the entity classes.
                                *    Nec. if ent1 of schemaA has attribute ent2 from schemaB.
                                */
    FILE * names;               /**< MAP Nov 2011 - header with namespace for entity and attr descriptors */
    struct {
        struct {
            FILE * impl;
            FILE * hdr;
        } entity, type;
    } unity;
}  File_holder, FILES;

/**  these fields are used so that ENTITY types are processed in order
 *   when appearing in different schemas
 */
typedef struct EntityTag_ * EntityTag;
struct EntityTag_ {
    unsigned int  started : 1;   /**<  marks the beginning of processing */
    unsigned int  complete : 1;  /**<  marks the end of processing */
    Entity        superclass;    /**< the entity being used as the supertype - with multiple inheritance only chose one */
};

/** these fields are used so that SELECT types are processed in order */
typedef struct SelectTag_ * SelectTag;
struct SelectTag_ {
    unsigned int started : 1;   /**<  marks the beginning of processing */
    unsigned int complete : 1;  /**<  marks the end of processing */
};

const char   *  GetTypeDescriptorName( Type t );
char      *     format_for_stringout( char * orig_buf, char * return_buf );
void            format_for_std_stringout( FILE * f, char * orig_buf );
const char   *  CheckWord( const char * );
const char   *  StrToLower( const char * );
const char   *  StrToUpper( const char * );
const char   *  FirstToUpper( const char * );
const char   *  SelectName( const char * );
FILE      *     FILEcreate( const char * );
void            FILEclose( FILE * );
const char   *  ClassName( const char * );
void            FUNCPrint( Function, FILES *, Schema );
void            RULEPrint( Rule, FILES *, Schema );
const char   *  StrToConstant( const char * );
void            MODELPrint( Entity, FILES *, Schema, int );
void            MODELprint_new( Entity entity, FILES * files );
void            MODELPrintConstructorBody( Entity, FILES *, Schema/*, int*/ );
const char   *  PrettyTmpName( const char * oldname );
const char   *  EnumName( const char * oldname );
void            print_file( Express );
void            resolution_success( void );
void            SCHEMAprint( Schema schema, FILES * files, void * complexCol, int suffix );
const char   *  FundamentalType( const Type t, int report_reftypes );
void            numberAttributes( Scope scope );

/*Variable*/
#define VARis_simple_explicit(a)  (!VARis_type_shifter(a))

/*Variable*/
#define VARis_simple_derived(a)  (!VARis_overrider(a))

Variable VARis_overrider( Entity e, Variable a );

/* Added for multiple schema support: */
void            print_schemas_separate( Express, void *, FILES * );
void            getMCPrint( Express, FILE *, FILE * );
int             sameSchema( Scope, Scope );

void            USEREFout( Schema schema, Dictionary refdict, Linked_List reflist, char * type, FILE * file );

#include "classes_attribute.h"
#include "classes_type.h"
#include "classes_entity.h"

#endif

