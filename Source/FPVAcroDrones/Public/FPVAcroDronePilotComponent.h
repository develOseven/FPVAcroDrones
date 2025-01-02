/* SPDX-License-Identifier: LGPL-3.0-or-later */

#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPVAcroDronePilotAnimBlueprint.h"
#include "FGCharacterPlayer.h"
#include "FPVAcroDronePilotComponent.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPVACRODRONES_API UFPVAcroDronePilotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFPVAcroDronePilotComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Custom Animations")
	const TSubclassOf<UFPVAcroDronePilotAnimBlueprint> CustomAnimClass1P;
	UPROPERTY(EditDefaultsOnly, Category = "Custom Animations")
	const TSubclassOf<UFPVAcroDronePilotAnimBlueprint> CustomAnimClass3P;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintGetter)
	UFPVAcroDronePilotAnimBlueprint* GetCustomAnimInstance1P();

	UFUNCTION(BlueprintGetter)
	USkeletalMeshComponent* GetCustomSkeletalMesh1P();

	UFUNCTION(BlueprintGetter)
	UFPVAcroDronePilotAnimBlueprint* GetCustomAnimInstance3P();

	UFUNCTION(BlueprintGetter)
	USkeletalMeshComponent* GetCustomSkeletalMesh3P();

private:
	UPROPERTY(BlueprintGetter = GetCustomAnimInstance1P)
	UFPVAcroDronePilotAnimBlueprint* customAnimInstance1P;

	UPROPERTY(BlueprintGetter = GetCustomSkeletalMesh1P)
	USkeletalMeshComponent* customSkeletalMesh1P;

	UPROPERTY(BlueprintGetter = GetCustomAnimInstance3P)
	UFPVAcroDronePilotAnimBlueprint* customAnimInstance3P;

	UPROPERTY(BlueprintGetter = GetCustomSkeletalMesh3P)
	USkeletalMeshComponent* customSkeletalMesh3P;

	AFGCharacterPlayer* GetPlayer();
	USkeletalMeshComponent* GetMesh1P();
	USkeletalMeshComponent* GetMesh3P();


	void SetupCustomAnimationComponent1P();
	void SetupCustomAnimationComponent3P();
	void DestroyCustomAnimationComponent1P();
	void DestroyCustomAnimationComponent3P();
	void ChangeCustomAnimationState1P(EVPVADAnimState newState);
	void ChangeCustomAnimationState3P(EVPVADAnimState newState);
	void CustomAnimationTick1P(float deltaTime);
	void CustomAnimationTick3P(float deltaTime);
	void CaptureVanillaPose1P();
	void CaptureVanillaPose3P();

	std::function<void()> unhookSetCustomPrimitiveData1P;
	std::function<void()> unhookSetCustomPrimitiveData3P;
};
