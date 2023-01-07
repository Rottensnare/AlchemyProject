// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyProject/Public/AI/Decorators/TargetVisibilityCheck.h"

#include "AI/AIBase.h"
#include "AlchemyProject/PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UTargetVisibilityCheck::UTargetVisibilityCheck()
{
	
}


bool UTargetVisibilityCheck::CheckVisibility(AAIController* AIController, APawn* InPawn, float DeltaSeconds)
{
	
	if(InPawn == nullptr || AIController == nullptr) return false;
	
	AAIBase* BaseAI = Cast<AAIBase>(InPawn);
	if(!BaseAI) return false;
	
	if(ABaseAIController* BaseAIController = Cast<ABaseAIController>(AIController))
	{
		if(BaseAIController->GetBlackboardComponent() == nullptr) return false;
		
		FVector TargetLocation = FVector();
		if(BaseAIController->GetBlackboardComponent()->GetValueAsObject(FName("Target")) == nullptr) TargetLocation = BaseAIController->GetBlackboardComponent()->GetValueAsVector(FName("LastTargetLocation"));
		else TargetLocation = Cast<AActor>(BaseAIController->GetBlackboardComponent()->GetValueAsObject(FName("Target")))->GetActorLocation();
		//UE_LOG(LogTemp, Warning, TEXT("TargetLocation: %s"), *TargetLocation.ToString())
		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams;
		//CollisionQueryParams.AddIgnoredActor(InPawn);
		bool bHitSomething = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				InPawn->GetActorLocation(),
				TargetLocation,
				ECC_Visibility,
				CollisionQueryParams);
		//if(IsValid(HitResult.GetActor())) UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName())
		DrawDebugLine(GetWorld(), InPawn->GetActorLocation(), TargetLocation, FColor::Red, false, 5.f);
		//UE_LOG(LogTemp, Warning, TEXT("Hit Something: %d"), bHitSomething)
		if(Cast<APlayerCharacter>(HitResult.GetActor()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Cast Succeeded"))
			BaseAI->SetPlayerSeen(true);
			BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("PlayerSeen"), true);
			return true;
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Cast failed"))
			BaseAI->SetPlayerSeen(false);
			BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("PlayerSeen"), false);
			return false;
		}
	}
	return false;
}

void UTargetVisibilityCheck::SetBlackboardValueToFalse(ABaseAIController* InController)
{
	InController->GetBlackboardComponent()->SetValueAsBool(FName("PlayerSeen"), false);
}


