/* SPDX-License-Identifier: LGPL-3.0-or-later */

#pragma once

#include "CoreMinimal.h"
#include "Module/GameWorldModule.h"
#include "FPVAcroDronesGameWorldModule.generated.h"


UCLASS(Abstract)
class FPVACRODRONES_API UFPVAcroDronesGameWorldModule : public UGameWorldModule
{
	GENERATED_BODY()

public:
	virtual void DispatchLifecycleEvent(ELifecyclePhase Phase) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnControllerAnalog(FName KeyName, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId, float AnalogValue);

	UFUNCTION(BlueprintCallable)
	float GetGNearClippingPlane() const;

	UFUNCTION(BlueprintCallable)
	void SetGNearClippingPlane(float NewGNearClippingPlane);
};
