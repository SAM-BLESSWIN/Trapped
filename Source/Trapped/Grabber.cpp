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
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

	if(PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	
}


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

void UGrabber::SetUpInputComponent()
{
	Inputbind=GetOwner()->FindComponentByClass<UInputComponent>();
	if(Inputbind)
	{
		Inputbind->BindAction("Grab",IE_Pressed,this,&UGrabber::Grab);
		Inputbind->BindAction("Grab",IE_Released,this,&UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp,Warning,TEXT("Grabbed"));
	FHitResult Hitresult=GetGrabbableObject();

	UPrimitiveComponent* Grabcomponent=Hitresult.GetComponent();

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

	if(Hitresult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation
		(
			Grabcomponent,
			NAME_None,
			LineTraceEnd
		);
	}	
}

void UGrabber::Release()
{
	UE_LOG(LogTemp,Warning,TEXT("Grab Released"));
	if(PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->ReleaseComponent();
	}
}

FHitResult UGrabber::GetGrabbableObject()
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

	//Draw a debug line to visualize where Player looks
	DrawDebugLine
	(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
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
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* HitObject= Hit.GetActor();
	if(HitObject) UE_LOG(LogTemp,Warning,TEXT("raycast hit %s"),*(HitObject->GetName()));

	return Hit;
}



