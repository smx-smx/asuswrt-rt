
#include <stdlib.h>
#include <string.h>
//#include <malloc.h>
#include <assert.h>
//#include "global.h"

  /** Retrieves the parent {\bf Node} for a {\bf Node}.
   *
   *  @return [Node*] A pointer to the parent {\bf Node} or {\tt NULL} if the 
   *          {\bf Node} has no parent.
   */
#if CWMP_REDUCE_CODE
DeviceNodePtr cwmpNodeGetParentNode( IN DeviceNodePtr nodeptr );
#endif

  /** Retrieves the first child {\bf Node} of a {\bf Node}.
   *
   *  @return [Node*] A pointer to the first child {\bf Node} or {\tt NULL} 
   *                  if the {\bf Node} does not have any children.
   */

DeviceNodePtr cwmpNodeGetFirstChild( IN DeviceNodePtr nodeptr );

  /** Retrieves the last child {\bf Node} of a {\bf Node}.
   *
   *  @return [Node*] A pointer to the last child {\bf Node} or {\tt NULL} if 
   *                  the {\bf Node} does not have any children.
   */
#if CWMP_REDUCE_CODE
DeviceNodePtr cwmpNodeGetLastChild( IN DeviceNodePtr nodeptr );
#endif

  /** Retrieves the sibling {\bf Node} immediately preceding this {\bf Node}.
   *
   *  @return [Node*] A pointer to the previous sibling {\bf Node} or 
   *                  {\tt NULL} if no such {\bf Node} exists.
   */

DeviceNodePtr cwmpNodeGetPreviousSibling( IN DeviceNodePtr nodeptr );

  /** Retrieves the sibling {\bf Node} immediately following this {\bf Node}.
   *
   *  @return [Node*] A pointer to the next sibling {\bf Node} or {\tt NULL} 
   *                  if no such {\bf Node} exists.
   */

DeviceNodePtr cwmpNodeGetNextSibling( IN DeviceNodePtr nodeptr );
#if CWMP_REDUCE_CODE
int cwmpNodeInsertBefore( IN DeviceNodePtr nodeptr,IN DeviceNodePtr newChild,IN DeviceNodePtr refChild );
#endif

#if CWMP_REDUCE_CODE
int cwmpNodeReplaceChild( IN DeviceNodePtr nodeptr,IN DeviceNodePtr newChild,IN DeviceNodePtr oldChild, OUT DeviceNodePtr* returnNode );
#endif

  /** Removes a child from the list of children of a {\bf Node}.
   *  {\bf returnNode} will contain the {\bf oldChild} {\bf Node}, 
   *  appropriately removed from the tree (i.e. it will no longer have an 
   *  owner document).
   *
   *  @return [int] An integer representing one of the following:
   *    \begin{itemize}
   *      \item {\tt IXML_SUCCESS}: The operation completed successfully.
   *      \item {\tt IXML_INVALID_PARAMETER}: Either {\bf nodeptr} or 
   *            {\bf oldChild} is {\tt NULL}.
   *      \item {\tt IXML_NO_MODIFICATION_ALLOWED_ERR}: {\bf nodeptr} or its 
   *            parent is read-only.
   *      \item {\tt IXML_NOT_FOUND_ERR}: {\bf oldChild} is not among the 
   *            children of {\bf nodeptr}.
   *    \end{itemize}
   */

int cwmpNodeRemoveChild( IN DeviceNodePtr nodeptr,IN DeviceNodePtr oldChild,OUT DeviceNodePtr* returnNode );

  /** Appends a child {\bf Node} to the list of children of a {\bf Node}.  If
   *  {\bf newChild} is already in the tree, it is removed first.
   *
   *  @return [int] An integer representing one of the following:
   *    \begin{itemize}
   *      \item {\tt IXML_SUCCESS}: The operation completed successfully.
   *      \item {\tt IXML_INVALID_PARAMETER}: Either {\bf nodeptr} or 
   *            {\bf newChild} is {\tt NULL}.
   *      \item {\tt IXML_HIERARCHY_REQUEST_ERR}: {\bf newChild} is of a type 
   *            that cannot be added as a child of {\bf nodeptr} or 
   *            {\bf newChild} is an ancestor of {\bf nodeptr}.
   *      \item {\tt IXML_WRONG_DOCUMENT_ERR}: {\bf newChild} was created from 
   *            a different document than {\bf nodeptr}.
   *      \item {\tt IXML_NO_MODIFICATION_ALLOWED_ERR}: {\bf nodeptr} is a 
   *            read-only {\bf Node}.
   */

int cwmpNodeAppendChild( IN DeviceNodePtr nodeptr,IN DeviceNodePtr newChild );
void cwmpNodeFree( IN DeviceNodePtr  nodeptr );
