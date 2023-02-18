// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIPerceptionComponent.h"

#include "AI/AIBase.h"
#include "AI/BaseAIController.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

void UMyAIPerceptionComponent::HandleExpiredStimulus(FAIStimulus& StimulusStore)
{
	TArray<AActor*> OutActors;
	
	if(StimulusStore.Type.Name.ToString().Contains("Sight"))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Sight Exprired"))
		//GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), OutActors);
		GetKnownPerceivedActors(UAISense_Sight::StaticClass(), OutActors);
		for(const AActor* const PerceivedActor : LastPerceivedActors_Sight)
		{
			if(!OutActors.Contains(PerceivedActor))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s stimulus expired"), *PerceivedActor->GetName())
				ABaseAIController* BaseAIController = Cast<ABaseAIController>(GetOwner());
				if(BaseAIController)
				{
					BaseAIController->SetLastStimulusType(ELastStimulusType::ELST_MAX, PerceivedActor);
					if(BaseAIController->GetAIBase())
					{
						if(!LastPerceivedActors_Sight.Contains(PerceivedActor) && !LastPerceivedActors_Hearing.Contains(PerceivedActor)) BaseAIController->GetAIBase()->SetAIState(BaseAIController->GetAIBase()->GetLastAIState());
					}
				}
			}
		}
		LastPerceivedActors_Sight = OutActors;
		OnSightStimulusExpired.Broadcast();
	}
	else if(StimulusStore.Type.Name.ToString().Contains("Hearing"))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hearing Exprired"))
		//GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(), OutActors);
		GetKnownPerceivedActors(UAISense_Hearing::StaticClass(), OutActors);
		for(const AActor* const PerceivedActor : LastPerceivedActors_Hearing)
		{
			if(!OutActors.Contains(PerceivedActor))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s stimulus expired"), *PerceivedActor->GetName())
				ABaseAIController* BaseAIController = Cast<ABaseAIController>(GetOwner());
				if(BaseAIController)
				{
					BaseAIController->SetLastStimulusType(ELastStimulusType::ELST_MAX, PerceivedActor);
					if(BaseAIController->GetAIBase())
					{
						if(!LastPerceivedActors_Sight.Contains(PerceivedActor) && !LastPerceivedActors_Hearing.Contains(PerceivedActor)) BaseAIController->GetAIBase()->SetAIState(BaseAIController->GetAIBase()->GetLastAIState());
					}
				}
			}
		}
		LastPerceivedActors_Hearing = OutActors;
		OnHearingStimulusExpired.Broadcast();
	}
	
	Super::HandleExpiredStimulus(StimulusStore);
}
