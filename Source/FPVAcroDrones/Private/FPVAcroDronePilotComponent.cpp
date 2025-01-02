/* SPDX-License-Identifier: LGPL-3.0-or-later */

#include "FPVAcroDronePilotComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Patching/NativeHookManager.h"


UFPVAcroDronePilotComponent::UFPVAcroDronePilotComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UFPVAcroDronePilotComponent::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("[FPVAcroDrones] Pilot Component: BeginPlay"))
	SetupCustomAnimationComponent1P();
	SetupCustomAnimationComponent3P();
	Super::BeginPlay();
}

void UFPVAcroDronePilotComponent::BeginDestroy()
{
	if (unhookSetCustomPrimitiveData1P)
	{
		unhookSetCustomPrimitiveData1P();
		unhookSetCustomPrimitiveData1P = nullptr;
	}
	if (unhookSetCustomPrimitiveData3P)
	{
		unhookSetCustomPrimitiveData3P();
		unhookSetCustomPrimitiveData3P = nullptr;
	}
	Super::BeginDestroy();
}

AFGCharacterPlayer* UFPVAcroDronePilotComponent::GetPlayer()
{
	return dynamic_cast<AFGCharacterPlayer*>(GetOwner());
}

USkeletalMeshComponent* UFPVAcroDronePilotComponent::GetMesh1P()
{
	return GetPlayer()->GetMesh1P();
}

USkeletalMeshComponent* UFPVAcroDronePilotComponent::GetMesh3P()
{
	return GetPlayer()->GetMesh3P();
}

