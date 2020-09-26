// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAPPED_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float GetTotalMass();

	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere)
	float TargetYaw=90.0f;

	AActor* Player;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	float DoorLastOpened=0.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay=1.0f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed=2.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed=3.0f;	

	UPROPERTY(EditAnywhere)
	float DoorOpenMass=0.0f;

	UPROPERTY()
	UAudioComponent* DoorSound=nullptr;

	bool OpenDoorSound;
	bool CloseDoorSound;
};
