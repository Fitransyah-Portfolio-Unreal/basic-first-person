// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// POINTER AND DEREFERENCING
	//float MyFloat = 5;
	//float* YourFloat = &MyFloat; //unnecessary storing variable  &is for  taking the original adress??
	//
	//UE_LOG(LogTemp, Display, TEXT("Your float : %f"), *YourFloat); 
	//
	// AActor* Owner = GetOwner();
	//UE_LOG(LogTemp, Display, TEXT("Mover Owner Adreess : %u"), Owner); //print Adress memory
	//FString Name = (*Owner).GetActorNameOrLabel(); // * and () is dereferencing or can do below -> 
	// FString Name = Owner->GetActorNameOrLabel();
	// UE_LOG(LogTemp, Display, TEXT("Mover Owner : %s"), *Name);
	//
	// FVector OwnerLocation = Owner->GetActorLocation();
	// FString OwnerLocationString = OwnerLocation.ToCompactString();
	// UE_LOG(LogTemp, Display, TEXT("Mover Owner : %s"), *OwnerLocationString); 

	if (ShouldMove)
	{
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		FVector TargetLocation = OriginalLocation + MoveOffset;
		float Speed = FVector::Distance(OriginalLocation, TargetLocation) / MoveTime;

		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
		GetOwner()->SetActorLocation(NewLocation);
	}

}

