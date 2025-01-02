/* SPDX-License-Identifier: LGPL-3.0-or-later */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Equipment/FGEquipment.h"
#include "FPVAcroDronePilotAnimBlueprint.generated.h"

UENUM(BlueprintType)
enum class EVPVADAnimState : uint8 {
	VANILLA = 0         UMETA(DisplayName = "Vanilla"),
	IDLE_ONEHANDED      UMETA(DisplayName = "Idle One-Handed"),
	IDLE_BOTHHANDED     UMETA(DisplayName = "Idle Both-Handed"),
	IDLE_HOLDING_DRONE  UMETA(DisplayName = "Idle Holding a Drone"),
	PILOTING            UMETA(DisplayName = "Piloting"),
};

UCLASS()
class FPVACRODRONES_API UFPVAcroDronePilotAnimBlueprint : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPoseSnapshot vanillaPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EVPVADAnimState animState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool googlesEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf< AFGEquipment > armsEquipmentClass;

};