void UFPVAcroDronePilotComponent::SetupCustomAnimationComponent1P()
{
	if (customSkeletalMesh1P)
	{
		return;
	}

	if (!CustomAnimClass1P)
	{
		UE_LOG(LogTemp, Fatal, TEXT("[FPVAcroDrones] Failed to find the custom animation component blueprint"));
		return;
	}

	// Build custom SkeletalMeshComponent
	USkeletalMeshComponent* mesh1P = GetMesh1P();
	customSkeletalMesh1P = DuplicateObject<USkeletalMeshComponent>(mesh1P, mesh1P->GetOuter(), "FPVADSkeletalMeshComponent1P"); // Make sure customSkeletalMesh is a UPROPERTY to avoid GC
	customSkeletalMesh1P->AnimScriptInstance = nullptr; // Breaks link to original AnimScriptInstance, ensures it not cleared during SetAnimClass
	customSkeletalMesh1P->SetAnimClass(CustomAnimClass1P);
	customSkeletalMesh1P->RegisterComponent();
	AddTickPrerequisiteComponent(mesh1P);
	customSkeletalMesh1P->AddTickPrerequisiteComponent(this);
	customAnimInstance1P = Cast<UFPVAcroDronePilotAnimBlueprint>(customSkeletalMesh1P->GetAnimInstance());

	// Hide custom SkeletalMeshComponent (visibility is decided in blueprint depending on camera)
	customSkeletalMesh1P->SetVisibility(false);

	if (!IsRunningDedicatedServer())
	{
		auto SetCustomPrimitiveDataFloat = [this, mesh1P](auto& scope, UPrimitiveComponent* self, int32 DataIndex, float Value) {
			if (self == mesh1P)
			{
				UE_LOG(LogTemp, Warning, TEXT("[FPVAcroDrones] SetCustomPrimitiveDataFloat %s %d %f"), *self->GetName(), DataIndex, Value);
				customSkeletalMesh1P->SetCustomPrimitiveDataFloat(DataIndex, Value);
			}
		};

		auto SetCustomPrimitiveDataVector2 = [this, mesh1P](auto& scope, UPrimitiveComponent* self, int32 DataIndex, FVector2D Value) {
			if (self == mesh1P) {
				UE_LOG(LogTemp, Warning, TEXT("[FPVAcroDrones] SetCustomPrimitiveDataVector2 %s %d (%f, %f)"), *self->GetName(), DataIndex, Value.X, Value.Y);
				customSkeletalMesh1P->SetCustomPrimitiveDataVector2(DataIndex, Value);
			}
		};

		auto SetCustomPrimitiveDataVector3 = [this, mesh1P](auto& scope, UPrimitiveComponent* self, int32 DataIndex, FVector Value) {
			if (self == mesh1P) {
				UE_LOG(LogTemp, Warning, TEXT("[FPVAcroDrones] SetCustomPrimitiveDataVector3 %s %d (%f, %f, %f)"), *self->GetName(), DataIndex, Value.X, Value.Y, Value.Z);
				customSkeletalMesh1P->SetCustomPrimitiveDataVector3(DataIndex, Value);
			}
		};

		auto SetCustomPrimitiveDataVector4 = [this, mesh1P](auto& scope, UPrimitiveComponent* self, int32 DataIndex, FVector4 Value) {
			if (self == mesh1P) {
				UE_LOG(LogTemp, Warning, TEXT("[FPVAcroDrones] SetCustomPrimitiveDataVector4 %s %d (%f, %f, %f, %f)"), *self->GetName(), DataIndex, Value.X, Value.Y, Value.Z, Value.W);
				customSkeletalMesh1P->SetCustomPrimitiveDataVector4(DataIndex, Value);
			}
		};

		auto SetCustomPrimitiveDataFloatHandle = SUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataFloat, SetCustomPrimitiveDataFloat);
		auto SetCustomPrimitiveDataVector2Handle = SUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataVector2, SetCustomPrimitiveDataVector2);
		auto SetCustomPrimitiveDataVector3Handle = SUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataVector3, SetCustomPrimitiveDataVector3);
		auto SetCustomPrimitiveDataVector4Handle = SUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataVector4, SetCustomPrimitiveDataVector4);

		if (unhookSetCustomPrimitiveData1P)
		{
			unhookSetCustomPrimitiveData1P();
		}

		unhookSetCustomPrimitiveData1P = [SetCustomPrimitiveDataFloatHandle, SetCustomPrimitiveDataVector2Handle, SetCustomPrimitiveDataVector3Handle, SetCustomPrimitiveDataVector4Handle] {
			UNSUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataFloat, SetCustomPrimitiveDataFloatHandle);
			UNSUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataVector2, SetCustomPrimitiveDataVector2Handle);
			UNSUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataVector3, SetCustomPrimitiveDataVector3Handle);
			UNSUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataVector4, SetCustomPrimitiveDataVector4Handle);
		};
	}

	if (!customAnimInstance1P)
	{
		UE_LOG(LogTemp, Fatal, TEXT("[FPVAcroDrones] Failed to override player's skeletal mesh"));
		return;
	}

	// Keep original SkeletalMeshComponent updating
	mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	CaptureVanillaPose1P();
	ChangeCustomAnimationState1P(EVPVADAnimState::VANILLA);
	UE_LOG(LogTemp, Warning, TEXT("[FPVAcroDrones] SetupCustomAnimationComponent done"));
}

