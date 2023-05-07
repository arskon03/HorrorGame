// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class HORRORGAME_API AItem : public AActor
{
	GENERATED_BODY()
	
private:
	/** The hash of the item's name */
	int32 NameHash;

	/** The target quaternion of the actor */
	FQuat QuaternionTarget;

	/** Determines if actor is currently rotating */
	bool bIsRotating;

	/** The start location of the actor */
	FVector LocationStart;

	/** The target location of the actor */
	FVector LocationTarget;

	/** Determines if actor is currently moving */
	bool bIsMoving;

	/** How long the actor has been in play measured in seconds */
	double Lifetime;

	/** The previous sine wave movement of the actor */
	FVector PreviousSineMovement;

	/** Determines if the target location is random */
	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bGenerateRandomLocationTarget;

	/** If the target location isn't random, then this will be it's offset from the location that the movement stops */
	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	FVector WorldOffset;

	/** The frequency of the sine wave movement of the actor in Hertz */
	UPROPERTY(VisibleAnywhere, Category = "SineWave", meta = (AllowPrivateAccess = "true"))
	float SineWaveFrequency;

	/** The amplitude of the sine wave movement of the actor */
	UPROPERTY(VisibleAnywhere, Category = "SineWave", meta = (AllowPrivateAccess = "true"))
	float SineWaveAmplitude;

	/** The Color of the actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	FColor ActorColor;

public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when an instance of this class is placed (in editor) or spawned.
	virtual void OnConstruction(const FTransform& Transform) override;

	/**
	 * Interpolates the item's location to a random location within a 50 unit radius of the start location
	 * @param DeltaTime Time between last and current frame
	 * @param EqualityThreshold The threshold at which the item is considered to have reached its target location
	 * @param MovementSpeed The speed at which the item moves
	*/
	void MoveItemInterp(float DeltaTime, float MovementSpeed, float EqualityThreshold);

	/**
	 * Updates the item's movement target
	*/
	void UpdateMovementTarget();

	/**
	 * Updates the item's rotation target
	*/
	void UpdateRotationTarget();

	/**
	 * Updates the item's movement state
	*/
	void UpdateMovementState(float EqualityThreshold);

	/**
	 * Updates the item's rotation state
 	*/
	void UpdateRotationState(float EqualityThreshold);

	/**
	 * Moves the item's location to a random location at a constant speed within a 50 unit radius of the start location
	 * @param DeltaTime Time between last and current frame
	 * @param EqualityThreshold The threshold at which the item is considered to have reached its target location
	 * @param MovementSpeed The speed at which the item moves
	*/
	void MoveItemConstant(float DeltaTime, float MovementSpeed, float EqualityThreshold);

	/**
	 * Interpolates the item's rotation to a random rotation
	 * @param DeltaTime Time between last and current frame
	 * @param EqualityThreshold The threshold at which the item is considered to have reached its target rotation
	 * @param RotationalSpeed The speed at which the item rotates
	*/
	void RotateItemInterp(float DeltaTime, float RotationalSpeed, float EqualityThreshold);


	/**
	 * Rotates the item's location to a random rotation at a constant speed
	 * @param DeltaTime Time between last and current frame
 	 * @param EqualityThreshold The threshold at which the item is considered to have reached its target rotation
	 * @param RotationalSpeed The speed at which the item rotates
	*/
	void RotateItemConstant(float DeltaTime, float RotationalSpeed, float EqualityThreshold);

	/**
	* Displays a debug message in the top left corner of the screen
	*/
	void DisplayDebugMessage();

	/**
	 * Draws a debug actor to the screen
	*/
	void DrawDebugActor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
