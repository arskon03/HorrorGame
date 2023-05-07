// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Internationalization/TextLocalizationResource.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "HorrorGame/DebuggingBasics.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Variables
	const FString ActorName = GetName();
	NameHash = FTextLocalizationResource::HashString(ActorName);

	bIsRotating = false;
	bIsMoving = false;

	Lifetime = 0.f;
	PreviousSineMovement = FVector::ZeroVector;

	SineWaveFrequency = 1.f;
	SineWaveAmplitude = 1.f;

	bGenerateRandomLocationTarget = true;
	WorldOffset = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	// Set the start location of the actor
	LocationStart = GetActorLocation();

	Lifetime = 0.f;
	PreviousSineMovement = FVector::ZeroVector;
}

void AItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Generate a random color for the actor every time it is spawned or changed in the editor
	const int32 RandomColor = UKismetMathLibrary::RandomInteger(INT32_MAX);
	ActorColor = FColor(RandomColor);
	ActorColor.A = 255;	// Set the alpha to 255 so the color is not transparent

	SineWaveFrequency = UKismetMathLibrary::RandomFloatInRange(0.5f, 2.f);
	SineWaveAmplitude = UKismetMathLibrary::RandomFloatInRange(18.f, 60.f);
}

void AItem::UpdateMovementTarget()
{
	// If actor is not moving, generate a target location
	if (!bIsMoving)
	{
		// Set the random location within a 50 unit radius of the start location
		if (bGenerateRandomLocationTarget)
		{
			LocationTarget = UKismetMathLibrary::RandomPointInBoundingBox(LocationStart, FVector(50, 50, 50));
		}
		else
		{
			LocationTarget = GetActorLocation() + WorldOffset;
		}
		bIsMoving = true;
	}
}

void AItem::UpdateRotationTarget()
{
	// If actor is not rotating, generate a target quaternion
	if (!bIsRotating)
	{
		FRotator RotationTarget;
		RotationTarget.Roll = UKismetMathLibrary::RandomFloatInRange(-180, 180);
		RotationTarget.Pitch = UKismetMathLibrary::RandomFloatInRange(-180, 180);
		RotationTarget.Yaw = UKismetMathLibrary::RandomFloatInRange(-180, 180);

		// Convert target rotator to a quaternion
		QuaternionTarget = RotationTarget.Quaternion();
		bIsRotating = true;
	}
}

void AItem::UpdateMovementState(float EqualityThreshold)
{
	// Stop moving if the target location is equal to the current location
	if (GetActorLocation().Equals(LocationTarget, EqualityThreshold))
	{
		bIsMoving = false;
	}
}

void AItem::UpdateRotationState(float EqualityThreshold)
{
	// Stop rotating if the new rotation is equal to the current rotation
	if (GetActorQuat().Equals(QuaternionTarget, EqualityThreshold))
	{
		bIsRotating = false;
	}
}

void AItem::MoveItemInterp(float DeltaTime, float MovementSpeed = 1.5f, float EqualityThreshold = 15.f)
{
	UpdateMovementTarget();

	// Interp current location to next step
	const FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, LocationTarget, DeltaTime, MovementSpeed);

	// Move Actor
	SetActorLocation(NewLocation);

	UpdateMovementState(EqualityThreshold);
}

void AItem::MoveItemConstant(float DeltaTime, float MovementSpeed = 15.f, float EqualityThreshold = 2.f)
{
	UpdateMovementTarget();

	// Get the direction of the target location
	FVector MovementVector = LocationTarget - GetActorLocation();
	MovementVector.Normalize(0.01);
	MovementVector *= MovementSpeed * DeltaTime;

	// Move Actor
	AddActorWorldOffset(MovementVector);

	UpdateMovementState(EqualityThreshold);
}

void AItem::RotateItemInterp(float DeltaTime, float RotationalSpeed = 1.5f, float EqualityThreshold = 0.3f)
{
	UpdateRotationTarget();

	// Interp current rotation to next step
	const FQuat CurrentQuaternion = GetActorQuat();
	FQuat NewQuaternion = FMath::QInterpTo(CurrentQuaternion, QuaternionTarget, DeltaTime, RotationalSpeed);

	// Rotate Actor
	SetActorRotation(NewQuaternion);

	UpdateRotationState(EqualityThreshold);
}

void AItem::RotateItemConstant(float DeltaTime, float RotationalSpeed = 1.5f, float EqualityThreshold = 0.01f)
{
	UpdateRotationTarget();

	// Get the delta quaternion between the current rotation and the target rotation
	const FQuat CurrentQuaternion = GetActorQuat();
	const FQuat NewQuaternion = FMath::QInterpConstantTo(CurrentQuaternion, QuaternionTarget, DeltaTime, RotationalSpeed);
	const FQuat RotationQuaternion = NewQuaternion * CurrentQuaternion.Inverse();

	// Rotate Actor
	AddActorWorldRotation(RotationQuaternion);

	UpdateRotationState(EqualityThreshold);
}

void AItem::DisplayDebugMessage()
{
	bool bIsEngineNull = GEngine == nullptr;

	/* See actor's target rotation and rotating values */
	if (!bIsEngineNull)
	{
		const int32 MessageKey			= NameHash;
		const float SecondsToDisplay	= 5.f;
		const FColor MessageColor		= ActorColor;

		const FString MessageText = FString::Printf(TEXT("%s says:\nTargetQuaternion: (%f, %f, %f, %f)\nTargetLocation: (%f, %f, %f)\n"),
													*GetName(), QuaternionTarget.X, QuaternionTarget.Y, QuaternionTarget.Z, QuaternionTarget.W,
													LocationTarget.X, LocationTarget.Y, LocationTarget.Z);
		GEngine->AddOnScreenDebugMessage(MessageKey, SecondsToDisplay, MessageColor, MessageText);
	}
}

void AItem::DrawDebugActor()
{
	UWorld* GameWorld = GetWorld();
	if (GameWorld != nullptr)
	{
		/* Draw The Forward Vector Of The Actor */
		const FVector StartLocation	= GetActorLocation();
		const bool bIsPersistent	= false;
		const float Thickness		= 1.2f;
		const FVector EndLocation	= StartLocation + GetActorForwardVector() * 60.f;
		const float ArrowSize		= 12.5f;
		const float Duration		= -1.f; // One Frame
		DrawDebugDirectionalArrow(GameWorld, StartLocation, EndLocation, ArrowSize, ActorColor, bIsPersistent, Duration, 0U, Thickness);

		/* Draw Debug Sphere */
		const float SphereRadius	= 30.f;
		const int32 Segments		= 12;
		DrawDebugSphere(GameWorld, StartLocation, SphereRadius, Segments, ActorColor, bIsPersistent, Duration, 0U, Thickness);

		/* Draw Debug Point */
		DRAW_BASIC_POINT(StartLocation, Duration);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Cancel previous frame's sine movement
	AddActorWorldOffset(-PreviousSineMovement);

	// Update actor's lifetime
	Lifetime += DeltaTime;

	/* Call behaviour functions */

	RotateItemConstant(DeltaTime);

	MoveItemConstant(DeltaTime);

	// Get new offset for sine movement
	const double SineWavePhase = Lifetime * (2.f * 3.141592f) * SineWaveFrequency;
	PreviousSineMovement.Z = FMath::Sin(SineWavePhase) * SineWaveAmplitude;
	AddActorWorldOffset(PreviousSineMovement);

	/*** Debug start ***/
	DisplayDebugMessage();

	DrawDebugActor();
	/*** Debug end ***/
}


