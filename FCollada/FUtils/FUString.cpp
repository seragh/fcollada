// SPDX-License-Identifier: MIT

#include "FUtils/FUtils.h"
#include "FUString.h"

template class fm::stringT<char>;
#ifdef UNICODE
template class fm::stringT<fchar>;
#endif // UNICODE
