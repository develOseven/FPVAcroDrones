/* SPDX-License-Identifier: LGPL-3.0-or-later */

#include "FPVAcroDronesGameWorldModule.h"

#include "Patching/NativeHookManager.h"
#include "Misc/CoreMisc.h"
#include "SlateApplication.h"

void UFPVAcroDronesGameWorldModule::DispatchLifecycleEvent(ELifecyclePhase Phase)
{
    if (Phase == ELifecyclePhase::CONSTRUCTION)
    {
        if (!IsRunningDedicatedServer())
        {
            FSlateApplication& SampleObject = FSlateApplication::Get();

            Invoke( [&]() {
                HookInvoker<decltype(static_cast<bool(FSlateApplication::*)(FGamepadKeyNames::Type KeyName, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId, float AnalogValue)>(&FSlateApplication::OnControllerAnalog)), static_cast<bool(FSlateApplication::*)(FGamepadKeyNames::Type KeyName, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId, float AnalogValue)>(&FSlateApplication::OnControllerAnalog)>::InstallHook("FSlateApplication::OnControllerAnalog", &SampleObject); \
                return HookInvoker<decltype(static_cast<bool(FSlateApplication::*)(FGamepadKeyNames::Type KeyName, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId, float AnalogValue)>(&FSlateApplication::OnControllerAnalog)), static_cast<bool(FSlateApplication::*)(FGamepadKeyNames::Type KeyName, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId, float AnalogValue)>(&FSlateApplication::OnControllerAnalog)>::AddHandlerBefore([this](auto& scope, FSlateApplication* self, FGamepadKeyNames::Type KeyName, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId, float AnalogValue) {
                    OnControllerAnalog(KeyName, PlatformUserId, InputDeviceId, AnalogValue);
                });
            } );
        }
    }

    Super::DispatchLifecycleEvent(Phase);
}

float UFPVAcroDronesGameWorldModule::GetGNearClippingPlane() const
{
    return GNearClippingPlane;
}

void UFPVAcroDronesGameWorldModule::SetGNearClippingPlane(float NewGNearClippingPlane)
{
    GNearClippingPlane = NewGNearClippingPlane;
}