void UFPVAcroDronePilotComponent::SetupCustomAnimationComponent3P()
{
	if (customSkeletalMesh3P)
	{
		return;
	}

	if (!CustomAnimClass3P)
	{
		UE_LOG(LogTemp, Fatal, TEXT("[FPVAcroDrones] Failed to find the custom animation component blueprint"));
		return;
	}

	// Build custom SkeletalMeshComponent
	USkeletalMeshComponent* mesh3P = GetMesh3P();
	customSkeletalMesh3P = DuplicateObject<USkeletalMeshComponent>(mesh3P, mesh3P->GetOuter(), "FPVADSkeletalMeshComponent3P"); // Make sure customSkeletalMesh is a UPROPERTY to avoid GC
	customSkeletalMesh3P->AnimScriptInstance = nullptr; // Breaks link to original AnimScriptInstance, ensures it not cleared during SetAnimClass
	customSkeletalMesh3P->SetAnimClass(CustomAnimClass3P);
	customSkeletalMesh3P->RegisterComponent();
	AddTickPrerequisiteComponent(mesh3P);
	customSkeletalMesh3P->AddTickPrerequisiteComponent(this);
	customAnimInstance3P = Cast<UFPVAcroDronePilotAnimBlueprint>(customSkeletalMesh3P->GetAnimInstance());

	// Hide custom SkeletalMeshComponent (visibility is decided in blueprint depending on camera)
	customSkeletalMesh3P->SetVisibility(false);

	if (!IsRunningDedicatedServer())
	{
		auto SetCustomPrimitiveDataFloat = [this, mesh3P](auto& scope, UPrimitiveComponent* self, int32 DataIndex, float Value) {
			if (self == mesh3P)
			{
				UE_LOG(LogTemp, Warning, TEXT("[FPVAcroDrones] SetCustomPrimitiveDataFloat %s %d %f"), *self->GetName(), DataIndex, Value);
				customSkeletalMesh3P->SetCustomPrimitiveDataFloat(DataIndex, Value);
			}
		};

		auto SetCustomPrimitiveDataVector2 = [this, mesh3P](auto& scope, UPrimitiveComponent* self, int32 DataIndex, FVector2D Value) {
			if (self == mesh3P) {
				UE_LOG(LogTemp, Warning, TEXT("[FPVAcroDrones] SetCustomPrimitiveDataVector2 %s %d (%f, %f)"), *self->GetName(), DataIndex, Value.X, Value.Y);
				customSkeletalMesh3P->SetCustomPrimitiveDataVector2(DataIndex, Value);
			}
		};

		auto SetCustomPrimitiveDataVector3 = [this, mesh3P](auto& scope, UPrimitiveComponent* self, int32 DataIndex, FVector Value) {
			if (self == mesh3P) {
				UE_LOG(LogTemp, Warning, TEXT("[FPVAcroDrones] SetCustomPrimitiveDataVector3 %s %d (%f, %f, %f)"), *self->GetName(), DataIndex, Value.X, Value.Y, Value.Z);
				customSkeletalMesh3P->SetCustomPrimitiveDataVector3(DataIndex, Value);
			}
		};

		auto SetCustomPrimitiveDataVector4 = [this, mesh3P](auto& scope, UPrimitiveComponent* self, int32 DataIndex, FVector4 Value) {
			if (self == mesh3P) {
				UE_LOG(LogTemp, Warning, TEXT("[FPVAcroDrones] SetCustomPrimitiveDataVector4 %s %d (%f, %f, %f, %f)"), *self->GetName(), DataIndex, Value.X, Value.Y, Value.Z, Value.W);
				customSkeletalMesh3P->SetCustomPrimitiveDataVector4(DataIndex, Value);
			}
		};

		auto SetCustomPrimitiveDataFloatHandle = SUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataFloat, SetCustomPrimitiveDataFloat);
		auto SetCustomPrimitiveDataVector2Handle = SUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataVector2, SetCustomPrimitiveDataVector2);
		auto SetCustomPrimitiveDataVector3Handle = SUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataVector3, SetCustomPrimitiveDataVector3);
		auto SetCustomPrimitiveDataVector4Handle = SUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataVector4, SetCustomPrimitiveDataVector4);

		if (unhookSetCustomPrimitiveData3P)
		{
			unhookSetCustomPrimitiveData3P();
		}

		unhookSetCustomPrimitiveData3P = [SetCustomPrimitiveDataFloatHandle, SetCustomPrimitiveDataVector2Handle, SetCustomPrimitiveDataVector3Handle, SetCustomPrimitiveDataVector4Handle] {
			UNSUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataFloat, SetCustomPrimitiveDataFloatHandle);
			UNSUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataVector2, SetCustomPrimitiveDataVector2Handle);
			UNSUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataVector3, SetCustomPrimitiveDataVector3Handle);
			UNSUBSCRIBE_UOBJECT_METHOD(UPrimitiveComponent, SetCustomPrimitiveDataVector4, SetCustomPrimitiveDataVector4Handle);
		};
	}

	if (!customAnimInstance3P)
	{
		UE_LOG(LogTemp, Fatal, TEXT("[FPVAcroDrones] Failed to override player's skeletal mesh"));
		return;
	}

	// Keep original SkeletalMeshComponent updating
	mesh3P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	CaptureVanillaPose3P();
	ChangeCustomAnimationState3P(EVPVADAnimState::VANILLA);
	UE_LOG(LogTemp, Warning, TEXT("[FPVAcroDrones] SetupCustomAnimationComponent done"))
}

