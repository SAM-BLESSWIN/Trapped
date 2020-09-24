// Fill out your copyright notice in the Description page of Project Settings.

#include"Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetUpInputComponent();
}

// Called every frame
//attach the object to physics handle
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
	
}

//Check Physics Handle
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle=GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle)
	{}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("%s no physics Handle found"),*(GetOwner()->GetName()));
	}
	
}

//Setting Inputs
void UGrabber::SetUpInputComponent()
{
	Inputbind=GetOwner()->FindComponentByClass<UInputComponent>();
	if(Inputbind)
	{
		Inputbind->BindAction("Grab",IE_Pressed,this,&UGrabber::Grab);
		Inputbind->BindAction("Grab",IE_Released,this,&UGrabber::Release);
	}
}

//get Grab object at reach position 
void UGrabber::Grab()
{
	FHitResult Hitresult=GetGrabbableObject();
	UPrimitiveComponent* Grabcomponent=Hitresult.GetComponent();

	if(Hitresult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation
		(
			Grabcomponent,
			NAME_None,
			GetPlayerReach()
		);
	}	
}

//Releasing an object
void UGrabber::Release()
{
	if(PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->ReleaseComponent();
	}
}

//RayCasting to point to grabable object
FHitResult UGrabber::GetGrabbableObject() const
{
	//Draw a debug line to visualize where Player looks
	DrawDebugLine
	(
		GetWorld(),
		GetPlayerPosition(),
		GetPlayerReach(),
		FColor(255,0,0),
		false,
		0.0f,
		0,
		5.0f
	);
	
	//Raycasting
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner() );

	GetWorld()->LineTraceSingleByObjectType
	(
		Hit,
		GetPlayerPosition(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* HitObject= Hit.GetActor();
	if(HitObject) UE_LOG(LogTemp,Warning,TEXT("raycast hit %s"),*(HitObject->GetName()));

	return Hit;
}

//Return LineTraceends
FVector UGrabber::GetPlayerReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	//Get Player viewpoint -position and direction
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	//Set length of raycast
	FVector PlayerViewPointRotationVector=PlayerViewPointRotation.Vector();
	FVector LineTraceEnd=PlayerViewPointLocation + (PlayerViewPointRotationVector * Reach);

	return LineTraceEnd;
}

//Retrun Player current position
FVector UGrabber::GetPlayerPosition() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	//Get Player viewpoint -position and direction
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}