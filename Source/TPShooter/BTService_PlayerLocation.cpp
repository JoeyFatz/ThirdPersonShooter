// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gameframework/Pawn.h"
#include "Kismet/GameplayStatics.h"

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
	NodeName = TEXT("Update Player Location");
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!PlayerPawn) { return; }

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
}
