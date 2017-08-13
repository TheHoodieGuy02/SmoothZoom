// 837 Studios - 2016 - Michael Gaskin (teak421) -- MIT License

#include "SmoothZoomPrivatePCH.h"
#include "SmoothZoom.h"
#include "AC_SmoothZoom.h"

DEFINE_LOG_CATEGORY(ZoomLog);

UAC_SmoothZoom::UAC_SmoothZoom()
{
	PrimaryComponentTick.bCanEverTick = true;
	// Default Values
	CurrentVersion = 1.3f;
	MinTargetLength = 150.0f;
	MaxTargetLength = 1500.0f;
	ZoomUnits = 125.0f;
	ZoomSmoothness = 9.0f;
}

void UAC_SmoothZoom::BeginPlay() { 	Super::BeginPlay(); }

void UAC_SmoothZoom::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Only call when need to, saves ms's 
	if (!FMath::IsNearlyEqual(SpringArm->TargetArmLength, DesiredArmLength, 1.0f))
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, DesiredArmLength, DeltaTime, ZoomSmoothness);
	}
}

// Assigns the SpringArm component
void UAC_SmoothZoom::SetSpringArmComponent(UPARAM(ref)USpringArmComponent* AssignedSpringArm)
{
	DesiredArmLength = AssignedSpringArm->TargetArmLength;
	SpringArm = AssignedSpringArm;
}

// Sets the DesiredArmLength
void UAC_SmoothZoom::SmoothCameraZoom(bool bZoomOut)
{
	DesiredArmLength = bZoomOut ? SpringArm->TargetArmLength + ZoomUnits
		: SpringArm->TargetArmLength + (ZoomUnits * -1);
	if (DesiredArmLength > MaxTargetLength || DesiredArmLength < MinTargetLength)
	{
		DesiredArmLength = FMath::Min<float>(FMath::Max<float>(DesiredArmLength, MinTargetLength), MaxTargetLength);
	}

	if (bDebug) { SmoothZoomLog(); }
}

void UAC_SmoothZoom::SmoothZoomLog()
{
	UE_LOG(ZoomLog, Log, TEXT("ArmLength: %f, Min: %f Max: %f, ZUnits: %f, Smoothness: %f"),
		DesiredArmLength, MinTargetLength, MaxTargetLength, ZoomUnits, ZoomSmoothness)
}

