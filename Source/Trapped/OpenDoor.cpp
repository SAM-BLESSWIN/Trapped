// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw=GetOwner()->GetActorRotation().Yaw;
	CurrentYaw=InitialYaw;
	TargetYaw+=CurrentYaw;

	Player=GetWorld()->GetFirstPlayerController()->GetPawn();

	if(!PressurePlate) 
	{
		FString Name=GetOwner()->GetName();
		UE_LOG(LogTemp,Warning,TEXT("%s component with Pressure plate not initialized"),*Name);
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PressurePlate && PressurePlate->IsOverlappingActor(Player))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened=GetWorld()->GetTimeSeconds();
	}
	else
	{
		if(GetWorld()->GetTimeSeconds()-DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}		
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw=FMath::FInterpTo(CurrentYaw,TargetYaw,DeltaTime,DoorOpenSpeed);

	FRotator DoorRotation=GetOwner()->GetActorRotation();
	DoorRotation.Yaw=CurrentYaw;

	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw=FMath::FInterpTo(CurrentYaw,InitialYaw,DeltaTime,DoorCloseSpeed);

	FRotator DoorRotation=GetOwner()->GetActorRotation();
	DoorRotation.Yaw=CurrentYaw;

	GetOwner()->SetActorRotation(DoorRotation);
}
	