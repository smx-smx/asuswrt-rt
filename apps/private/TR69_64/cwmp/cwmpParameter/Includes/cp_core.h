#ifndef CP_CORE_H_
#define CP_CORE_H_

/* 0: Static Node Add, will add parent node; 1: Dynamic Node Add, will add parent .1 .2 .3 ... Node*/
typedef enum _NodeAddType
{
	Static = 0,
	Dynamic,
}NodeAddType;

typedef enum _NodeType
{
	ObjectR = 0,
	ObjectW,
	StrW,
	StrR,
	BooleanW,
	BooleanR,
	UnsignIntW,
	UnsignIntR,
	IntW,
	IntR,
	DateTimeTypeW,
	DateTimeTypeR,
	Base64W,
	Base64R,
}NodeType;

/* 0: Can't set attribute to active type; 1: No limitation; 2: Can't set attribute to active or passive type.  See DeviceNodePtr struct define*/
typedef enum _NodeAttrType
{
	NoActiveSet = 0,
	NoLimite,
	NoPassiveActiveSet,
}NodeAttrType;

typedef int (*DynamicObjectFun)(struct DeviceNode*  parent);

typedef struct _nodefunc
{
	AddObjectFun addObject;
	DeleteObjectFun deleteObject;
	GetValueInfoFun getValueInfo;
	SetValueInfoFun setValueInfo;
	GetAttributeFun getAttribute;
	SetAttributeFun setAttribute;
	DynamicObjectFun dynFunc;
}NodeFunc;

typedef struct _CwmpParameterStruct
{
	char  *name; /* Name of the node*/
	NodeType  type;  /* Type of the parameter*/
	NodeAttrType        attrFlag; /* 0: Can't set attribute to active type; 1: No limitation; 2: Can't set attribute to active or passive type.  See DeviceNodePtr struct define*/
	NodeFunc  *func;
	struct _CwmpParameterStruct *next;
}CwmpParameterStruct, *pCwmpParameterStruct;

typedef struct _CwmpDynamicNodeStruct
{
	char  *name; /* Name of the parent node*/
	//struct CwmpParameterStruct *nodetable; /*Node Table want to registe Dynamic*/
	CwmpParameterStruct *nodetable; /*Node Table want to registe Dynamic*/
	NodeAddType type; /*Static or Dynamic Node add*/
}CwmpDynamicNodeStruct, *pCwmpDynamicNodeStruct;


DeviceNodePtr DeviceNodeAlloc(char *IndexStr, NodeType type, NodeAttrType attrFlag, NodeFunc *nodeFunc);
DeviceNodePtr RegisterNodeFromTable(DeviceNodePtr parent, CwmpParameterStruct *pNodeStruct);
int RegisterNodeDynamic(DeviceNodePtr root, char *parentNodeName, CwmpParameterStruct *pNodeStruct, NodeAddType Flag);

int InitDynamicNode(DeviceNodePtr root, char *FreeNodeName);
int RegisterDynNodeFromIndex( DeviceNodePtr deviceNode, CwmpParameterStruct *pNodeStruct, int NUM );

#endif
