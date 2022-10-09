// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//UE_LOG(LogTemp, Display, TEXT("Constructing"));
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Trigger Component is Alive"));
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(LogTemp, Display, TEXT("Trigger Component is Ticking"));

	AActor* FoundActor = GetAcceptableActor();
	if (FoundActor != nullptr)
	{
		// Is still being grabbed?
		//if (FoundActor->ActorHasTag("Grabbed"))
		//{
		//	return;
		//}
		// Alternative way in GetAcceptableActor function
		 
		
		//UE_LOG(LogTemp, Display, TEXT("Unlocking"));
		UPrimitiveComponent* Component =  Cast<UPrimitiveComponent>(FoundActor->GetRootComponent());
		if (Component != nullptr)
		{
			Component->SetSimulatePhysics(false);			
		}
		FoundActor->AttachToComponent(this,	FAttachmentTransformRules::KeepWorldTransform, NAME_None);

		Mover->SetShouldMove(true);
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("Relocking"));
		Mover->SetShouldMove(false);
	}
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	//AActor* ReturnActor = nullptr; V1

	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	/*if (Actors.Num() > 0)
	{
		FString ActorName = Actors[0]->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("Overlapping actor name : %s"), *ActorName);
	}*/

	/*int32 index = 0;
	while (index < Actors.Num())
	{
		FString ActorName = Actors[index]->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT(" Overlapping actor name : %s"), *ActorName);
		index++;
	}*/

	/*for (int32 i = 0; i < Actors.Num(); i++)
	{
		FString ActorName = Actors[0]->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("Overlapping actor name : %s"), *ActorName);
	}*/

	for (AActor* Actor : Actors)
	{		

		//if (Actor->ActorHasTag(AcceptableActorTag))
		//{
		//	//UE_LOG(LogTemp, Display, TEXT("Unlocking"));
		//	//ReturnActor = Actor; V1
		//	return Actor;
		//}

		// true && true = true
		// true && false = false
		// true || false = true
		// true || true = true
		// false || false = false
		// !false || false = true

		bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
		bool IsGrabbed = Actor->ActorHasTag("Grabbed");
		if (HasAcceptableTag && !IsGrabbed)
		{
			return Actor;
		}
	}

	//return ReturnActor; V1
	return nullptr;
}