/* SPDX-License-Identifier: LGPL-3.0-or-later */

#pragma once

#include "CoreMinimal.h"
#include "Module/GameWorldModule.h"
#include "FPVActoDronesGameWorldModule.generated.h"


UCLASS(Abstract)
class FPVACRODRONES_API UFPVActoDronesGameWorldModule : public UGameWorldModule
{
	GENERATED_BODY()

public:
	virtual void DispatchLifecycleEvent(ELifecyclePhase Phase) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnControllerAnalog(FName KeyName, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId, float AnalogValue);
};
