/******************************************************************************
   Copyright 2020 Embedded Office GmbH & Co. KG

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************************/

#ifndef CO_INTEGER8_H_
#define CO_INTEGER8_H_

#ifdef __cplusplus               /* for compatibility with C++ environments  */
extern "C" {
#endif

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "../../core/co_types.h"
#include "../../core/co_err.h"
#include "../../core/co_obj.h"

/******************************************************************************
* PUBLIC DEFINES
******************************************************************************/

#define CO_TUNSIGNED8  ((const CO_OBJ_TYPE *)&COTInt8)
#define CO_TSIGNED8    ((const CO_OBJ_TYPE *)&COTInt8)

/******************************************************************************
* PUBLIC CONSTANTS
******************************************************************************/

/*! \brief OBJECT TYPE SIGNED8 / UNSIGNED8
*
*    This type is a basic type for 8bit values, regardless of signed or
*    unsigned usage.
*/
extern const CO_OBJ_TYPE COTInt8;

#ifdef __cplusplus               /* for compatibility with C++ environments  */
}
#endif

#endif  /* #ifndef CO_INTEGER8_H_ */