void UFPVAcroDronePilotComponent::DestroyCustomAnimationComponent1P() {
	if (!customSkeletalMesh1P)
	{
		return;
	}

	customAnimInstance1P = nullptr;

	customSkeletalMesh1P->DestroyComponent();
	customSkeletalMesh1P = nullptr;
}

void UFPVAcroDronePilotComponent::DestroyCustomAnimationComponent3P() {
	if (!customSkeletalMesh3P)
	{
		return;
	}

	customAnimInstance3P = nullptr;

	customSkeletalMesh3P->DestroyComponent();
	customSkeletalMesh3P = nullptr;
}

void UFPVAcroDronePilotComponent::ChangeCustomAnimationState1P(EVPVADAnimState newState)
{
	if (customAnimInstance1P && newState != customAnimInstance1P->animState)
	{
		customAnimInstance1P->animState = newState;
	}
}

void UFPVAcroDronePilotComponent::ChangeCustomAnimationState3P(EVPVADAnimState newState)
{
	if (customAnimInstance3P && newState != customAnimInstance3P->animState)
	{
		customAnimInstance3P->animState = newState;
	}
}

void UFPVAcroDronePilotComponent::CustomAnimationTick1P(float deltaTime)
{
	if (customSkeletalMesh1P)
	{
		CaptureVanillaPose1P();
		customSkeletalMesh1P->SetRelativeTransform(GetMesh1P()->GetRelativeTransform());
	}
}

void UFPVAcroDronePilotComponent::CustomAnimationTick3P(float deltaTime)
{
	if (customSkeletalMesh3P)
	{
		CaptureVanillaPose3P();
		customSkeletalMesh3P->SetRelativeTransform(GetMesh3P()->GetRelativeTransform());
	}
}

void UFPVAcroDronePilotComponent::CaptureVanillaPose1P()
{
	if (customAnimInstance1P && GetMesh1P() && GetMesh1P()->GetAnimInstance())
	{
		FPoseSnapshot vanillaPoseSnapshot;
		GetMesh1P()->GetAnimInstance()->SnapshotPose(vanillaPoseSnapshot);
		customAnimInstance1P->vanillaPose = vanillaPoseSnapshot;
	}
}

void UFPVAcroDronePilotComponent::CaptureVanillaPose3P()
{
	if (customAnimInstance3P && GetMesh3P() && GetMesh3P()->GetAnimInstance())
	{
		FPoseSnapshot vanillaPoseSnapshot;
		GetMesh3P()->GetAnimInstance()->SnapshotPose(vanillaPoseSnapshot);
		customAnimInstance3P->vanillaPose = vanillaPoseSnapshot;
	}
}

void UFPVAcroDronePilotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	CustomAnimationTick1P(DeltaTime);
	CustomAnimationTick3P(DeltaTime);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UFPVAcroDronePilotAnimBlueprint* UFPVAcroDronePilotComponent::GetCustomAnimInstance1P()
{
	SetupCustomAnimationComponent1P();
	SetupCustomAnimationComponent3P();
	return customAnimInstance1P;
}

USkeletalMeshComponent* UFPVAcroDronePilotComponent::GetCustomSkeletalMesh1P()
{
	SetupCustomAnimationComponent1P();
	SetupCustomAnimationComponent3P();
	return customSkeletalMesh1P;
}

UFPVAcroDronePilotAnimBlueprint* UFPVAcroDronePilotComponent::GetCustomAnimInstance3P()
{
	SetupCustomAnimationComponent1P();
	SetupCustomAnimationComponent3P();
	return customAnimInstance3P;
}

USkeletalMeshComponent* UFPVAcroDronePilotComponent::GetCustomSkeletalMesh3P()
{
	SetupCustomAnimationComponent1P();
	SetupCustomAnimationComponent3P();
	return customSkeletalMesh3P;
}

