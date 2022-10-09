// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
//#include "PhysicsEngine/PhysicsHandleComponent.h"

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

	// CHECKING
	//UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	//if (PhysicsHandle != nullptr)
	//{
	//	//PhysicsHandle->GetName();
	//	UE_LOG(LogTemp, Display, TEXT(" Got Physics handle : %s"), *PhysicsHandle->GetName());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("No Physics handle found"));
	//}
	
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
	//UE_LOG(LogTemp, Display, TEXT(" Original is : %f"), Damage);  --> Both are 20
	//UE_LOG(LogTemp, Display, TEXT(" Reference is : %f"), DamageRef); --> Both are 20
	//POINTER
	//float Health = 0;
	//float* HealthPointer = &Health;
	//*HealthPointer = 20;
	//UE_LOG(LogTemp, Display, TEXT(" Original Health : %f"), Health); --> Original still 0
	//UE_LOG(LogTemp, Display, TEXT(" Pointer Health : %f"), *HealthPointer); --> Reference are 20

	// OUT PARAMETER
	//float Damage = 5;
	//if (HasDamage(Damage))
	//{
	//	PrintDamage(Damage);
	//}
	

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	/*if (PhysicsHandle == nullptr)
	{
		return;
	}
	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}*/

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
	
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr)
	{	
		return;
	}
	
	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);

	if (HasHit)
	{
		//DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 2);
		DrawDebugSphere(GetWorld(),
		HitResult.ImpactPoint,
		10,
		10,
		FColor::Red,
		false,
		2);
		
		//AActor* HitObject = HitResult.GetActor();
		//UE_LOG(LogTemp, Display, TEXT(" Actor get hit : %s"), *HitObject->GetActorNameOrLabel());
		// ALTERNATIVE
		//FString HitObjectName = HitResult.GetActor()->GetActorNameOrLabel();
		//UE_LOG(LogTemp, Display, TEXT(" Actor Name : %s"), *HitObjectName);

		// Wake RB
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();

		// Adding Tag
		HitResult.GetActor()->Tags.Add("Grabbed");

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Release Grabber"));

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	/*if (PhysicsHandle == nullptr)
	{
		return;
	}
	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		// Wake RB
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		// Remove Tag
		PhysicsHandle->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}*/

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent()) // check left first before go to right operator
	{
		// Wake RB
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();

		// Remove Tag
		PhysicsHandle->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");

		PhysicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber require Physic Handle Component"));
	}
	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	bool Result;

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	Result = GetWorld()->SweepSingleByChannel(OutHitResult,
	Start, 
	End,
	FQuat::Identity,
	ECC_GameTraceChannel2,
	Sphere);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 2);
	return Result;
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

//TRACE CHANNEL FOR COLLISION
// To create :
// Project Setting -> Collision -> Create Trace Channel
// To find :
// Open Projecct Folder
// Look for Config Folder
// Look for "DefaultEngine" file
// Open and find the component name



