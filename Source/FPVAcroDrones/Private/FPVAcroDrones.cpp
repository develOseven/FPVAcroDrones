/* SPDX-License-Identifier: LGPL-3.0-or-later */

#include "FPVAcroDrones.h"

#define LOCTEXT_NAMESPACE "FFPVAcroDronesModule"

void FFPVAcroDronesModule::StartupModule()
{
	#if !WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("[FPVAcroDrones] Starting module"))
	#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFPVAcroDronesModule, FPVAcroDrones)
