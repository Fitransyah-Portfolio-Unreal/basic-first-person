// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle != nullptr)
	{
		//PhysicsHandle->GetName();
		UE_LOG(LogTemp, Display, TEXT(" Got Physics handle : %s"), *PhysicsHandle->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Physics handle found"));
	}
	
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// GET FAMILIAR WITH UWORLD CLASS
	//FRotator CurrentRotation = GetOwner()->GetActorRotation();
	//FString Name = GetOwner()->GetActorNameOrLabel();
	//FString CurrentRotationString = CurrentRotation.ToCompactString();
	//UE_LOG(LogTemp, Display, TEXT("%s Rotate : %s"), *Name , *CurrentRotationString);
	//
	//float Time = GetWorld()->TimeSeconds;
	//UE_LOG(LogTemp, Display, TEXT("%f : is passed in this world"), Time);
	
	//REFERENCE
	//float Damage = 0;
	//float& DamageRef = Damage;
	//DamageRef = 20;
	//UE_LOG(LogTemp, Display, TEXT(" Original is : %f"), Damage);
	//UE_LOG(LogTemp, Display, TEXT(" Reference is : %f"), DamageRef);
	//POINTER
	//float Health = 0;
	//float* HealthPointer = &Health;
	//*HealthPointer = 20;
	//UE_LOG(LogTemp, Display, TEXT(" Original Health : %f"), Health);
	//UE_LOG(LogTemp, Display, TEXT(" Pointer Health : %f"), *HealthPointer);

	// OUT PARAMETER
	//float Damage = 5;
	//if (HasDamage(Damage))
	//{
	//	PrintDamage(Damage);
	//}
	
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT(" Dont find any physics handle component in Grabber Owner"), );
		return;
	}

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 2);
	
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);
	if (HasHit)
	{
		//DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 2);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 2);
		
		//AActor* HitObject = HitResult.GetActor();
		//UE_LOG(LogTemp, Display, TEXT(" Actor get hit : %s"), *HitObject->GetActorNameOrLabel());
		// ALTERNATIVE
		//FString HitObjectName = HitResult.GetActor()->GetActorNameOrLabel();
		//UE_LOG(LogTemp, Display, TEXT(" Actor Name : %s"), *HitObjectName);

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.ImpactPoint,
			HitResult.GetComponent()->GetComponentRotation()
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Release Grabber"));
}

//OUT PARAMETER
//void UGrabber::PrintDamage(const float& Damage) 
//{
//	//Damage = 2;
//	UE_LOG(LogTemp, Display, TEXT(" Damage : %f"), Damage);
//}
//
//bool UGrabber::HasDamage(float& OutDamage)
//{
//	OutDamage = 7;
//	return true;
//}



